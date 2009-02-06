//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iotools/WriteBuffer.h"
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
    ASSERT (m_limit >= m_current);		\
    ASSERT (m_dirty <= m_current)

//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Allocate a new read-write buffer with buffer of @a size bytes.  */
WriteBuffer::WriteBuffer (IOSize bufsize /* = DEFAULT_SIZE */)
    : m_buffer (new unsigned char [bufsize]),
      m_current (m_buffer),
      m_limit (m_buffer + bufsize),
      m_dirty (m_buffer)
{}

/** Copy a read-write buffer.  Copies all buffered data as well.  */
WriteBuffer::WriteBuffer (const WriteBuffer &x)
    : m_buffer (0),
      m_current (0),
      m_limit (0),
      m_dirty (0)
{ *this = x; }

/** Copy a read-write buffer.  Copies all buffered data as well.  */
WriteBuffer &
WriteBuffer::operator= (const WriteBuffer &x)
{
    if (this != &x)
    {
	unsigned char	*buf = new unsigned char [x.m_limit - x.m_buffer];
	delete [] m_buffer;

	m_buffer	= buf;
	m_current	= buf + (x.m_current - x.m_buffer);
	m_limit		= buf + (x.m_limit - x.m_buffer);
	m_dirty		= buf + (x.m_dirty - x.m_buffer);
	
	memcpy (m_buffer, x.m_buffer, x.m_limit - x.m_buffer);
    }
    return *this;
}

/** Deallocate the buffer.  */
WriteBuffer::~WriteBuffer (void)
{ CHECK_BUFFER (); delete [] m_buffer; }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Return the number of bytes buffered with #put().  */
IOSize
WriteBuffer::size (void) const
{ CHECK_BUFFER (); return m_current - m_buffer; }

/** Return the capacity left in the buffer.  Returns zero if no more
    data can be buffered.  */
IOSize
WriteBuffer::capacity (void) const
{ CHECK_BUFFER (); return m_limit - m_current; }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Get the beginning of the dirty data waiting for write.  */
unsigned char *
WriteBuffer::dirty (void) const
{ CHECK_BUFFER (); return m_dirty; }

/** Get the beginning of the buffer.  */
unsigned char *
WriteBuffer::buffer (void) const
{ CHECK_BUFFER (); return m_buffer; }

/** Get the current buffer position: the location at which the next
    #put() would occur.  Data between #dirty() and #current() is
    waiting to be written out.  */
unsigned char *
WriteBuffer::current (void) const
{ CHECK_BUFFER (); return m_current; }

/** Get the pointer to the byte past the allocated buffer.  No buffer
    access may exceed #limit(), inclusive.  */
unsigned char *
WriteBuffer::limit (void) const
{ CHECK_BUFFER (); return m_limit; }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Reset the buffer.  */
void
WriteBuffer::reset (void)
{
    CHECK_BUFFER ();
    m_current = m_dirty = m_buffer;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Mark @a n bytes of the buffer used as if #put() was called.  This
    method should be used by clients copying data directly into the
    buffer: instead of calling #put() they should call this method
    always immediately after touching the buffer contents.  */
void
WriteBuffer::bump (IOSize n)
{
    CHECK_BUFFER ();
    ASSERT (n <= IOSize (m_limit - m_current));
    m_current += n;
}

/** Put @a n bytes from @a data into the buffer.  Use this method
    for write buffering; use #capacity() to determine how much data
    the buffer can store before synchronisation with external store
    is required.  */
void
WriteBuffer::put (const void *data, IOSize n)
{
    CHECK_BUFFER ();
    ASSERT (n <= IOSize (m_limit - m_current));
    memcpy (m_current, data, n);
    m_current += n;
}

/** Indicate @a n bytes has been written from the buffer at
    #dirty().  If @a n is all remaining dirty data, the buffer
    is made clean again.  */
void
WriteBuffer::taken (IOSize n)
{
    CHECK_BUFFER ();
    ASSERT (n <= IOSize (m_current - m_dirty));
    m_dirty += n;
    if (m_dirty == m_current)
	reset ();
}

} // namespace lat
