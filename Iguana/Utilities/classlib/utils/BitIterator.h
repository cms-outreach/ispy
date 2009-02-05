#ifndef CLASSLIB_BIT_ITERATOR_H
# define CLASSLIB_BIT_ITERATOR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/utils/BitOps.h"
# include <iterator>

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** A bit-extracting random access iterator over a set of data.

    This iterator extracts chunks of size @c BITS from an underlying
    vector of values stored as values of unsigned integral type @c T.
    The bit size of @c T must be at least @c BITS, but need not be an
    integral multiple of it.  The extracted values have the type @c T.
    Dereferencing the iterator returns a value, not a reference -- the
    bits cannot be changed through this iterator.

    The underlying data (in type @c T) is treated in its natural bit
    order.  That is, the bit position 0 is the least significant bit
    of the first "word" of @c T.  If the size of @c T is N bits, then
    the bit position N-1 is the most significant bit of the first word
    and position N is the least significant bit of the second word,
    and mso on.  (FIXME: Provide machine independent treatment as a
    little-endian bit stream: bit 0 is the "left-most" bit, and all
    others go "right" from it?).

    Bit iterators ranges, in particular the beginning and the end,
    should be given the same @c data pointer in their constructors,
    and only the @c bit positions should differ.  */
template <class T, unsigned int BITS>
class BitIterator ITERATOR_BASE(input_iterator, unsigned int, STDC::ptrdiff_t)
{
public:
    typedef std::input_iterator_tag	iterator_category;
    typedef unsigned int		value_type;
    typedef STDC::ptrdiff_t		difference_type;
    typedef void			reference;
    typedef void			pointer;

    BitIterator (void);
    BitIterator (const T *data, unsigned int bitpos);
    // implicit copy constructor
    // implicit destructor
    // implicit assignment operator

    bool		operator== (const BitIterator &x) const;
    bool		operator!= (const BitIterator &x) const;
    bool		operator< (const BitIterator &x) const;
    bool		operator<= (const BitIterator &x) const;
    bool		operator> (const BitIterator &x) const;
    bool		operator>= (const BitIterator &x) const;

    T			operator* (void) const;
    T			operator[] (difference_type n) const;
    BitIterator &	operator++ (void);
    BitIterator		operator++ (int);
    BitIterator &	operator-- (void);
    BitIterator		operator-- (int);

    BitIterator &	operator+= (difference_type n);
    difference_type	operator- (const BitIterator &x) const;
    BitIterator &	operator-= (difference_type n);

private:
    const T		*m_data;
    unsigned int	m_bit;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

/** Construct an empty uninitialised bit iterator.   */
template <class T, unsigned int BITS>
inline
BitIterator<T,BITS>::BitIterator (void)
    : m_data (0),
      m_bit (0)
{}

/** Initialise a bit iterator.  All iterators for the same range, for
    example the beginning and the end of a range, should be given the
    same @a data argument; only the @a bit positions should differ.

    @param data		The data on which to operate.  This should be
    			the same for all iterators in a data range.
    @param bit		The position of the iterator relative to the
    			base @a data.  The end of the range should be
			one bit past the last position.  */
template <class T, unsigned int BITS>
inline
BitIterator<T,BITS>::BitIterator (const T *data, unsigned int bit)
    : m_data (data),
      m_bit (bit)
{}

/** Compare bit iterators for equality.  Returns @c true if both point
    to the same data region and have the same bit offset.  */
template <class T, unsigned int BITS>
inline bool
BitIterator<T,BITS>::operator== (const BitIterator &x) const
{ return m_data == x.m_data && m_bit == x.m_bit; }

/** Compare bit iterators for inequality.  Returns @c true if the
    iterators point to different data regions or have different bit
    offsets.  */
template <class T, unsigned int BITS>
inline bool
BitIterator<T,BITS>::operator!= (const BitIterator &x) const
{ return !(*this == x); }

/** Compare bit iterator offsets.  Meangingful only for iterators to
    the same data range.  Returns @c true if this one's bit offset is
    less than @a x's.  */
template <class T, unsigned int BITS>
inline bool
BitIterator<T,BITS>::operator< (const BitIterator &x) const
{ return m_bit < x.m_bit; }

/** Compare bit iterator offsets.  Meangingful only for iterators to
    the same data range.  Returns @c true if this one's bit offset is
    less than or equal to @a x's.  */
template <class T, unsigned int BITS>
inline bool
BitIterator<T,BITS>::operator<= (const BitIterator &x) const
{ return m_bit <= x.m_bit; }

/** Compare bit iterator offsets.  Meangingful only for iterators to
    the same data range.  Returns @c true if this one's bit offset is
    greater than @a x's.  */
template <class T, unsigned int BITS>
inline bool
BitIterator<T,BITS>::operator> (const BitIterator &x) const
{ return m_bit > x.m_bit; }

/** Compare bit iterator offsets.  Meangingful only for iterators to
    the same data range.  Returns @c true if this one's bit offset is
    greater than or equal to @a x's.  */
template <class T, unsigned int BITS>
inline bool
BitIterator<T,BITS>::operator>= (const BitIterator &x) const
{ return m_bit >= x.m_bit; }

/** Dereference the bit iterator.  Returns @c BITS from the current
    bit offset to the underlying data.  The bits can only be read,
    not changed.  */
template <class T, unsigned int BITS>
inline T
BitIterator<T,BITS>::operator* (void) const
{ return (BitTraits<T>::Bits % BITS == 0
	  ? BitOps<T>::extractSafe (m_data, m_bit * BITS, BITS)
	  : BitOps<T>::extractStraddledSafe (m_data, m_bit * BITS, BITS)); }

/** Dereference the bit iterator at an offset.  Returns @c BITS from
    the current offset plus @a n times @c BITS (@a n can be positive
    or negative but must fall within a valid range).  The bits can
    only be read, not changed.  */
template <class T, unsigned int BITS>
inline T
BitIterator<T,BITS>::operator[] (difference_type n) const
{ return *(*this + n); }

/** Move the iterator forward by @c BITS.  Changes the iterator and
    then returns itself (pre-increment).  */
template <class T, unsigned int BITS>
inline BitIterator<T,BITS> &
BitIterator<T,BITS>::operator++ (void)
{ m_bit++; return *this; }

/** Move the iterator forward by @c BITS.  Changes the iterator but
    returns the old value (post-increment).  */
template <class T, unsigned int BITS>
inline BitIterator<T,BITS>
BitIterator<T,BITS>::operator++ (int)
{ BitIterator<T,BITS> old (*this); ++*this; return old; }

/** Move the iterator backwards by @c BITS.  Changes the iterator and
    then returns itself (pre-decrement).  */
template <class T, unsigned int BITS>
inline BitIterator<T,BITS> &
BitIterator<T,BITS>::operator-- (void)
{ m_bit--; return *this; }

/** Move the iterator backwards by @c BITS.  Changes the iterator but
    returns the old value (post-decrement).  */
template <class T, unsigned int BITS>
inline BitIterator<T,BITS>
BitIterator<T,BITS>::operator-- (int)
{ BitIterator<T,BITS> old (*this); --*this; return old; }

/** Return a bit iterator @a n times @c BITS forwards from @a x (@a n
    can be positive or negative but must fall within a valid range).  */
template <class T, unsigned int BITS>
inline BitIterator<T,BITS>
operator+ (const BitIterator<T,BITS> &x,
	   typename BitIterator<T,BITS>::difference_type n)
{ BitIterator<T,BITS> r (x); r += n; return r; }

/** Return a bit iterator @a n times @c BITS forwards from @a x (@a n
    can be positive or negative but must fall within a valid range).  */
template <class T, unsigned int BITS>
inline BitIterator<T,BITS>
operator+ (typename BitIterator<T,BITS>::difference_type n,
	   const BitIterator<T,BITS> &x)
{ BitIterator<T,BITS> r (x); r += n; return r; }

/** Move the forwards iterator by @a n times @c BITS (@a n can be
    positive or negative but must fall within a valid range).  */
template <class T, unsigned int BITS>
inline BitIterator<T,BITS> &
BitIterator<T,BITS>::operator+= (difference_type n)
{ return m_bit += n; }

/** Return a bit iterator @a n times @c BITS backwards from @a x (@a n
    can be positive or negative but must fall within a valid range).  */
template <class T, unsigned int BITS>
inline BitIterator<T,BITS>
operator- (const BitIterator<T,BITS> &x,
	   typename BitIterator<T,BITS>::difference_type n)
{ BitIterator<T,BITS> r (x); r -= n; return r; }

/** Return the distance between this and @a x.  For consistency with
    the other methods and functions, the actual distance in bits is
    the returned value times @c BITS, not the returned value itself.  */
template <class T, unsigned int BITS>
inline typename BitIterator<T,BITS>::difference_type
BitIterator<T,BITS>::operator- (const BitIterator<T,BITS> &x) const
{ return m_bit - x.m_bit; }

/** Move the iterator backwards by @a n times @c BITS (@a n can be
    positive or negative but must fall within a valid range).  */
template <class T, unsigned int BITS>
inline BitIterator<T,BITS> &
BitIterator<T,BITS>::operator-= (difference_type n)
{ return m_bit -= n; }

} // namespace lat
#endif // CLASSLIB_BIT_ITERATOR_H
