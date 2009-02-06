//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iotools/BufferInputStream.h"
#include "classlib/utils/DebugAids.h"
#include <algorithm>
#include <cstring>

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

/** Allocate a new input stream with read-ahead buffer of @a size bytes.  */
BufferInputStream::BufferInputStream (InputStream *input, IOSize bufsize /*=0*/)
    : FilterInputStream (input),
      m_buffer (0),
      m_bottom (0),
      m_top (0),
      m_limit (0)
{
    if (! bufsize)
	bufsize = 4096;

    m_bottom = m_top = m_buffer = new unsigned char [bufsize];
    m_limit = m_buffer + bufsize;
}

/** FIXME.  */ // doesn't delete but closes the Input
BufferInputStream::~BufferInputStream (void)
{ delete m_buffer; /* FIXME: close (); */ }

/** Check if more input is available.  Returns the number of bytes in
    buffer, or if that is empty, whatever upstream returns.  */
IOSize
BufferInputStream::available (void)
{ return m_bottom < m_top ? m_top-m_bottom : FilterInputStream::available(); }

/** Read data from buffer, or if it is empty, from upstream.  Reads
    from upstream are buffered if the request is smaller than the
    buffer size, otherwise it is passed on directly. */
IOSize
BufferInputStream::read (void *into, IOSize n)
{
    ASSERT (m_buffer);
    ASSERT (m_limit - m_buffer > 0);
    ASSERT (m_bottom >= m_buffer);
    ASSERT (m_top >= m_bottom);
    ASSERT (m_top <= m_limit);

    // First copy from our buffer at most what we have and what was
    // requested, whichever is smaller.  If this is enough, return.
    unsigned char *buf = static_cast<unsigned char *> (into);
    IOSize	  buffered = std::min (n, IOSize (m_top - m_bottom));
    IOSize	  left = n - buffered;

    memcpy (buf, m_bottom, buffered);
    m_bottom += buffered;
    if (! left)
	return buffered;

    // Read some more.  If the remaining amount is more than our buffer
    // size, just pass the request upstream.  (FIXME: Upstream may not
    // return as much as asked, should we buffer in that case?)
    if (left >= IOSize (m_limit - m_buffer))
	return buffered + FilterInputStream::read (buf + buffered, left);

    // The request fits in our buffer, refill and return data from it.
    // This read may not return enough data, in which case another read
    // later on may return some more or we will return zero to indicate
    // end of stream.  (FIXME: Do we need to track zero return for eof?)
    m_bottom = m_top = m_buffer;
    m_top += FilterInputStream::read (m_buffer, m_limit-m_buffer);
    IOSize copy = std::min (IOSize (m_top - m_bottom), left);
    memcpy (buf + buffered, m_bottom, copy);
    m_bottom += copy;
    return buffered + copy;
}

/** Skip @a n bytes.  Skips first bytes from the buffer, and if more
    than that should be skipped, skips upstream.  Returns the number
    of bytes skipped from the buffer and upstream.  */
IOSize
BufferInputStream::skip (IOSize n)
{
    ASSERT (m_buffer);
    ASSERT (m_limit - m_buffer > 0);
    ASSERT (m_bottom >= m_buffer);
    ASSERT (m_top >= m_bottom);
    ASSERT (m_top <= m_limit);

    IOSize mine = std::min (IOSize (m_top - m_bottom), n);
    m_bottom += mine;
    if (n - mine > 0)
	return mine + FilterInputStream::skip (n - mine);
    else
	return mine;
}

/** Close the input.  Clears the buffer and closes upstream.  */
void
BufferInputStream::close (void)
{ m_top = m_bottom = m_buffer; FilterInputStream::close (); }

/** Reset the input.  Clears the buffer and resets upstream.  */
void
BufferInputStream::reset (void)
{ m_top = m_bottom = m_buffer; FilterInputStream::reset (); }

} // namespace lat
