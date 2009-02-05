//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/DirIterator.h"
#include "classlib/iobase/Filename.h"
#include "classlib/iobase/FileSystemError.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/sysapi/DirIterator.h"

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** @fn bool		DirIterator::sysnext (void) */
/** @fn void		DirIterator::sysinitialise (void) */
/** @fn void		DirIterator::sysclose (bool throwable) */
/** @fn std::string	DirIterator::sysderef (void) const */

/** Steal the iterator state and "invalidate" all other copies by
    putting them into read-only state.  Called whenever the iterator
    is about to modified, i.e. advanced.  This implements the iterator
    semantics mentioned in the class documentation: this iterator is
    "unshared" from any possible copies, and all those other copies
    are put into a state where they remember the file we currently
    point to, but cannot be advanced any more. */
void
DirIterator::steal (void)
{
    if (m_data->m_refs > 1)
    {
	DirIteratorData *data = new DirIteratorData (*m_data);

	m_data->m_last_name = new std::string (**this);
	m_data->m_valid = false;
	m_data->m_refs--;
	m_data = data;
	m_data->m_refs = 1;
    }
}

/** Actually advance this iterator.  This may translate to more than
    one system level advance of the iteration context if the iterator
    has been told to ignore "dot" entries: the directory itself and
    the parent directory.  */
void
DirIterator::next (void)
{
    ASSERT (m_data);
    ASSERT (m_data->m_valid);
    ASSERT (! m_data->m_last_name);
    steal ();

    ASSERT (m_data->m_dir);
    bool skip;
    do
	skip = sysnext ();
    while (! m_data->m_last && skip);
}

/** Common initialisation code.  Initialises the directory to the
    system directory @a dirname, and prepares the iterator to
    ignore the "dot" entries (directory itself and its parent) if
    @a dots is @c false.  Advances the directory context to the
    first file.  */
inline void
DirIterator::initialise (const char *dirname, bool dots)
{
    ASSERT (m_data);

    // If the system method throws, delete the internal representation
    // -- we are being called from a constructor, so throwing means
    // the object will be never fully constructed and thus won't be
    // destroyed.  So just short-circuit the destruction here.
    // Should probably just use a smart pointer for m_data.
    try
    {
        m_data->m_refs = 1;
        m_data->m_last = dirname ? false : true;
        m_data->m_valid = true;
        m_data->m_dots = dots;
        m_data->m_last_name = 0;

        sysinitialise (dirname);
    }
    catch (...)
    {
	delete m_data;
	m_data = 0;
	throw;
    }
}

/** Release the current iteration state.  Unshares the iterator
    from its current context, and if this is a last instance,
    deletes the context object.  If @a throwable is @c true,
    allows exceptions raised by the operating system to pass
    through.  Otherwise exceptions are swallowed, mainly to
    prevent exceptions to be raised while unwinding the stack. */
void
DirIterator::release (bool throwable)
{
    if (--m_data->m_refs == 0)
    {
	close (throwable);
	delete m_data;
	m_data = 0;
    }
}

//////////////////////////////////////////////////////////////////////
/** Construct an empty directory iterator.  This will point to a
    virtual "empty" directory.  Also used to construct a default
    "end" iterator: all directory iterators will compare equal
    to the default-constructed iterator when they've exhausted
    the contents of the directory.  */
DirIterator::DirIterator (void)
    : m_data (new DirIteratorData)
{ initialise (0, false); }

/** Construct a directory iterator on directory @a dirname.  Sets the
    iterator to by default to skip "dot" entries "." (directory itself)
    and ".." (parent directory) if @a dots is the default @c false.
    If @a dots is @c true, those entries are returned if they exist;
    it depends on the system and file system whether those entries
    exist at all, and are returned to the application.  */
DirIterator::DirIterator (const char *dirname, bool dots /* = false */)
    : m_data (new DirIteratorData)
{ initialise (dirname, dots); }

/** Overloaded copy of the constructor.  */
DirIterator::DirIterator (const std::string &dirname, bool dots /* = false */)
    : m_data (new DirIteratorData)
{ initialise (dirname.c_str (), dots); }

/** Overloaded copy of the constructor.  */
DirIterator::DirIterator (const Filename &dirname, bool dots /* = false */)
    : m_data (new DirIteratorData)
{ initialise (STATIC_CAST (const char *, dirname), dots); }

/** Copy an iterator.  Note that while the iterators can be copied,
    once any of the copies is advanced all the copies become read-only:
    they can be referenced and destroyed, but not advanced.  */
DirIterator::DirIterator (const DirIterator &x)
    : m_data (x.m_data)
{
    ASSERT (m_data);
    m_data->m_refs++;
}

/** Destruct a directory iterator.  This closes the iterator (see
    #close()) but ignores any errors from doing so.  If you care
    about possible errors, call @c close(true) yourself.  */
DirIterator::~DirIterator (void)
{ release (false); }

/** Copy an iterator.  Note that while the iterators can be copied,
    once any of the copies is advanced all the copies become read-only:
    they can be referenced and destroyed, but not advanced.  */
DirIterator &
DirIterator::operator= (const DirIterator &x)
{
    x.m_data->m_refs++;
    release (true);
    m_data = x.m_data;
    return *this;
}

/** Close the iterator.  If @a throwable is @c true, allows exceptions
    to be raised.  Otherwise the exceptions are swallowed, e.g. to
    prevent a nested exception to be raised when another one is
    already being handled.  */
void
DirIterator::close (bool throwable)
{
    ASSERT (m_data);
    if (m_data->m_valid)
	sysclose (throwable);
    else
	delete m_data->m_last_name;
}

/** Dereference the iterator.  Returns the file name at the current
    iteration position.  Note that the returned name is a bare name;
    it does not include the name of the directory being scanned.
    Note also that the entries are returned in no particular order,
    the order may vary between different scans of the directory and
    if some other process is currently modifying the directory the
    iterator may return a state that is a mixture of the old and
    the new contents of the directory.  Note that if the iterator
    was created with the default @c dots=false, dereferencing the
    iterator will never return the "dot" entries ("." and "..").  */
std::string
DirIterator::operator* (void) const
{
    // Watch it here; we already have a value, but it may be in m_dir
    // or in m_last_value depending on whether this is a current state
    // iterator or a discarded old value (e.g. from *x++).  Only `next'
    // ensures that only one iterator copy is ever incremented.
    ASSERT (m_data);
    ASSERT (! m_data->m_last);
    ASSERT (m_data->m_valid || m_data->m_last_name);

    if (m_data->m_valid)
    {
	ASSERT (m_data->m_dir);
	return sysderef ();
    }
    else
	return *m_data->m_last_name;
}

/** Advance the iterator.  Doing so will render all outstanding copies
    of the iterator read-only.  This pre-increment operator will return
    the iterator itself after having advanced it.  */
DirIterator &
DirIterator::operator++ (void)
{
    ASSERT (m_data);
    ASSERT (m_data->m_valid);
    ASSERT (! m_data->m_last);

    next ();
    return *this;
}

/** Advance the iterator.  Doing so will render all outstanding copies
    of the iterator read-only.  Note that this post-increment version
    will return the old state of the directory, which can be read
    (dereferenced), but cannot be advanced any more; you'll have to
    subsequently advance this iterator, not the copy returned.  */
DirIterator
DirIterator::operator++ (int)
{
    ASSERT (m_data);
    ASSERT (m_data->m_valid);
    ASSERT (! m_data->m_last);

    DirIterator old (*this);
    next ();
    return old;
}

/** Compare iterators for equality.  Returns @c true if both iterators
    point to the same iteration state: the same file in the iteration
    over the contents of the directory.  It doesn't matter whether the
    iterators are the current head, or point to some "past history".
    Note that once the iteration has exhausted the files in the
    directory, the iterator will compare equal to default-constructed
    iterator.  */
bool
DirIterator::operator== (const DirIterator &x) const
{
    ASSERT (m_data);
    ASSERT (x.m_data);

    return ((m_data->m_last && x.m_data->m_last)
	    || (m_data == x.m_data
		&& m_data->m_valid == x.m_data->m_valid
		&& m_data->m_last_name == x.m_data->m_last_name));
}

/** Inverse of equality.  */
bool
DirIterator::operator!= (const DirIterator &x) const
{ return ! (*this == x); }

} // namespace lat
