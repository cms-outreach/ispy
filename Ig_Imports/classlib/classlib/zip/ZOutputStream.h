#ifndef CLASSLIB_Z_OUTPUT_STREAM_H
# define CLASSLIB_Z_OUTPUT_STREAM_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/zip/CompressOutputStream.h"
# include "classlib/zip/ZConstants.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class ZCompressor;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Compress output data using the ZLIB algorithm.  */
class ZOutputStream : public CompressOutputStream, public ZConstants
{
public:
    ZOutputStream (OutputStream	*output,
		   ZCompressor	*engine = 0,
		   IOSize	bufsize = DEFAULT_BUF_SIZE);
    ZOutputStream (OutputStream	*output,
		   ZCompressor	*engine,
		   Checksum	*checksum,
		   IOSize	bufsize = DEFAULT_BUF_SIZE);
    // implicit destructor
    // implicit copy constructor (= none)
    // implicit assignment operator (= none)
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_Z_OUTPUT_STREAM_H
