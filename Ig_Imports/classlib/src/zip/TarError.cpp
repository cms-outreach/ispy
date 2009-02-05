//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/zip/TarError.h"
#include <string>

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

/** Construct a new TAR error object. */
TarError::TarError (void)
{}

/** Return a string describing this error.  FIXME: I18N? */
std::string
TarError::explainSelf (void) const
{ return "TAR: Bad archive format"; }

/** @reimp */
Error *
TarError::clone (void) const
{ return new TarError (*this); }

/** @reimp */
void
TarError::rethrow (void)
{ throw *this; }

} // namespace lat
