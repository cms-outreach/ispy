//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iotools/PushBackInputStream.h"
#include "classlib/utils/BitOps.h"
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

/** FIXME.  */
PushBackInputStream::PushBackInputStream (InputStream *input)
    : FilterInputStream (input),
      m_buffer (new unsigned char [100]),
      m_bottom (m_buffer + 100),
      m_top (m_buffer + 100)
{}

/** FIXME.  */
PushBackInputStream::~PushBackInputStream (void)
{ delete m_buffer; }

/** Check if more input is available.  Returns the number of bytes in
    push-back area, or if that is empty, whatever upstream returns.  */
IOSize
PushBackInputStream::available (void)
{ return m_top != m_bottom ? m_top-m_bottom : FilterInputStream::available(); }

/** Read data from push-back area, or if it is empty, from upstream.  */
IOSize
PushBackInputStream::read (void *into, IOSize n)
{
    if (m_top != m_bottom)
    {
	IOSize buffered = std::min (n, IOSize (m_top - m_bottom));
	memcpy (into, m_bottom, buffered);
	m_bottom += buffered;
	return buffered;
    }
    else
	return FilterInputStream::read (into, n);
}

/** Skip @a n bytes.  Skips first bytes from the push-back area, and
    if more than that should be skipped, skips upstream.  Returns the
    number of bytes skipped from the push-back area and upstream.  */
IOSize
PushBackInputStream::skip (IOSize n)
{
    IOSize mine = std::min (n, IOSize (m_top - m_bottom));
    m_bottom += mine;
    if (n > mine)
	return mine + FilterInputStream::skip (n - mine);
    else
	return mine;
}

/** Close the input.  Clears the push-back area and closes upstream.  */
void
PushBackInputStream::close (void)
{ m_bottom = m_top; FilterInputStream::close (); }

/** Reset the input.  Clears the push-back area and resets upstream.  */
void
PushBackInputStream::reset (void)
{ m_bottom = m_top; FilterInputStream::reset (); }

/** Unreads some data.  The data is put in the push-back buffer held
    by this filter.  Subsequent read operations will first read from
    the push-back area and only then from the upstream.  The bytes are
    put in the push-back area in the order given: the first byte of @a
    from will be the first byte to be re-read.  */
void
PushBackInputStream::unread (IOBuffer from)
{ unread (from.data (), from.size ()); }

/** Unreads some data.  The data is put in the push-back buffer held
    by this filter.  Subsequent read operations will first read from
    the push-back area and only then from the upstream.  The bytes are
    put in the push-back area in the order given: the first byte of @a
    from will be the first byte to be re-read.  */
void
PushBackInputStream::unread (const void *data, IOSize n)
{
    if (IOSize (m_bottom - m_buffer) < n)
    {
	// Make more room; round the new size up to the next power of two.
	unsigned	bufsize = m_top-m_bottom;
	unsigned	newsize = BitOps<unsigned>::ceil2 (bufsize + n);
	unsigned char	*buf = new unsigned char [newsize];

	memcpy (buf + newsize - bufsize, m_bottom, bufsize);
	delete [] m_buffer;
	m_buffer = buf;
	m_top = buf + newsize;
	m_bottom = buf + newsize - bufsize;
    }

    // There's now enough room in the buffer.  Copy the data into the
    // top end of it.
    ASSERT (IOSize (m_bottom - m_buffer) >= n);
    memcpy (m_bottom -= n, data, n);
}

} // namespace lat
