//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/zip/DecompressInputStream.h"
#include "classlib/zip/Decompressor.h"
#include "classlib/zip/Checksum.h"
#include "classlib/iotools/PushBackInputStream.h"
#include "classlib/utils/DebugAids.h"
#include <algorithm>

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Create a new decompressing input stream using the specified
    decompressor, reading compressed data from @a input.

    @param decompressor	The decompression engine.

    @param checksum	Optional checksum calculator.  If given,
    			it will be updated with all the output
			produced by read().  Otherwise checksum()
			defaults to the decompressor's checksum.

    @param input	Underlying input stream from which to
    			read compressed data.

    @param size		Size of the internal buffer into which data is
			decompressed by @decompressor; see arguments
			to Decompressor::decompress().  Impacts the
			decompression efficiency as the size defines
			how often the decompressor has to be called
			and how much data can be read in one go from
			the underlying stream.  */
DecompressInputStream::DecompressInputStream (Decompressor	*decompressor,
					      Checksum		*checksum,
					      InputStream	*input,
					      IOSize		bufsize)
    : FilterInputStream (new PushBackInputStream (input)),
      m_decompressor (decompressor),
      m_checksum (checksum),
      m_buffer (new unsigned char [bufsize]),
      m_size (bufsize),
      m_last (0)
{
    ASSERT (m_input);
    ASSERT (m_decompressor);
    ASSERT (m_size);
}

/** Destroys the stream and releases any resources used by it.  */
DecompressInputStream::~DecompressInputStream (void)
{
    delete m_decompressor;
    delete m_checksum;
    delete [] m_buffer;
}

/** Returns zero if end of compressed data has been reached, otherwise
    one if there is data available upstream.

    Clients should not assume that the return value indicates anything
    useful.  It is primarily a logical indication of whether more data
    should be available (i.e. decompressor has not yet detected the
    end of input), not that more output can actually be produced.  In
    particular, it does not indicate that reading from this stream
    would not block, nor does it indicate how much data could be
    decompressed from the input available upstream.  */
IOSize
DecompressInputStream::available (void)
{
    ASSERT (m_input);
    ASSERT (m_decompressor);
    ASSERT (m_buffer);
    ASSERT (m_size);

    // We are not actually telling if we are going to get more data
    // from upstream, only that logically there should be more data.
    // Check anyway if the upstream has data to have some truth.
    return ! m_decompressor->finished () && m_input->available ();
}

/** Read and decompress data from upstream into @a buffer of size
    @a length.

    Blocks until more input can be read from upstream and at least
    some decompressed data can be placed into @a buffer.  May not fill
    the full buffer however.  Please note that read may block even if
    input available on the underlying stream: the decompressor may not
    make any output available from that input and this method waits
    until it can make at least some output available.

    @return The number of bytes of uncompressed data placed into @a
    buffer.  */
IOSize
DecompressInputStream::read (void *into, IOSize n)
{
    ASSERT (m_input);
    ASSERT (m_decompressor);
    ASSERT (m_buffer);
    ASSERT (m_size);
    ASSERT (into);

    // If we are already at the end, return zero to indicate end.
    if (m_decompressor->finished ())
	return 0;

    // Decompress until we get something out.
    IOSize out = 0;
    while (! out && ! m_decompressor->finished ())
    {
	// Read some more input if the decompressor needs it
	if (m_decompressor->more ())
	{
	    // Gotta have more input.
	    m_last = fill (m_buffer, m_size);
	    ASSERT (m_last && m_last <= m_size);
	    m_decompressor->input (m_buffer, m_last);
	}

	// Try to get something out.  This may return zero if we've
	// reached the end of the compressed stream (even if we read
	// some more input!).
	out = m_decompressor->decompress (into, n);
    }

    // If the decompressor finished, push unused input back upstream.
    if (m_decompressor->finished ())
	unread ();

    // Update checksum if that was requested
    if (out && m_checksum)
	m_checksum->update (into, out);

    // Return how much decompressed output is now in buffer.
    return out;
}

/** Skip n bytes of decompressed data.

    Returns the number of bytes really skipped.  This can be less than
    @a n if the end of the compressed input stream is discovered.  */
IOSize
DecompressInputStream::skip (IOSize n)
{
    ASSERT (m_input);
    ASSERT (m_decompressor);
    ASSERT (m_buffer);
    ASSERT (m_size);

    // Read data into a local temporary buffer until we've read
    // as much as requested or we hit the end.
    unsigned char	buf [SKIP_BUFFER_SIZE];
    IOSize		left = n;

    while (left && ! m_decompressor->finished ())
	left -= read (buf, std::min (left, IOSize (sizeof (buf))));

    // If the decompressor finished, push unused input back upstream.
    if (m_decompressor->finished ())
	unread ();

    // Return the number of bytes actually read
    return n - left;
}

/** Unread the extra input read for the decompressor.  Call this
    method once the decompressor is finished.  */
void
DecompressInputStream::unread (void)
{
    // If the decompressor finished, push unused data back upstream
    ASSERT (m_decompressor->finished ());
    ASSERT (dynamic_cast<PushBackInputStream *> (m_input));
    ASSERT (m_buffer);
    ASSERT (m_size);

    IOSize length = m_decompressor->rest ();
    static_cast<PushBackInputStream *> (m_input)
	->unread (m_buffer + m_last - length, length);
    m_last = 0;
}

/** Close the input stream and release resources allocated.  */
void
DecompressInputStream::close (void)
{
    FilterInputStream::close ();
    delete m_decompressor;
    m_decompressor = 0;
}

/** Give decompressor more data from upstream by filling @a buffer
    with at most @a length bytes of input; return to the number of
    bytes actually read from upstream.  */
IOSize
DecompressInputStream::fill (void *buffer, IOSize length)
{
    // The decompressor needs more input.  Get some.
    return m_input->read (buffer, length);
}

/** Return the checksum of the uncompressed output data.

    If the optional checksum object was given to the constructor, its
    current value is returned.  Otherwise the checksum maintained by
    the decompressor is returned.  In either case the value matches
    input consumed so far only after end of the stream has been
    reached, or if a flush point has been reached.  Note however that
    this class provides no interface to determine where the flush
    points are -- the client must know that from some other source.  */
unsigned
DecompressInputStream::checksum (void) const
{
    if (m_checksum)
	return m_checksum->value ();
    else
	return m_decompressor->checksum ();
}

} // namespace lat
