//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/zip/CompressOutputStream.h"
#include "classlib/zip/Compressor.h"
#include "classlib/zip/Checksum.h"
#include "classlib/utils/DebugAids.h"

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

/** Create a new compressing output stream using the specified
    compressor, writing the compressed data to @a output.

    @param compressor	The compression engine.

    @param checksum	Optional checksum calculator.  If given, it
			will be updated with all input data to
			write().  Otherwise checksum() defaults to
			the compressor's checksum.

    @param output	Underlying output stream into which to
    			output the compressed data.

    @param size		Size of the internal buffer into which data is
			compressed by @a compressor; see arguments to
			Compressor::compress().  Impacts compression
			efficiency as the size defines how often the
			compressor has to be called and how much data
			can be written in one go to the underlying
			stream; does not impact the quality of
			the compression however.  */
CompressOutputStream::CompressOutputStream (Compressor		*compressor,
					    Checksum		*checksum,
					    OutputStream	*output,
					    IOSize		bufsize)
    : FilterOutputStream (output),
      m_compressor (compressor),
      m_checksum (checksum),
      m_buffer (new unsigned char [bufsize]),
      m_size (bufsize),
      m_finished (false)
{
    ASSERT (m_output);
    ASSERT (m_compressor);
    ASSERT (m_size);
}

/** Destroys the stream and any system resources used by it.

    Destroying the stream does @em not close it.  You need to invoke
    close() explicitly to properly flush and close the stream.  If
    this stream is not closed, the output will probably be left in
    corrupt state as the compressor will not have been flushed.  */
CompressOutputStream::~CompressOutputStream (void)
{
    delete m_compressor;
    delete m_checksum;
    delete [] m_buffer;
}

/** Compress @a length bytes from @a buffer and write them to the
    underlying ouput stream.  */
IOSize
CompressOutputStream::write (const void *from, IOSize n)
{
    ASSERT (m_output);
    ASSERT (m_compressor);
    ASSERT (m_buffer);
    ASSERT (m_size);
    ASSERT (from || ! n);

    // We can't be at the end already and we must have already flushed
    // the compressor on the previous run.
    ASSERT (! m_finished);
    ASSERT (! m_compressor->finished ());
    ASSERT (m_compressor->more ());

    // Update checksum if one was specified.
    if (m_checksum)
	m_checksum->update (from, n);

    // Tell compressor to use this buffer as input.
    m_compressor->input (from, n);

    // Compress this input.
    compress ();

    // Make sure we've left the compressor in a sensible state
    ASSERT (m_compressor->finished () || m_compressor->more ());
    return n;
}

/** Flush any input data gobbled up in the compressor.  */
void
CompressOutputStream::flush (void)
{
    ASSERT (m_output);
    ASSERT (m_compressor);
    ASSERT (m_buffer);
    ASSERT (m_size);

    // We can't be at the end already or flushing already.
    ASSERT (! m_finished);
    ASSERT (! m_compressor->finished ());

    // Tell the compressor to flush its state.
    m_compressor->flush ();

    // Flush out the data from the compressor.
    compress ();

    // Make sure we've left the compressor in a sensible state.
    ASSERT (m_compressor->more ());
    ASSERT (! m_compressor->finished ());
    ASSERT (m_compressor->flushed ());
}

/** Finish writing compressed data to the output stream without
    closing it.

    Use this method when applying multiple successive filters to the
    same output.  */
void
CompressOutputStream::finish (void)
{
    if (m_finished)
	return;

    ASSERT (m_output);
    ASSERT (m_compressor);
    ASSERT (m_buffer);
    ASSERT (m_size);

    // This method may get called twice: once by the user and another
    // time from close().  So don't be too picky about whether we've
    // already been finished.
    if (! m_compressor->finished ())
    {
	// Tell the compressor to flush its state.
	m_compressor->finish ();

	// Flush out remaining data from the compressor.
	compress ();

	// Make sure we've left the compressor in a sensible state.
	ASSERT (m_compressor->finished ());
    }

    // Mark ourselves finished
    m_finished = true;
}

/** Write remaining compressed data to the underlyingn stream and then
    close it; also frees allocated resources.  */
void
CompressOutputStream::close (void)
{
    ASSERT (m_output);
    ASSERT (m_compressor);
    ASSERT (m_buffer);
    ASSERT (m_size);

    // Finish, then close the underlying stream.
    FilterOutputStream::close ();

    // Get rid of the compression engine and compression buffer.  This
    // could be left to the destructor, but as we are closed, there is
    // no point leaving them hanging around consuming memory.
    delete m_compressor;
    m_compressor = 0;

    delete [] m_buffer;
    m_buffer = 0;
}

/** Restart producing output after #finish() has been called.
    This allows the stream to be reused for writing multiple
    consequtive compressed streams (with possibly higher-level
    meta-data in between).  */
void
CompressOutputStream::restart (void)
{
    m_compressor->reset ();
    m_checksum->reset ();
    m_finished = false;
}

/** Process current input or flush to end of stream.

    Either the compressor input has already been set (from write()),
    or the compressor has been told to finish (from finish()) or flush
    (from flush()).  Compress all available input data and write it
    downstream.  Leave the internal buffer empty.  If flush() has not
    been called, may leave data gobbled up in the compressor.  */
void
CompressOutputStream::compress (void)
{
    // This loop may not produce output even if input was given, the
    // compressor can consume lots of input before producing output.
    // Oonce we do get output, we may sit here a while spitting it
    // out.  NB: No need to check for flushed() -- we'll stop when the
    // compressor wants more input, which is the right thing to do.
    while (! m_compressor->finished () && ! m_compressor->more ())
	if (IOSize out = m_compressor->compress (m_buffer, m_size))
	{
	    ASSERT (out <= m_size);
	    m_output->xwrite (m_buffer, out);
	}
}

/** Return the checksum of the uncompressed input data.

    If the optional checksum object was given to the constructor, its
    current value is returned.  Otherwise the checksum maintained by
    the compressor is returned.  In either case the value matches
    input given so far only after finish() or immediately after
    flush().  */
unsigned
CompressOutputStream::checksum (void) const
{
    if (m_checksum)
	return m_checksum->value ();
    else
	return m_compressor->checksum ();
}

} // namespace lat
