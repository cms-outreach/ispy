//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/zip/GZIPOutputStream.h"
#include "classlib/zip/Compressor.h"
#include "classlib/utils/Time.h"
#include "classlib/utils/DebugAids.h"
#include <zlib.h>

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>

// FIXME: Replicated definitions from zutil.h which we can't include
#if defined MSDOS
# define OS_CODE 0x00
#elif defined OS2
# define OS_CODE 0x06
#elif defined WIN32
# define OS_CODE 0x0b
#elif defined VAXC || defined VMS
# define OS_CODE 0x02
#elif defined AMIGA
# define OS_CODE 0x01
#elif defined ATARI || defined atarist
# define OS_CODE 0x05
#elif defined MACOS || defined TARGET_OS_MAC
# define OS_CODE 0x07
#elif defined __50SERIES // Prime/PRIMOS
# define OS_CODE 0x0f
#elif defined TOPS20
# define OS_CODE 0x0a
#else // default: unix
# define OS_CODE 0x03
#endif

//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

GZIPOutputStream::GZIPOutputStream (OutputStream *output,
				    ZCompressor  *engine /* = 0 */,
				    IOSize	 bufsize /* = DEFAULT_BUF_SIZE */)
    : ZOutputStream (output, engine, bufsize)
{
    // Write out the GZIP header.  FIXME: We should ask the compressor
    // for its settings and not cheat here by "knowing" what our base
    // class is doing!  FIXME: allow caller to set time!  FIXME: set
    // extra flags = 2 if max compression, = 4 if fastest algorithm
    unsigned char buf [GZHDR_SIZE];
    put (buf + GZHDR_MAGIC_OFF,  GZHDR_MAGIC_LEN,  GZIP_MAGIC);
    put (buf + GZHDR_METHOD_OFF, GZHDR_METHOD_LEN, Z_DEFLATED);
    put (buf + GZHDR_FLAGS_OFF,  GZHDR_FLAGS_LEN,  0);
    put (buf + GZHDR_TIME_OFF,   GZHDR_TIME_LEN,   (Time::current().ns ()
						    / TimeConst::SEC_NSECS));
    put (buf + GZHDR_XFLAGS_OFF, GZHDR_XFLAGS_LEN, 0);
    put (buf + GZHDR_OS_OFF,     GZHDR_OS_LEN,     OS_CODE);
    output->xwrite (buf, sizeof (buf));
}

void
GZIPOutputStream::finish (void)
{
    if (m_finished)
	return;

    // Flush compressor
    ZOutputStream::finish ();

    // Write out the GZIP trailer.
    unsigned char buf [8];
    put (buf + 0, 4, checksum ());
    put (buf + 4, 4, m_compressor->in ());
    m_output->xwrite (buf, sizeof (buf));
    ASSERT (m_finished); // FIXME: ok to rely on CompressOutputStream?
}

} // namespace lat
