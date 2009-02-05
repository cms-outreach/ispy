//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/Pipe.h"
#include "classlib/iobase/IOError.h"
#include "classlib/sysapi/Pipe.h"
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
Pipe::open (void)
{
    // make the pipe handles inherited
    SECURITY_ATTRIBUTES	security; 
    security.nLength = sizeof (security); 
    security.bInheritHandle = TRUE; 
    security.lpSecurityDescriptor = NULL; 

    HANDLE fds [2];
    if (! CreatePipe (&fds[0], &fds [1], &security, 0))
	throw IOError ("CreatePipe()", GetLastError ());

    m_source.fd (fds [0]);
    m_sink.fd (fds [1]);
}

} // namespace lat
#endif // _WIN32
