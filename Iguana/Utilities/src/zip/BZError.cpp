//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/zip/BZError.h"
#include "classlib/utils/BitTraits.h"
#define BZ_NO_STDIO
#include <bzlib.h>
#include <string>
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

/** Construct a new error object from the BZLIB error @a code. */
BZError::BZError (int code)
    : m_code (code)
{}

/** Return the BZLIB error code: why this exception was thrown.  */
int
BZError::code (void) const
{ return m_code; }

/** Return a string describing code().

    FIXME: I18N? */
std::string
BZError::explainSelf (void) const
{
#define UNKNOWN_MESSAGE	"BZLIB: Unknown error %d"
    char buf [sizeof(UNKNOWN_MESSAGE)-2 + BitTraits<int>::Digits];

    switch (m_code)
    {
    // Low-level/common interface errors
    case BZ_SEQUENCE_ERROR:
	return "BZLIB: Operation not permitted at this time";

    case BZ_PARAM_ERROR:
	return "BZLIB: Invalid parameter value";

    case BZ_MEM_ERROR:
	return "BZLIB: Out of memory";

    case BZ_DATA_ERROR:
	return "BZLIB: Corrupt compressed input data";

    case BZ_DATA_ERROR_MAGIC:
	return "BZLIB: Corrupt compressed input data header";

    // High-level interface errors 
    case BZ_IO_ERROR:
	return "BZLIB: I/O Error";

    case BZ_UNEXPECTED_EOF:
	return "BZLIB: Unexpected end of input data";

    case BZ_OUTBUFF_FULL:
	return "BZLIB: Output buffer full";

    // Something we don't know about
    default:
	sprintf (buf, UNKNOWN_MESSAGE, m_code);
	return buf;
    }
}

Error *
BZError::clone (void) const
{ return new BZError (*this); }

void
BZError::rethrow (void)
{ throw *this; }

} // namespace lat
