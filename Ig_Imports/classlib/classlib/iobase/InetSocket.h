#ifndef CLASSLIB_INET_SOCKET_H
# define CLASSLIB_INET_SOCKET_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/iobase/Socket.h"
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

struct sockaddr_in;

namespace lat {
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

// FIXME: This is IPv4, need IPv6 too; share a common base class?
class InetAddress : public SocketAddress
{
public:
    InetAddress (unsigned short port = 0);
    InetAddress (unsigned long address, unsigned short port);
    InetAddress (unsigned long address, const char *service,
		 const char *proto = "tcp");
    InetAddress (const char *host, unsigned short port);
    InetAddress (const char *host, const char *service,
		 const char *proto = "tcp");
    InetAddress (const InetAddress &a);
    ~InetAddress (void);
    InetAddress &operator= (const InetAddress &x);

    virtual size_t	size (void) const;
    virtual int		family (void) const;
    virtual sockaddr *	address (void) const;

    int			port (void) const;
    std::string		hostname (void) const;

protected:
    void		port (const char *service, const char *proto = "tcp");
    void		address (const char *host);

private:
    sockaddr_in		*m_address;
};

//////////////////////////////////////////////////////////////////////
/** Internet domain stream socket.  */
class InetSocket : public Socket
{
public:
    InetSocket (void);
    InetSocket (int type, int protocol = 0);
    InetSocket (const InetAddress &to,
		int type = SocketConst::TypeStream,
		int protocol = 0);
    ~InetSocket (void);

    virtual void	create (int type = SocketConst::TypeStream,
				int protocol = 0);

    virtual void	bind (const InetAddress &a);
    virtual void	connect (const InetAddress &a);

    virtual InetAddress	sockname (void);
    virtual InetAddress	peername (void);

    // Overrides
    using Socket::read;
    using Socket::peek;
    using Socket::write;

    virtual IOSize	read (void *into, IOSize n, InetAddress &source);
    IOSize		read (IOBuffer into, InetAddress &source);
    virtual IOSize	peek (void *into, IOSize n, InetAddress &source);
    IOSize		peek (IOBuffer into, InetAddress &source);
    virtual IOSize	write (const void *from, IOSize n, const InetAddress &dest);
    IOSize		write (IOBuffer from, const InetAddress &dest);

protected:
    virtual Socket *	copy (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_INET_SOCKET_H
