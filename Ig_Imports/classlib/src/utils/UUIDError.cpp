//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/utils/UUIDError.h"

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

UUIDError::UUIDError (void)
    : Error (0)
{}

std::string
UUIDError::explainSelf (void) const
{ return "UUID Format Error"; }

Error *
UUIDError::clone (void) const
{ return new UUIDError (*this); }

void
UUIDError::rethrow (void)
{ throw *this; }

} // namespace lat
