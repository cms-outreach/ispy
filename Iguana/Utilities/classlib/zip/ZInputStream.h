#ifndef CLASSLIB_Z_INPUT_STREAM_H
# define CLASSLIB_Z_INPUT_STREAM_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/zip/DecompressInputStream.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class ZDecompressor;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Decompress input data using the ZLIB algorithm.  */
class ZInputStream : public DecompressInputStream
{
public:
    ZInputStream (InputStream	*output,
		  ZDecompressor	*engine = 0,
		  IOSize	bufsize = DEFAULT_BUF_SIZE);
    ZInputStream (InputStream	*output,
		  ZDecompressor	*engine,
		  Checksum	*checksum,
		  IOSize	bufsize = DEFAULT_BUF_SIZE);
    // implicit destructor
    // implicit copy constructor (= none)
    // implicit assignment operator (= none)
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_Z_INPUT_STREAM_H
