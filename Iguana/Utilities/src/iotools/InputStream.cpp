//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iotools/InputStream.h"
#include "classlib/iotools/IOException.h"
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

/** Construct an input stream.  A no-op.  */
InputStream::InputStream (void)
{}

/** Destruct an input stream.  A no-op.  */
InputStream::~InputStream (void)
{}

/** Return the number of bytes that can be read or skipped over from
    this input stream without blocking on the next call to read().

    Note that the next caller may be in a different thread and the
    available data may have already been read by the time this method
    returns.  The base class implementation always returns zero.  */
IOSize
InputStream::available (void)
{ return 0; }

/** Skip over and discard @a n bytes of input.

    This method skips @a n bytes of input.  It may for a number of
    reasons skip less than that, such as hitting the end of input.
    The actual number of bytes skipped is returned.

    The base class implementation reads from the stream until @a n
    bytes have been skipped, or end of input is reached.  It uses
    an internal buffer.  Derived classes are encouraged to provide
    a better implementation.

    This method should not be used on non-blocking streams.

    @throw  Any error other than end of input data causes an
            IOException to be throw.  */
IOSize
InputStream::skip (IOSize n)
{
    // Read data into a local buffer until we've read as much as
    // requested or hit the end.
    unsigned char	buf [SKIP_BUFFER_SIZE];
    IOSize		left = n;

    for (IOSize m = 1; m && left; left -= m)
	m = read (buf, std::min (left, IOSize (sizeof (buf))));

    // Return the number of bytes actually read
    return n - left;
}

/** Close the stream and release resources used by it.  */
void
InputStream::close (void)
{}

/** Reset the stream.

    The stream may either throw an #IOException or reset itself to
    some fixed state that depends on the type of the stream and how it
    was created; subsequent reads will then return bytes in a
    stream-dependent fashion.

    The base class implementation always does nothing but throw an
    #IOException.  */
void
InputStream::reset (void)
{ throw IOException (); }

} // namespace lat
