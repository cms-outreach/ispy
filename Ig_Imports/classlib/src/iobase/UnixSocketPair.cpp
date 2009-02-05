//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/SocketPair.h"
#include "classlib/iobase/NetworkError.h"
#include "classlib/sysapi/SocketPair.h"
#ifndef _WIN32

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
SocketPair::open (void)
{
    // FIXME: what if there is no socketpair -- just simulate it?
    // FIXME: domain, type, ...?
    int fds [2];
    if (::socketpair (AF_UNIX, SOCK_STREAM, 0, fds) == -1)
	throw NetworkError ("socketpair()", errno);

    m_source.fd (fds [0]);
    m_sink.fd (fds [1]);
}

} // namespace lat
#endif // !_WIN32
