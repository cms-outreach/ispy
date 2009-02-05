//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/FilenameError.h"
#include "classlib/utils/DebugAids.h"

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

/** Construct a filename expansion error.  */
FilenameError::FilenameError (Reason cause, const std::string &spec /* = "" */)
    : m_reason (cause),
      m_spec (spec)
{}

/** Return the cause for this error.  */
FilenameError::Reason
FilenameError::reason (void) const
{ return m_reason; }

/** Return the variable reference that triggered this error.  For
    #BAD_SYNTAX the value is empty.  */
const std::string &
FilenameError::variable (void) const
{ return m_spec; }

/** Return a printable explanation describing this error.  */
std::string
FilenameError::explainSelf (void) const
{
    switch (m_reason)
    {
    case UNTERMINATED_VAR:
	return std::string ("no closing `}' in variable reference `")
	    + m_spec + "'";

    case BAD_SYNTAX:
	return std::string ("`$' not followed by a variable name or `$'");

    case NO_SUCH_VAR:
	return std::string ("variable `")
	    + m_spec + "' referenced but has no value";

    default:
	ASSERT (false);
	return "oops, internal error -- unknown reason";
    }
}

/** @reimp  */
Error *
FilenameError::clone (void) const
{ return new FilenameError (*this); }

/** @reimp */
void
FilenameError::rethrow (void)
{ throw *this; }

} // namespace lat
