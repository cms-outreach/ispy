#ifndef CLASSLIB_LOCAL_SERVER_SOCKET_H
# define CLASSLIB_LOCAL_SERVER_SOCKET_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/iobase/LocalSocket.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Unix domain socket set up to listen as a server.  */
class LocalServerSocket : public LocalSocket
{
public:
    LocalServerSocket (const LocalAddress &bindAddr,
		       int backlog = 1,
		       int type = SocketConst::TypeStream);

protected:
    virtual Socket *	copy (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_LOCAL_SERVER_SOCKET_H
