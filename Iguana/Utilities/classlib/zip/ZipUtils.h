#ifndef CLASSLIB_UTILS_H
# define CLASSLIB_UTILS_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"
# include "classlib/sysapi/IOTypes.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IOInput;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Useful tools in various places, but scoped into our classes.  */
struct ZipUtils
{
    // Open modes; FIXME: DELETE on close?
    enum { OPEN_READ			= 1 };
    enum { OPEN_WRITE			= 2 };

protected:
    // Operators to simplify extraction of values from buffers
    static unsigned	get (const void *buffer, IOSize bytes);
    static void		put (void *buffer, IOSize bytes, unsigned value);

    static unsigned	getOctal (const void *buffer, IOSize bytes);
    static void		putOctal (void *buffer, IOSize bytes, unsigned value,
				  bool null = true);

    static unsigned	getHex (const void *buffer, IOSize bytes);
    static void		putHex (void *buffer, IOSize bytes, unsigned value);

    static bool		extract (IOInput	*input,
				 void		*buffer,
				 IOSize		size,
				 IOSize		n);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

/** Read a @a bytes-long little-endian integer value from @a buffer.  */
inline unsigned
ZipUtils::get (const void *buffer, IOSize bytes)
{
    const unsigned char *buf = static_cast<const unsigned char *> (buffer);
    unsigned value = 0;
    for (unsigned shift = 0; bytes; shift += 8, --bytes, ++buf)
	value += *buf << shift;

    return value;
}

/** Read a @a bytes-long ASCII-octal integer value from @a buffer.  */
inline unsigned
ZipUtils::getOctal (const void *buffer, IOSize bytes)
{
    const unsigned char *buf = static_cast<const unsigned char *> (buffer);
    unsigned value = 0;
    for ( ; bytes && *buf >= '0' && *buf <= '7'; --bytes, ++buf)
	value = (value << 3) + (*buf - '0');

    return value;
}

/** Write @a value into @a buffer as @a bytes-long integer in
    little-endian byte order.  */
inline void
ZipUtils::put (void *buffer, IOSize bytes, unsigned value)
{
    unsigned char *buf = static_cast<unsigned char *> (buffer);
    for ( ; bytes; value >>= 8, --bytes, ++buf)
	*buf = value & 0xff;
}

/** Write @a value into @a buffer as @a bytes-long integer in ASCII
    octal.  */
inline void
ZipUtils::putOctal (void *buffer, IOSize bytes, unsigned value,
		    bool null /* = false */)
{
    unsigned char *buf = static_cast<unsigned char *> (buffer);
    if (null)
	buf [--bytes] = '\0';

    for (; bytes; value >>= 3)
	buf [--bytes] = (value & 7) + '0';
}

} // namespace lat
#endif // CLASSLIB_UTILS_H
