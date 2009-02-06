//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/zip/ZCompressor.h"
#include "classlib/zip/ZError.h"
#include "classlib/utils/DebugAids.h"
#include <cstring>
#include <zlib.h>

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>

/** Private dummy type that allows us to hide the actual stream type.  */
struct ZCompressor::Stream : z_stream {};

//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Create a new compressor using the specified compression level.

    @param level	Compression level.
    @param wrap		Defines whether the output will be wrapped
    			with ZLIB headers or not.  The default is
			raw unwrapped output compatible with GZIP
			and PKZIP.

    @throw ZError if ZLIB initialisation fails.  */
ZCompressor::ZCompressor (unsigned	level /* = DEFAULT_COMPRESSION */,
			  bool		wrap /* = false */)
    : m_stream (0),
      m_level (level),
      m_strategy (Z_DEFAULT_STRATEGY),
      m_method (Z_DEFLATED),
      // Undocumented feature: setting window bits to its negative
      // value tells deflateInit2 to suppress the ZLIB headers.
      m_window (wrap ? DEFAULT_WINDOW_BITS : -DEFAULT_WINDOW_BITS),
      m_mem (DEFAULT_MEM_LEVEL),
      m_wrap (wrap),
      m_action (Z_STREAM_ERROR),
      m_state (Z_STREAM_ERROR)
{ reset (); }

/** Close the compressor.  */
ZCompressor::~ZCompressor (void)
{
    if (m_stream)
	end ();
}

/** Set current compression level.

    This function sets the current compression level to @a level.  The
    change will take effect immediately.  */
void
ZCompressor::level (unsigned level)
{
    ASSERT (level <= BEST_COMPRESSION);
    ASSERT (m_stream);
    deflateParams (m_stream, m_level = level, m_strategy);
}

/** Get current compression level.  */
unsigned
ZCompressor::level (void) const
{ return m_level; }

/** Reset the compression engine so that a new set of input can be
    processed.

    This is the only time new compression settings will take effect.

    @throw  ZError if ZLIB initialisation fails.  */
void
ZCompressor::reset (void)
{
    if (m_stream) 
	end ();

    // Set up the stream object.  (FIXME: Capture allocation functions
    // back to C++ land so that they can throw and call new_handler?
    // Is throwing through ZLIB safe?)
    ASSERT (! m_stream);
    ASSERT (m_state == Z_STREAM_ERROR);
    ASSERT (m_action == Z_STREAM_ERROR);
    m_stream = new Stream;
    memset (m_stream, 0, sizeof (Stream));

    // Initialise ZLIB.  Let ZLIB worry about argument validity.
    int status;
    if ((status = deflateInit2 (m_stream, m_level, m_method,
				m_window, m_mem, m_strategy)) != Z_OK)
	throw ZError (status, m_stream->msg);

    // Initialise state machine.
    m_state = Z_OK;
    m_action = 0;
}

/** Close the compressor and discard any unprocessed input.

    This method should be called when the compressor is no longer
    needed; it will also be called automatically by the destructor.
    Once this method has been called, the behaviour of the object
    becomes undefined (except that it is of course safe to destruct
    it).  */
void
ZCompressor::end (void)
{
    // This eats the possible error produced by ZLIB.  This method is
    // called from the destructor, so it is not possible to throw an
    // exception from here.  At any rate the possible error codes are
    // Z_DATA_ERROR (input or output was discarded) or Z_STREAM_ERROR
    // (m_stream was inconsistent).  The former we can't do much about
    // since it needs to be possible to destruct the object in the
    // middle of compression, and the latter Just Shouldn't Happen.
    ASSERT (m_stream);
    deflateEnd (m_stream);
    delete m_stream;
    m_stream = 0;

    m_action = Z_STREAM_ERROR;
    m_state = Z_STREAM_ERROR;
}

//////////////////////////////////////////////////////////////////////
/** Initialise the compression dictionary for the current input.

    This method should be called if the client wants to define a
    custom compression dictionary.  The dictionary must be set after
    initialisation but before the compression begins.  The compressor
    and decompressor must both use the exact same dictionary; see
    #ZDecompressor::needsDictionary() and #ZDecompressor::dictionary()
    for details. */
void
ZCompressor::dictionary (void *buf, IOSize length)
{
    ASSERT (m_stream);
    int status;
    unsigned char *buffer = static_cast<unsigned char *> (buf);
    if ((status = inflateSetDictionary (m_stream, buffer, length)) != Z_OK)
	throw ZError (status, m_stream->msg);
}

/** Check if the compressor needs more input data through input().

    Returns @c true if the compressor should be given more input by
    calling input().  Never give the engine more input if this
    function is not first returning @c true.  */
bool
ZCompressor::more (void) const
{
    ASSERT (m_stream);
    ASSERT (m_state != Z_STREAM_END);

    // In addition for checking for space on the input side, check to
    // see if the last compression filled the whole output buffer.
    // Don't request more input until we are left with space in the
    // output buffer.  This avoids us gobbling input faster than the
    // client is consuming compressed output, and in particular, it
    // avoids overrunning input if the client consumes output slower
    // than we can be fed input.
    return (m_action == 0
	    && ! m_stream->avail_in
	    && (! m_stream->next_out || m_stream->avail_out));
}

/** Set input data for compression.

    This method should be called when more() returns true indicating
    that the compressor needs more input.  It should not be called if
    unprocessed input is still left for the engine to process.  */
void
ZCompressor::input (const void *buffer, IOSize length)
{
    // Update input buffers.  This should be called only if we have
    // not yet been told to finish.  The buffer should be valid.
    ASSERT (buffer);
    ASSERT (m_stream);
    ASSERT (m_state == Z_OK);
    ASSERT (m_action == 0);
    m_stream->next_in = (unsigned char *) buffer; // cast away const, to char
    m_stream->avail_in = length;
}

/** Indicate that the compression should end with the current contents
    of the input buffer.

    Call this function when you have given the compressor last input
    with input().  This will tell the compressor to start flushing its
    internal state out and make all the remaining output available.  */
void
ZCompressor::finish (void)
{
    ASSERT (m_stream);
    ASSERT (m_action == 0);
    m_action = Z_FINISH;
}

/** Indicate that the output should be flushed to match to current
    contents of the input buffer.

    This method tells the compressor that it should not accept any
    more input until the internal state has been flushed such that
    output matches the currently available input.  Call flushed() to
    find out when the output is complete.  Once it returns @c true,
    more input can be provided with input() again.

    Call this function when you need a precise synchronisation point
    in the middle of the compressed output.  This can be useful for
    error control purposes for instance.  Flushing degrades the
    quality of the compression however, so use it sparingly.

    This method cannot be called after finish() or while another flush
    is active.  */
void
ZCompressor::flush (void)
{
    // FIXME: Need Z_SYNC_FLUSH support?
    ASSERT (m_stream);
    ASSERT (m_action == 0);
    ASSERT (m_state == Z_OK);
    m_action = Z_FULL_FLUSH;
}

//////////////////////////////////////////////////////////////////////
/** Check to see if the end of compressed output has been reached.  */
bool
ZCompressor::finished (void) const
{
    ASSERT (m_stream);
    return m_state == Z_STREAM_END;
}

/** Check to see if the current input has been flushed.  */
bool
ZCompressor::flushed (void) const
{
    ASSERT (m_stream);
    return m_state == Z_OK && m_action != Z_FULL_FLUSH;
}

/** Fill the given buffer with compressed data.

    Returns the number of bytes actually written into the buffer.  If
    the return value is zero, more() should be called to determine if
    more input needs to be given.

    It is quite possible -- if not even normal -- for the compressor
    to give no output while consuming large amounts of input, or to
    produce large amounts of output while consuming little or not
    input.  It may also happen that the compressor does not provide
    any output yet does not consume all available input; just call
    this method again (FIXME: a possibility of the state machine or a
    reality?).

    Zero return value does not necessarily mean that the produced
    output matches the input consumed so far.  This is so only when
    flushed() returns @c true, otherwise input may be gobbled up
    inside the compression engine.

    It is permitted to call this method with a null buffer, though at
    some point the compressor will stall without output space.  It is
    permitted to read the output in any size of chunks that suits the
    application, reading in larger chunks allows the compressor to
    operate more efficiently.  The entire output for the current input
    can be read in one operation provided the output buffer is large
    enough.

    @param into		The buffer to compress into.
    @param length	The size of the buffer; at most this
    			many bytes will be written to @a into.

    @return The number of bytes of data written to @a into; if zero,
	    call more() to find out if more input data is needed, or
	    finished() to find out if the end of input stream has been
	    reached.

    @throw  ZError in case the compressor is stalled and no progress
            is possible (error code will Z_BUF_ERROR), or in case of
	    an internal error (such as incorrect compression state
            machine state or corrupted stream).  */
IOSize
ZCompressor::compress (void *into, IOSize length)
{
    ASSERT (m_stream);
    ASSERT (! length || into);
    ASSERT (m_state == Z_OK);

    // Compress some data
    m_stream->next_out = (unsigned char *) into;
    m_stream->avail_out = length;
    switch (m_state = deflate (m_stream, m_action))
    {
    case Z_OK:
	// If the action completed, reset back to run (from flush);
	// otherwise retain it for the next time around.
	if (m_stream->avail_out != 0)
	    m_action = 0;
	break;

    case Z_STREAM_END:
	// Finish is now complete.
	ASSERT (m_action == Z_FINISH);
	m_action = Z_STREAM_ERROR;
	break;
	
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
	// Oops.  Z_STREAM_ERROR or something unknown.
	throw ZError (m_state, m_stream->msg);
    }

    // Return the amount of compressed data produced; if zero and
    // !`finished()', we should be given more input data (see also
    // `more()').
    return length - m_stream->avail_out;
}

//////////////////////////////////////////////////////////////////////
/** Return the Adler-32 checksum of uncompressed input data.

    The checksum is in general not synchronised with the output.  The
    value is correct for the input() specified so far and matches the
    output only when finished() or flushed() returns true.

    @note This is the ZLIB-maintained Adler-32 checksum.  If you want
    a CRC-32 of the input data, maintain it yourself.  */
unsigned
ZCompressor::checksum (void) const
{ ASSERT (m_stream); return m_stream->adler; }

/** Return the number of bytes of uncompressed input processed.  */
IOOffset
ZCompressor::in (void) const
{ ASSERT (m_stream); return m_stream->total_in; }

/** Return the number of bytes of compressed output produced.  */
IOOffset
ZCompressor::out (void) const
{ ASSERT (m_stream); return m_stream->total_out; }

} // namespace lat
