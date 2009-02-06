//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/zip/BZCompressor.h"
#include "classlib/zip/BZError.h"
#include "classlib/utils/DebugAids.h"
#include <cstring>
#define BZ_NO_STDIO
#include <bzlib.h>
#include <zlib.h>

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>

#define bzCompressInit		BZ2_bzCompressInit
#define bzCompressEnd		BZ2_bzCompressEnd
#define bzCompress		BZ2_bzCompress

//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>

/** Private dummy type that allows us to hide the actual stream type.  */
struct BZCompressor::Stream : bz_stream {};

//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Create a new compressor using the specified compression level and
    work factor.

    @throw BZError if BZLIB initialisation fails.  */
BZCompressor::BZCompressor (unsigned level /* = DEFAULT_COMPRESSION */,
			    unsigned workFactor /* = DEFAULT_WORK_FACTOR */)
    : m_stream (0),
      m_level (level),
      m_workFactor (workFactor),
      m_checksum (0),
      m_action (BZ_SEQUENCE_ERROR),
      m_state (BZ_SEQUENCE_ERROR)
{ reset (); }

/** Close the compressor.  */
BZCompressor::~BZCompressor (void)
{
    if (m_stream)
	end ();
}

/** Set current compression level.

    This function sets the current compression level to @a level.  The
    setting will take effect on next call to reset().  */
void
BZCompressor::level (unsigned level)
{ ASSERT (level <= BEST_COMPRESSION); m_level = level; }

/** Get current compression level.  */
unsigned
BZCompressor::level (void) const
{ return m_level; }

/** Set current work factor.

    This function sets the current work factor to @a value.  The
    factor controls how the compression engine behaves when presented
    with worse case highly repetitive data: how much effort the engine
    should be expend before falling back on the slower but safer sort
    algorithm.  The value should be at most MAX_WORK_FACTOR.  Zero
    means using the BZLIB default value (30).  The setting will take
    effect on next call to reset().  */
void
BZCompressor::workFactor (unsigned value)
{ ASSERT (value <= MAX_WORK_FACTOR); m_workFactor = value; }

/** Get current compression work factor.  */
unsigned
BZCompressor::workFactor (void) const
{ return m_workFactor; }

/** Reset the compression engine so that a new set of input can be
    processed.

    This is the only time new compression settings will take effect.

    @throw BZError if BZLIB initialisation fails.  */
void
BZCompressor::reset (void)
{
    if (m_stream) 
	end ();

    // Set up the stream object.  (FIXME: Capture allocation functions
    // back to C++ land so that they can throw and call new_handler?
    // Is throwing through BZLIB safe?)
    ASSERT (! m_stream);
    ASSERT (m_state == BZ_SEQUENCE_ERROR);
    ASSERT (m_action == BZ_SEQUENCE_ERROR);
    m_stream = new Stream;
    memset (m_stream, 0, sizeof (Stream));

    // Initialise BZLIB.  Let BZLIB worry about argument validity.
    int status = bzCompressInit (m_stream, m_level, 0, m_workFactor);
    if (status != BZ_OK)
	throw BZError (status);

    // Initialise state machine.
    m_state = BZ_RUN_OK;
    m_action = BZ_RUN;

    // Reset checksum.  See note on checksum().
    m_checksum = crc32 (0, 0, 0);
}

/** Close the compressor and discard any unprocessed input.

    This method should be called when the compressor is no longer
    needed; it will also be called automatically by the destructor.
    Once this method has been called, the behaviour of the object
    becomes undefined (except that it is of course safe to destruct
    it).  */
void
BZCompressor::end (void)
{
    ASSERT (m_stream);
    VERIFY (bzCompressEnd (m_stream) == BZ_OK);
    delete m_stream;
    m_stream = 0;

    m_action = BZ_SEQUENCE_ERROR;
    m_state = BZ_SEQUENCE_ERROR;
}

//////////////////////////////////////////////////////////////////////
/** Check if the compressor needs more input data through input().

    Returns @c true if the compressor should be given more input by
    calling input().  Never give the engine more input if this
    function is not first returning @c true.  */
bool
BZCompressor::more (void) const
{
    ASSERT (m_stream);
    ASSERT (m_state != BZ_STREAM_END);

    // In addition for checking for space on the input side, check to
    // see if the last compression filled the whole output buffer.
    // Don't request more input until we are left with space in the
    // output buffer.  This avoids us gobbling input faster than the
    // client is consuming compressed output, and in particular, it
    // avoids overrunning input if the client consumes output slower
    // than we can be fed input.
    return (m_action == BZ_RUN
	    && ! m_stream->avail_in
	    && (! m_stream->next_out || m_stream->avail_out));
}

/** Set input data for compression.

    This method should be called when more() returns true indicating
    that the compressor needs more input.  It should not be called if
    unprocessed input is still left for the engine to process.  */
void
BZCompressor::input (const void *buffer, IOSize length)
{
    // Update input buffers.  This should be called only if we have
    // not yet been told to finish.  The buffer should be valid.
    ASSERT (buffer);
    ASSERT (m_stream);
    ASSERT (m_state == BZ_RUN_OK);
    ASSERT (m_action == BZ_RUN);
    m_stream->next_in = (char *) buffer; // cast away const and to char
    m_stream->avail_in = length;

    // Update checksum.  See note on checksum().
    m_checksum = crc32 (m_checksum, (const unsigned char *) buffer, length);
}

/** Indicate that the compression should end with the current contents
    of the input buffer.

    Call this function when you have given the compressor last input
    with input().  This will tell the compressor to start flushing its
    internal state out and make all the remaining output available.  */
void
BZCompressor::finish (void)
{
    ASSERT (m_stream);
    ASSERT (m_action == BZ_RUN);
    m_action = BZ_FINISH;
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
BZCompressor::flush (void)
{
    ASSERT (m_stream);
    ASSERT (m_action == BZ_RUN);
    ASSERT (m_state == BZ_RUN_OK);
    m_action = BZ_FLUSH;
}

//////////////////////////////////////////////////////////////////////
/** Check to see if the end of compressed output has been reached.  */
bool
BZCompressor::finished (void) const
{
    ASSERT (m_stream);
    return m_state == BZ_STREAM_END;
}

/** Check to see if the current input has been flushed.  */
bool
BZCompressor::flushed (void) const
{
    ASSERT (m_stream);
    return m_state == BZ_RUN_OK && m_action != BZ_FLUSH;
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

    @throw  BZError in case of an internal error (such as incorrect
            compression state machine state or corrupted stream).  */
IOSize
BZCompressor::compress (void *into, IOSize length)
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
    switch (m_state = bzCompress (m_stream, m_action))
    {
    case BZ_RUN_OK:
	// Action completed, reset back to run (for flush)
	m_action = BZ_RUN;
	break;

    case BZ_FLUSH_OK:
    case BZ_FINISH_OK: // FIXME: Apparently this doesn't exist: BZ_FINISHING
	// Action not yet completed, retain it
	break;

    case BZ_STREAM_END:
	// End of stream, invalidate action.
	m_action = BZ_SEQUENCE_ERROR;
	break;

    default:
	// Oops.
	throw BZError (m_state);
    }

    // Return the amount of compressed data produced; if zero and
    // !`finished()', we should be given more input data (see also
    // `more()').
    return length - m_stream->avail_out;
}

//////////////////////////////////////////////////////////////////////
/** Return the CRC-32 checksum of uncompressed input data.

    The checksum is in general not synchronised with the output.  The
    value is correct for the input() specified so far and matches the
    output only when finished() or flushed() returns true.

    @note BZLIB maintains a checksum internally, stores it in the
    compressed output and automatically checks it on decompression.
    The checksum maintained here is merely a convenience should the
    client need it for any other purpose.  */
unsigned
BZCompressor::checksum (void) const
{ return m_checksum; }

/** Return the number of bytes of uncompressed input processed.  */
IOOffset
BZCompressor::in (void) const
{
    ASSERT (m_stream);
    return ((IOOffset) m_stream->total_in_hi32 << 32)
	+ m_stream->total_in_lo32;
}

/** Return the number of bytes of compressed output produced.  */
IOOffset
BZCompressor::out (void) const
{
    ASSERT (m_stream);
    return ((IOOffset) m_stream->total_out_hi32 << 32)
	+ m_stream->total_out_lo32;
}

} // namespace lat
