//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/InetSocket.h"
#include "classlib/iobase/NetworkError.h"
#include "classlib/utils/HostInfo.h"
#include "classlib/sysapi/InetSocket.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>

#if HAVE_INET_ATON && !HAVE_INET_ATON_DECL
extern "C" int inet_aton (const char *, struct in_addr *);
#elif !HAVE_INET_ATON

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>

/** Convert internet numeric dot notation address into	an address structure.

    @param psz The address string.
    @param addr The resulting address.

    @return @c true if the address is valid, @c false otherwise.

    @note   This implementation is a minimal one for losing platforms
    	    without an appropriate definition.  It doesn't handle all
	    obscure cases or IPv6.  */
static int
inet_aton (const char *psz, struct in_addr *addr)
{
    // FIXME: Russ Alberry's code?

    static const char	lcdigits [] = "0123456789abcdef";
    static const char	ucdigits [] = "0123456789ABCDEF";
    unsigned int	naddress = 0;
    unsigned long	address [4];
    unsigned long	digit = 0;

    while (true)
    {
	unsigned int	base = 10;
	unsigned int	ch;

	// check radix
	if (*psz == '0')
	{
	    if (*++psz == 'x' || *psz == 'X')
	    {
		base = 16;
		psz++;
	    }
	    else
		base = 8;
	}

	// collect next digit
	while ((ch = *psz) != '\0')
	{
	    unsigned int d = 0;
	    while (d < base && lcdigits[d] != ch && ucdigits[d] != ch)
		++d;

	    if (d >= base)
		break;

	    digit = digit*base + d;
	}

	// check what to do with it
	if (*psz == '.')
	{
	    // a.b.c.d / a.b.c (c = 16 bits) / a.b (b = 24 bits)
	    if (naddress >= 3 || digit > 0xff)
		return false;
	    address [naddress++] = digit;
	    ++psz;
	}
	else
	    break;
    }

    // check for trailing garbage
    while (*psz && isspace (*psz))
	++psz;

    if (*psz)
	return false;

    // create an address based on the number of given parts
    unsigned long value = 0;
    switch (naddress + 1) {
    case 1:
	value = digit;
	break;

    case 2:
	if (digit > 0xffffff)
	    return false;
	value = (address[0] << 24) | digit;
	break;

    case 3:
	if (digit > 0xffff)
	    return false;
	value = (address[0] << 24) | (address[1] << 16) | digit;
	break;

    case 4:
	if (digit > 0xff)
	    return false;
	value = (address[0] << 24) | (address[1] << 16)
		| (address[2] << 8) | digit;
	break;

    case 5:
	if (digit != 0)
	    return false;
	value = (address[0] << 24) | (address[1] << 16)
		| (address[2] << 8) | (address[3]);
	break;
    }

    if (addr)
	addr->s_addr = htonl(value);

    return true;
}
#endif //!HAVE_INET_ATON

namespace lat {
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Create a local internet domain address to port number @a port.  */
InetAddress::InetAddress (unsigned short port /* = 0 */)
    : m_address (new sockaddr_in)
{
    m_address->sin_family      = AF_INET;
    m_address->sin_addr.s_addr = htonl (INADDR_ANY);
    m_address->sin_port        = htons (port);
}

/** Create an internet domain address to remote address @a address
    and port number @a port.  */
InetAddress::InetAddress (unsigned long address, unsigned short port /* = 0 */)
    : m_address (new sockaddr_in)
{
    m_address->sin_family      = AF_INET;
    m_address->sin_addr.s_addr = htonl (address);
    m_address->sin_port        = htons (port);
}

/** Create an internet domain address to a remote address with
    a named service using the named protocol.  */
InetAddress::InetAddress (unsigned long address,
			  const char *service,
			  const char *proto /* = "tcp" */)
    : m_address (new sockaddr_in)
{
    m_address->sin_family      = AF_INET;
    m_address->sin_addr.s_addr = htonl (address);
    port (service, proto);
}

/** Create an internet domain address to a named host and port.  */
InetAddress::InetAddress (const char *host, unsigned short port /* = 0 */)
    : m_address (new sockaddr_in)
{
    address (host);
    m_address->sin_port = htons (port);
}

/** Create an internet domain address to a named host using a
    named service using the named protocol.  */
InetAddress::InetAddress (const char *host,
			  const char *service,
			  const char *proto /* = "tcp" */)
    : m_address (new sockaddr_in)
{
    address (host);
    port (service, proto);
}

/** Copy an internet domain address.  */
InetAddress::InetAddress (const InetAddress &a)
    : SocketAddress (), m_address (new sockaddr_in)
{
    m_address->sin_family      = AF_INET;
    m_address->sin_addr.s_addr = a.m_address->sin_addr.s_addr;
    m_address->sin_port        = a.m_address->sin_port;
}

InetAddress::~InetAddress (void)
{ delete m_address; }

size_t
InetAddress::size (void) const
{ return sizeof (sockaddr_in); }

int
InetAddress::family (void) const
{ return m_address->sin_family; }

sockaddr *
InetAddress::address (void) const
{ return (sockaddr *) m_address; }

/** Figure out the port number using service name and the name
    of the protocol used with that service. */
void
InetAddress::port (const char *service, const char *proto /* ="tcp" */)
{
    if (servent *p = getservbyname (service, proto))
	m_address->sin_port = p->s_port;
    else
	throw NetworkError ("getservbyname()", ERRNO);
    
}

/** Get the port number in host byte order.  */
int
InetAddress::port (void) const
{ return ntohs (m_address->sin_port); }

/** Figure out the internet address of a named remote host (either in
    dot notation or a symbolic name).  */
void
InetAddress::address (const char *host)
{
    hostent *h;
    
    // FIXME: check against "255.255.255.255" for errors?
    // FIXME: h_errno vs errno
    if (::inet_aton (host, &m_address->sin_addr))
    {
	if (! (h = ::gethostbyaddr ((const char *) &m_address->sin_addr,
				    sizeof (m_address->sin_addr),
				    family ())))
	    throw NetworkError ("gethostbyaddr()", ERRNO);
    }
    else if (! (h = ::gethostbyname (host)))
	throw NetworkError ("gethostbyname()", ERRNO);

    ::memcpy (&m_address->sin_addr, h->h_addr_list[0], h->h_length);
    m_address->sin_family = h->h_addrtype;
}

/** Get the host name designated by the address.  Returns an empty
    string if the name cannot be resolved.  */
std::string
InetAddress::hostname (void) const
{
    // FIXME: h_errno vs errno
    hostent *h;
    if (m_address->sin_addr.s_addr == htonl (INADDR_ANY))
	return HostInfo::name ();
    else if (! (h = ::gethostbyaddr ((const char *) &m_address->sin_addr,
				     sizeof (m_address->sin_addr), family ()))
	     || ! h->h_name)
	return "";
    else
	return h->h_name;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
InetSocket::InetSocket (void)
{}

InetSocket::InetSocket (int type, int protocol /* = 0 */)
{ Socket::create (AF_INET, type, protocol); }

InetSocket::InetSocket (const InetAddress &to,
			int type /* = TypeStream */,
			int protocol /* = 0 */)
{ Socket::create (AF_INET, type, protocol); Socket::connect (to); }

InetSocket::~InetSocket (void)
{ }

/// @reimp
void
InetSocket::create (int type /* = Stream */, int protocol /* = 0*/)
{ Socket::create (AF_INET, type, protocol); }

/// @reimp
void
InetSocket::bind (const InetAddress &a)
{ Socket::bind (a); }

/// @reimp
void
InetSocket::connect (const InetAddress &a)
{ Socket::connect (a); }

/** Retrieve the local address of the socket.  */
InetAddress
InetSocket::sockname (void)
{
    InetAddress addr;
    Socket::sockname (addr);
    return addr;
}

/** Retrieve the address of the peer of the socket.  */
InetAddress
InetSocket::peername (void)
{
    InetAddress addr;
    Socket::peername (addr);
    return addr;
}

/// @reimp
IOSize
InetSocket::read (void *into, IOSize n, InetAddress &source)
{ return Socket::read (into, n, source); }

/// @reimp
IOSize
InetSocket::read (IOBuffer into, InetAddress &source)
{ return read (into.data (), into.size (), source); }

/// @reimp
IOSize
InetSocket::peek (void *into, IOSize n, InetAddress &source)
{ return Socket::peek (into, n, source); }

/// @reimp
IOSize
InetSocket::peek (IOBuffer into, InetAddress &source)
{ return peek (into.data (), into.size (), source); }

/// @reimp
IOSize
InetSocket::write (const void *from, IOSize n, const InetAddress &dest)
{ return Socket::write (from, n, dest); }

/// @reimp
IOSize
InetSocket::write (IOBuffer from, const InetAddress &dest)
{ return write (from.data (), from.size (), dest); }

/// @reimp
Socket *
InetSocket::copy (void)
{ return new InetSocket (*this); }

} // namespace lat
