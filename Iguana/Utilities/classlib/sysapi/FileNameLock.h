#ifndef CLASSLIB_SYSAPI_FILE_NAME_LOCK_H
# define CLASSLIB_SYSAPI_FILE_NAME_LOCK_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"
# include <cerrno>
# ifndef _WIN32
#  include <utmp.h>
#  if HAVE_SYS_SYSCTL_H
#   include <sys/sysctl.h>
#  endif
# endif

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>
//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // CLASSLIB_SYSAPI_FILE_NAME_LOCK_H
