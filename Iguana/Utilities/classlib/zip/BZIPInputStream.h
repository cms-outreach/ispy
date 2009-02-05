#ifndef CLASSLIB_BZIP_INPUT_STREAM_H
# define CLASSLIB_BZIP_INPUT_STREAM_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/zip/DecompressInputStream.h"
# include "classlib/zip/BZConstants.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class BZDecompressor;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** A stream filter to read BZLIB-compressed data.  */
class BZIPInputStream : public DecompressInputStream, public BZConstants
{
public:
    BZIPInputStream (InputStream	*input,
		     BZDecompressor	*engine = 0,
		     IOSize		bufsize = DEFAULT_BUF_SIZE);
    // implicit destructor
    // no copy constructor
    // no assignment operator
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_BZIP_INPUT_STREAM_H
