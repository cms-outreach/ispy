#ifndef CLASSLIB_GZIP_OUTPUT_STREAM_H
# define CLASSLIB_GZIP_OUTPUT_STREAM_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/zip/ZOutputStream.h"
# include "classlib/zip/ZConstants.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class ZCompressor;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** A stream filter to write GZIP-compressed data.  */
class GZIPOutputStream : public ZOutputStream
{
public:
    GZIPOutputStream (OutputStream	*output,
		      ZCompressor	*engine = 0,
		      IOSize		bufsize = DEFAULT_BUF_SIZE);
    // implicit destructor
    // implicit copy constructor (= none)
    // implicit assignment operator (= none)

    virtual void	finish (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_GZIP_OUTPUT_STREAM_H
