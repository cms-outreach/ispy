//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/FileError.h"

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

FileError::FileError (const char *context, int code /* = 0 */)
    : IOError (context, code)
{}

FileError::FileError (const char *context, Error *chain)
    : IOError (context, chain)
{}

FileError::FileError (Error *chain)
    : IOError (chain)
{}

std::string
FileError::explainSelf (void) const
{ return IOError::doexplain ("File operation"); }

Error *
FileError::clone (void) const
{ return new FileError (*this); }

void
FileError::rethrow (void)
{ throw *this; }

} // namespace lat
