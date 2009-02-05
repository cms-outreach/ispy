#ifndef CLASSLIB_SYSAPI_HOST_INFO_H
# define CLASSLIB_SYSAPI_HOST_INFO_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"
# include <cctype>
# ifdef _WIN32
#  include <windows.h>
#  include <wininet.h>
# else
#  include <netdb.h>
#  include <unistd.h>
#  include <sys/param.h>
# endif

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>

#if !HAVE_GETHOSTNAME_DECL && ! defined _WIN32
extern "C" int gethostname (char *, STDC::size_t);
#endif

//<<<<<< CLASS DECLARATIONS                                             >>>>>>
//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // CLASSLIB_SYSAPI_HOST_INFO_H
