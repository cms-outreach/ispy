//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/zip/ZError.h"
#include "classlib/utils/BitTraits.h"
#include <zlib.h>
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

/** Construct a new error object from the ZLIB error @a code. */
ZError::ZError (int code, const char *message /* = 0 */)
    : m_code (code)
{
    if (message)
    {
	m_message = " (";
	m_message += message;
	m_message += ')';
    }
}

/** Return the ZLIB error code: why this exception was thrown.  */
int
ZError::code (void) const
{ return m_code; }

/** Return the ZLIB error message: why this exception was thrown.  */
const std::string &
ZError::message (void) const
{ return m_message; }

/** Return a string describing this error.  FIXME: I18N? */
std::string
ZError::explainSelf (void) const
{
#define UNKNOWN_MESSAGE	"ZLIB: Unknown error %d"
    char	buf [sizeof(UNKNOWN_MESSAGE)-2 + BitTraits<int>::Digits];
    const char	*msg;

    switch (m_code)
    {
    case Z_VERSION_ERROR:
	msg = "ZLIB: Incompatible library version";
	break;

    case Z_STREAM_ERROR:
	msg = "ZLIB: Invalid parameter value";
	break;

    case Z_MEM_ERROR:
	msg = "ZLIB: Out of memory";
	break;

    case Z_DATA_ERROR:
	msg = "ZLIB: Corrupt stream data";
	break;

    case Z_BUF_ERROR:
	msg = "ZLIB: No buffer space available, progress not possible";
	break;

    // Something we don't know about
    default:
	sprintf (buf, UNKNOWN_MESSAGE, m_code);
	msg = buf;
    }

    return msg + m_message;
}

/** @reimp */
Error *
ZError::clone (void) const
{ return new ZError (*this); }

/** @reimp */
void
ZError::rethrow (void)
{ throw *this; }

} // namespace lat
