//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/InetServerSocket.h"

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

// FIXME: Default ctor + open?

InetServerSocket::InetServerSocket (const InetAddress &bindAddr,
				    int backLog /* = 1 */,
				    int type /* = TypeStream */)
    : InetSocket (type)
{ bind (bindAddr); listen (backLog); }

Socket *
InetServerSocket::copy (void)
{ return new InetServerSocket (*this); }

} // namespace lat
