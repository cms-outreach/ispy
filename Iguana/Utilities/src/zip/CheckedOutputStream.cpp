//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/zip/CheckedOutputStream.h"
#include "classlib/zip/Checksum.h"
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

/** FIXME.  FIXME takes ownership of output and validator.  */
CheckedOutputStream::CheckedOutputStream (OutputStream *output,
					  Checksum *validator)
    : FilterOutputStream (output),
      m_checksum (validator)
{
    ASSERT (m_output);
    ASSERT (m_checksum);
}

/** FIXME */
CheckedOutputStream::~CheckedOutputStream (void)
{
    ASSERT (m_output);
    ASSERT (m_checksum);
    delete m_checksum;
}

/** FIXME */
Checksum *
CheckedOutputStream::checksum (void) const
{
    ASSERT (m_checksum);
    return m_checksum;
}

/** FIXME */
IOSize
CheckedOutputStream::write (const void *from, IOSize n)
{
    ASSERT (m_output);
    ASSERT (m_checksum);
    ASSERT (from);

    n = m_output->write (from, n);
    m_checksum->update (from, n);
    return n;
}

} // namespace lat
