//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/utils/RegexpError.h"
#include "classlib/utils/Regexp.h"
#include "classlib/utils/BitTraits.h"
#include <cstdio>

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

RegexpError::RegexpError (const char *context, int syntax, int error)
    : m_context (context),
      m_syntax (syntax),
      m_error (error)
{}

RegexpError::RegexpError (const char *context, int syntax, const char *msg)
    : m_context (context),
      m_syntax (syntax),
      m_error (0),
      m_message (msg ? msg : "")
{}

std::string
RegexpError::explainSelf (void) const
{
    std::string message;

    switch (m_syntax)
    {
    case Regexp::Perl:
	message += "PERL";
	break;

    case Regexp::POSIXBasic:
	message += "POSIX basic";
	break;

    case Regexp::POSIXExtended:
	message += "POSIX extended";
	break;

    default:
	message += "Unknown";
	break;
    }

    message += " regular expression failed";
    if (m_context)
    {
	message += " in ";
	message += m_context;
    }

    if (m_message.length ())
    {
	message += " with error message: ";
	message += m_message;
    }
    else
    {
	message += " with error code ";
	char buf [BitTraits<int>::Digits + 1];
	sprintf (buf, "%d", m_error);
	message += buf;
    }

    return message;
}

Error *
RegexpError::clone (void) const
{ return new RegexpError (*this); }

void
RegexpError::rethrow (void)
{ throw *this; }

} // namespace lat
