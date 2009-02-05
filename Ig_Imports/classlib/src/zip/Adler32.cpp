//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/zip/Adler32.h"
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

/** Initialise a new Adler-32 checksum. */
Adler32::Adler32 (void)
    : m_value (adler32 (0, 0, 0))
{}

/** Return the current checksum value.  */
unsigned
Adler32::value (void) const
{ return m_value; }

/** Update the checksum with the bytes in the specified buffer.  */
void
Adler32::update (const void *data, IOSize n)
{
    ASSERT (! n || data);
    m_value = adler32 (m_value, (const unsigned char *) data, n);
}

/** Reset the checksum to its initial value.  */
void
Adler32::reset (void)
{ m_value = adler32 (0, 0, 0); }

} // namespace lat
