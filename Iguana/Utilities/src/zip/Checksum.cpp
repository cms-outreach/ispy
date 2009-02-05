//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/zip/Checksum.h"

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

/** Destroy a checksum.  No-op.  */
Checksum::~Checksum (void)
{}

/** Update the checksum by one @a byte.  Redirects the call to
    #update(const void *, IOSize).  */
void
Checksum::update (unsigned char byte)
{ update (&byte, 1); }

/** Update the checksum by @a data.  Redirects the call to
    #update(const void *, IOSize).  */
void
Checksum::update (IOBuffer data)
{ update (data.data (), data.size ()); }

} // namespace lat
