#ifndef CLASSLIB_INET_SERVER_SOCKET_H
# define CLASSLIB_INET_SERVER_SOCKET_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/iobase/InetSocket.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Internet domain socket set up to listen as a server.  */
class InetServerSocket : public InetSocket
{
public:
    InetServerSocket (const InetAddress &bindAddr,
		      int backlog = 1,
		      int type = SocketConst::TypeStream);

protected:
    virtual Socket *	copy (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_INET_SERVER_SOCKET_H
