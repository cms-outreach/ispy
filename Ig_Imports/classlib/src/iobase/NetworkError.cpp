//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/NetworkError.h"

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

NetworkError::NetworkError (const char *context, int code /* = 0 */)
    : IOError (context, code)
{}

NetworkError::NetworkError (const char *context, Error *chain)
    : IOError (context, chain)
{}

NetworkError::NetworkError (Error *chain)
    : IOError (chain)
{}

std::string
NetworkError::explainSelf (void) const
{ return IOError::doexplain ("Network operation"); }

Error *
NetworkError::clone (void) const
{ return new NetworkError (*this); }

void
NetworkError::rethrow (void)
{ throw *this; }

} // namespace lat
