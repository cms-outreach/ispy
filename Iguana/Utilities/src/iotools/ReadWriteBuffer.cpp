//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iotools/ReadWriteBuffer.h"
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
    ASSERT (m_reference >= m_buffer);		\
    ASSERT (m_reference <= m_limit);		\
    ASSERT (! m_pback || m_pback >= m_current);	\
    ASSERT (! m_pback || m_pback <= m_end);	\
    ASSERT (! m_pback || ! m_dirty);		\
    ASSERT (! m_dirty || m_current == m_end);	\
    ASSERT (! m_dirty || m_dirty <= m_current)

//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Allocate a new read-write buffer with buffer of @a size bytes.  */
ReadWriteBuffer::ReadWriteBuffer (IOSize bufsize /* = DEFAULT_SIZE */)
    : m_buffer (new unsigned char [bufsize]),
      m_current (m_buffer),
      m_end (m_buffer),
      m_limit (m_buffer + bufsize),
      m_reference (m_buffer),
      m_pback (0),
      m_dirty (0)
{}

/** Copy a read-write buffer.  Copies all buffered data as well.  */
ReadWriteBuffer::ReadWriteBuffer (const ReadWriteBuffer &x)
    : m_buffer (0),
      m_current (0),
      m_end (0),
      m_limit (0),
      m_reference (0),
      m_pback (0),
      m_dirty (0)
{ *this = x; }

/** Copy a read-write buffer.  Copies all buffered data as well.  */
ReadWriteBuffer &
ReadWriteBuffer::operator= (const ReadWriteBuffer &x)
{
    if (this != &x)
    {
	unsigned char	*buf = new unsigned char [x.m_limit - x.m_buffer];
	delete [] m_buffer;

	m_buffer	= buf;
	m_current	= buf + (x.m_current - x.m_buffer);
        m_end		= buf + (x.m_end - x.m_buffer);
	m_limit		= buf + (x.m_limit - x.m_buffer);
	m_reference	= buf + (x.m_reference - x.m_buffer);
	m_pback		= x.m_pback ? buf + (x.m_pback - x.m_buffer) : 0;
	m_dirty		= x.m_dirty ? buf + (x.m_dirty - x.m_buffer) : 0;
	
	memcpy (m_buffer, x.m_buffer, x.m_limit - x.m_buffer);
    }
    return *this;
}

/** Deallocate the buffer.  */
ReadWriteBuffer::~ReadWriteBuffer (void)
{ CHECK_BUFFER (); delete [] m_buffer; }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Return the number of bytes left to #get() from the buffer.  If
    the buffer is #dirty(), the return value is always zero; extract
    dirty data with #take() first before filling the buffer again.  */
IOSize
ReadWriteBuffer::left (void) const
{ CHECK_BUFFER (); return m_end - m_current; }

/** Return the capacity left in the buffer.  Returns zero if no more
    data can be buffered: to #put() for writes or to #fill() for reads.  */
IOSize
ReadWriteBuffer::capacity (void) const
{ CHECK_BUFFER (); return m_limit - m_end; }

/** Return the offset to the external file location.  If the buffer
    is #dirty(), returns the offset from the external file location to
    the position at which the dirty data should be written.  Otherwise
    returns offset to the #current() position.  */
ptrdiff_t
ReadWriteBuffer::offset (void) const
{ CHECK_BUFFER (); return m_reference - (m_dirty ? m_dirty : m_current); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Get the end of the push-back data.  Returns null if #current() is
    not inside push-back area, otherwise pointer to real buffered data
    that has not been pushed back (i.e. #current() at the time data
    was first pushed back).  Resets automatically to null when all
    pushed back data has been fetched via #get().  */
unsigned char *
ReadWriteBuffer::pback (void) const
{ CHECK_BUFFER (); return m_pback; }

/** Get the current dirty location.  This indicates the position
    starting at which data has been buffered for write; the dirty data
    ends at #current().  If the buffer is not dirty, returns a null
    pointer.  If the buffer is dirty, it can not buffer for reads; all
    dirty data must be extracted with #take() (and synchronised with
    external storage) before using the buffer for reading again.  */
unsigned char *
ReadWriteBuffer::dirty (void) const
{ CHECK_BUFFER (); return m_dirty; }

/** Get the start of the allocated area.  */
unsigned char *
ReadWriteBuffer::buffer (void) const
{ CHECK_BUFFER (); return m_buffer; }

/** Get the current buffer position: the location at which the next
    #get() or #put() would occur.  If in pushed-back data, #current()
    is less than #pback().  */
unsigned char *
ReadWriteBuffer::current (void) const
{ CHECK_BUFFER (); return m_current; }

/** Get the current buffer end: one byte past the last valid data in
    the buffer.  Clients reading data directly into the buffer should
    fill starting at #end() and not exceed #limit(), and should update
    the #end() with #filled().  */
unsigned char *
ReadWriteBuffer::end (void) const
{ CHECK_BUFFER (); return m_end; }

/** Get the pointer to the byte past the allocated buffer.  No buffer
    access may exceed #limit(), inclusive.  */
unsigned char *
ReadWriteBuffer::limit (void) const
{ CHECK_BUFFER (); return m_limit; }

/** Get the current reference location.  This indicates the position
    in buffer that corresponds to the external file position.  The
    value is automatically updated by #take() and #taken().  */
unsigned char *
ReadWriteBuffer::reference (void) const
{ CHECK_BUFFER (); return m_reference; }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Reset the buffer.  */
void
ReadWriteBuffer::reset (void)
{
    CHECK_BUFFER ();
    m_current = m_end = m_reference = m_buffer;
    m_dirty = m_pback = 0;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Mark @a n bytes of the buffer used as if #put() was called.  This
    method should be used by clients @em before copying data directly
    into the buffer: instead of calling #put() they should call this
    method, then immediately copy @a n bytes into #current()-@a.  Call
    this method with zero argument to prepare for a write; then use
    #capacity() to find out much data could be written in one go; it
    is guaranteed to return non-zero value.  */
void
ReadWriteBuffer::bump (IOSize n)
{
    CHECK_BUFFER ();

    // First discard push-back data.  Warp current back into pback.
    if (m_pback)
    {
	m_current = m_pback;
	m_pback = 0;
    }

    // We could reset the buffer here, but don't do that to avoid
    // having to recalculate reference offset in a way that does
    // not go outside allocated buffer bounds.  If the buffer has
    // been used for reading, reference will remain at the last
    // value for end (note that we warp end to current below, so
    // end will generally be less than reference).  However we do
    // need to cater for one case: we must leave some space in the
    // buffer for writing.  If an empty buffer was used for push
    // back, m_current == m_end == m_reference == m_limit, and in
    // that case we can safely warp the buffer.
    if (m_current == m_limit)
    {
	ASSERT (m_end == m_limit);
	ASSERT (m_reference == m_limit);
	m_current = m_end = m_reference = m_buffer;
    }

    // Now remember the start for dirty section.
    if (! m_dirty)
	m_dirty = m_current;

    // Update pointers.
    ASSERT (m_current != m_limit);
    ASSERT (n <= IOSize (m_limit - m_current));
    m_end = m_current = m_current + n;
}

/** Put @a n bytes from @a data into the buffer.  Use this method
    for write buffering; use #capacity() to determine how much data
    the buffer can store before synchronisation with external store
    is required.  Subsequent reads from the buffer must first write
    out the buffer to the external store (see class documentation
    for further details).  */
void
ReadWriteBuffer::put (const void *data, IOSize n)
{
    bump (n);
    memcpy (m_current - n, data, n);
}

/** Indicate @a n bytes has been written from the buffer at
    #dirty().  If @a n is all remaining dirty data, the buffer
    is made clean again.  */
void
ReadWriteBuffer::taken (IOSize n)
{
    CHECK_BUFFER ();
    ASSERT (m_dirty);
    ASSERT (! m_pback);
    ASSERT (n <= IOSize (m_current - m_dirty));
    m_reference = m_dirty = m_dirty + n;
    if (m_dirty == m_current)
	reset ();
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Indicate @a n bytes have been added to the buffer at #end().  */
void
ReadWriteBuffer::filled (IOSize n)
{
    CHECK_BUFFER ();
    ASSERT (! m_dirty);
    ASSERT (n <= IOSize (m_limit - m_end));
    m_reference = m_end = m_end + n;
}

/** Mark @a n bytes of the buffer used as if #get() was called.  This
    method should be used by clients copying data directly from the
    buffer: instead of calling #get() they should call this method
    always immediately after accessing the buffer contents.  */
void
ReadWriteBuffer::skip (IOSize n)
{
    CHECK_BUFFER ();
    ASSERT (! m_dirty);
    ASSERT (n <= IOSize (m_end - m_current));
    m_current += n;

    // If we passed all pushed-back data, deactivate push-back pointer
    if (m_pback && m_current >= m_pback)
	m_pback = 0;

    // If we have exhausted all data and reached reference point,
    // warp pointers back to the beginning.  This enables client
    // to use buffer space better.
    if (m_current == m_end && m_current == m_reference)
	m_current = m_end = m_reference = m_buffer;
}

/** Get @a n bytes of data from buffer into @a into.  */
void
ReadWriteBuffer::get (void *into, IOSize n)
{
    CHECK_BUFFER ();
    ASSERT (! m_dirty);
    ASSERT (n <= IOSize (m_end - m_current));

    // Copy data and update current position
    memcpy (into, m_current, n);
    skip (n);
}

/** Unread data back into the buffer.  Subsequent get operations
    will first read the bytes from the push-back area in the order
    given: the first of @a data will be the first byte to re-get.  */
void
ReadWriteBuffer::unget (const void *data, IOSize n)
{
    CHECK_BUFFER ();

    // Can't unget while write buffering.
    ASSERT (! m_dirty);

    // Since we are read buffering, reference point should always be
    // at the end of the cached region.  Knowing this avoids logic
    // below to keep both reference and end inside buffer bounds.
    ASSERT (m_reference == m_end);

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
	    m_reference	+= distance;
	    if (m_pback) m_pback += distance;
	}
	else
	{
	    // There neither room in the beginning nor at the end.  Make
	    // the buffer larger, round the new size up to the next power
	    // of two.  Copy current buffer to the new one's tail end.
	    IOSize	oldsize = m_limit - m_buffer;
	    IOSize	oldused = m_end - m_current;
	    ptrdiff_t	oldoffset = m_reference - m_current;
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
	    m_reference	= m_current + oldoffset;
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
