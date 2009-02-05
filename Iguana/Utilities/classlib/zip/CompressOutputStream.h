#ifndef CLASSLIB_COMPRESS_OUTPUT_STREAM_H
# define CLASSLIB_COMPRESS_OUTPUT_STREAM_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"
# include "classlib/iotools/FilterOutputStream.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class Compressor;
class Checksum;
class OutputStream;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** A stream filter to compress the output data.

    This class is a filtering stream that compresses the output sent
    to it using the specified compression engine.  You will probably
    want to refer to the inherited classes for implementations with
    specific compression engines.

    Objects of this class have an internal buffer in which they buffer
    the compressed output before writing it out downstream.  It has
    nothing to do with the compression level or the buffers internal
    to the compression engine -- it is merely used to collect output
    from the compressor before writing it out downstream.  Thus its
    size does not affect the quality of the compression, only the
    efficiency of the output operations.  Choose the buffer size
    accordingly.

    Writes to this stream block: each write loops pushing input data
    into the compressor and compressed data downstream until all the
    provided input has been processed.  Depending on the compressor
    engine and the provided input, large amounts of input -- hundreds
    of kilobytes -- may be consumed before any output is generated.
    On the other hand, a single byte of input may in the end trigger
    the output of large amounts of compressed data.  This class is
    therefore not suited for interactive use and cannot be used with
    non-blocking output streams; @InteractiveCompressor provides a
    replacement designed to work in such conditions.  */
class CompressOutputStream : public FilterOutputStream
{
public:
    enum { DEFAULT_BUF_SIZE = 4096 };

    CompressOutputStream (Compressor	*compressor,
			  Checksum	*checksum,
			  OutputStream	*output,
			  IOSize	bufsize = DEFAULT_BUF_SIZE);
    ~CompressOutputStream (void);

    using FilterOutputStream::write;
    virtual IOSize	write (const void *from, IOSize n);
    virtual void	flush (void);
    virtual void	finish (void);
    virtual void	close (void);

    virtual unsigned	checksum (void) const;

protected:
    virtual void	compress (void);
    virtual void	restart (void);

    Compressor		*m_compressor;	//< Compression engine
    Checksum		*m_checksum;	//< Optional checksum accumulator
    unsigned char	*m_buffer;	//< Compression buffer
    IOSize		m_size;		//< Compression buffer size
    bool		m_finished;	//< Inidicates that #finish() was done

private:
    // undefined, no semantics
    CompressOutputStream (const CompressOutputStream &);
    CompressOutputStream &operator= (const CompressOutputStream &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_COMPRESS_OUTPUT_STREAM_H
