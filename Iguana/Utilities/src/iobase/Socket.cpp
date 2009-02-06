//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/Socket.h"
#include "classlib/iobase/NetworkError.h"
#include "classlib/utils/HostInfo.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/sysapi/Socket.h"
#include <climits>

namespace lat {
using namespace SocketConst;

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

SocketAddress::~SocketAddress (void)
{ }

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

Socket::Socket (void)
    : m_extra (0),
      m_oob (false)
{}

Socket::~Socket (void)
{ abort (); }

/** Indicate whether subsequent reads and writes to the socket should
    operate on out-of-band data.  */
void
Socket::oob (bool process /* = true */)
{ m_oob = process; }

/** Shut down all or part of a full-duplex connection to the socket.

    @param mode Close mode telling what actions will be prohibited. */
void
Socket::shutdown (int mode)
{
    ASSERT (fd () != IOFD_INVALID);
    ASSERT (mode == CloseDisallowReceive
	    || mode == CloseDisallowSend
	    || mode == CloseDisallowAll);

    if (::shutdown (SOCKETFD (), mode) < 0 && ERRNO != ENOTCONN)
	throw NetworkError ("shutdown()", ERRNO);
}


/** Close the socket in shutdown() mode #DisallowAll. */
void
Socket::close (void)
{
    shutdown (CloseDisallowAll);
    CLOSE (SOCKETFD ());
    fd (IOFD_INVALID);
    m_oob = false;
    // FIXME: Win32 event?
}

/** Like #close() but ignores all errors. */
void
Socket::abort (void)
{
    if (fd () != IOFD_INVALID)
	try 
	{
	    shutdown (CloseDisallowAll);
	    CLOSE (SOCKETFD ());
	    fd (IOFD_INVALID);
	    m_oob = false;
	    // FIXME: Win32 event?
	}
	catch (...)
	{
	}
}

/** Make this a server socket that can accept incoming connections.
    At most @a queuelen outstanding connection requests waiting at
    any time.  */
void
Socket::listen (int queuelen /* = 1 */)
{
    ASSERT (fd () != IOFD_INVALID);
    ASSERT (queuelen > 0);
    ASSERT (queuelen <= MaxListen);

    if (::listen (SOCKETFD (), queuelen) < 0)
	throw NetworkError ("listen()", ERRNO);
}

//////////////////////////////////////////////////////////////////////
/** Check if the given @a option (from #SocketConst namespace) actually
    exists on this platform.  Returns @c true if the option is valid. */
bool
Socket::hasopt (int option)
{ return option != INT_MIN; }

/** Get the value of an integer @a option for the socket.  The @a
    option is for protocol layer @a level.  The @a option should be
    one of the @c Opt values defined in #SocketConst namespace, or a
    value specific to the platform (see system manual pages for
    details).  Similarly, @a level should be one of the @c OptLevel
    values.  If the option is not valid on this platform, the function
    always returns zero; use #hasopt() to determine if the option
    actually exists if it matters to you.  Otherwise returns the value
    of the option; the option must be of @c int type. */
int
Socket::getopt (int option, int level /* = OptLevelSocket */) const
{
    ASSERT (fd () != IOFD_INVALID);

    // Ignore options that don't exist on this platform.  The caller
    // can always check themselves if they consider it fatal.
    if (! hasopt (option))
	return 0;

    int			data;
    SOCKOPT_LEN_TYPE	length = sizeof (data);
    if (::getsockopt (SOCKETFD (), level, option, (char *) &data, &length) < 0)
	throw NetworkError ("getsockopt()", ERRNO);
    return data;
}

/** Get the value of an @a option for the socket.  The @a option is
    for protocol layer @a level.  The @a option should be one of the
    @c Opt values defined in #SocketConst namespace, or a value
    specific to the platform (see system manual pages for details).
    Similarly, @a level should be one of the @c OptLevel values.  If
    the option is not valid on this platform, the function always
    returns zero; use #hasopt() to determine if the option actually
    exists if it matters to you.  Otherwise returns the number of
    bytes copied into @a p (which has at most @a n bytes of
    space).  */
int
Socket::getopt (int option, void *p, size_t n,
		int level /* = OptLevelSocket */) const
{
    ASSERT (fd () != IOFD_INVALID);

    // Ignore options that don't exist on this platform.  The caller
    // can always check themselves if they consider it fatal.
    if (! hasopt (option))
	return 0;

    SOCKOPT_LEN_TYPE length = n;
    if (::getsockopt (SOCKETFD (), level, option, (char *) p, &length) < 0)
	throw NetworkError ("getsockopt()", ERRNO);
    return length;
}

/** Set the value of an integer @a option for the socket.  The @a
    option is for protocol layer @a level.  The @a option should be
    one of the @c Opt values defined in #SocketConst namespace, or a
    value specific to the platform (see system manual pages for
    details).  Similarly, @a level should be one of the @c OptLevel
    values.  If the option is not valid on this platform, the request
    is ignored; use #hasopt() to determine if the option actually
    exists if it matters to you.  Otherwise sets the the value of the
    option to @a n; the option must be of @c int type. */
void
Socket::setopt (int option, int n, int level /* = OptLevelSocket */) const
{
    ASSERT (fd () != IOFD_INVALID);

    // Ignore options that don't exist on this platform.  The caller
    // can always check themselves if they consider it fatal.
    if (! hasopt (option))
	return;

    if (::setsockopt (SOCKETFD (), level, option, (char *) &n, sizeof (n)) < 0)
	throw NetworkError ("setsockopt()", ERRNO);
}

/** Set the value of an @a option for the socket.  The @a option is
    for protocol layer @a level.  The @a option should be one of the
    @c Opt values defined in #SocketConst namespace, or a value
    specific to the platform (see system manual pages for details).
    Similarly, @a level should be one of the @c OptLevel values.  If
    the option is not valid on this platform, the request is ignored;
    use #hasopt() to determine if the option actually exists if it
    matters to you.  Otherwise sets the option to the value pointed to
    by @a p and of size @a n bytes.  */
void
Socket::setopt (int option, const void *p, size_t n,
		int level /* = OptLevelSocket */) const
{
    ASSERT (fd () != IOFD_INVALID);

    // Ignore options that don't exist on this platform.  The caller
    // can always check themselves if they consider it fatal.
    if (! hasopt (option))
	return;

    if (::setsockopt (SOCKETFD (), level, option, (char *) p, n) < 0)
	throw NetworkError ("setsockopt()", ERRNO);
}

/** Get socket type.  */
int
Socket::type (void) const
{ return getopt (OptSockType); }

/** Get and set the socket linger time.  If @a time is negative, only
    gets the current value.  If @a time is zero, turns linger off.  If
    @a time is positive, sets the linger time to it.  Returns the
    previous linger time, or -1 if linger was off.  */
int
Socket::linger (int time /* = -1 */) const
{
    struct linger old;

    old.l_onoff = 0;
    old.l_linger = 0;
    getopt (OptSockLinger, &old, sizeof (old));

    if (time > 0)
    {
	struct linger newl;
	newl.l_onoff = 1;
	newl.l_linger = time;
	setopt (OptSockLinger, &newl, sizeof (newl));
    }
    else if (time == 0)
    {
	struct linger newl;
	newl.l_onoff = 0;
	newl.l_linger = old.l_linger;
	setopt (OptSockLinger, &newl, sizeof (newl));
    }
    return old.l_onoff ? old.l_linger : -1;
}

//////////////////////////////////////////////////////////////////////
/** Create a socket (with #OptSockReuseAddress enabled). */
void
Socket::create (int af, int type, int protocol)
{
    sysinit ();

    IOFD desc;
    if ((desc = (IOFD) ::socket (af, type, protocol)) == IOFD_INVALID)
	throw NetworkError ("socket()", ERRNO);

    fd (desc);
    setopt (OptSockReuseAddress, true);
}

/** Get the local name of this socket.  */
size_t
Socket::sockname (SocketAddress &addr)
{
    ASSERT (fd () != IOFD_INVALID);

    SOCKET_LEN_TYPE n = addr.size ();
    if (::getsockname (SOCKETFD (), addr.address (), &n) < 0)
	throw NetworkError ("getsockname()", ERRNO);

    return n;
}

/** Get the peer name of this socket.  */
size_t
Socket::peername (SocketAddress &addr)
{
    ASSERT (fd () != IOFD_INVALID);

    SOCKET_LEN_TYPE n = addr.size ();
    if (::getpeername (SOCKETFD (), addr.address (), &n) < 0)
	throw NetworkError ("getpeername()", ERRNO);

    return n;
}

/** Bind the socket to a specific unique address @a addr. */
void
Socket::bind (const SocketAddress &addr)
{
    ASSERT (fd () != IOFD_INVALID);

    if (::bind (SOCKETFD (), addr.address (), addr.size ()) < 0)
	throw NetworkError ("bind()", ERRNO);
}

/** Connect the socket to remote address @a addr.  */
void
Socket::connect (const SocketAddress &addr)
{
    ASSERT (fd () != IOFD_INVALID);

    if (::connect (SOCKETFD (), addr.address (), addr.size ()) < 0
	&& ERRNO != EINPROGRESS)
	throw NetworkError ("connect()", ERRNO);
}

/** Accept a remote connection to the socket.  This returns a copy of
    the socket object opened for the new connection.  Use #sockname()
    and #peername() to determine the addresses of the new socket.  */
Socket *
Socket::accept (void)
{
    ASSERT (fd () != IOFD_INVALID);

    // FIXME: what if the socket is non-blocking?
    // FIXME: linux says pending errors on the socket will be
    // returned from accept and should be treated as EAGAIN?
    struct { sockaddr prefix; char buf [256]; } from;
    SOCKET_LEN_TYPE	n = sizeof (from);
    IOFD		s;

    do
	s = (IOFD) ::accept (SOCKETFD (), (sockaddr *) &from, &n);
    while (s == IOFD_INVALID && ERRNO == EINTR);

    if (s < 0)
	throw NetworkError ("accept()", ERRNO);

    // Make a duplicate
    Socket *dup = copy ();
    dup->fd (s);
    dup->m_oob = false;
    return dup;
}

/** Read from the socket @a n bytes into buffer @a into.  */
IOSize
Socket::read (void *into, IOSize n)
{
    ASSERT (fd () != IOFD_INVALID);

    int status = ::recv (SOCKETFD (), (RECV_ARG_TYPE) into,
			 n, m_oob ? MSG_OOB : 0);

    if (status < 0)
	throw NetworkError ("recv()", ERRNO);

    return status;
}

/** Read from the socket, retrieving the source address, too.  */
IOSize
Socket::read (void *into, IOSize n, SocketAddress &source)
{
    ASSERT (fd () != IOFD_INVALID);

    SOCKET_LEN_TYPE	size = source.size ();
    int			status = ::recvfrom (SOCKETFD (),
					     (RECV_ARG_TYPE) into, n,
					     m_oob ? MSG_OOB : 0,
					     source.address (), &size);

    if (status < 0)
	throw NetworkError ("recvfrom()", ERRNO);

    return status;
}

/** Peek into the socket, retrieving data into buffer @a into.  */
IOSize
Socket::peek (IOBuffer into)
{ return peek (into.data (), into.size ()); }

/** Peek into the socket, retrieving data into buffer @a into.  */
IOSize
Socket::peek (void *into, IOSize n)
{
    ASSERT (fd () != IOFD_INVALID);

    int status = ::recv (SOCKETFD (), (RECV_ARG_TYPE) into, n,
			 MSG_PEEK | (m_oob ? MSG_OOB : 0));

    if (status < 0)
	throw NetworkError ("recv", ERRNO);
    
    return status;
}

/** Peek into the socket, retrieving the source address, too.  */
IOSize
Socket::peek (void *into, IOSize n, SocketAddress &source)
{
    ASSERT (fd () != IOFD_INVALID);

    SOCKET_LEN_TYPE	size = source.size ();
    int			status = ::recvfrom (SOCKETFD (),
					     (RECV_ARG_TYPE) into, n,
					     MSG_PEEK | (m_oob ? MSG_OOB : 0),
					     source.address (), &size);

    if (status < 0)
	throw NetworkError ("recvfrom()", ERRNO);
    
    return status > 0 ? status : 0;
}

/** Write to the socket.  */
IOSize
Socket::write (const void *from, IOSize n)
{
    ASSERT (fd () != IOFD_INVALID);

    int s;
    int written = 0;
    for (; n > 0; n -= s, written += s)
    {
	const unsigned char *start = (const unsigned char *) from + written;
	if ((s = ::send (SOCKETFD (), (const RECV_ARG_TYPE) start,
			 n, m_oob ? MSG_OOB : 0)) < 0)
	{
	    if (written)
		break;
	    else
		throw NetworkError ("send()", ERRNO);
	}
    }

    return written;
}

/** Writes to the socket with a specific destination address.  */
IOSize
Socket::write (const void *from, IOSize n, const SocketAddress &dest)
{
    ASSERT (fd () != IOFD_INVALID);

    int s;
    int written = 0;
    for (; n > 0; n -= s, written += s)
    {
	const unsigned char *start = (const unsigned char *) from + written;
	if ((s = ::sendto (SOCKETFD (), (const RECV_ARG_TYPE) start,
			   n, m_oob ? MSG_OOB : 0, dest.address (),
			   dest.size ())) < 0)
	{
	    if (written)
		break;
	    else
		throw NetworkError ("sendto()", ERRNO);
	}
    }

    return written;
}

} // namespace lat
