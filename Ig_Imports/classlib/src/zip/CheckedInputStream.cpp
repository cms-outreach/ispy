//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/zip/CheckedInputStream.h"
#include "classlib/zip/Checksum.h"
#include "classlib/utils/DebugAids.h"
#include <algorithm>

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

// FIXME: Mark support not available.  Possible only if underlying
// stream supports it and we were able to reset checksum to a previous
// value (now can only reset to zero).

/** FIXME.  FIXME: Takes ownership of input stream and checksum. */
CheckedInputStream::CheckedInputStream (InputStream *input,Checksum *validator)
    : FilterInputStream (input),
      m_checksum (validator)
{
    ASSERT (m_input);
    ASSERT (m_checksum);
}

/** FIXME */
CheckedInputStream::~CheckedInputStream (void)
{
    ASSERT (m_input);
    ASSERT (m_checksum);
    delete m_checksum;
}

/** FIXME */
Checksum *
CheckedInputStream::checksum (void) const
{
    ASSERT (m_checksum);
    return m_checksum;
}

/** FIXME */
IOSize
CheckedInputStream::read (void *into, IOSize n)
{
    ASSERT (m_input);
    ASSERT (m_checksum);
    ASSERT (into);

    n = m_input->read (into, n);
    m_checksum->update (into, n);
    return n;
}

/** FIXME */
IOSize
CheckedInputStream::skip (IOSize n)
{
    // Use a local buffer to avoid reading single characters
    ASSERT (m_input);
    ASSERT (m_checksum);
    char	buf [SKIP_BUFFER_SIZE];
    IOSize	total = 0;

    for (IOSize m = 1; m && n > 0; n -= m, total += m)
    {
	m = m_input->read (buf, std::min (n, IOSize (sizeof (buf))));
	m_checksum->update (buf, m);
    }

    return total;
}

/** FIXME */
void
CheckedInputStream::reset (void)
{
    ASSERT (m_input);
    m_input->reset ();
    m_checksum->reset ();
}

} // namespace lat
