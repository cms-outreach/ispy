//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/SubProcessError.h"
#include "classlib/utils/SystemError.h"
#include "classlib/utils/DebugAids.h"

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

SubProcessError::SubProcessError (const char *context, int code /* = 0 */)
    : Error (code ? new SystemError (code) : 0),
      m_context (context ? context : "")
{}

SubProcessError::SubProcessError (const char *context, Error *chain)
    : Error (chain),
      m_context (context ? context : "")
{}

std::string
SubProcessError::explainSelf (void) const
{ return m_context + " failed"; } 

Error *
SubProcessError::clone (void) const
{ return new SubProcessError (*this); }

void
SubProcessError::rethrow (void)
{ throw *this; }

} // namespace lat
