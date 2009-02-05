//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iotools/IOException.h"

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
IOException::IOException (Error *next /* = 0 */)
    : Error (next)
{}

/** Provide an explanation of this error.  */
std::string
IOException::explainSelf (void) const
{ return "I/O Error"; } 

/** @reimp  */
Error *
IOException::clone (void) const
{ return new IOException (*this); }

/** @reimp  */
void
IOException::rethrow (void)
{ throw *this; }

} // namespace lat
