#ifndef CLASSLIB_SYSAPI_PROCESS_INFO_H
# define CLASSLIB_SYSAPI_PROCESS_INFO_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"
# ifdef _WIN32
#  include <windows.h>
# else
#  include <unistd.h>
#  if HAVE_CRT_EXTERNS_H // Mac OS X
#   include <crt_externs.h>
#  endif
#  if HAVE_MACH_O_DYLD_H // Mac OS X
#   include <mach-o/dyld.h>
#  endif
# endif

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>

extern "C" {
#if HAVE_SYM_ARGV // WIN32
    extern char **__argv;
    extern int __argc;
#elif HAVE_SYM_DL_ARGV
    extern char **_dl_argv;
#endif
}

//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>
//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // CLASSLIB_SYSAPI_PROCESS_INFO_H
