//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/Socket.h"
#include "classlib/iobase/NetworkError.h"
#include "classlib/sysapi/Socket.h"
#include "IOSelectWindows.h"
#ifdef _WIN32

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

void
Socket::select (IOSelectMethod *to, const IOSelectRequest &req)
{
    IOSelectWindows *wto = dynamic_cast<IOSelectWindows *> (to);
    ASSERT (wto);

    // FIXME: provide appropriate selector handler
    wto->attach (req);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
Socket::sysinit (void)
{
    // FIXME: WSACleanup?
    static bool init = false;
    if (! init)
    {
	int	err;
	WSADATA wsaData;

	if ((err = ::WSAStartup (WS_VERSION_REQD, &wsaData)))
	    throw NetworkError ("WSAStartup()", err);

	if ((LOBYTE (wsaData.wVersion) < WS_VERSION_MAJOR)
	    || (LOBYTE (wsaData.wVersion) == WS_VERSION_MAJOR
		&& HIBYTE (wsaData.wVersion) < WS_VERSION_MINOR)
	    || wsaData.iMaxSockets < MIN_SOCKETS_REQD)
	{
	    WSACleanup();
	    throw NetworkError ("WSA version");
	}
	init = true;
    }
}

} // namespace lat
#endif // _WIN32
