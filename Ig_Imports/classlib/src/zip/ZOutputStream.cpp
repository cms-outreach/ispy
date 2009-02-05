//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/zip/ZOutputStream.h"
#include "classlib/zip/ZCompressor.h"
#include "classlib/zip/CRC32.h"

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

/** FIXME */
ZOutputStream::ZOutputStream (OutputStream	*output,
			      ZCompressor	*engine /* = 0 */,
			      IOSize		bufsize /* = DEFAULT_BUF_SIZE */)
    : CompressOutputStream (engine ? engine : new ZCompressor,
			    new CRC32, output, bufsize)
{}

/** FIXME */
ZOutputStream::ZOutputStream (OutputStream	*output,
			      ZCompressor	*engine,
			      Checksum		*checksum,
			      IOSize		bufsize /* = DEFAULT_BUF_SIZE */)
    : CompressOutputStream (engine ? engine : new ZCompressor,
			    checksum ? checksum : new CRC32, output, bufsize)
{}

} // namespace lat
