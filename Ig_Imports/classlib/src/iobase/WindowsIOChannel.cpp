//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/IOChannel.h"
#include "classlib/iobase/IOError.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/sysapi/IOChannel.h"
#include "IOSelectWindows.h"
#ifdef _WIN32

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

IOSize
IOChannel::read (void *into, IOSize n)
{
    DWORD read;
    if (! ReadFile (fd (), into, n, &read, 0))
	throw IOError ("ReadFile()", GetLastError ());

    return read;
}

IOSize
IOChannel::readv (IOBuffer *into, IOSize buffers)
{ return IOInput::readv (into, buffers); } // FIXME: WIN32 implementation?

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
IOSize
IOChannel::write (const void *from, IOSize n)
{
    DWORD written;
    if (! WriteFile (fd (), from, n, &written, 0))
	throw IOError ("WriteFile()", GetLastError ());

    return written;
}

IOSize
IOChannel::writev (const IOBuffer *from, IOSize buffers)
{ return IOOutput::writev (from, buffers); } // FIXME: WIN32 implementation?

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
IOChannel::setBlocking (bool value)
{
    // FIXME: How about pipes etc?
    unsigned long mode = value;
    if (ioctlsocket ((SOCKET) fd (), FIONBIO, &mode) == -1)
	throw IOError ("ioctlsocket()", errno);
}

bool
IOChannel::isBlocking (void) const
{
    // FIXME
    return true;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
IOChannel::select (IOSelectMethod *to, const IOSelectRequest &req)
{
    IOSelectWindows *wto = dynamic_cast<IOSelectWindows *> (to);
    ASSERT (wto);

    // FIXME: check file handle type and provide appropriate selector
    // handler
    wto->attach (req);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
bool
IOChannel::sysclose (IOFD fd, int *error /* = 0 */)
{
    bool ret = CloseHandle (fd);
    if (error) *error = GetLastError ();
    return ret;
}

} // namespace lat
#endif // _WIN32
