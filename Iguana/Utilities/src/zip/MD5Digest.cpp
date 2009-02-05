//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/zip/MD5Digest.h"
extern "C" {
  #define PROTOTYPES 1
  #include "global.h"
  #include "md5.h"
}

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>

struct MD5Digest::Context : MD5_CTX {};

//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Construct a new MD5 digest.  */
MD5Digest::MD5Digest (void)
    : m_context (new Context)
{ MD5Init (m_context); }

/** Destroy a digest.  No-op.  */
MD5Digest::~MD5Digest (void)
{ delete m_context; }

/** Return the final digest value.  */
Digest::Value
MD5Digest::digest (void) const
{ Value val (16, '\0'); MD5Final (&val [0], m_context); return val; }

/** Update the digest with @a data of length @a n.  */
void
MD5Digest::update (const void *data, IOSize n)
{ MD5Update (m_context, (unsigned char *) data, n); }

/** Reset the digest.  */
void
MD5Digest::reset (void)
{ MD5Init (m_context); }

} // namespace lat
