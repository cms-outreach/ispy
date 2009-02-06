//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iotools/PushBackBuffer.h"
#include "classlib/utils/BitOps.h"
#include "classlib/utils/DebugAids.h"
#include <algorithm>
#include <cstring>

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>

#define CHECK_BUFFER()				\
    ASSERT (m_buffer);				\
    ASSERT (m_limit > m_buffer);		\
    ASSERT (m_current >= m_buffer);		\
    ASSERT (m_limit >= m_current)

//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Construct a new push-back buffer of @a size bytes.  */
PushBackBuffer::PushBackBuffer (IOSize bufsize /* = DEFAULT_SIZE */)
    : m_buffer (new unsigned char [bufsize]),
      m_current (m_buffer + bufsize),
      m_limit (m_buffer + bufsize)
{ CHECK_BUFFER (); }

/** Copy the buffer.  */
PushBackBuffer::PushBackBuffer (const PushBackBuffer &x)
    : m_buffer (new unsigned char [x.m_limit - x.m_buffer]),
      m_current (m_buffer + (x.m_current - x.m_buffer)),
      m_limit (m_buffer + (x.m_limit - x.m_buffer))
{ CHECK_BUFFER (); memcpy (m_current, x.m_current, x.m_limit - x.m_current); }

/** Copy the buffer.  */
PushBackBuffer &
PushBackBuffer::operator= (const PushBackBuffer &x)
{
    CHECK_BUFFER ();

    if (this != &x)
    {
	unsigned char *buf = new unsigned char [x.m_limit - x.m_buffer];
	delete [] m_buffer;
	m_buffer = buf;
	m_current = buf + (x.m_current - x.m_buffer);
	m_limit = buf + (x.m_limit - x.m_buffer);
	memcpy (m_current, x.m_current, x.m_limit - x.m_current);
    }
    return *this;
}

/** Free the buffer.  */
PushBackBuffer::~PushBackBuffer (void)
{ CHECK_BUFFER (); delete m_buffer; }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Get the number of bytes remaining in the buffer to be read.  */
IOSize
PushBackBuffer::size (void) const
{ CHECK_BUFFER (); return m_limit - m_current; }

/** Get the current buffer capacity.  */
IOSize
PushBackBuffer::capacity (void) const
{ CHECK_BUFFER (); return m_limit - m_buffer; }

/** Get the pointer to the next byte to be read.  */
unsigned char *
PushBackBuffer::current (void) const
{ CHECK_BUFFER (); return m_current; }

/** Get the pointer to one past the last byte to read.  */
unsigned char *
PushBackBuffer::end (void) const
{ CHECK_BUFFER (); return m_limit; }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Clears the push-back area.  */
void
PushBackBuffer::reset (void)
{ CHECK_BUFFER (); m_current = m_limit; }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Skip @a n bytes in the buffer.  */
void
PushBackBuffer::skip (IOSize n)
{
    CHECK_BUFFER ();
    ASSERT (n <= IOSize (m_limit - m_current));
    m_current += n;
}

/** Read @a n bytes of data into @a into from the buffer.  */
void
PushBackBuffer::get (void *into, IOSize n)
{
    CHECK_BUFFER ();
    ASSERT (n <= IOSize (m_limit - m_current));
    memcpy (into, m_current, n);
    m_current += n;
}

/** Put @a data into the buffer.  Subsequent #get() calls will read the
    bytes from the buffer in the order given: the first byte of @a data
    will be the first byte to be re-read.  */
void
PushBackBuffer::unget (const void *data, IOSize n)
{
    CHECK_BUFFER ();
    if (IOSize (m_current - m_buffer) < n)
    {
	// Make more room; round the new size up to the next power of two.
	IOSize		oldsize = m_limit - m_current;
	IOSize		newsize = BitOps<IOSize>::ceil2 (oldsize + n);
	unsigned char	*buf = new unsigned char [newsize];

	memcpy (buf + newsize - oldsize, m_current, oldsize);
	delete [] m_buffer;
	m_buffer = buf;
	m_limit = buf + newsize;
	m_current = buf + newsize - oldsize;
    }

    // There's now enough room in the buffer.  Copy the data into the
    // top end of it.
    CHECK_BUFFER ();
    ASSERT (IOSize (m_current - m_buffer) >= n);
    memcpy (m_current -= n, data, n);
}

} // namespace lat
