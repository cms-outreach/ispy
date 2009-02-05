#ifndef CLASSLIB_SYSAPI_SOCKET_H
# define CLASSLIB_SYSAPI_SOCKET_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"
# ifdef _WIN32
#  include <windows.h>
# else
#  include <sys/param.h>
#  include <sys/socket.h>
#  include <netinet/in.h>
#  include <netinet/tcp.h>
#  include <unistd.h>
#  include <fcntl.h>
#  if HAVE_SYS_FILIO_H
#   include <sys/filio.h>
#  endif
#  if HAVE_SYS_SOCKIO_H
#   include <sys/sockio.h>
#  endif
#  include <cerrno>
#  include <cstring>
# endif

//<<<<<< PUBLIC DEFINES                                                 >>>>>>

# ifdef _WIN32
#  define WS_VERSION_REQD 	0x0200
#  define WS_VERSION_MAJOR	HIBYTE(WS_VERSION_REQD)
#  define WS_VERSION_MINOR	LOBYTE(WS_VERSION_REQD)

#  define ERRNO			WSAGetLastError()
#  define CLOSE			::closesocket

#  define EWOULDBLOCK		WSAEWOULDBLOCK
#  define ENOTCONN		WSAENOTCONN
#  define EINPROGRESS		WSAEINPROGRESS

#  ifndef MIN_SOCKETS_REQD
#   define MIN_SOCKETS_REQD	1
#  endif

#  define SOCKETFD()		(SOCKET) fd ()
# else
#  define ERRNO			errno
#  define CLOSE			::close
#  define SOCKETFD()		fd ()
# endif

//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>
//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // CLASSLIB_SYSAPI_SOCKET_H

