#ifndef CLASSLIB_BIT_OPS_H
# define CLASSLIB_BIT_OPS_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/utils/BitTraits.h"
# include "classlib/utils/BitPattern.h"
# include <algorithm>

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Miscellanous bit operations on unsigned integral type @c T.

    Most of the methods use maths faster than the naive loops or table
    lookups.  In some cases (hot inner loops) a suitably tuned table
    look-up could be faster than the implementation used here.  The
    best size of such tables is somewhat application dependent and
    therefore no such generic service is provided here. */
template <class T> class BitOps
{
public:
    // Various math ops

    static T		count (T n);
    static T		reverse (T n);
    static T		rzero (T n);
    static T		log2 (T n);
    static bool		islog2 (T n);
    static T		ceil2 (T n);
    static bool		parity (T n);
    static T		mortonInterleave (T x, T y);
    static void		mortonSplit (T n, T &x, T &y);
    static void		swap (T &x, T &y);

    // Bit extraction from a bit array of words T; requested bits may
    // (4, 5) or may not (2, 3) straddle `T' word boundaries; `count'
    // must be less than BitTraits<T>::Bits; bounded versions return 0
    // if `from > maxbit'; otherwise `from+count' must be less than or
    // equal to `maxbit'.  No attention is paid to word byte order.
    // See also BitIterator.
    static T		extract			(T n, unsigned int from,
						 unsigned int count);

    static T		extractSafe		(const T *restrict p,
						 unsigned int from,
						 unsigned int count);
    static T		extractUnstraddled	(const T *restrict p,
						 unsigned int maxbit,
						 unsigned int from,
						 unsigned int count);
    // may straddle, except for the last array element
    static T		extractStraddledSafe	(const T *restrict p,
						 unsigned int from,
						 unsigned int count);
    static T		extract			(const T *restrict p,
						 unsigned int maxbit,
						 unsigned int from,
						 unsigned int count);
};

//////////////////////////////////////////////////////////////////////
/** Bit operation helpers using Binary Magic Numbers.

    Magic numbers have unusual or special properties in certain
    calculations.  With binary magic numbers discussed by [Freed,
    Edwin E.  1983.  "Binary Magic Numbers---Some Applications and
    Algorithms", Dr Dobb's Journal, 8(4: April), pp 24--37.] it is
    possible to write algorithms that are typically faster by a factor
    of $N/log_2(N)$ than the more obvious ones.  They can be used to:
      - determine the positions of bits within words;
      - reverse, permute and map bits within words;
      - compute sideways unweighted and weighted sums and parity; and
      - convert Gray code values to their binary equivalents.

    The binary magic numbers come in a sequence (hence this recursive
    template) where the Nth number is itself an infinite binary number
    from right to left of $2^N$ ones follwed by $2^N$ zeroes, followed
    by $2^N$ ones and so on.  The first five numbers in the sequence
    are ...0101010101010101, ...0011001100110011, ...0000111100001111
    and ...0000000011111111.  Often one will use just these patterns,
    but occasionally also the inverse (= one's complement) reverse
    pattern.

    The patterns have B (which must be a power of two, i.e. $2^N$
    above) consequtive ones and or zeros.  It is assumed that T, the
    unsigned integral type used as the "word" in the operations, has a
    size that is a power of two.  */
template <unsigned int B> struct BitOpsMagic {
    template <class T> struct Type {
	// the binary magic number bit patterns
	enum { Pattern    = BitPattern<T,BitPattern<T,1,1,B>::Pattern,B*2,
				       BitTraits<T>::Bits/(B*2)>::Pattern };
	enum { RevPattern = ~ Pattern };
    };
};

template <> struct BitOpsMagic<1> {
    template <class T> struct Type {
	enum { Pattern    = BitPattern<T,1,2,BitTraits<T>::Bits/2>::Pattern };
	enum { RevPattern = ~ Pattern };
    };
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Swap @a x and @a y in place without temporaries.  */
template <class T> inline void
BitOps<T>::swap (T &x, T &y)
{ x ^= y; y ^= x; x ^= y; }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Helper to count the number of bits set in @a n.  Progressively
    combs through the bits, starting with all odd and even bits, and
    accumulating these, resulting in a number of set bits (FIXME:
    explain exactly how).  */
template <unsigned int B> struct BitOpsCount {
    template <class T> struct Op : BitOpsMagic<B>::template Type<T> {
	enum { Pattern = BitOpsMagic<B>::template Type<T>::Pattern };
	static T compute (T n) {
	    n = BitOpsCount<B/2>::template Op<T>::compute (n);
	    return ((n >> B) & Pattern) + (n & Pattern);
	}
    };
};

template <> struct BitOpsCount<1> {
    template <class T> struct Op : BitOpsMagic<1>::template Type<T> {
	enum { Pattern = BitOpsMagic<1>::template Type<T>::Pattern };
	static T compute (T n)
	{ return ((n >> 1) & Pattern) + (n & Pattern); }
    };
};

/** Count the number of bits set in @a n.  */
template <class T> inline T
BitOps<T>::count (T n)
{ return BitOpsCount<BitTraits<T>::Bits/2>::template Op<T>::compute (n); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Helper to reverse bits in n.  The operation for B swaps B adacent
    bits.  The operation starts with B=1 (neighbouring odd and even
    bits are swapped), and then works on recursively larger adjacent
    bit sequences until the whole word has been swapped front-to-back.  */
template <unsigned int B> struct BitOpsReverse {
    template <class T> struct Op : BitOpsMagic<B>::template Type<T> {
	enum { Pattern = BitOpsMagic<B>::template Type<T>::Pattern };
	enum { RevPattern = BitOpsMagic<B>::template Type<T>::RevPattern };
	static T compute (T n) {
	    n = BitOpsReverse<B/2>::template Op<T>::compute (n);
	    n = ((n >> B) & Pattern) | ((n << B) & RevPattern);
	    return n;
	}
    };
};

template <> struct BitOpsReverse<1> {
    template <class T> struct Op : BitOpsMagic<1>::template Type<T> {
	enum { Pattern = BitOpsMagic<1>::template Type<T>::Pattern };
	enum { RevPattern = BitOpsMagic<1>::template Type<T>::RevPattern };
	static T compute (T n)
	{ return ((n >> 1) & Pattern) | ((n << 1) & RevPattern); }
    };
};

/** Reverse the order of bits in @a n: the least significant bit
    becomes the most signficant one and vice versa.  */
template <class T> inline T
BitOps<T>::reverse (T n)
{ return BitOpsReverse<BitTraits<T>::Bits/2>::template Op<T>::compute (n); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Helper to compute the number of zero bits on the right side of n;
    c is a result accumulator.  [FIXME: explain how; similar to log2.]
    [FIXME: is this really efficient?  with the condtional code we
    might just as well have a small tight loop... or inline asm]  */
template <unsigned int B> struct BitOpsRZero {
    template <class T> struct Op : BitOpsMagic<B>::template Type<T> {
	enum { Pattern = BitOpsMagic<B>::template Type<T>::Pattern };
	static T compute (T n, unsigned int c) {
	    return n & Pattern
		? BitOpsRZero<B/2>::template Op<T>::compute (n << B, c - B)
		: BitOpsRZero<B/2>::template Op<T>::compute (n, c);
	}
    };
};

template <> struct BitOpsRZero<1> {
    template <class T> struct Op : BitOpsMagic<1>::template Type<T> {
	enum { Pattern = BitOpsMagic<1>::template Type<T>::Pattern };
	static T compute (T n, unsigned int c) {
	    if (n & Pattern)
	    {
		n <<= 1;
		c--;
	    }
	    return n ? c-1 : c;
	}
    };
};

/** Compute the number of zero bits on the right edge of @a n, i.e the
    log2 of the least significant set bit in @a n.  */
template <class T> inline T
BitOps<T>::rzero (T n)
{ return BitOpsRZero<BitTraits<T>::Bits/2>::template Op<T>::compute
      (n, BitTraits<T>::Bits); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Helper to compute the log2 base of n; c is the result accumulator.
    [FIXME: explain how.]  [FIXME: is this really efficient?  with the
    condtional code we might just as well have a small tight loop...
    or inline asm]  */
template <unsigned int B> struct BitOpsLog2 {
    template <class T> struct Op : BitOpsMagic<B>::template Type<T> {
	enum { RevPattern = BitOpsMagic<B>::template Type<T>::RevPattern };
	static T compute (T n, unsigned int c) {
	    return n & RevPattern
		? BitOpsLog2<B/2>::template Op<T>::compute (n >> B, c | B)
		: BitOpsLog2<B/2>::template Op<T>::compute (n, c);
	}
    };
};

template <> struct BitOpsLog2<1> {
    template <class T> struct Op : BitOpsMagic<1>::template Type<T> {
	enum { RevPattern = BitOpsMagic<1>::template Type<T>::RevPattern };
	static T compute (T n, unsigned int c)
	{ return c | (n & RevPattern ? 1 : 0); }
    };
};

/** Compute log2 base of @a n, i.e. the log2 of the most signficant
    set bit in @a n.  */
template <class T> inline T
BitOps<T>::log2 (T n)
{ return BitOpsLog2<BitTraits<T>::Bits/2>::template Op<T>::compute (n, 0); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Return true if @a n is a power of two.  (FIXME: This should also
    be available as a constant expression for constant values of n.)  */
template <class T> inline bool
BitOps<T>::islog2 (T n)
{ return (n & (n - 1)) == 0; }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Helper to compute the next power of two of n.  [FIXME: explain how.] */
template <unsigned int B> struct BitOpsCeil2 {
    template <class T> struct Op : BitOpsMagic<B>::template Type<T> {
	static T compute (T n)
	{ n = BitOpsCeil2<B/2>::template Op<T>::compute (n); return n | (n >> B); }
    };
};

template <> struct BitOpsCeil2<1> {
    template <class T> struct Op : BitOpsMagic<1>::template Type<T> {
	static T compute (T n)
	{ return n | (n >> 1); }
    };
};

/** Round @a n up to next power of two.  */
template <class T> inline T
BitOps<T>::ceil2 (T n)
{ return BitOpsCeil2<BitTraits<T>::Bits/2>::template Op<T>::compute (n-1)+1; }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Bit operation helper for the @c parity function.  */
template <unsigned int B> struct BitOpsParity {
    template <class T> struct Op : BitOpsMagic<B>::template Type<T> {
        /* n ^= n >> 16; n ^= n >> 8; n ^= n >> 4; n ^= n >> 2; n ^= n >> 1; */
	static T compute (T n)
	{ return BitOpsParity<B/2>::template Op<T>::compute (n ^ (n >> B)); }
    };
};

template <> struct BitOpsParity<1> {
    template <class T> struct Op : BitOpsMagic<1>::template Type<T> {
	static T compute (T n)
	{ return n ^ (n >> 1); }
    };
};

/** Compute parity of @a n.  Returns @c true if @a n has an odd number
    of bits set.  */
template <class T> inline bool
BitOps<T>::parity (T n)
{ return BitOpsParity<BitTraits<T>::Bits/2>::template Op<T>::compute (n) & 1; }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Spread bits of n to be one bit apart from their neighbour to the
    even bit positions in T; the odd bits will be left zero.

    To construct a Morton sequence number of two numbers x and y, do
    mortonInterleave(x) | (mortonInterleave(y) << 1).

    Each operation splits the bits in n into chunks of B bits, and
    moves the chunks B bits away from its neighbours, and fills the
    spaces with zeroes---like this (`o' denotes bit that is zero, `0'
    the least significant bit):
      - xxxx xxxx 7654 3210 =>  (B = 4, pattern = 0x0f0f0f0f)
      - oooo 7654 oooo 3210 =>  (B = 2, pattern = 0x33333333)
      - oo76 oo54 oo32 oo10 =>  (B = 1, pattern = 0x55555555)
      - o7o6 o5o4 o3o2 o1o0.  */
template <unsigned int B> struct BitOpsMortonInterleave {
    template <class T> struct Op : BitOpsMagic<B>::template Type<T> {
	enum { Pattern = BitOpsMagic<B>::template Type<T>::Pattern };
	static T compute (T n)
	{ return BitOpsMortonInterleave<B/2>::template Op<T>::compute ((n | (n << B)) & Pattern); }
    };
};

template <> struct BitOpsMortonInterleave<1> {
    template <class T> struct Op : BitOpsMagic<1>::template Type<T> {
	enum { Pattern = BitOpsMagic<1>::template Type<T>::Pattern };
	static T compute (T n)
	{ return (n | (n << 1)) & Pattern; }
    };
};

/** Merge even bits of n to the right end of the result value.

    This operation undoes mortonInterleave.  To decompose a Morton
    sequence number n to its components x and y, do
    x = mortonExtract (n); y = mortonExtract (n >> 1).

    Each operation moves a chunk of B bits closer to its neighbours by
    a step of B bits, reversing mortonInterleave---like this:
      - x7x6 x5x4 x3x2 x1x0 =>  (B = 1, pattern = 0x55555555)
      - oo76 oo54 oo32 oo10 =>  (B = 2, pattern = 0x33333333)
      - oooo 7654 oooo 3210 =>  (B = 4, pattern = 0x0f0f0f0f)
      - oooo oooo 7654 3210.  */
template <unsigned int B> struct BitOpsMortonExtract {
    template <class T> struct Op : BitOpsMagic<B>::template Type<T> {
	enum { Pattern = BitOpsMagic<B>::template Type<T>::Pattern };
	static T compute (T n) {
	    n = BitOpsMortonExtract<B/2>::template Op<T>::compute (n) & Pattern;
	    return n | (n >> B);
	}
    };
};

template <> struct BitOpsMortonExtract<1> {
    template <class T> struct Op : BitOpsMagic<1>::template Type<T> {
	enum { Pattern = BitOpsMagic<1>::template Type<T>::Pattern };
	static T compute (T n)
	{ n &= Pattern; return n | (n >> 1); }
    };
};

/** Compute interleaved Morton Number for @a x and @a y.  Both @a x
    and @a y must use only the lower half bits of @c T.

    Morton Numbers interleave the bits of @a x and @a y such that 2D
    plane squares close to (@a x, @a y) have Morton Numbers close to
    the Morton Number of (@a x, @a y).  The interleaving has also the
    property that the numbers are proportional to @a x and @a y and
    leave no "gaps".  2D squares can thus be put in a vector indexed
    by the Morton Numbers of their (@a x, @a y) coordinates.  The
    vector will have no gaps and squares close to each other will be
    close to each other in the linear sequence. */
template <class T> inline T
BitOps<T>::mortonInterleave (T x, T y)
{ return BitOpsMortonInterleave<BitTraits<T>::Bits/4>::template Op<T>::compute (x)
      | (BitOpsMortonInterleave<BitTraits<T>::Bits/4>::template Op<T>::compute (y) << 1); }

/** Decode @a x and @a y from the interleaved Morton Number @a n.

    @sa #mortonInterleave() */
template <class T> inline void
BitOps<T>::mortonSplit (T n, T &x, T &y)
{
    x = BitOpsMortonExtract<BitTraits<T>::Bits/4>::template Op<T>::compute (n);
    y = BitOpsMortonExtract<BitTraits<T>::Bits/4>::template Op<T>::compute (n >> 1);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Extract @a count bits from word @a n, starting at bit position @a
    from.  The caller guarantees that [@a from, @a from + @a count)
    fits in @a n.  */
template <class T> inline T
BitOps<T>::extract (T n, unsigned int from, unsigned int count)
{
    enum { AllOnes = BitPattern<T,0xff,8,BitTraits<T>::Bytes>::Pattern };
    return (n >> (BitTraits<T>::Bits - from - count)) & ~(AllOnes << count);
}

/** Extract @a count bits from bit vector @a p, starting at bit
    position @a from.

    The vector of unsigned integral words of type @c T starting at @a
    p is treated as one long bit vector.  The words are treated in
    their natural bit order: the bit position zero is the least
    significant bit of the first word in @a p, bit position N-1 (where
    N is the number of bits in @c T) is the most significant bit of
    the first word, bit position N is the least significant bit of the
    second word in @a p, and so on.

    The caller guarantees that [@a from, @a from + @a count) does not
    straddle word boundaries of @a p elements, and that the requested
    bits do not exceed the end of the valid range of @a p.  */
template <class T> inline T
BitOps<T>::extractSafe (const T *restrict p, unsigned int from,
			 unsigned int count)
{
    return extract (* (p + from/BitTraits<T>::Bits),
		    from % BitTraits<T>::Bits,
		    count);
}

/** Extract @a count bits from bit vector @a p, starting at bit
    position @a from, but not exceeding @a maxbit.

    The vector of unsigned integral words of type @c T starting at @a
    p is treated as one long bit vector.  The words are treated in
    their natural bit order: the bit position zero is the least
    significant bit of the first word in @a p, bit position N-1 (where
    N is the number of bits in @c T) is the most significant bit of
    the first word, bit position N is the least significant bit of the
    second word in @a p, and so on.

    The caller guarantees that [@a from, @a from + @a count) does not
    straddle word boundaries of @a p elements.  If @a from is less or
    equal to @a maxbit, @a from + @a count is also assumed to be.  If
    @a from is greater than @a maxbit, zero is returned.  */
template <class T> inline T
BitOps<T>::extractUnstraddled (const T *restrict p, unsigned int maxbit,
			       unsigned int from, unsigned int count)
{ return from > maxbit ? 0 : extractSafe (p, from, count); }

/** Extract @a count bits from bit vector @a p, starting at bit
    position @a from, possibly straddling word boundaries.

    The vector of unsigned integral words of type @c T starting at @a
    p is treated as one long bit vector.  The words are treated in
    their natural bit order: the bit position zero is the least
    significant bit of the first word in @a p, bit position N-1 (where
    N is the number of bits in @c T) is the most significant bit of
    the first word, bit position N is the least significant bit of the
    second word in @a p, and so on.

    This method performs a fetch slower than the other @c extract
    methods but deals correctly with situations where the requested
    bits straddle the word boundaries of @a p elements.  The caller
    guarantees that @a count does not exceed the number of bits in @c
    T and that the requested bits do not exceed the end of the valid
    range of @a p.  */
template <class T> inline T
BitOps<T>::extractStraddledSafe (const T *restrict p, unsigned int from,
				 unsigned int count)
{
    unsigned int count_word_1 = std::min(count,
					 BitTraits<T>::Bits
					 - from % BitTraits<T>::Bits);
    unsigned int count_word_2 = count - count_word_1;

    return (extractSafe (p, from, count_word_1) << count_word_2
	    | extractSafe (p, from+count_word_1, count_word_2));
}

/** Extract @a count bits from bit vector @a p, starting at bit
    position @a from, but not exceeding @a maxbit, possibly straddling
    word boundaries.

    The vector of unsigned integral words of type @c T starting at @a
    p is treated as one long bit vector.  The words are treated in
    their natural bit order: the bit position zero is the least
    significant bit of the first word in @a p, bit position N-1 (where
    N is the number of bits in @c T) is the most significant bit of
    the first word, bit position N is the least significant bit of the
    second word in @a p, and so on.

    This method performs a fetch slower than the other @c extract
    methods but deals correctly with situations where the requested
    bits straddle the word boundaries of @a p elements.  If @a from is
    less or equal to @a maxbit, @a from + @a count is also assumed to
    be.  If @a from is greater than @a maxbit, zero is returned.  */
template <class T> inline T
BitOps<T>::extract (const T *restrict p, unsigned int maxbit,
		    unsigned int from, unsigned int count)
{ return from > maxbit ? 0 : extractStraddledSafe (p, from, count); }

} // namespace lat
#endif // CLASSLIB_BIT_OPS_H
