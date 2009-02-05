//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iotools/StorageStreamBuf.h"
#include "classlib/iobase/Storage.h"
#include "classlib/utils/DebugAids.h"
#include <algorithm>

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

// FIXME: setbuf?
// FIXME: locale conversion?
// FIXME: syncwrite error handling

StorageStreamBuf::StorageStreamBuf (Storage *data /* = 0 */)
    : m_data (data)
{}

StorageStreamBuf::~StorageStreamBuf (void)
{}

void
StorageStreamBuf::attach (Storage *data)
{ if (m_data) detach (); m_data = data; }

bool
StorageStreamBuf::attached (void) const
{ return m_data != 0; }

void
StorageStreamBuf::detach (void)
{
    if (m_buffer.dirty ())
	syncwrite ();

    m_buffer.reset ();
    m_data = 0;
}

bool
StorageStreamBuf::close (void)
{
    if (m_buffer.dirty () && ! syncwrite ())
	return false;

    if (m_data)
	m_data->close ();

    m_buffer.reset ();
    return true;
}

Storage *
StorageStreamBuf::storage (void)
{ return m_data; }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
bool
StorageStreamBuf::syncwrite (void)
{
    ASSERT (m_data);
    ASSERT (m_buffer.dirty ());

    // FIXME: Exception handling -> turn into stream error state!
    // FIXME: What should we do if write() doesn't write all?
    IOSize   ndirty = m_buffer.current () - m_buffer.dirty ();
    IOOffset off = m_buffer.offset ();

    m_data->position (off, Storage::CURRENT);
    m_data->xwrite (m_buffer.dirty (), ndirty);

    m_buffer.taken (ndirty);
    ASSERT (! m_buffer.dirty ());
    ASSERT (m_buffer.current () == m_buffer.buffer ());
    ASSERT (m_buffer.current () == m_buffer.reference ());
    ASSERT (m_buffer.current () == m_buffer.end ());

    return true; // FIXME: false for failure?
}

StorageStreamBuf::size_type
StorageStreamBuf::writable (void)
{
    size_type cap;
    if (! (cap = m_buffer.capacity ()))
    {
	// The buffer capacity can be zero either if we filled
	// it with writing, or if last read buffer op filled it
	// up.  For the latter the write will always succeed,
	// but call bump(0) to find out how much space we have.
	if (m_buffer.dirty ())
	    syncwrite ();
	else
	    m_buffer.bump (0);

	cap = m_buffer.capacity ();
	ASSERT (cap);
    }

    return cap;
}

StorageStreamBuf::size_type
StorageStreamBuf::refill (void)
{
    ASSERT (m_data);
    if (m_buffer.dirty ())
	syncwrite ();

    IOSize capacity = m_buffer.capacity ();
    IOSize read = m_data->read (m_buffer.end (), capacity);
    m_buffer.filled (read);
    return read;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
StorageStreamBuf::size_type
StorageStreamBuf::showmanyc (void)
{ return m_buffer.left (); }

StorageStreamBuf::int_type
StorageStreamBuf::overflow (int_type c /* = CLASSLIB_STREAM_EOF */)
{
    if (c != CLASSLIB_STREAM_EOF)
    {
	// Normal character, write it out
        VERIFY (writable ());
	m_buffer.bump (1);
	*(m_buffer.current ()-1) = c;
	return 1; // FIXME?
    }
    else
    {
	// EOF: flush the buffer out
	if (m_buffer.dirty ())
	    syncwrite ();  // FIXME: failure?

	return CLASSLIB_STREAM_EOF; // FIXME?
    }
}

StorageStreamBuf::int_type
StorageStreamBuf::underflow (void)
{
    int_type c = CLASSLIB_STREAM_EOF;
    if (m_buffer.left () || refill ())
    {
	ASSERT (m_buffer.left ());
	c = *m_buffer.current ();
	m_buffer.skip (1);
    }
    return c;
}

StorageStreamBuf::int_type
StorageStreamBuf::uflow (void)
{
    int_type c = CLASSLIB_STREAM_EOF;
    if (m_buffer.left () || refill ())
    {
	ASSERT (m_buffer.left ());
	c = *m_buffer.current ();
    }
    return c;
}

StorageStreamBuf::int_type
StorageStreamBuf::pbackfail (int_type c /* = CLASSLIB_STREAM_EOF */)
{
    // This shouldn't happen post-output -- there should have been input first?
    ASSERT (! m_buffer.dirty ());
    if (m_buffer.dirty ())
	syncwrite (); // FIXME: failure?

    // If we are at the beginning of the buffer, make putback available.
    if (m_buffer.current () == m_buffer.buffer ())
    {
	seekoff (-1, CLASSLIB_IOS_BASE::cur, CLASSLIB_IOS_BASE::in);
	underflow ();
    }

    ASSERT (m_buffer.current () != m_buffer.buffer ());

    // There is now a character available in the putback area.
    // Put the requested character now in there: whatever is
    // there if c == EOF, otherwise c.
    unsigned char ch = c;
    if (c == CLASSLIB_STREAM_EOF)
	ch = m_buffer.current () [-1];

    m_buffer.unget (&ch, 1);
    return c == CLASSLIB_STREAM_EOF ? 0 : ch;
}

StorageStreamBuf::int_type
StorageStreamBuf::sync (void)
{
    ASSERT (m_data);
    if (m_buffer.dirty ())
	syncwrite ();

    m_data->flush ();
    return 0; // FIXME?
}

StorageStreamBuf::size_type
StorageStreamBuf::xsputn (const char_type *data, size_type length)
{
    size_type n = 0;
    size_type left;
    while (n != length)
    {
	left = writable ();
	left = std::min (length - n, left);
	m_buffer.put (data, left);
	data += left;
	n += left;
    }

    return n; // FIXME: EOF?
}

StorageStreamBuf::size_type
StorageStreamBuf::xsgetn (char_type *data, size_type length)
{
    size_type n = 0;
    size_type left;
    while (n != length)
    {
	if (! (left = m_buffer.left ()))
	    if (! (left = refill ()))
		break;

	left = std::min (length - n, left);
	m_buffer.get (data, left);
	data += left;
	n += left;
    }

    return n; // FIXME: EOF?
}

StorageStreamBuf::pos_type
StorageStreamBuf::seekoff (offset_type off, seekdir dir, openmode /* which */)
{
    ASSERT (m_data);
    if (m_buffer.dirty ())
	syncwrite ();

    return m_data->position (off,
			     dir == CLASSLIB_IOS_BASE::cur ? Storage::CURRENT
			     : dir == CLASSLIB_IOS_BASE::end ? Storage::END
			     : Storage::SET);
}

StorageStreamBuf::pos_type
StorageStreamBuf::seekpos (pos_type pos, openmode /* which */)
{
    ASSERT (m_data);
    if (m_buffer.dirty ())
	syncwrite ();

    return m_data->position (pos, Storage::SET);
}

} // namespace lat
