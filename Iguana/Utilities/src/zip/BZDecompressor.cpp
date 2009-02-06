//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/zip/BZDecompressor.h"
#include "classlib/zip/BZError.h"
#include "classlib/utils/DebugAids.h"
#include <cstring>
#define BZ_NO_STDIO
#include <bzlib.h>
#include <zlib.h>

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>

#define bzDecompressInit	BZ2_bzDecompressInit
#define bzDecompressEnd		BZ2_bzDecompressEnd
#define bzDecompress		BZ2_bzDecompress

//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>

/** Private dummy type that allows us to hide the actual stream type.  */
struct BZDecompressor::Stream : bz_stream {};

//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Create a new decompressor.

    The decompression parameters will be determined automatically from
    the input stream.  */
BZDecompressor::BZDecompressor (void)
    : m_stream (0),
      m_checksum (0),
      m_state (BZ_SEQUENCE_ERROR)
{ reset (); }

/** Close the decompressor.  */
BZDecompressor::~BZDecompressor (void)
{
    if (m_stream)
	end ();
}

/** Reset the decompression engine so that a new set of input can be
    processed.  */
void
BZDecompressor::reset (void)
{
    if (m_stream) 
	end ();

    // Set up the stream object.  (FIXME: Capture allocation functions
    // back to C++ land so that they can throw and call new_handler?
    // Is throwing through BZLIB safe?)
    ASSERT (! m_stream);
    ASSERT (m_state == BZ_SEQUENCE_ERROR);
    m_stream = new Stream;
    memset (m_stream, 0, sizeof (Stream));

    // Initialise BZLIB.
    int status = bzDecompressInit (m_stream, 0, 0);
    if (status != BZ_OK)
	throw BZError (status);

    // Initialise state machine.
    m_state = BZ_RUN_OK;

    // Reset checksum.  See note on checksum().
    m_checksum = crc32 (0, 0, 0);
}

/** Close the decompressor and discard any unprocessed input.

    This method should be called when the decompressor is no longer
    needed; it will also be called automatically by the destructor.
    Once this method has been called, the behaviour of the object
    becomes undefined (except that it is of course safe to destruct
    it).  */
void
BZDecompressor::end (void)
{
    ASSERT (m_stream);
    VERIFY (bzDecompressEnd (m_stream) == BZ_OK);
    delete m_stream;
    m_stream = 0;
    m_state = BZ_SEQUENCE_ERROR;
}

//////////////////////////////////////////////////////////////////////
/** Check if the decompressor needs more input data through input().

    Returns @c true if the decompressor should be given more input by
    calling input().  Never give the engine more input if this
    function is not first returning @c true.  */
bool
BZDecompressor::more (void) const
{
    ASSERT (m_stream);
    ASSERT (m_state != BZ_STREAM_END);

    // In addition for checking for space on the input side, check to
    // see if the last decompression filled the whole output buffer.
    // Don't request more input until we are left with space in the
    // output buffer.  This avoids us gobbling input faster than the
    // client is consuming decompressed output, and in particular, it
    // avoids overrunning input if the client consumes output slower
    // than we can be fed input.
    return (! m_stream->avail_in
	    && (! m_stream->next_out || m_stream->avail_out));
}

/** Set input data from decompression.

    This method should be called when more() returns true indicating
    that the decompressor needs more input.  It should not be called
    if unprocessed input is still left for the engine to process.
    
    Note that there is no way to tell how much more input is required
    to reach the end of the compressed data stream.  You'll just have
    to give some and then in the end ask how much was not used by the
    decompressor by calling unused().  */
void
BZDecompressor::input (const void *buffer, IOSize length)
{
    ASSERT (buffer);
    ASSERT (m_stream);
    ASSERT (m_state == BZ_RUN_OK);
    m_stream->next_in = (char *) buffer; // cast away const and to char
    m_stream->avail_in = length;
}

/** Return the number of bytes of unprocessed input still left in the
    input buffer.

    Call this method after finished() returns @c true to determine how
    much input was left in the current input buffer unprocessed by the
    decompressor.  The return value is the number of bytes left in the
    input buffer (at the end of the buffer) last given to input().  */
IOSize
BZDecompressor::rest (void) const
{
    ASSERT (m_stream);
    return m_stream->avail_in;
}

//////////////////////////////////////////////////////////////////////
/** Check to see if the end of compressed data stream has been reached.

    The decompressor determines itself when it has reached the end of
    the compressed stream.  Call this method to find out when that has
    happened; until the method returns @c true, keep on providing more
    input.  Once the method returns @c true, you can use rest() to
    determine how much data the decompressor did not use from the last
    input buffer. */
bool
BZDecompressor::finished (void) const
{
    ASSERT (m_stream);
    return m_state == BZ_STREAM_END;
}

/** Fill the given buffer with decompressed data.

    Returns the number of bytes actually written into the buffer.  If
    the return value is zero, more() should be called to determine if
    more input needs to be give.

    It is quite possible -- if not even normal -- for the decompressor
    to give no output while consuming some amounts of input, or to
    produce large amounts of output from little or no input.  It may
    also happen that the decompressor does not provide any output yet
    does not consume all available input; just call this method again
    (FIXME: a possibility of the state machine or a reality?).

    Zero return value does not necessarily mean that the produced
    output matches the input consumed so far.  This is so only when
    input has been provided up to a flush point (BZLIB block boundary)
    and all input has been used.  Otherwise input may be gobbled up
    inside the decompression engine.  (FIXME: Does BZLIB automatically
    recognise flush points (= block boundaries) on decompression and
    returns from decompress() even if more input was available?  Does
    the caller need to track of the flush points on providing input?)

    It is permitted to call this method with a null buffer, though at
    some point the decompressor will stall without output space.  The
    client can read the output in any size of chunks suitable to it,
    reading in larger chunks allows the decompressor to operate more
    efficiently.  The entire output for the current input can be read
    in one go provided the output buffer is large enough.  (FIXME: Can
    the output buffer be null?  Docs say no, code seems to indicate
    yes.)

    @param into		The buffer to decompress into.
    @param length	The size of the buffer; at most this
    			many bytes will be written to @a into.

    @return The number of bytes of data written to @a into; if zero,
	    call more() to find out if more input data is needed, or
	    finished() to find out if the end of input stream has been
	    reached.

    @throw  BZError if the input data is corrupted, not enough memory
            is available for decompression (the memory required can
            only be determined from the data), or in case of an
            internal error such as incorrect state machine state or
            corrupted stream object.  */
IOSize
BZDecompressor::decompress (void *into, IOSize length)
{
    ASSERT (m_stream);
    ASSERT (! length || into);

    // If the output buffer is empty and we have no input, do nothing.
    // This guard avoids clients having to special case reads into
    // zero size output buffer, allowing them to use more flexible
    // constructs (BZLIB generally requires some output area).
    if (! length && ! m_stream->avail_in)
	return 0;

    // Compress some data
    m_stream->next_out = (char *) into; // cast to char
    m_stream->avail_out = length;
    switch (int status = bzDecompress (m_stream))
    {
    case BZ_OK:
	// Action completed, no change
	ASSERT (m_state == BZ_RUN_OK);
	break;

    case BZ_STREAM_END:
	// End of stream
	m_state = BZ_STREAM_END;
	break;

    default:
	// Oops.
	throw BZError (status);
    }

    // Update checksum.  See note on checksum().
    m_checksum = crc32 (m_checksum, (const unsigned char *) into,
			length - m_stream->avail_out);

    // Return the amount of compressed data produced; if zero and
    // !`finished()', we should be given more input data (see also
    // `more()').
    return length - m_stream->avail_out;
}

//////////////////////////////////////////////////////////////////////
/** Return the CRC-32 checksum of uncompressed output data.

    The checksum is in general not synchronised with the input.  The
    value is correct for the output() received so far and matches the
    input only at the end of decompression or after a flush point has
    been received.  (The client needs to know where the flush points
    are however -- the decompressor does not care and cannot tell.
    See note on decompress() for more details.)

    @note BZLIB maintains a checksum internally, stores it in the
    compressed output and automatically checks it on decompression.
    The checksum maintained here is merely a convenience should the
    client need it for any other purpose.  If the input stream has
    wrong checksum (or in fact any other format error), decompress()
    will throw a BZError.  */
unsigned
BZDecompressor::checksum (void) const
{ return m_checksum; }

/** Return the number of bytes of compressed input processed.  */
IOOffset
BZDecompressor::in (void) const
{
    ASSERT (m_stream);
    return ((IOOffset) m_stream->total_in_hi32 << 32)
	+ m_stream->total_in_lo32;
}

/** Return the number of bytes of uncompressed output produced.  */
IOOffset
BZDecompressor::out (void) const
{
    ASSERT (m_stream);
    return ((IOOffset) m_stream->total_out_hi32 << 32)
	+ m_stream->total_out_lo32;
}

} // namespace lat
