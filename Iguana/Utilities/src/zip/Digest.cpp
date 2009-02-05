//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/zip/Digest.h"
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

/** Destroy a digest.  No-op.  */
Digest::~Digest (void)
{}

/** Format the digest.  Gets the current #digest() value and formats
    it as a hexadecimal string.  */
std::string
Digest::format (void) const
{
    Value	value = digest ();
    std::string	result;
    char	tmp [3];

    result.reserve (value.size () * 2);
    for (size_t i = 0; i < value.size (); ++i)
    {
	sprintf (tmp, "%02x", (unsigned int) value [i]);
	result += tmp;
    }

    return result;
}

/** Update the digest value by one @a byte.  Redirects the call to
    #update(const void *, IOSize).  */
void
Digest::update (unsigned char byte)
{ update (&byte, 1); }

/** Update the digest value by @a data.  Redirects the call to
    #update(const void *, IOSize).  */
void
Digest::update (IOBuffer data)
{ update (data.data (), data.size ()); }

} // namespace lat
