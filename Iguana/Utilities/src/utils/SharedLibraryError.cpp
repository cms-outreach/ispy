//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/utils/SharedLibraryError.h"
#include "classlib/utils/SystemError.h"

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

SharedLibraryError::SharedLibraryError (const char *context,
					const std::string &cause)
    : m_context (context ? context : ""),
      m_cause (cause)
{}

SharedLibraryError::SharedLibraryError (const char *context, int code)
    : Error (new SystemError (code)),
      m_context (context ? context : "")
{}

SharedLibraryError::SharedLibraryError (const char *context, Error *next)
    : Error (next),
      m_context (context ? context : "")
{}

SharedLibraryError::SharedLibraryError (Error *next)
    : Error (next)
{}

std::string
SharedLibraryError::explainSelf (void) const
{
    std::string message = "Shared library operation";
    if (! m_context.empty ())
    {
	message += " ";
	message += m_context;
    }

    message += " failed";

    if (! m_cause.empty ())
    {
	message += " because: ";
	message += m_cause;
    }

    return message;
}

Error *
SharedLibraryError::clone (void) const
{ return new SharedLibraryError (*this); }

void
SharedLibraryError::rethrow (void)
{ throw *this; }

} // namespace lat
