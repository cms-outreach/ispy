//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/Pipe.h"
#include "classlib/iobase/IOError.h"
#include "classlib/sysapi/Pipe.h"

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

Pipe::Pipe (bool create /* = true */)
{ if (create) open (); }

IOChannel *
Pipe::source (void)
{ return &m_source; }

IOChannel *
Pipe::sink (void)
{ return &m_sink; }

} // namespace lat
