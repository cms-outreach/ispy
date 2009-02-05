#ifndef CLASSLIB_DECOMPRESS_INPUT_STREAM_H
# define CLASSLIB_DECOMPRESS_INPUT_STREAM_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"
# include "classlib/iotools/FilterInputStream.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class Decompressor;
class Checksum;
class InputStream;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** A stream filter to decompress the input data.

    This class is a filtering stream that decompresses the input using
    the specified decompression engine.  Please refer to the inherited
    classes for implementations with specific decompression engines.

    Objects of this class have an internal buffer in which they buffer
    the compressed input for the decompressor to work on.  It has
    nothing to do with the compression level or the buffers internal
    to the decompression engine -- it is merely used as a working area
    for the decompressor as data is read from upstream.  Thus its size
    only affects the efficiency of the input operations and the
    decompressor call overhead (the amount of data that can be
    decompressed at a time).  Choose the buffer size accordingly.

    Reads from this stream block: each read loops reading input data
    into the decompressor until at least some output can be returned.
    Depending on the decompressor engine and the input stream, large
    amounts of input may be consumed before any output is generated.
    On the other hand, little or no input may in the end trigger the
    output of large amounts of uncompressed data.  This class is
    therefore not suited for interactive use and cannot be used with
    non-blocking output streams; #InteractiveDecompressor provides a
    class designed to work in such conditions.

    It is not possible to tell beforehand when the end of the
    compressed input stream will be reached.  As this class reads
    input from upstream channel in chunks of its internal buffer size,
    it may read up to one buffer's worth too much ahead.  The input
    that was not used is pushed back to the upstream input via an
    intermediate PushBackInputStream inserted by this class before
    the upstream input.  Derived classes should make sure to use
    #m_input, not the input stream they pass to the constructor.  */
class DecompressInputStream : public FilterInputStream
{
public:
    enum { DEFAULT_BUF_SIZE = 4096 };

    DecompressInputStream (Decompressor *decompressor,
			   Checksum	*checksum,
			   InputStream	*input,
			   IOSize	bufsize = DEFAULT_BUF_SIZE);
    ~DecompressInputStream (void);

    using FilterInputStream::read;
    virtual IOSize	available (void);
    virtual IOSize	read (void *into, IOSize n);
    virtual IOSize	skip (IOSize n);

    // virtual void	reset (void); // FIXME: reset engine, buffers?
    virtual void	close (void);

    virtual unsigned	checksum (void) const;

protected:
    virtual IOSize	fill (void *buffer, IOSize length);

    Decompressor	*m_decompressor;//< Decompression engine
    Checksum		*m_checksum;	//< Checksum of uncompressed data
    unsigned char	*m_buffer;	//< Decompression buffer
    IOSize		m_size;		//< Decompression buffer size
    IOSize		m_last;		//< Amount of input now in m_buffer

private:
    void		unread (void);

    // undefined, no semantics
    DecompressInputStream (const DecompressInputStream &);
    DecompressInputStream &operator= (const DecompressInputStream &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_DECOMPRESS_INPUT_STREAM_H
