#ifndef CLASSLIB_REGEXP_ERROR_H
# define CLASSLIB_REGEXP_ERROR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/utils/Error.h"
# include <string>

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** File-system related exception class. */
class RegexpError : public Error
{
public:
    RegexpError (const char *context, int syntax, int error);
    RegexpError (const char *context, int syntax, const char *msg);

    virtual std::string	explainSelf (void) const;
    virtual Error *	clone (void) const;
    virtual void	rethrow (void);

private:
    const char	*m_context;
    int		m_syntax;
    int		m_error;
    std::string	m_message;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_REGEXP_ERROR_H
