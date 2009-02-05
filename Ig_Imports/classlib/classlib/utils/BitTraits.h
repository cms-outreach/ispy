#ifndef CLASSLIB_BIT_TRAITS_H
# define CLASSLIB_BIT_TRAITS_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"
# include <climits>

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Describe the bit features of an integral type @c T.

    This traits class describes the number of bits and bytes in the
    scalar template argument type @c T.  It also describes the number
    of base 10 and base 16 digits required to represent the largest
    value of @c T; a leading sign for signed types is not accounted
    for.  @c Bytes are defined as octets: 8 bits.

    The class should be specialised for types @c T that have holes or
    unused bits in them.  Most modern architectures use all the bits
    so no specialisation is required.  The base class implementation
    correctly describes scalar types whose sizes are not multiples of
    8 bits, but most uses of this class will probably break on such
    architectures.  */
template <class T>
struct BitTraits
{
    /// Number of bits in @c T.
    enum { Bits		= sizeof (T) * CHAR_BIT };

    /// Number of 8-bit bytes in @c T.
    enum { Bytes	= Bits / 8 + ((Bits % 8) > 0) };

    /// Number of base-10 digits in @c T (without leading sign).
    enum { Digits	= (Bits * 30103) / 100000 + 1 };
    // 30103 =~ M_LN2 / M_LN10 * 100000

    /// Number of base-16 digits in @c T (without leading sign).
    enum { HexDigits	= Bits / 4 + ((Bits % 4) > 0) };
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_BIT_TRAITS_H
