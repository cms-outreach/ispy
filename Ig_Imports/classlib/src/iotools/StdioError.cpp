//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iotools/StdioError.h"
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

/** Construct an error object.  Simply constructs the chain.  */
StdioError::StdioError (int code /* = 0 */, Error *next /* = 0 */)
    : IOException (next),
      m_code (code)
{}

/** Provide an explanation of this error.  */
std::string
StdioError::explainSelf (void) const
{
    std::string text = "Standard C Stream Error";
    char buf [BitTraits<int>::Digits + 12];
    sprintf (buf, " (state = %d)", m_code);
    text += buf;

    return text;
}

/** @reimp  */
Error *
StdioError::clone (void) const
{ return new StdioError (*this); }

/** @reimp  */
void
StdioError::rethrow (void)
{ throw *this; }

} // namespace lat
