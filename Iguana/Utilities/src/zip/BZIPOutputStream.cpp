//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/zip/BZIPOutputStream.h"
#include "classlib/zip/BZCompressor.h"

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

/** Create a new output stream with the specified (non-zero) buffer size.  */
BZIPOutputStream::BZIPOutputStream (OutputStream *output,
				    BZCompressor *engine /* = 0 */,
				    IOSize	 bufsize /* = DEFAULT_BUF_SIZE */)
    : CompressOutputStream (engine ? engine : new BZCompressor, 0, output,bufsize)
{
    // BZLIB writes completely wrapped data with a magic header and
    // checksum at the end.  The output of the compression program and
    // that of the bare library are the same.  Hence we do not need to
    // do write anything here before letting the base class handle the
    // output.  BZLIB will also write the checksum at the end
    // automatically.
}

} // namespace lat
