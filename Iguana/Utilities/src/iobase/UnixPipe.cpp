//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/Pipe.h"
#include "classlib/iobase/IOError.h"
#include "classlib/sysapi/Pipe.h"
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
Pipe::open (void)
{
    int fds [2];
    if (::pipe (fds) == -1)
	throw IOError ("pipe()", errno);

    m_source.fd (fds [0]);
    m_sink.fd (fds [1]);
}

} // namespace lat
#endif // !_WIN32
