#ifndef CLASSLIB_DIR_ITERATOR_H
# define CLASSLIB_DIR_ITERATOR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"
# include <iterator>
# include <string>

namespace lat {

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

struct DirIteratorData;
class DirIteratorError;
class Filename;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** An iterator for scanning over directory contents.

    An input iterator to reads the contents of a file system directory
    using @c readdir() or equivalent system implementation.  Certain
    restrictions apply due to the limits of the underlying system
    interfaces; the most important one is that all copies of an
    iterator share the reading state.  The iterators can be copied
    freely and all copies can be dereferenced.  However once the
    original or any of the copies is advanced, all other outstanding
    copies become read-only: they can still be read (dereferenced;
    they will remember the most recent file returned) and destructed,
    but they can no longer be advanced.  Even with these restrictions
    it is possible to use most common iteration and looping patterns,
    including most generic algorithms in the standard library.

    The iterator returns the names of the files in the directory in
    whatever order the system happens to be returning them: there is
    no guaranteed order within one scan or across several scans.  As
    usual, if the directory is being modified while an iterator is
    scanning it, the returned results are unpredictable -- the results
    may include an inconsistent mixture of the old and the new state
    of the directory.

    The iterator returns the name of each file in the directory, not
    the combination of the directory name and the file name.  The
    entries for `.' and `..' are returned only if the additional
    constructor argument is @c true.

    If you care about errors raised when the iteration ends, you must
    invoke #close() with @c true argument yourself.  The iterator's
    destructor will silently swallow the errors to avoid throwing an
    exception while another exception is causing an unwind.

    The iterator throws a #SystemError exception when:
     - an iterator is created and the directory cannot be accessed,
     - an iterator is copied and the old state is closed,
     - an iterator is advanced and a problem is encountered.

    The end indicator for the iteration range is the default
    constructed iterator.  */
class DirIterator ITERATOR_BASE(input_iterator, std::string, STDC::ptrdiff_t)
{
public:
    typedef std::input_iterator_tag	iterator_category;
    typedef std::string			value_type;
    typedef STDC::ptrdiff_t		difference_type;
    typedef void			reference;
    typedef void			pointer;

    DirIterator (void);
    DirIterator (const char *dirname, bool dots = false);
    DirIterator (const std::string &dirname, bool dots = false);
    DirIterator (const Filename &dirname, bool dots = false);
    DirIterator (const DirIterator &x);
    ~DirIterator (void);
    DirIterator&	operator= (const DirIterator &x);

    bool		operator== (const DirIterator &x) const;
    bool		operator!= (const DirIterator &x) const;

    std::string		operator* (void) const;
    DirIterator &	operator++ (void);
    DirIterator		operator++ (int);

    void		close (bool throwable);

private:
    void		initialise (const char *dirname, bool dots);
    void		release (bool throwable);
    void		next (void);
    void		steal (void);

    bool		sysnext (void);
    void		sysinitialise (const char *dirname);
    void		sysclose (bool throwable);
    std::string		sysderef (void) const;

    DirIteratorData	*m_data;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_DIR_ITERATOR_H
