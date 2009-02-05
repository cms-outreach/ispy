//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iotools/SubStorage.h"
#include "classlib/utils/DebugAids.h"

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

SubStorage::SubStorage (Storage *base, IOOffset from, IOOffset len)
    : m_base (base),
      m_bias (from),
      m_length (len),
      m_position (0)
{ ASSERT (base); }

SubStorage::~SubStorage (void)
{}

//////////////////////////////////////////////////////////////////////
IOSize
SubStorage::read (void *into, IOSize n)
{
    // Limit reads to the specified section.  Seeks can move file pointer
    // beyond end, but don't let them read beyond the set limits.
    IOSize available = std::min (n, IOSize (m_length > m_position
					    ? m_length - m_position : 0));
    IOSize did = m_base->read (into, available, m_bias + m_position);

    // Update our concept of the file pointer according the read result.
    ASSERT (did <= available);
    ASSERT ((m_position >= m_length && did == 0)
	    || IOOffset (m_position + did) <= m_length);
    m_position += did;
    return did;
}

IOSize
SubStorage::write (const void *from, IOSize n)
{
    // Limit writes to the specified section.  (FIXME: allow growth?)
    IOSize available = std::min (n, IOSize (m_length > m_position
					    ? m_length - m_position : 0));
    IOSize did = m_base->write (from, available, m_bias + m_position);

    // Update our concept of the file pointer according to the write result.
    ASSERT (did <= available);
    ASSERT ((m_position >= m_length && did == 0)
	    || IOOffset (m_position + did) <= m_length);
    m_position += did;
    return did;
}

//////////////////////////////////////////////////////////////////////
IOOffset
SubStorage::position (void) const
{ return m_position; }

IOOffset
SubStorage::position (IOOffset offset, Relative whence /* = SET */)
{
    // Update our virtual position; the underlying storage position won't
    // change.  This means that multiple SubStorage objects to the same
    // Storage don't interfere -- and don't collaborate!
    //
    // We allow the file pointer to be moved beyond the range of specified
    // for this SubStorage, but read()/write() won't allow any operations
    // beyond the given range.
    switch (whence)
    {
    case SET:		m_position = offset; break;
    case CURRENT:	m_position += offset; break;
    case END:		m_position = m_length + offset; break;
    }

    // Don't let position go negative.  If it goes beyond start, clamp
    // at start.  Move before start is non-sensical and no Storage
    // support sthat.
    if (m_position <= 0)
	m_position = 0;

    return m_position;
}

void
SubStorage::rewind (void)
{ m_position = 0; }

//////////////////////////////////////////////////////////////////////
void
SubStorage::resize (IOOffset /* size */)
{
    // Ineffective, just ignore it.  (FIXME: throw?)
}

//////////////////////////////////////////////////////////////////////
void
SubStorage::flush (void)
{
    // Well, let's flush owner...
    m_base->flush ();
}

void
SubStorage::close (void)
{
    // Ignore it.  Close the main storage if you want real close.
}

} // namespace lat
