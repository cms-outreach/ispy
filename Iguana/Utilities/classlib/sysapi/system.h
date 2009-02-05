#ifndef CLASSLIB_SYSTEM_H
# define CLASSLIB_SYSTEM_H

# include "classlib/sysapi/config.h"
# include <sys/types.h>

/** @def CLASSLIB_API
  @brief A macro that controls how entities of this shared library are
         exported or imported on Windows platforms (the macro expands
         to nothing on all other platforms).  The definitions are
         exported if #CLASSLIB_BUILD_DLL is defined, imported
         if #CLASSLIB_BUILD_ARCHIVE is not defined, and left
         alone if latter is defined (for an archive library build).  */

/** @def CLASSLIB_BUILD_DLL
  @brief Indicates that the header is included during the build of
         a shared library of this package, and all entities marked
	 with #CLASSLIB_API should be exported.  */

/** @def CLASSLIB_BUILD_ARCHIVE
  @brief Indicates that this library is or was built as an archive
         library, not as a shared library.  Lack of this indicates
         that the header is included during the use of a shared
         library of this package, and all entities marked with
         #CLASSLIB_API should be imported.  */

# ifndef CLASSLIB_API
#  ifdef _WIN32
#    if defined CLASSLIB_BUILD_DLL
#      define CLASSLIB_API __declspec(dllexport)
#    elif ! defined CLASSLIB_BUILD_ARCHIVE
#      define CLASSLIB_API __declspec(dllimport)
#    endif
#  endif
# endif

# ifndef CLASSLIB_API
#  define CLASSLIB_API
# endif

/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */

# if HAVE_SOCKADDR_UN
#  define HAVE_LOCAL_SOCKETS 1
# endif

# if HAVE_U_STACK_TRACE \
     || HAVE_BACKTRACE_SYMBOLS_FD \
     || (HAVE_EXCPT_H && HAVE_RLD_INTERFACE_H && HAVE_PDSC_H) \
     || (HAVE_EXCEPTION_H && HAVE_RLD_INTERFACE_H) \
     || defined PROG_PSTACK \
     || defined _WIN32 \
     || __GNUC__ > 3 \
     || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4)
#  define HAVE_STACK_BACKTRACE 1
# endif

/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */

/* FIXME: Calibrated values.  */
# define CLASSLIB_FASTEST_TYPE_INT 1
/* # define HAVE_COPYING_PUTENV 1 */

/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */

#if !HAVE_NAMESPACE_STD
# define std
#endif

#if HAVE_CXX_STDC_HEADERS
# define STDC std
#else
# define STDC
#endif

# if !HAVE_BOOL
#  ifndef bool
    typedef int bool;
#  endif
#  ifndef true
#   define true 1
#  endif
#  ifndef false
#   define false 0
#  endif
# endif

# if !HAVE_TYPENAME
#  define typename
# endif

# if !HAVE_EXPLICIT
#  define explicit
# endif

# if !HAVE_RESTRICT
#  if HAVE_PRIVATE_RESTRICT
#   define restrict __restrict
#  else
#   define restrict
#  endif
# endif

# if !HAVE_USING
#  define USING(x)
# else
#  define USING(x) using x
# endif

# if HAVE_NEW_STYLE_CASTS
#  define STATIC_CAST(type,expr)	static_cast<type> (expr)
#  define CONST_CAST(type,expr)		const_cast<type> (expr)
#  define REINTERPRET_CAST(type,expr)	reinterpret_cast<type> (expr)
# else
#  define STATIC_CAST(type,expr)	((type)(expr))
#  define CONST_CAST(type,expr)		((type)(expr))
#  define REINTERPRET_CAST(type,expr)	((type)(expr))
# endif

# if HAVE_DYNAMIC_CAST && HAVE_TYPEID 
#  define HAVE_FUNCTIONAL_RTTI 1
# endif

# if HAVE_TEMPLATE_NULL_ARGS
#  define CLASSLIB_TEMPLATE_NULL_ARGS <>
# else
#  define CLASSLIB_TEMPLATE_NULL_ARGS
# endif

# if HAVE_TEMPLATE_NULL_SPEC
#  define CLASSLIB_TEMPLATE_NULL_SPEC template<>
# else
#  define CLASSLIB_TEMPLATE_NULL_SPEC
# endif

# undef CLASSLIB_FUNCTION
# if HAVE_PRETTY_FUNCTION
#  define CLASSLIB_FUNCTION	__PRETTY_FUNCTION__
# elif HAVE_FUNCTION
#  define CLASSLIB_FUNCTION	__FUNCTION__
# elif HAVE_FUNC
#  define CLASSLIB_FUNCTION	__func__
# endif

# if HAVE_ITERATOR
#  define ITERATOR_BASE(cat,val,diff)	: public std::iterator<std::cat##_tag,val,diff>
# elif HAVE_ITERATOR_TRAITS
#  define ITERATOR_BASE(cat,val,diff)
# else
#  define ITERATOR_BASE(cat,val,diff)	: public std::cat<val,diff>
# endif

# if HAVE_REVERSE_ITERATOR_STYLE == 1
#  define REVERSE_ITERATOR(tname,tag,iter,value,reference,pointer,difference) \
	std::reverse_iterator<iter>
# elif HAVE_REVERSE_ITERATOR_STYLE == 2
#  define REVERSE_ITERATOR(tname,tag,iter,value,reference,pointer,difference) \
	std::reverse_iterator<iter,std::tag,value,reference,pointer,difference>
# elif HAVE_REVERSE_ITERATOR_STYLE == 3
#  define REVERSE_ITERATOR(tname,tag,iter,value,reference,pointer,difference) \
	std::tname<iter,value,reference,pointer,difference>
# elif HAVE_REVERSE_ITERATOR_STYLE == 4
#  define REVERSE_ITERATOR(tname,tag,iter,value,reference,pointer,difference) \
	std::os_##tname<iter,value,reference,pointer,difference>
# else
#  define REVERSE_ITERATOR(tname,tag,iter,value,reference,pointer,difference) \
	I_don_t_know_how_to_define_reverse_iterator
# endif

# ifdef _MSC_VER
#  pragma warning(disable:4786)		/* '255' characters in the debug information */
#  pragma warning(disable:4305)		/* conversion from double to float (5.0) */
#  pragma warning(disable:4800)		/* forcing value to bool 'true' or 'false' (5.0) */
# endif /* _MSC_VER */

#endif /* CLASSLIB_SYSTEM_H */
