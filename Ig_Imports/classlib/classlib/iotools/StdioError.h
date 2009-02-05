#ifndef CLASSLIB_STDIO_ERROR_H
# define CLASSLIB_STDIO_ERROR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/iotools/IOException.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** An I/O-related error condition.  */
class StdioError : public IOException
{
public:
    StdioError (int code = 0, Error *next = 0);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual std::string	explainSelf (void) const;
    virtual Error *	clone (void) const;
    virtual void	rethrow (void);

private:
    int			m_code;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_STDIO_ERROR_H
