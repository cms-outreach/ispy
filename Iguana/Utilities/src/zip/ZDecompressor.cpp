//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/zip/ZDecompressor.h"
#include "classlib/zip/ZError.h"
#include "classlib/utils/DebugAids.h"
#include <cstring>
#include <zlib.h>

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>

/** Private dummy type that allows us to hide the actual stream type.  */
struct ZDecompressor::Stream : z_stream {};

//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Create a new decompressor.

    The decompression parameters will be determined automatically from
    the input stream.  If @a wrap, the stream is expected to have ZLIB
    wrapper; otherwise it is expected to be raw compressed data.  */
ZDecompressor::ZDecompressor (bool wrap /* = false */)
    : m_stream (0),
      m_state (Z_STREAM_ERROR),
      m_wrap (wrap)
{ reset (); }

/** Close the decompressor.  */
ZDecompressor::~ZDecompressor (void)
{
    if (m_stream)
	end ();
}

/** Reset the decompression engine so that a new set of input can be
    processed.  */
void
ZDecompressor::reset (void)
{
    if (m_stream) 
	end ();

    // Set up the stream object.  (FIXME: Capture allocation functions
    // back to C++ land so that they can throw and call new_handler?
    // Is throwing through ZLIB safe?)
    ASSERT (! m_stream);
    ASSERT (m_state == Z_STREAM_ERROR);
    m_stream = new Stream;
    memset (m_stream, 0, sizeof (Stream));

    // Initialise ZLIB.
    int status = m_wrap ? inflateInit (m_stream)
		 : inflateInit2 (m_stream, -MAX_WBITS);
    if (status != Z_OK)
	throw ZError (status, m_stream->msg);

    // Initialise state machine.
    m_state = Z_OK;
}

/** Close the decompressor and discard any unprocessed input.

    This method should be called when the decompressor is no longer
    needed; it will also be called automatically by the destructor.
    Once this method has been called, the behaviour of the object
    becomes undefined (except that it is of course safe to destruct
    it).  */
void
ZDecompressor::end (void)
{
    // This eats the possible error produced by ZLIB.  This method is
    // called from the destructor, so it is not possible to throw an
    // exception from here.  At any rate the possible error code is
    // Z_STREAM_ERROR (m_stream was inconsistent) which Just Shouldn't
    // Happen.
    ASSERT (m_stream);
    inflateEnd (m_stream);
    delete m_stream;
    m_stream = 0;
    m_state = Z_STREAM_ERROR;
}

//////////////////////////////////////////////////////////////////////
/** Check if the decompressor needs more input data through input().

    Returns @c true if the decompressor should be given more input by
    calling input().  Never give the engine more input if this
    function is not first returning @c true.  */
bool
ZDecompressor::more (void) const
{
    ASSERT (m_stream);
    ASSERT (m_state != Z_STREAM_END);

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
ZDecompressor::input (const void *buffer, IOSize length)
{
    ASSERT (buffer);
    ASSERT (m_stream);
    ASSERT (m_state == Z_OK || m_state == Z_DATA_ERROR);
    m_stream->next_in = (unsigned char *) buffer; // cast away const, to char
    m_stream->avail_in = length;
}

/** Test if a preset dictionary is required to decompress the current
    input data.

    If the first call to decompress() returns with zero, call this
    method to determine if a preset dictionary is required.  If so,
    call dictionary() to set the same preset dictionary that was used
    to compress the input data; checksum() will return the Adler-32
    checksum of the required dictionary.

    @return @c true if a preset dictionary is required at this time.  */
bool
ZDecompressor::needsDictionary (void) const
{
    ASSERT (m_stream);
    return m_state == Z_NEED_DICT;
}

/** Initialise the decompression dictionary for the current input.

    This method must be called immediately after decompress() returned
    zero and needsDictionary() is @c true.  The dictionary used by the
    compressor can be determined from the Adler-32 value returned by
    checksum().  The compressor and decompressor must use the exact
    same dictionary. */
void
ZDecompressor::dictionary (void *buf, IOSize length)
{
    ASSERT (m_stream);
    int status;
    unsigned char *buffer = static_cast<unsigned char *> (buf);
    if ((status = inflateSetDictionary (m_stream, buffer, length)) != Z_OK)
	throw ZError (status, m_stream->msg);
}

/** Return the number of bytes of unprocessed input still left in the
    input buffer.

    Call this method after finished() returns @c true to determine how
    much input was left in the current input buffer unprocessed by the
    decompressor.  The return value is the number of bytes left in the
    input buffer (at the end of the buffer) last given to input().  */
IOSize
ZDecompressor::rest (void) const
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
ZDecompressor::finished (void) const
{
    ASSERT (m_stream);
    return m_state == Z_STREAM_END;
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
    input has been provided up to a flush point and all input has been
    used.  Otherwise input may be gobbled up inside the decompression
    engine.

    It is permitted to call this method with a null buffer, though at
    some point the decompressor will stall without output space.  The
    client can read the output in any size of chunks suitable to it,
    reading in larger chunks allows the decompressor to operate more
    efficiently.  The entire output for the current input can be read
    in one go provided the output buffer is large enough.  (FIXME: Can
    the output buffer be null?  Docs say no, code seems to indicate
    yes.)

    If a preset dictionary is needed at this point, the method returns
    zero, needsDictionary() will return @c true and checksum() will
    return the Adler-32 value of the required dictionary.  The preset
    dictionary used for compression of this data should then be given
    to the decompressor with dictionary(), and decompress() called
    again with the same set of input.

    @param into		The buffer to decompress into.
    @param length	The size of the buffer; at most this
    			many bytes will be written to @a into.

    @return The number of bytes of data written to @a into; if zero,
	    call more() to find out if more input data is needed, or
	    finished() to find out if the end of input stream has been
	    reached.

    @throw  ZError if the input data is corrupted, not enough memory
            is available for decompression (the memory required can
            only be determined from the data), or in case of an
            internal error such as incorrect state machine state or
            corrupted stream object.  If input data is corrupted, the
            client can call sync() until the invalid data has been
            skipped and a good compression block is found again.  */
IOSize
ZDecompressor::decompress (void *into, IOSize length)
{
    ASSERT (m_stream);
    ASSERT (! length || into);

    // Compress some data
    m_stream->next_out = (unsigned char *) into;
    m_stream->avail_out = length;
    switch (m_state = inflate (m_stream, 0))
    {
    case Z_OK:		// Action completed, no change
    case Z_STREAM_END:	// End of stream
	break;

    case Z_NEED_DICT:	// Need a dictionary().
	return 0;

    case Z_BUF_ERROR:
	// No progress was possible due to lack of buffer space.  This
	// is caused mainly because our input optimisation logic in
	// #input() tends to withhold too much data from ZLIB.  (This
	// may happen also if `length' is zero, but that is trivial.)
	// Return zero to our caller to indicate that we need more
	// input data or a proper output buffer.
	m_state = Z_OK;
	m_stream->avail_out = 1;
	return 0;

    default:
	// Oops.  Z_STREAM_ERROR/Z_MEM_ERROR or something unknown.
	throw ZError (m_state, m_stream->msg);
    }

    // Return the amount of compressed data produced; if zero and
    // !`finished()', we should be given more input data (see also
    // `more()').
    return length - m_stream->avail_out;
}

/** Skip corrupt input data and look for a good compression block.

    If decompress() fails by throwing an exception indicating corrupt
    input data (error code Z_DATA_ERROR), the client may call this
    method to look for a good compression block.  A good block will
    begin at a full flush point created with ZCompressor::flush().

    @return The method returns @c true if a flush point has been
            found.  In that case the caller may record the current
            value of in() which indicates where valid compressed data
            was found.  If the opposite case, more input can be
            provided with input() until success or end of input data.  */
bool
ZDecompressor::sync (void)
{
    ASSERT (m_stream);
    ASSERT (m_state == Z_DATA_ERROR);
    int status;
    if ((status = inflateSync (m_stream)) == Z_OK)
	return true;
    else if (status == Z_BUF_ERROR || status == Z_DATA_ERROR)
	return false;
    else // Z_STREAM_ERROR
	throw ZError (status, m_stream->msg);
}

//////////////////////////////////////////////////////////////////////
/** Return the Adler-32 checksum of uncompressed output data or the
    required preset dictionary if first call to decompress() returned
    with zero and needsDictionary() is @c true.

    The checksum is in general not synchronised with the input.  The
    value is correct for the output() received so far and matches the
    input only at the end of decompression or after a flush point has
    been received.  (The client needs to know where the flush points
    are however -- the decompressor does not care and cannot tell.
    See note on decompress() for more details.)

    @note This is the ZLIB-maintained Adler-32 checksum.  If you need
    CRC-32, you'll have to maintain it yourself.  */
unsigned
ZDecompressor::checksum (void) const
{ ASSERT (m_stream); return m_stream->adler; }

/** Return the number of bytes of compressed input processed.  */
IOOffset
ZDecompressor::in (void) const
{ ASSERT (m_stream); return m_stream->total_in; }

/** Return the number of bytes of uncompressed output produced.  */
IOOffset
ZDecompressor::out (void) const
{ ASSERT (m_stream); return m_stream->total_out; }

} // namespace lat
