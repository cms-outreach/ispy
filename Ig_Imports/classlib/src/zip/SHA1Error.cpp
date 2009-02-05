//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/zip/SHA1Error.h"
#include "classlib/utils/StringFormat.h"

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

/** Construct a new SHA1 error object. */
SHA1Error::SHA1Error (int error)
    : m_error (error)
{}

/** Return a string describing this error.  FIXME: I18N? */
std::string
SHA1Error::explainSelf (void) const
{ return StringFormat ("SHA1: Error %1").arg (m_error).value (); }

/** @reimp  */
Error *
SHA1Error::clone (void) const
{ return new SHA1Error (*this); }

/** @reimp  */
void
SHA1Error::rethrow (void)
{ throw *this; }

} // namespace lat
