//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/zip/CPIOError.h"
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

/** Construct a new CPIO error object. */
CPIOError::CPIOError (void)
{}

/** Return a string describing this error.  FIXME: I18N? */
std::string
CPIOError::explainSelf (void) const
{ return "CPIO: Bad archive format"; }

/** @reimp  */
Error *
CPIOError::clone (void) const
{ return new CPIOError (*this); }

/** @reimp  */
void
CPIOError::rethrow (void)
{ throw *this; }

} // namespace lat
