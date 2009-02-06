//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/zip/ZipUtils.h"
#include "classlib/iobase/IOInput.h"
#include <algorithm>
#include <cstring>
#include <cctype>

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

/** Read a @a bytes-long ASCII-hex integer value from @a buffer.  */
unsigned
ZipUtils::getHex (const void *buffer, IOSize bytes)
{
    static const char HEX [] = "0123456789abcdef";
    const char *buf = static_cast<const char *> (buffer);
    unsigned value = 0;

    for (; bytes; buf++, --bytes)
	value = (value << 4) + (strchr (HEX, tolower (*buf)) - HEX);

    return value;
}

/** Write @a value into @a buffer as @a bytes-long integer in ASCII
    hex.  */
void
ZipUtils::putHex (void *buffer, IOSize bytes, unsigned value)
{
    static const char HEX [] = "0123456789abcdef";
    unsigned char *buf = static_cast<unsigned char *> (buffer);
    for (; bytes; value >>= 4)
        buf [--bytes] = HEX [value & 0xf];
}

/** Read @a n bytes from @a input, placing them into @a buffer of size
    @a size.  If @a size is less than @a n, this implies multiple reads
    that overwrite previously read data in the buffer.

    @return @c false if end of input data was seen, @c true otherwise.  */
bool
ZipUtils::extract (IOInput *input, void *buffer, IOSize size, IOSize n)
{
    unsigned char *buf = static_cast<unsigned char *> (buffer);
    IOSize read, done, left;
    for (; n; n -= done)
	for (done = 0, left = std::min (n, size); done != left; done += read)
	    if (! (read = (input->read (buf + done, left - done))))
		return false;

    return true;
}

} // namespace lat
