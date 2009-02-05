//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/IOError.h"
#include "classlib/utils/SystemError.h"

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

IOError::IOError (const char *context, int code /* = 0 */)
    : Error (code ? new SystemError (code) : 0),
      m_context (context ? context : "")
{}

IOError::IOError (const char *context, Error *chain)
    : Error (chain),
      m_context (context ? context : "")
{}

IOError::IOError (Error *chain)
    : Error (chain)
{}

std::string
IOError::explainSelf (void) const
{ return doexplain ("I/O operation"); }

std::string
IOError::doexplain (const char *type) const
{
    std::string message (type);

    if (! m_context.empty ())
    {
	message += ' ';
	message += m_context;
    }

    message += " failed";

    return message;
}

Error *
IOError::clone (void) const
{ return new IOError (*this); }

void
IOError::rethrow (void)
{ throw *this; }

} // namespace lat
