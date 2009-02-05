#ifndef CLASSLIB_SYSAPI_INET_SOCKET_H
# define CLASSLIB_SYSAPI_INET_SOCKET_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"
# ifdef _WIN32
#  include <windows.h>
# else
#  include <sys/socket.h>
#  include <netinet/in.h>
#  include <netdb.h>
#  include <unistd.h>
#  include <arpa/inet.h>
#  include <cerrno>
# endif

# include <cstring>
# include <cctype>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>

// FIXME: h_errno vs. errno?

#ifdef _WIN32
# define ERRNO			WSAGetLastError()
#else
# define ERRNO			errno
#endif

//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>
//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // CLASSLIB_SYSAPI_INET_SOCKET_H
