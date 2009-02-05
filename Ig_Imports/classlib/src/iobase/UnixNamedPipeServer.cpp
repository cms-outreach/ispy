//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/NamedPipeServer.h"
#include "classlib/iobase/FileSystemError.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/sysapi/NamedPipeServer.h"
#ifndef _WIN32

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
    // FIXME: #if HAVE_MKFIFO
    if (mkfifo (name, access.native ()) == -1)
	throw FileSystemError ("mkfifo()", errno);

    // FIXME: #elif HAVE_MKNOD
    // if (mknod (name, access | S_IFIFO, 0) == -1)
    //   throw FileSystemError ("mknod()", errno);

    // FIXME: #else what?
}

void
NamedPipeServer::sysclose (void)
{
    if (unlink (m_name.c_str ()) < 0)
	throw FileSystemError ("unlink()", errno);
}

void
NamedPipeServer::sysdetach (void)
{}

} // namespace lat
#endif // !_WIN32
