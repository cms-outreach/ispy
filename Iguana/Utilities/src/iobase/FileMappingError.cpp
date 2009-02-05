//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/FileMappingError.h"

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

FileMappingError::FileMappingError (const char *context, int code /* = 0 */)
    : IOError (context, code)
{}

FileMappingError::FileMappingError (const char *context, Error *chain)
    : IOError (context, chain)
{}

FileMappingError::FileMappingError (Error *chain)
    : IOError (chain)
{}

std::string
FileMappingError::explainSelf (void) const
{ return IOError::doexplain ("Memory mapping operation"); }

Error *
FileMappingError::clone (void) const
{ return new FileMappingError (*this); }

void
FileMappingError::rethrow (void)
{ throw *this; }

} // namespace lat
