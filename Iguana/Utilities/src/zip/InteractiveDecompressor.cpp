//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/zip/InteractiveDecompressor.h"
#include "classlib/zip/Decompressor.h"
#include "classlib/iotools/InputStream.h"
#include "classlib/iotools/OutputStream.h"
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

InteractiveDecompressor::InteractiveDecompressor (Decompressor	*decompressor,
						  InputStream	*input,
						  OutputStream	*output,
						  IOSize	inBufSize,
						  IOSize	outBufSize)
    : m_decompressor (decompressor),
      m_input      (input),
      m_inBuf      (new unsigned char [inBufSize]),
      m_inBufSize  (inBufSize),
      m_inputting  (false),
      m_output     (output),
      m_outBuf     (new unsigned char [outBufSize]),
      m_outBufSize (outBufSize),
      m_out        (0),
      m_bottom     (0),
      m_outputting (false)
{
    ASSERT (m_decompressor);
    ASSERT (m_inBufSize);
    ASSERT (m_outBufSize);
}

InteractiveDecompressor::~InteractiveDecompressor (void)
{
    delete m_decompressor;
    delete m_input;
    delete m_output;
    delete m_inBuf;
    delete m_outBuf;
}

/** Configure input stream.  This should be invoked before #begin()
    (or any other methods) if the input stream wasn't given or was
    null when the object was constructed.  */
void
InteractiveDecompressor::setInputStream (InputStream *input)
{
    ASSERT (! m_input);
    ASSERT (input);
    m_input = input;
}

/** Configure output stream.  This should be invoked before #begin()
    (or any other methods) if the output stream wasn't given or was
    null when the object was constructed.  */
void
InteractiveDecompressor::setOutputStream (OutputStream *output)
{
    ASSERT (! m_output);
    ASSERT (output);
    m_output = output;
}

/** Begin incremental compression.

    It is recommended that derived classes do not call this method in
    their constructors as it prevents further derived classes from
    doing preparatory work as described below.

    If the derived class needs to write out a header before the
    compressed data stream, the easiest way to do so is to format it
    into `outbuffer', update `out' and `bottom' respectively and
    (optionally) invoke `waitForOutput()' to begin flushing it out (if
    @a outputting is not set).  This will automatically cause the
    header to be written out before the compressed data.  Obviously
    the header cannot be larger than `outbuffer', but the derived
    class can choose a buffer large enough.

    Multiple headers can be written in multiple inheritance layers as
    long as `outbuffer' is large enough to contain them all, each
    class writes its own header before invoking the base class
    `begin()' method, and uses and updates the values of `out' and
    `bottom' while formatting the header.

    If `outbuffer' cannot be guaranteed to be large enough, the
    derived classes should register an output handler similar to
    `doOutput()' to write out the header, and invoke base class
    `begin()' only once the header is fully written out.  To deal with
    non-blocking output stream correctly, it is easiest to format the
    header in a buffer and write from that in an implicit loop like
    `doOutput()' does.  */
void
InteractiveDecompressor::begin (void)
{
    // Register for input.
    waitForInput ();
}

/** Register doInput() to be called whenever @m_input is readable
    (has data or is at eof = read will return zero).

    The derived class must implement this method and call the base
    class implementation.  The registration for doInput() is assumed
    to be single-shot; if not, derived class override doInput(),
    de-register doInput() and then invoke the base class method.  */
void
InteractiveDecompressor::waitForInput (void)
{ m_inputting = true; }

/** Register doOutput() to be called whenever @m_output is writable.

    The derived class must implement this method and call the base
    class implementation.  The registration for doOutput() is assumed
    to be single-shot; if not, derived class override doOutput(),
    de-register doOutput() and then invoke the base class method.  */
void
InteractiveDecompressor::waitForOutput (void)
{ m_outputting = true; }

void
InteractiveDecompressor::doInput (void)
{
    // Tell output side we are no longer pending.
    m_inputting = false;

    // Give compressor more input data.  We can't tell it to finish,
    // either there's enough data or the decompressor will be unhappy.
    // (FIXME: Check below?)
    ASSERT (! m_decompressor->finished ());
    IOSize in = m_input->read (m_inBuf, m_inBufSize);
    if (in)
        m_decompressor->input (m_inBuf, in);

    // Consume as much input as possible.  This may gobble up much
    // input before producing any output, so do not assume the latter
    // will happen but listen to input again if compressor sucked in
    // all the input.  However, if the compressor is unable to take
    // more input, post-pone waiting on input until output side has
    // freed space in the output buffer.
    decompress ();
    if (! m_decompressor->finished () && m_decompressor->more ())
    {
	if (! in)
	    // FIXME: Oops, decompressor wants more input but we are
	    // at eof.  Should throw some sort of an exception here.
	    ASSERT (false);
	else
	    waitForInput ();
    }

     // Make sure output side will wake up.
    if (/* FIXME? m_out && */ ! m_outputting)
	waitForOutput ();
}

void
InteractiveDecompressor::doOutput (void)
{
    // Tell input side we are no longer pending.
    m_outputting = false;

    // If there's compressed output in the buffer, spit it out first.
    // Continue even if we can't write it all out, we might be able
    // to compress some more input into the buffer.
    IOSize n;
    if (m_out && (n = m_output->write (m_outBuf + m_bottom, m_out)))
	m_out -= n, m_bottom += n;

    // Compress some more input.  We do not loop back up to writing
    // the result out even though the output stream might be able to
    // accept more output.  This way we never block for more than
    // `outbuffer' size quantum of work; if the output is writable,
    // we will get called again immediately.  The application will
    // however have a chance to do something else in the mean time.
    decompress ();

    // Check if we are finished; if so, signal end, otherwise make
    // ourselves called again when output becomes available.
    if (m_decompressor->finished () && ! m_out && flush ())
    {
	finish ();
	return;
    }

    if (m_out)
	waitForOutput ();

    // Register for input if we can use some more.
    if (! m_decompressor->finished ()
	&& m_decompressor->more ()
	&& ! m_inputting)
	waitForInput ();
}

void
InteractiveDecompressor::decompress (void)
{
    // Consume as much input into the output buffer as possible.  The
    // buffer might have remains of previous output, in which case
    // append.  Do nothing if the buffer is full, we need more input,
    // or the compression is already complete.

    // Reset bottom pointer if the buffer has been consumed.
    if (! m_out)
	m_bottom = 0;

    // Compress more input if we can fit it into the buffer.
    IOSize max = m_outBufSize - m_bottom;
    if (! (m_decompressor->finished() || m_decompressor->more() || m_out==max))
	m_out += m_decompressor->decompress(m_outBuf+m_bottom+m_out,max-m_out);
}

/** Flush out the decompressor output.

    This method will be called internally when all input data has been
    written into and flushed from @m_outBuf.  If the derived class
    desires to write a trailer to the output stream (such as a
    checksum), it override this method.

    On the first call the flush will begin.  If there will be no
    trailer, the method should return @c true.  Otherwise the derived
    class should format the trailer into @m_outBuf, set @m_out and
    @m_bottom accordingly, invoke waitForOutput() and return @c false.
    More calls to flush() will be made when @m_outBuf is exhausted;
    the implementation can then put more data into it and return @c
    false, or return @c true to indicate that there is no more data.

    Once flush() returns @c true, finish() will be called.  */
bool
InteractiveDecompressor::flush (void)
{ return true; }

/** Finish compressor output.

    This method will be called internally when the compression is
    complete.  The derived class can override this method to notify
    some other instance of the completion.  It should invoke the
    base class method first however.

    Note that the output stream will still be in non-blocking state so
    it probably isn't safe to write anything more to it, at least not
    without schemes like those used by doOutput() and flush().  */
void
InteractiveDecompressor::finish (void)
{
    // FIXME: Is this necessary and/or even safe?
    // m_output->finish ();

    // Tell compressor to terminate.
    m_decompressor->end ();
}

} // namespace lat
