#ifndef CLASSLIB_SYSAPI_TIME_INFO_H
# define CLASSLIB_SYSAPI_TIME_INFO_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"
# include <cerrno>

// FIXME: use the autoconf logic to include time.h and sys/time.h
# ifdef _WIN32
#  include <windows.h>
#  include <time.h>
# else
#  include <unistd.h>
#  include <fcntl.h>
#  if TIME_WITH_SYS_TIME
#   include <sys/time.h>
#   include <time.h>
#  else
#   if HAVE_SYS_TIME_H
#    include <sys/time.h>
#   else
#    include <time.h>
#   endif
#  endif
#  if HAVE_SYS_TIMES_H
#   include <sys/times.h>
#  endif
#  if HAVE_SYS_SYSCTL_H
#   include <sys/sysctl.h>
#  endif
#  if HAVE_SYS_PROCESSOR_H
#   include <sys/processor.h>
#  endif
#  if HAVE_SYS_RESOURCE_H
#   include <sys/resource.h>
#  endif
#  if HAVE_PMAPI_H
#   include <pmapi.h>
#  endif
#  if HAVE_INVENT_H
#   include <invent.h>
#  endif
#  if HAVE_MACH_MACH_TIME_H
#   include <mach/mach_time.h>
#  endif
//   #if HAVE_MACHINE_INLINE_H
//   # include <machine/inline.h>
//   #endif
#  if HAVE_SYS_PSTAT_H
#   define _PSTAT64
#   include <sys/param.h>
#   include <sys/pstat.h>
#  endif
#  if HAVE_PROCFS_H
#   include <procfs.h>
#  endif
# endif
# include <limits.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>

// A gross hack for linux which lies that CLK_TCK is 1000000 when the
// values are really 100.
# ifdef __linux
#  undef CLK_TCK
#  define CLK_TCK 100
# endif

// Define some helper states to simplify the logic

#if HAVE_READ_REAL_TIME || defined CPU_REAL_CYCLES_ASM || HAVE_MACH_MACH_TIME_H || defined _WIN32
# define HAVE_CPU_REAL_CYCLE_COUNTER 1
#endif

#if (HAVE_GETHRTIME					\
     || (HAVE_CLOCK_GETTIME && HAVE_CLOCK_SGI_CYCLE)	\
     || HAVE_CPU_REAL_CYCLE_COUNTER)
# define HAVE_CPU_REAL_TIME_COUNTER 1
#endif

#if defined CPU_VIRTUAL_CYCLES_ASM
# define HAVE_CPU_VIRTUAL_CYCLE_COUNTER 1
#endif

//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>
//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // CLASSLIB_SYSAPI_TIME_INFO_H
