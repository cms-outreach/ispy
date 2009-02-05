//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/NamedPipeServer.h"
#include "classlib/iobase/FileSystemError.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/sysapi/NamedPipeServer.h"
#ifdef _WIN32

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
NamedPipeServer::sysinit (const char *name, FileAcl access)
{
    // FIXME: CreateNamedPipeServer -- gets handle
    // FIXME: non-blocking open needs CreateNamedPipeServer, not File!
    //   --> this needs to be a IOChannel of its own; required for
    //       IOSelector support anyway, but need to separate
    //       NamedPipeServerObject and NamedPipeServerChannel from each other
    static const int PIPE_BUF_SIZE = 4096; // Size reserved for buffers -- unix like
    if ((m_handle = CreateNamedPipe (name,
				     PIPE_ACCESS_DUPLEX
				     // | FILE_FLAG_OVERLAPPED (FIXME?)
				     // | FILE_FLAG_WRITE_THROUGH (FIXME?)
				     | FILE_FLAG_FIRST_PIPE_INSTANCE,
				     PIPE_TYPE_BYTE
				     // | PIPE_NOWAIT (for immediate *open*)
				     | PIPE_READMODE_BYTE,
				     PIPE_UNLIMITED_INSTANCES,
				     PIPE_BUF_SIZE, PIPE_BUF_SIZE,
				     0 /* default time out */,
				     0 /* FIXME security attrs */))
	== IOFD_INVALID)
	throw FileSystemError ("CreateNamedPipeServer()", GetLastError ());
}

void
NamedPipeServer::sysclose (void)
{
    // FIXME: Destroy pipe object -- just CloseHandle()?
    CloseHandle (m_handle);
    m_handle = IOFD_INVALID;
}

void
NamedPipeServer::sysdetach (void)
{ m_handle = IOFD_INVALID; }

} // namespace lat
#endif // _WIN32
