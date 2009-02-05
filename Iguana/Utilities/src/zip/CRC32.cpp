//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/zip/CRC32.h"
#include "classlib/utils/DebugAids.h"
#include <zlib.h>

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

/** Initialise a new CRC-32 checksum. */
CRC32::CRC32 (void)
    : m_value (crc32 (0, 0, 0))
{}

/** Return the current checksum value.  */
unsigned
CRC32::value (void) const
{ return m_value; }

/** Update the checksum with the bytes in the specified buffer.  */
void
CRC32::update (const void *data, IOSize n)
{
    ASSERT (! n || data);
    m_value = crc32 (m_value, (const unsigned char *) data, n);
}

/** Reset the checksum to its initial value.  */
void
CRC32::reset (void)
{ m_value = crc32 (0, 0, 0); }

} // namespace lat
