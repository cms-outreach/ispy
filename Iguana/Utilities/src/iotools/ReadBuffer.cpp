//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iotools/ReadBuffer.h"
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
    ASSERT (m_end >= m_current);		\
    ASSERT (m_limit >= m_end);			\
    ASSERT (! m_pback || m_pback >= m_current);	\
    ASSERT (! m_pback || m_pback <= m_end)

//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Allocate a new read buffer with buffer of @a size bytes.  */
ReadBuffer::ReadBuffer (IOSize bufsize /* = DEFAULT_SIZE */)
    : m_buffer (new unsigned char [bufsize]),
      m_current (m_buffer),
      m_end (m_buffer),
      m_limit (m_buffer + bufsize),
      m_pback (0)
{}

/** Copy a read-write buffer.  Copies all buffered data as well.  */
ReadBuffer::ReadBuffer (const ReadBuffer &x)
    : m_buffer (0),
      m_current (0),
      m_end (0),
      m_limit (0),
      m_pback (0)
{ *this = x; }

/** Copy a read-write buffer.  Copies all buffered data as well.  */
ReadBuffer &
ReadBuffer::operator= (const ReadBuffer &x)
{
    if (this != &x)
    {
	unsigned char	*buf = new unsigned char [x.m_limit - x.m_buffer];
	delete [] m_buffer;

	m_buffer	= buf;
	m_current	= buf + (x.m_current - x.m_buffer);
        m_end		= buf + (x.m_end - x.m_buffer);
	m_limit		= buf + (x.m_limit - x.m_buffer);
	m_pback		= x.m_pback ? buf + (x.m_pback - x.m_buffer) : 0;
	
	memcpy (m_buffer, x.m_buffer, x.m_limit - x.m_buffer);
    }
    return *this;
}

/** Deallocate the buffer.  */
ReadBuffer::~ReadBuffer (void)
{ CHECK_BUFFER (); delete [] m_buffer; }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Return the number of bytes left to #get() from the buffer.  */
IOSize
ReadBuffer::left (void) const
{ CHECK_BUFFER (); return m_end - m_current; }

/** Return the capacity left in the buffer.  Returns zero if no more
    data can be buffered.  */
IOSize
ReadBuffer::capacity (void) const
{ CHECK_BUFFER (); return m_limit - m_end; }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Get the end of the push-back data.  Returns null if #current() is
    not inside push-back area, otherwise pointer to real buffered data
    that has not been pushed back (i.e. #current() at the time data
    was first pushed back).  Resets automatically to null when all
    pushed back data has been fetched via #get().  */
unsigned char *
ReadBuffer::pback (void) const
{ CHECK_BUFFER (); return m_pback; }

/** Get the beginning of the buffer.  */
unsigned char *
ReadBuffer::buffer (void) const
{ CHECK_BUFFER (); return m_buffer; }

/** Get the current buffer position: the location at which the next
    #get() would occur.  If in pushed-back data, #current() is less
    than #pback().  */
unsigned char *
ReadBuffer::current (void) const
{ CHECK_BUFFER (); return m_current; }

/** Get the current buffer end: one byte past the last valid data in
    the buffer.  Fill data into the buffer at #end() but do not exceed
    #limit().  Then update buffer status with #filled().  */
unsigned char *
ReadBuffer::end (void) const
{ CHECK_BUFFER (); return m_end; }

/** Get the pointer to the byte past the allocated buffer.  No buffer
    access may exceed #limit(), inclusive.  */
unsigned char *
ReadBuffer::limit (void) const
{ CHECK_BUFFER (); return m_limit; }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Reset the buffer.  */
void
ReadBuffer::reset (void)
{
    CHECK_BUFFER ();
    m_current = m_end = m_buffer;
    m_pback = 0;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Indicate @a n bytes have been added to the buffer at #end().  */
void
ReadBuffer::filled (IOSize n)
{
    CHECK_BUFFER ();
    ASSERT (n <= IOSize (m_limit - m_end));
    m_end += n;
}

/** Mark @a n bytes of the buffer used as if #get() was called.  This
    method should be used by clients copying data directly from the
    buffer: instead of calling #get() they should call this method
    always immediately after accessing the buffer contents.  */
void
ReadBuffer::skip (IOSize n)
{
    CHECK_BUFFER ();
    ASSERT (n <= IOSize (m_end - m_current));
    m_current += n;

    // If we passed all pushed-back data, deactivate push-back pointer
    if (m_pback && m_current >= m_pback)
	m_pback = 0;

    // If we have exhausted all data and reached reference point,
    // warp pointers back to the beginning.  This enables client
    // to use buffer space better.
    if (m_current == m_end)
	m_current = m_end = m_buffer;
}

/** Get @a n bytes of data from buffer into @a into.  */
void
ReadBuffer::get (void *into, IOSize n)
{
    CHECK_BUFFER ();
    ASSERT (n <= IOSize (m_end - m_current));

    // Copy data and update current position
    memcpy (into, m_current, n);
    skip (n);
}

/** Unread data back into the buffer.  Subsequent get operations
    will first read the bytes from the push-back area in the order
    given: the first of @a data will be the first byte to re-get.  */
void
ReadBuffer::unget (const void *data, IOSize n)
{
    CHECK_BUFFER ();

    if (IOSize (m_current - m_buffer) < n)
    {
	if (IOSize (m_limit - m_end) < n)
	{
	    // There's room at the end, move the data further up in the
	    // buffer to make up space for push-back area at the start.
	    IOSize distance = m_limit - m_end;
	    memmove (m_current + distance, m_current, m_end - m_current);
	    m_current	+= distance;
	    m_end	+= distance;
	    if (m_pback) m_pback += distance;
	}
	else
	{
	    // There neither room in the beginning nor at the end.  Make
	    // the buffer larger, round the new size up to the next power
	    // of two.  Copy current buffer to the new one's tail end.
	    IOSize	oldsize = m_limit - m_buffer;
	    IOSize	oldused = m_end - m_current;
	    ptrdiff_t	oldpback = m_pback ? m_pback - m_current : 0;
	    IOSize	newsize = BitOps<IOSize>::ceil2 (oldsize + n);
	    unsigned char *buf = new unsigned char [newsize];

	    ASSERT (newsize > oldsize);
	    memcpy (buf + newsize - oldused, m_current, oldused);
	    delete [] m_buffer;

	    m_buffer	= buf;
	    m_current	= buf + newsize - oldused;
	    m_end	= buf + newsize;
	    m_limit	= buf + newsize;
	    if (m_pback) m_pback = m_current + oldpback;
	}
    }

    // There's now enough room in the buffer.  Copy the data into the
    // front end of it.  Ensure again our buffer bounds look valid.
    CHECK_BUFFER ();
    ASSERT (IOSize (m_current - m_buffer) >= n);
    if (! m_pback)
	m_pback = m_current;
    m_current -= n;
    memcpy (m_current, data, n);
}

} // namespace lat
