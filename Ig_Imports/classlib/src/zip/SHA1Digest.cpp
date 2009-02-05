//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/zip/SHA1Digest.h"
#include "classlib/zip/SHA1Error.h"
extern "C" {
  #include "sha1.h"
}

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>

struct SHA1Digest::Context : SHA1Context {};

//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Construct a new SHA1 digest.  */
SHA1Digest::SHA1Digest (void)
    : m_context (new Context)
{
    if (int err = SHA1Reset (m_context))
	throw SHA1Error (err);
}

/** Destroy the digest.  */
SHA1Digest::~SHA1Digest (void)
{ delete m_context; }

/** Return the final digest value.  */
Digest::Value
SHA1Digest::digest (void) const
{
    Value val (20, '\0');
    if (int err = SHA1Result (m_context, &val [0]))
	throw SHA1Error (err);
    return val;
}

/** Update the digest with @a data of length @a n.  */
void
SHA1Digest::update (const void *data, IOSize n)
{
    if (int err = SHA1Input (m_context, (const unsigned char *) data, n))
	throw SHA1Error (err);
}

/** Reset the digest.  */
void
SHA1Digest::reset (void)
{
    if (int err = SHA1Reset (m_context))
	throw SHA1Error (err);
}

} // namespace lat
