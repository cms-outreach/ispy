//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iotools/InputStreamBuf.h"
#include "classlib/iotools/InputStream.h"
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

InputStreamBuf::InputStreamBuf (InputStream *data /* = 0 */)
    : m_data (data)
{}

InputStreamBuf::~InputStreamBuf (void)
{}

void
InputStreamBuf::attach (InputStream *data)
{ if (m_data) detach (); m_data = data; }

bool
InputStreamBuf::attached (void) const
{ return m_data != 0; }

void
InputStreamBuf::detach (void)
{ m_data = 0; m_buffer.reset (); }

bool
InputStreamBuf::close (void)
{
    if (m_data)
	m_data->close ();

    m_buffer.reset ();
    return true;
}

InputStream *
InputStreamBuf::stream (void)
{ return m_data; }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
InputStreamBuf::size_type
InputStreamBuf::refill (void)
{
    ASSERT (m_data);
    ASSERT (m_buffer.capacity ());
    IOSize capacity = m_buffer.capacity ();
    IOSize read = m_data->read (m_buffer.end (), capacity);
    m_buffer.filled (read);
    return read;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
InputStreamBuf::size_type
InputStreamBuf::showmanyc (void)
{ return m_buffer.left (); }

InputStreamBuf::int_type
InputStreamBuf::underflow (void)
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

InputStreamBuf::int_type
InputStreamBuf::uflow (void)
{
    int_type c = CLASSLIB_STREAM_EOF;
    if (m_buffer.left () || refill ())
    {
	ASSERT (m_buffer.left ());
	c = *m_buffer.current ();
    }
    return c;
}

InputStreamBuf::int_type
InputStreamBuf::pbackfail (int_type c /* = CLASSLIB_STREAM_EOF */)
{
    // If we are at the beginning of the buffer and want to reuse existing
    // stuff, fail.  (FIXME: Don't warp buffer when at the end to allow
    // us to push-back for longer?  It's not right that just because a
    // byte happens to be at a buffer boundary we can't use push-back.)
    if (m_buffer.current () == m_buffer.buffer () && c == CLASSLIB_STREAM_EOF)
	return CLASSLIB_STREAM_EOF;

    // There is now a character available in the putback area.
    // Put the requested character now in there: whatever is
    // there if c == EOF, otherwise c.
    unsigned char ch = c;
    if (c == CLASSLIB_STREAM_EOF)
	ch = m_buffer.current () [-1];

    m_buffer.unget (&ch, 1);
    return c == CLASSLIB_STREAM_EOF ? 0 : ch;
}

InputStreamBuf::size_type
InputStreamBuf::xsgetn (char_type *data, size_type length)
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

} // namespace lat
