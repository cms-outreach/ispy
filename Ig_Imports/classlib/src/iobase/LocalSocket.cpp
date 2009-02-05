//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/LocalSocket.h"
#include "classlib/utils/SystemError.h"
#include "classlib/utils/HostInfo.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/sysapi/LocalSocket.h"

#if !HAVE_LOCAL_SOCKETS
// Stub type just to keep compiler happy.  Don't expect this to actually work!
struct sockaddr_un { int sun_family; char sun_path [1]; };
#endif

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Create a UNIX domain address bound to the give path.  */
LocalAddress::LocalAddress (const char *path /* = "" */)
    : m_address (new sockaddr_un)
{
#if !HAVE_LOCAL_SOCKETS
    ASSERT (false);
#endif

    m_address->sun_family = AF_UNIX;
    ::strcpy (m_address->sun_path, path);
}

/** Copy a UNIX domain address.  */
LocalAddress::LocalAddress (const LocalAddress &a)
    : SocketAddress (), m_address (new sockaddr_un)
{
    m_address->sun_family = AF_UNIX;
    ::strcpy (m_address->sun_path, a.m_address->sun_path);
}

LocalAddress::~LocalAddress (void)
{ delete m_address; }

size_t
LocalAddress::size (void) const
{ return sizeof (sockaddr_un) - sizeof (m_address->sun_path)
      + strlen (m_address->sun_path); }

int
LocalAddress::family (void) const
{ return m_address->sun_family; }

sockaddr *
LocalAddress::address (void) const
{
#if !HAVE_LOCAL_SOCKETS
    ASSERT (false);
#endif
    return (sockaddr *) m_address;
}

const char *
LocalAddress::path (void) const
{ return m_address->sun_path; }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
LocalSocket::LocalSocket (void)
{}

LocalSocket::LocalSocket (int type, int protocol /* = 0 */)
{ Socket::create (AF_UNIX, type, protocol); }

LocalSocket::LocalSocket (const LocalAddress &to,
			  int type /* = TypeStream */,
			  int protocol /* = 0 */)
{ Socket::create (AF_UNIX, type, protocol); Socket::connect (to); }

LocalSocket::~LocalSocket (void)
{ }

/// @reimp
void
LocalSocket::create (int type /* = Stream */, int protocol /* = 0*/)
{ Socket::create (AF_UNIX, type, protocol); }

/// @reimp
void
LocalSocket::bind (const LocalAddress &a)
{ Socket::bind (a); }

/// @reimp
void
LocalSocket::connect (const LocalAddress &a)
{ Socket::connect (a); }

/** Retrieve the local address of the socket.  */
LocalAddress
LocalSocket::sockname (void)
{
    // FIXME: is this right?  do we need to adjust things for path length?
    LocalAddress addr;
    Socket::sockname (addr);
    return addr;
}

/** Retrieve the address of the peer of the socket.  */
LocalAddress
LocalSocket::peername (void)
{
    // FIXME: is this right?  do we need to adjust things for path length?
    LocalAddress addr;
    Socket::peername (addr);
    return addr;
}

/// @reimp
IOSize
LocalSocket::read (void *into, IOSize n, LocalAddress &source)
{ return Socket::read (into, n, source); }

/// @reimp
IOSize
LocalSocket::read (IOBuffer into, LocalAddress &source)
{ return read (into.data (), into.size (), source); }

/// @reimp
IOSize
LocalSocket::peek (void *into, IOSize n, LocalAddress &source)
{ return Socket::peek (into, n, source); }

/// @reimp
IOSize
LocalSocket::peek (IOBuffer into, LocalAddress &source)
{ return peek (into.data (), into.size (), source); }

/// @reimp
IOSize
LocalSocket::write (const void *from, IOSize n, const LocalAddress &dest)
{ return Socket::write (from, n, dest); }

/// @reimp
IOSize
LocalSocket::write (IOBuffer from, const LocalAddress &dest)
{ return write (from.data (), from.size (), dest); }

/// @reimp
Socket *
LocalSocket::copy (void)
{ return new LocalSocket (*this); }

} // namespace lat
