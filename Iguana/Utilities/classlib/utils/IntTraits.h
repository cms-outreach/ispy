#ifndef CLASSLIB_INT_TRAITS_H
# define CLASSLIB_INT_TRAITS_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/utils/LongLong.h"
# ifdef _MSC_VER
#  define CLASSLIB_BROKEN_TEMPLATE_CONSTANTS 1
# endif

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Define numerical features of integral types.  This is a poor man's
    @c std::numeric_limits while compilers are catching up.  */
template <class T> struct IntTraits;

#if !CLASSLIB_BROKEN_TEMPLATE_CONSTANTS
template <>
struct IntTraits<unsigned char>
{
    static const unsigned char	Min = 0;		//< Minimum value
    static const unsigned char	Max = UCHAR_MAX;	//< Maximum value
};

template <>
struct IntTraits<signed char>
{
    static const signed char	Min = SCHAR_MIN;	//< Minimum value
    static const signed char	Max = SCHAR_MAX;	//< Maximum value
};

template <>
struct IntTraits<char>
{
    static const char		Min = CHAR_MIN;		//< Minimum value
    static const char		Max = CHAR_MAX;		//< Maximum value
};

template <>
struct IntTraits<unsigned short>
{
    static const unsigned short	Min = 0;		//< Minimum value
    static const unsigned short	Max = USHRT_MAX;	//< Maximum value
};

template <>
struct IntTraits<signed short>
{
    static const signed short	Min = SHRT_MIN;		//< Minimum value
    static const signed short	Max = SHRT_MAX;		//< Maximum value
};

template <>
struct IntTraits<unsigned int>
{
    static const unsigned int	Min = 0;		//< Minimum value
    static const unsigned int	Max = UINT_MAX;		//< Maximum value
};

template <>
struct IntTraits<signed int>
{
    static const signed int	Min = INT_MIN;		//< Minimum value
    static const signed int	Max = INT_MAX;		//< Maximum value
};

template <>
struct IntTraits<unsigned long>
{
    static const unsigned long	Min = 0;		//< Minimum value
    static const unsigned long	Max = ULONG_MAX;	//< Maximum value
};

template <>
struct IntTraits<signed long>
{
    static const signed long	Min = LONG_MIN;		//< Minimum value
    static const signed long	Max = LONG_MAX;		//< Maximum value
};

#  if HAVE_LONG_LONG
template <>
struct IntTraits<unsigned long long>
{
    static const unsigned long long Min = 0;		//< Minimum value
    static const unsigned long long Max = ULONG_LONG_MAX;//< Maximum value
};

template <>
struct IntTraits<signed long long>
{
    static const signed long long Min = LONG_LONG_MIN;	//< Minimum value
    static const signed long long Max = LONG_LONG_MAX;	//< Maximum value
};
#  elif HAVE_INT64
template <>
struct IntTraits<unsigned __int64>
{
    static const unsigned __int64 Min = 0;		//< Minimum value
    static const unsigned __int64 Max = ULONG_LONG_MAX;	//< Maximum value
};

template <>
struct IntTraits<signed __int64>
{
    static const signed __int64	Min = LONG_LONG_MIN;	//< Minimum value
    static const signed __int64	Max = LONG_LONG_MAX;	//< Maximum value
};
#  endif

#else // CLASSLIB_BROKEN_TEMPLATE_CONSTANTS
template <>
struct IntTraits<unsigned char>
{
    enum { Min = 0 };					//< Minimum value
    enum { Max = UCHAR_MAX };				//< Maximum value
};

template <>
struct IntTraits<signed char>
{
    enum { Min = SCHAR_MIN };				//< Minimum value
    enum { Max = SCHAR_MAX };				//< Maximum value
};

template <>
struct IntTraits<char>
{
    enum { Min = CHAR_MIN };				//< Minimum value
    enum { Max = CHAR_MAX };				//< Maximum value
};

template <>
struct IntTraits<unsigned short>
{
    enum { Min = 0 };					//< Minimum value
    enum { Max = USHRT_MAX };				//< Maximum value
};

template <>
struct IntTraits<signed short>
{
    enum { Min = SHRT_MIN };				//< Minimum value
    enum { Max = SHRT_MAX };				//< Maximum value
};

template <>
struct IntTraits<unsigned int>
{
    enum { Min = 0 };					//< Minimum value
    enum { Max = UINT_MAX };				//< Maximum value
};

template <>
struct IntTraits<signed int>
{
    enum { Min = INT_MIN };				//< Minimum value
    enum { Max = INT_MAX };				//< Maximum value
};

template <>
struct IntTraits<unsigned long>
{
    enum { Min = 0 };					//< Minimum value
    enum { Max = ULONG_MAX };				//< Maximum value
};

template <>
struct IntTraits<signed long>
{
    enum { Min = LONG_MIN };				//< Minimum value
    enum { Max = LONG_MAX };				//< Maximum value
};

#  if 0 && HAVE_LONG_LONG // Disabled: MSVC++ can't do 64-bit enum constants
template <>
struct IntTraits<unsigned long long>
{
    enum { Min = 0 };					//< Minimum value
    enum { Max = ULONG_LONG_MAX };			//< Maximum value
};

template <>
struct IntTraits<signed long long>
{
    enum { Min = LONG_LONG_MIN };			//< Minimum value
    enum { Max = LONG_LONG_MAX };			//< Maximum value
};
#  elif 0 && HAVE_INT64 // Disabled: MSVC++ can't do 64-bit enum constants
template <>
struct IntTraits<unsigned __int64>
{
    enum { Min = 0 };					//< Minimum value
    enum { Max = ULONG_LONG_MAX };			//< Maximum value
};

template <>
struct IntTraits<signed __int64>
{
    enum { Min = LONG_LONG_MIN };			//< Minimum value
    enum { Max = LONG_LONG_MAX };			//< Maximum value
};
#  endif
# endif // CLASSLIB_BROKEN_TEMPLATE_CONSTANTS

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_INT_TRAITS_H
