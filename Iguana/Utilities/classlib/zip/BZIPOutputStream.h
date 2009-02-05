#ifndef CLASSLIB_BZIP_OUTPUT_STREAM_H
# define CLASSLIB_BZIP_OUTPUT_STREAM_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/zip/CompressOutputStream.h"
# include "classlib/zip/BZConstants.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class BZCompressor;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** A stream filter to write BZLIB-compressed data.  */
class BZIPOutputStream : public CompressOutputStream, public BZConstants
{
public:
    BZIPOutputStream (OutputStream	*output,
		      BZCompressor	*engine = 0,
		      IOSize		bufsize = DEFAULT_BUF_SIZE);
    // implicit destructor
    // no copy constructor
    // no assignment operator
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_BZIP_OUTPUT_STREAM_H
