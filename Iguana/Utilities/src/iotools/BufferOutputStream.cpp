//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iotools/BufferOutputStream.h"
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

/** Allocate a new output stream with a write buffer of @a size bytes.  */
BufferOutputStream::BufferOutputStream (OutputStream *output, IOSize bufsize)
    : FilterOutputStream (output),
      m_buffer (0),
      m_top (0),
      m_limit (0)
{
    if (! bufsize)
	bufsize = 4096;

    m_top = m_buffer = new unsigned char [bufsize];
    m_limit = m_buffer + bufsize;
}

/** FIXME.  */ // doesn't delete but closes the output
BufferOutputStream::~BufferOutputStream (void)
{ delete m_buffer; /* close (); */ }

/** Write to the buffer, or if it is full, to downstream.  Writes
    to downstream are buffered if the request is smaller than the
    buffer size, otherwise it is passed on directly. */
IOSize
BufferOutputStream::write (const void *from, IOSize n)
{
    ASSERT (m_buffer);
    ASSERT (m_limit - m_buffer > 0);
    ASSERT (m_top >= m_buffer);
    ASSERT (m_top <= m_limit);

    // First copy into our buffer at how much space we have and what
    // was requested, whichever is smaller.  If this is enough, return.
    const unsigned char *buf = static_cast<const unsigned char *> (from);
    IOSize	  buffered = std::min (n, IOSize (m_limit - m_top));
    IOSize	  left = n - buffered;

    memcpy (m_top, buf, buffered);
    m_top += buffered;
    if (! left)
	return buffered;

    // Write out our buffer.
    IOSize did = flush ();

    // Write the rest.  If the remaining amount is more than our
    // buffer size, just pass the request downstream.  We assume the
    // final downstream write() does an equivalent of xwrite() when
    // hopping to another object so we don't have to worry about
    // xwrite() calling back to this method.
    if (left >= IOSize (m_limit - m_buffer))
	return did + FilterOutputStream::write (buf + buffered, left);

    // It fits in our buffer, fill the buffer.
    memcpy (m_top, buf + buffered, left);
    m_top += left;
    return buffered + left;
}

IOSize
BufferOutputStream::flush (void)
{
    // Write out our buffer.  We assume the final downstream write()
    // does an equivalent of xwrite() when hopping to another object
    // so we don't have to worry about xwrite() calling back to this
    // method.
    IOSize left = m_top - m_buffer;
    IOSize did = FilterOutputStream::write (m_buffer, left);
    ASSERT (did == left);
    m_top = m_buffer;
    return did;
}

void
BufferOutputStream::finish (void)
{ flush (); FilterOutputStream::finish (); }

void
BufferOutputStream::close (void)
{ flush (); FilterOutputStream::close (); }

} // namespace lat
