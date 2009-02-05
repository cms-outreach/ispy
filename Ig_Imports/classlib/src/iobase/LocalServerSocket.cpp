//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/LocalServerSocket.h"

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

LocalServerSocket::LocalServerSocket (const LocalAddress &bindAddr,
				      int backLog /* = 1 */,
				      int type /* = TypeStream */)
    : LocalSocket (type)
{ bind (bindAddr); listen (backLog); }

Socket *
LocalServerSocket::copy (void)
{ return new LocalServerSocket (*this); }

} // namespace lat
