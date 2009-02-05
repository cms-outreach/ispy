//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/zip/BZIPInputStream.h"
#include "classlib/zip/BZDecompressor.h"

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

/** Create a new input stream with the specified (non-zero) buffer size.  */
BZIPInputStream::BZIPInputStream (InputStream	 *input,
				  BZDecompressor *engine /* = 0 */,
				  IOSize	 bufsize /* = DEFAULT_BUF_SIZE */)
    : DecompressInputStream (engine ? engine : new BZDecompressor,0,input,bufsize)
{
    // BZLIB writes completely wrapped data with a magic header and
    // checksum at the end.  The output of the compression program and
    // that of the bare library are the same.  Hence we do not need to
    // do any processing before letting the base class handle the
    // input.  BZLIB will tell our base class whether it recognises
    // the data stream and when the end of the stream has been
    // reached, checksum included.
}

} // namespace lat
