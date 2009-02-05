#ifndef CLASSLIB_SYSAPI_RESOURCE_INFO_H
# define CLASSLIB_SYSAPI_RESOURCE_INFO_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"
# ifdef _WIN32
#  include <windows.h>
# else
#  include <sys/param.h>
#  if HAVE_SYS_RESOURCE_H
#   include <sys/time.h>
#   include <sys/resource.h>
#   include <unistd.h>
#  endif
#  if HAVE_PROCFS_H
#   include <procfs.h>
#  endif
# endif
# include <stdio.h>
# include <limits.h>
# include <cerrno>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>

# if !defined RLIMIT_NOFILE && defined RLIMIT_OFILE
#  define RLIMIT_NOFILE RLIMIT_OFILE
# endif
# if !defined RLIMIT_AS && defined RLIMIT_VMEM
#  define RLIMIT_AS RLIMIT_VMEM
# endif

# if HAVE_GETRLIMIT
#  define GETRLIMIT(lim)					\
     do { rlimit l;						\
         if (::getrlimit (lim, &l) < 0)				\
	     throw SystemError (errno);				\
         if (l.rlim_cur != RLIM_INFINITY)			\
	     soft = l.rlim_cur;					\
         if (l.rlim_max != RLIM_INFINITY)			\
	     hard = l.rlim_max;					\
     } while (0)
#  define SETRLIMIT(lim)					\
     do { rlimit l;						\
         l.rlim_cur = (soft == LIM_INFINITE ? RLIM_INFINITY : soft);\
         l.rlim_max = (hard == LIM_INFINITE ? RLIM_INFINITY : hard);\
         if (::setrlimit (lim, &l) < 0)				\
	    throw SystemError (errno);				\
     } while (0)
# endif

# ifdef HAVE_ULIMIT
#  define GETULIMIT(t,scale)					\
     do { long val = ulimit (t);				\
	  if (val < 0) throw SystemError (errno);		\
	  soft = hard = val * scale;				\
     } while (0)
# endif

//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>

#if HAVE_SYM_BSS_START
extern "C" char __bss_start [];
#endif
#if HAVE_SYM_DATA_START
extern "C" char __data_start [];
#endif
#if HAVE_SYM_EDATA
extern "C" char _edata [];
#endif
#if HAVE_SYM_ETEXT
extern "C" char _etext [];
#endif
#if HAVE_SYM_END
extern "C" char _end [];
#endif

//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>
//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // CLASSLIB_SYSAPI_RESOURCE_INFO_H
