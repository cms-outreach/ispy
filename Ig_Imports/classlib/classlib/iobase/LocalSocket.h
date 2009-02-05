#ifndef CLASSLIB_LOCAL_SOCKET_H
# define CLASSLIB_LOCAL_SOCKET_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/iobase/Socket.h"
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

struct sockaddr_un;

namespace lat {
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class LocalAddress : public SocketAddress
{
public:
    LocalAddress (const std::string &path);
    LocalAddress (const char *path = "");
    LocalAddress (const LocalAddress &a);
    ~LocalAddress (void);
    LocalAddress &operator= (const LocalAddress &x);

    virtual size_t	size (void) const;
    virtual int		family (void) const;
    virtual sockaddr *	address (void) const;

    const char *	path (void) const;

private:
    sockaddr_un		*m_address;
};

//////////////////////////////////////////////////////////////////////
/** Unix domain stream sockets bound to file system paths. */
class LocalSocket : public Socket
{
public:
    LocalSocket (void);
    LocalSocket (int type, int protocol = 0);
    LocalSocket (const LocalAddress &to,
		 int type = SocketConst::TypeStream,
		 int protocol = 0);
    ~LocalSocket (void);
    
    void		create (int type = SocketConst::TypeStream,
				int protocol = 0);

    void		bind (const LocalAddress &a);
    void		connect (const LocalAddress &a);

    virtual LocalAddress sockname (void);
    virtual LocalAddress peername (void);

    // Overrides
    using Socket::read;
    using Socket::peek;
    using Socket::write;

    virtual IOSize	read (void *into, IOSize n, LocalAddress &source);
    IOSize		read (IOBuffer into, LocalAddress &source);
    virtual IOSize	peek (void *into, IOSize n, LocalAddress &source);
    IOSize		peek (IOBuffer into, LocalAddress &source);
    virtual IOSize	write (const void *from, IOSize n, const LocalAddress &dest);
    IOSize		write (IOBuffer from, const LocalAddress &dest);

protected:
    virtual Socket *	copy (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_LOCAL_SOCKET_H
