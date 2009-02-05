//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iotools/IOChannelStreamBuf.h"
#include "classlib/iobase/IOChannel.h"
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

IOChannelStreamBuf::IOChannelStreamBuf (IOChannel *data /* = 0 */)
    : m_data (data)
{}

IOChannelStreamBuf::~IOChannelStreamBuf (void)
{}

void
IOChannelStreamBuf::attach (IOChannel *data)
{ if (m_data) detach (); m_data = data; }

bool
IOChannelStreamBuf::attached (void) const
{ return m_data != 0; }

void
IOChannelStreamBuf::detach (void)
{
    if (m_wbuffer.dirty ())
	syncwrite ();

    m_rbuffer.reset ();
    m_wbuffer.reset ();
    m_data = 0;
}

bool
IOChannelStreamBuf::close (void)
{
    if (m_wbuffer.dirty () && ! syncwrite ())
	return false;

    if (m_data)
	m_data->close ();

    m_rbuffer.reset ();
    m_wbuffer.reset ();
    return true;
}

IOChannel *
IOChannelStreamBuf::channel (void)
{ return m_data; }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
bool
IOChannelStreamBuf::syncwrite (void)
{
    ASSERT (m_data);
    ASSERT (m_wbuffer.dirty ());

    // FIXME: Exception handling -> turn into stream error state!
    // FIXME: What should we do if write() doesn't write all?
    IOSize ndirty = m_wbuffer.size ();
    m_data->xwrite (m_wbuffer.dirty (), ndirty);
    m_wbuffer.taken (ndirty);
    ASSERT (! m_wbuffer.dirty ());
    ASSERT (m_wbuffer.current () == m_wbuffer.buffer ());

    return true; // FIXME: false for failure?
}

IOChannelStreamBuf::size_type
IOChannelStreamBuf::writable (void)
{
    size_type cap;
    if (! (cap = m_wbuffer.capacity ()))
    {
	syncwrite ();
	cap = m_wbuffer.capacity ();
	ASSERT (cap);
    }

    return cap;
}

IOChannelStreamBuf::size_type
IOChannelStreamBuf::refill (void)
{
    ASSERT (m_data);
    ASSERT (m_rbuffer.capacity ());
    if (m_wbuffer.size ())
	syncwrite ();

    IOSize capacity = m_rbuffer.capacity ();
    IOSize read = m_data->read (m_rbuffer.end (), capacity);
    m_rbuffer.filled (read);
    return read;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
IOChannelStreamBuf::size_type
IOChannelStreamBuf::showmanyc (void)
{ return m_rbuffer.left (); }

IOChannelStreamBuf::int_type
IOChannelStreamBuf::overflow (int_type c /* = CLASSLIB_STREAM_EOF */)
{
    if (c != CLASSLIB_STREAM_EOF)
    {
	// Normal character, write it out
        VERIFY (writable ());
	m_wbuffer.bump (1);
	*(m_wbuffer.current ()-1) = c;
	return 1; // FIXME?
    }
    else
    {
	// EOF: flush the buffer out
	if (m_wbuffer.dirty ())
	    syncwrite ();  // FIXME: failure?

	return CLASSLIB_STREAM_EOF; // FIXME?
    }
}

IOChannelStreamBuf::int_type
IOChannelStreamBuf::underflow (void)
{
    // FIXME: syncwrite?
    int_type c = CLASSLIB_STREAM_EOF;
    if (m_rbuffer.left () || refill ())
    {
	ASSERT (m_rbuffer.left ());
	c = *m_rbuffer.current ();
	m_rbuffer.skip (1);
    }
    return c;
}

IOChannelStreamBuf::int_type
IOChannelStreamBuf::uflow (void)
{
    // FIXME: syncwrite?
    int_type c = CLASSLIB_STREAM_EOF;
    if (m_rbuffer.left () || refill ())
    {
	ASSERT (m_rbuffer.left ());
	c = *m_rbuffer.current ();
    }
    return c;
}

IOChannelStreamBuf::int_type
IOChannelStreamBuf::pbackfail (int_type c /* = CLASSLIB_STREAM_EOF */)
{
    // If we are at the beginning of the buffer and want to reuse existing
    // stuff, fail.  (FIXME: Don't warp buffer when at the end to allow
    // us to push-back for longer?  It's not right that just because a
    // byte happens to be at a buffer boundary we can't use push-back.)
    if (m_rbuffer.current () == m_rbuffer.buffer () && c == CLASSLIB_STREAM_EOF)
	return CLASSLIB_STREAM_EOF;

    // There is now a character available in the putback area.
    // Put the requested character now in there: whatever is
    // there if c == EOF, otherwise c.
    unsigned char ch = c;
    if (c == CLASSLIB_STREAM_EOF)
	ch = m_rbuffer.current () [-1];

    m_rbuffer.unget (&ch, 1);
    return c == CLASSLIB_STREAM_EOF ? 0 : ch;
}

IOChannelStreamBuf::int_type
IOChannelStreamBuf::sync (void)
{
    ASSERT (m_data);
    if (m_wbuffer.dirty ())
	syncwrite ();

    return 0; // FIXME?
}

IOChannelStreamBuf::size_type
IOChannelStreamBuf::xsputn (const char_type *data, size_type length)
{
    size_type n = 0;
    size_type left;
    while (n != length)
    {
	left = writable ();
	left = std::min (length - n, left);
	m_wbuffer.put (data, left);
	data += left;
	n += left;
    }

    return n; // FIXME: EOF?
}

IOChannelStreamBuf::size_type
IOChannelStreamBuf::xsgetn (char_type *data, size_type length)
{
    size_type n = 0;
    size_type left;
    while (n != length)
    {
	if (! (left = m_rbuffer.left ()))
	    if (! (left = refill ()))
		break;

	left = std::min (length - n, left);
	m_rbuffer.get (data, left);
	data += left;
	n += left;
    }

    return n; // FIXME: EOF?
}

} // namespace lat
