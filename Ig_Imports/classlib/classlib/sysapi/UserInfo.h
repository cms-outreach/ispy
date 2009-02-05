#ifndef CLASSLIB_SYSAPI_USER_INFO_H
# define CLASSLIB_SYSAPI_USER_INFO_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"
# include <cerrno>
# include <cstdlib>

# ifdef _WIN32
#  include <windows.h>
#  include <wininet.h>
#  include <ntsecapi.h>
#  include <lm.h>
# else
#  if HAVE_PWD_H
#   include <pwd.h>
#  endif
#  include <unistd.h>
# endif

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>
//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // CLASSLIB_SYSAPI_USER_INFO_H
