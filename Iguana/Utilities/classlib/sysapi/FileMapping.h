#ifndef CLASSLIB_SYSAPI_FILE_MAPPING_H
# define CLASSLIB_SYSAPI_FILE_MAPPING_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"
# include <cerrno>
# ifdef _WIN32
#  include <windows.h>
# else
#  if HAVE_SYS_MMAN_H
#   include <sys/mman.h>
#  endif
# endif

//<<<<<< PUBLIC DEFINES                                                 >>>>>>

#if !defined MAP_ANONYMOUS && defined MAP_ANON
# define MAP_ANONYMOUS MAP_ANON
#endif

//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>
//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // CLASSLIB_SYSAPI_FILE_MAPPING_H
