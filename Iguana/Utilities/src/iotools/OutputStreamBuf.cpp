//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iotools/OutputStreamBuf.h"
#include "classlib/iotools/OutputStream.h"
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

OutputStreamBuf::OutputStreamBuf (OutputStream *data /* = 0 */)
    : m_data (data)
{}

OutputStreamBuf::~OutputStreamBuf (void)
{}

void
OutputStreamBuf::attach (OutputStream *data)
{ if (m_data) detach (); m_data = data; }

bool
OutputStreamBuf::attached (void) const
{ return m_data != 0; }

void
OutputStreamBuf::detach (void)
{
    if (m_buffer.dirty ())
	syncwrite ();

    m_buffer.reset ();
    m_data = 0;
}

bool
OutputStreamBuf::close (void)
{
    if (m_buffer.dirty () && ! syncwrite ())
	return false;

    if (m_data)
	m_data->close ();

    m_buffer.reset ();
    return true;
}

OutputStream *
OutputStreamBuf::stream (void)
{ return m_data; }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
bool
OutputStreamBuf::syncwrite (void)
{
    ASSERT (m_data);
    ASSERT (m_buffer.dirty ());

    // FIXME: Exception handling -> turn into stream error state!
    // FIXME: What should we do if write() doesn't write all?
    IOSize ndirty = m_buffer.size ();
    m_data->xwrite (m_buffer.dirty (), ndirty);
    m_buffer.taken (ndirty);
    ASSERT (! m_buffer.dirty ());
    ASSERT (m_buffer.current () == m_buffer.buffer ());

    return true; // FIXME: false for failure?
}

OutputStreamBuf::size_type
OutputStreamBuf::writable (void)
{
    size_type cap;
    if (! (cap = m_buffer.capacity ()))
    {
	syncwrite ();
	cap = m_buffer.capacity ();
	ASSERT (cap);
    }

    return cap;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
OutputStreamBuf::int_type
OutputStreamBuf::overflow (int_type c /* = CLASSLIB_STREAM_EOF */)
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

OutputStreamBuf::int_type
OutputStreamBuf::sync (void)
{
    ASSERT (m_data);
    if (m_buffer.dirty ())
	syncwrite ();

    return 0; // FIXME?
}

OutputStreamBuf::size_type
OutputStreamBuf::xsputn (const char_type *data, size_type length)
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

} // namespace lat
