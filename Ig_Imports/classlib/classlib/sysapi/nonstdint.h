#ifndef CLASSLIB_SYSTEM_H
// NB: You must include config.h before including this header.
// This header is used during "configure", so we can't include
// config.h here -- it doesn't yet exist at the time.
# error classlib/sysapi/nonstdint.h
#endif

#ifndef CLASSLIB_NONSTDINT_H
# define CLASSLIB_NONSTDINT_H 1
# if HAVE_STDINT_H
   // __STDC_LIMIT_MACROS must have set on invocation!
   // It's too late to set it here, as e.g. config.h may
   // have already included the header.
#  include <stdint.h>

# else // ! HAVE_STDINT_H
#  include <limits.h>

#  if UCHAR_MAX == 0xff
#   if !defined __cplusplus
#    define INT8_MIN		CHAR_MIN
#    define INT8_MAX		CHAR_MAX
#    define UINT8_MAX		UCHAR_MAX
#   endif
    typedef signed char		int8_t;
    typedef signed char		int_least8_t;
    typedef signed char		int_fast8_t;
    typedef unsigned char	uint8_t;
    typedef unsigned char	uint_least8_t;
    typedef unsigned char	uint_fast8_t;
#  else
#   error could not guess 8-bit stdint.h defaults, please update
#  endif
 
#  if USHRT_MAX == 0xffff
#   if !defined __cplusplus
#    define INT16_MIN		SHRT_MIN
#    define INT16_MAX		SHRT_MAX
#    define UINT16_MAX		USHRT_MAX
#   endif
    typedef signed short	int16_t;
    typedef signed short	int_least16_t;
    typedef signed short	int_fast16_t;
    typedef unsigned short	uint16_t;
    typedef unsigned short	uint_least16_t;
    typedef unsigned short	uint_fast16_t;
#  else
#   error could not guess 16-bit stdint.h defaults, please update
#  endif
 
#  if UINT_MAX == 0xffffffff
#   if !defined __cplusplus
#    define INT32_MIN		INT_MIN
#    define INT32_MAX		INT_MAX
#    define UINT32_MAX		UINT_MAX
#   endif
    typedef signed int		int32_t;
    typedef signed int		int_least32_t;
    typedef signed int		int_fast32_t;
    typedef unsigned int	uint32_t;
    typedef unsigned int	uint_least32_t;
    typedef unsigned int	uint_fast32_t;
#  else
#   error could not guess 32-bit stdint.h defaults, please update
#  endif
 
#  if defined _MSC_VER
#   if !defined __cplusplus
#    define INT64_MIN		(-INT64_MAX-1)
#    define INT64_MAX		9223372036854775807i64
#    define UINT64_MAX		(INT64_MAX * 2ui64 + 1)
#   endif
    typedef signed __int64	int64_t;
    typedef signed __int64	int_least64_t;
    typedef signed __int64	int_fast64_t;
    typedef unsigned __int64	uint64_t;
    typedef unsigned __int64	uint_least64_t;
    typedef unsigned __int64	uint_fast64_t;
#  elif HAVE_LONG_LONG \
   && ((defined ULONG_LONG_MAX && ULONG_LONG_MAX == 0xffffffffffffffffLLU) \
       || (defined ULLONG_MAX && ULLONG_MAX == 0xffffffffffffffffLLU) \
       || (defined ULONGLONG_MAX && ULONGLONG_MAX == 0xffffffffffffffffLLU))
#   if !defined __cplusplus
#    if defined ULONG_LONG_MAX
#     define INT64_MIN		LONG_LONG_MIN
#     define INT64_MAX		LONG_LONG_MAX
#     define UINT64_MAX		ULONG_LONG_MAX
#    elif defined ULLONG_MAX
#     define INT64_MIN		LLONG_MIN
#     define INT64_MAX		LLONG_MAX
#     define UINT64_MAX		ULLONG_MAX
#    elif defined ULONGLONG_MAX
#     define INT64_MIN		LONGLONG_MIN
#     define INT64_MAX		LONGLONG_MAX
#     define UINT64_MAX		ULONGLONG_MAX
#    endif
#   endif
    typedef signed long long	int64_t;
    typedef signed long long	int_least64_t;
    typedef signed long long	int_fast64_t;
    typedef unsigned long long	uint64_t;
    typedef unsigned long long	uint_least64_t;
    typedef unsigned long long	uint_fast64_t;
#  else
#   error could not guess 64-bit stdint.h defaults, please update
#  endif
 
# endif // HAVE_STDINT_H
#endif // CLASSLIB_NONSTDINT_H
