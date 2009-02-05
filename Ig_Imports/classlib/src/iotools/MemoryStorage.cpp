//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iotools/MemoryStorage.h"
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

MemoryStorage::MemoryStorage (IOBuffer data /* = IOBuffer (0, 0) */)
{
    if (data.data ())
    {
	m_current = m_buffer
	      = static_cast<unsigned char *> (data.data ());
	m_limit = m_top = m_buffer + data.size ();
	m_growable = false;
    }
    else
    {
	IOSize size = data.size () ? data.size () : 32;
	m_top = m_current = m_buffer = new unsigned char [size];
	m_limit = m_buffer + size;
	m_growable = true;
    }
}

MemoryStorage::MemoryStorage (const void *data, IOSize n)
{
    // Cast away const and introduce type.  We remember the constness
    // through m_growable (FIXME: do we -- it's only extend()? should
    // we remember writeable separately and check it in write()?).
    m_current = m_buffer = (unsigned char *) data;
    m_limit = m_top = m_buffer + n;
    m_growable = false;
}

MemoryStorage::~MemoryStorage (void)
{ if (m_growable) delete [] m_buffer; }

//////////////////////////////////////////////////////////////////////
void
MemoryStorage::extend (IOSize by)
{
    // FIXME: Throw an exception?
    ASSERT (m_growable);

    // Need more space.  Double the current size or add 1.5 times the
    // requested extra whichever is larger (FIXME: provide knobs for
    // groth policy?)
    IOSize	oldsize  = m_limit - m_buffer;
    IOSize	oldalloc = m_top - m_buffer;
    IOSize	oldpos   = m_current - m_buffer;
    IOSize	newsize  = std::max (2 * oldsize, oldalloc + by + by/2);

    // Check overflow.
    ASSERT (newsize >= oldsize);
    ASSERT (newsize >= by);
    ASSERT (newsize - by >= oldalloc);

    // Get a new buffer.
    unsigned char *buf = new unsigned char [newsize];
    memcpy (buf, m_buffer, oldalloc);
    delete [] m_buffer;
    m_buffer  = buf;
    m_current = buf + oldpos;
    m_top     = buf + oldalloc;
    m_limit   = buf + newsize;

    ASSERT (m_current >= m_buffer);
    ASSERT (m_top     >= m_buffer);
    ASSERT (m_current <= m_limit);
    ASSERT (m_top     <= m_limit);
}

//////////////////////////////////////////////////////////////////////
IOSize
MemoryStorage::read (void *into, IOSize n)
{
    ASSERT (m_buffer);
    ASSERT (m_current);
    ASSERT (m_top);
    ASSERT (m_limit);
    ASSERT (m_current >= m_buffer);
    ASSERT (m_top >= m_buffer);
    ASSERT (m_current <= m_limit);
    ASSERT (m_top <= m_limit);

    // Read as much input as is available and advance the current
    // position.  If past the end, return immediate end of file.
    IOSize available = std::min (n, IOSize (m_top > m_current
					    ? m_top-m_current : 0));
    memcpy (into, m_current, available);

    m_current += available;
    ASSERT (m_current <= m_limit);

    // Return the number actually copied into the read buffer.
    return available;
}

//////////////////////////////////////////////////////////////////////
IOSize
MemoryStorage::write (const void *from, IOSize n)
{
    ASSERT (m_buffer);
    ASSERT (m_current);
    ASSERT (m_top);
    ASSERT (m_limit);
    ASSERT (m_current >= m_buffer);
    ASSERT (m_top >= m_buffer);
    ASSERT (m_current <= m_limit);
    ASSERT (m_top <= m_limit);

    // If there isn't enough space, extend the buffer
    if (n > IOSize (m_limit - m_current))
	extend (n);

    // If we are writing past the end, null out the hole.
    if (m_current > m_top)
	memset (m_top, 0, m_current - m_top);

    // Copy data here.  Move top if we wrote past it.
    ::memcpy (m_current, from, n);
    if ((m_current += n) > m_top)
	m_top = m_current;

    // Make sure we left things in a sane state.
    ASSERT (m_top     <= m_limit);
    ASSERT (m_current <= m_limit);

    // Return the number of bytes written.
    return n;
}

//////////////////////////////////////////////////////////////////////
IOOffset
MemoryStorage::size (void) const
{
    ASSERT (m_buffer);
    ASSERT (m_current);
    ASSERT (m_top);
    ASSERT (m_limit);
    ASSERT (m_current >= m_buffer);
    ASSERT (m_top >= m_buffer);
    ASSERT (m_current <= m_top);
    ASSERT (m_top <= m_limit);

    return m_top - m_buffer;
}

IOOffset
MemoryStorage::position (void) const
{
    ASSERT (m_buffer);
    ASSERT (m_current);
    ASSERT (m_top);
    ASSERT (m_limit);
    ASSERT (m_current >= m_buffer);
    ASSERT (m_top     >= m_buffer);
    ASSERT (m_current <= m_limit);
    ASSERT (m_top     <= m_limit);

    return m_current - m_buffer;
}

IOOffset
MemoryStorage::position (IOOffset offset, Relative whence /* = SET */)
{
    ASSERT (m_buffer);
    ASSERT (m_current);
    ASSERT (m_top);
    ASSERT (m_limit);
    ASSERT (m_current >= m_buffer);
    ASSERT (m_top     >= m_buffer);
    ASSERT (m_current <= m_limit);
    ASSERT (m_top     <= m_limit);

    // Calculate the new position.
    IOOffset newpos = (whence == SET ? offset
		       : whence == CURRENT ? (m_current - m_buffer) + offset
		       : (m_top - m_buffer) + offset);
    ASSERT (newpos >= 0);

    // If it is beyond the end, allocate new space.  We don't extend
    // the top yet, we only move current.  If one tries to read from
    // there, you'll get nothing (end-of-file), and if you try to
    // write, the storage will automatically be extended.
    if (newpos > m_limit - m_buffer)
	extend (newpos - (m_top - m_buffer));

    m_current = m_buffer + newpos;

    ASSERT (m_current >= m_buffer);
    ASSERT (m_current <= m_limit);

    return m_current - m_buffer;
}

void
MemoryStorage::resize (IOOffset size)
{
    ASSERT (size >= 0);
    ASSERT (m_buffer);
    ASSERT (m_current);
    ASSERT (m_top);
    ASSERT (m_limit);
    ASSERT (m_current >= m_buffer);
    ASSERT (m_top     >= m_buffer);
    ASSERT (m_current <= m_limit);
    ASSERT (m_top     <= m_limit);

    // Shrink top if it is higher than `size'.  If that leaves current
    // position beyond the top, move it to the top.  (FIXME: Shrink
    // buffer if it is left "too large"?)
    if (size < m_top - m_buffer)
    {
	m_top     = m_buffer + size;
	m_current = std::min (m_current, m_top);
    }
}

//////////////////////////////////////////////////////////////////////
/** Return the data area of this memory storage.  The caller should
    not free the memory nor modify it (use the #write() methods to
    change the memory).  */
IOBuffer
MemoryStorage::data (void) const
{ return IOBuffer (m_buffer, m_top - m_buffer); }

} // namespace lat
