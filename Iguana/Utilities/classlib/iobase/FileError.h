#ifndef CLASSLIB_FILE_ERROR_H
# define CLASSLIB_FILE_ERROR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/iobase/IOError.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** File-system related exception class. */
class FileError : public IOError
{
public:
    FileError (const char *context, int code = 0);
    FileError (const char *context, Error *chain);
    FileError (Error *chain);

    virtual std::string	explainSelf (void) const;
    virtual Error *	clone (void) const;
    virtual void	rethrow (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_FILE_ERROR_H
