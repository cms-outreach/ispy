//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/zip/GZIPInputStream.h"
#include "classlib/zip/Decompressor.h"
#include "classlib/zip/ZError.h"
#include "classlib/zip/CRC32.h"
#include "classlib/utils/DebugAids.h"
#include <zlib.h>
#include <algorithm>
#include <cstdlib>

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

/** FIXME (FIXME: Note that we install our own CRC-32 checksum since
    that's what GZIP stream is checked against, not the Adler-32 from
    ZLIB.) */
GZIPInputStream::GZIPInputStream (InputStream	*input,
				  ZDecompressor	*engine /* = 0 */,
				  IOSize	bufsize /* = DEFAULT_BUF_SIZE */)
    : ZInputStream (input, engine, bufsize),
      m_eof (false)
{
    unsigned char	buf [GZHDR_SIZE];
    int			ch;

    // Read off the GZIP header and check the magic number.
    if (! extract (input, buf, GZHDR_SIZE, GZHDR_SIZE)
	|| get (buf, GZHDR_MAGIC_LEN) != GZIP_MAGIC)
	throw ZError (Z_DATA_ERROR);

    // Check that the method is deflate
    unsigned method = buf[GZHDR_METHOD_OFF];
    if (method != Z_DEFLATED)
	throw ZError (Z_DATA_ERROR);

    // Check that no flags unknown to us are set.
    unsigned flags = buf[GZHDR_FLAGS_OFF];
    if (flags & GZHDR_FLAG_RESERVED)
	throw ZError (Z_DATA_ERROR);

    // Read optional extra field (2-byte length and data).
    if (flags & GZHDR_FLAG_EXTRA_FIELD
	&& (! extract (input, buf, GZHDR_SIZE, 2)
	    || ! extract (input, buf, GZHDR_SIZE, buf [0] + (buf [1] << 8))))
	throw ZError (Z_DATA_ERROR);

    // Read optional null-terminated original file name.
    if (flags & GZHDR_FLAG_ORIG_NAME)
	while ((ch = input->read ()) != 0)
	    if (ch == -1)
		throw ZError (Z_DATA_ERROR);

    // Read optional null-terminated comment.
    if (flags & GZHDR_FLAG_COMMENT)
	while ((ch = input->read ()) != 0)
	    if (ch == -1)
		throw ZError (Z_DATA_ERROR);

    // Read optional 2-byte header crc.  FIXME: check it?  it's two least
    // signficant bytes of the CRC32 of the header up to and not including
    // the header CRC itself.
    if (flags & GZHDR_FLAG_HEAD_CRC && ! extract (input, buf, GZHDR_SIZE, 2))
	throw ZError (Z_DATA_ERROR);

    // Compressed data now begins.
}

IOSize
GZIPInputStream::read (void *into, IOSize n)
{
    n = ZInputStream::read (into, n);
    if (n || m_eof)
	return n;

    // At the end for the first time.  Read the trailing 4-byte CRC-32
    // and a 4-byte uncompressed data length and check the values.
    unsigned char buffer [8];
    if (! extract (m_input, buffer, 8, 8)
	|| get (buffer + 0, 4) != checksum ()
	|| get (buffer + 4, 4) != m_decompressor->out ())
	throw ZError (Z_DATA_ERROR);

    // Hokay, we are done (and don't want to check the values again).
    m_eof = true;
    return 0;
}

} // namespace lat
