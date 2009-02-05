#ifndef CLASSLIB_SYSTEM_ERROR_H
# define CLASSLIB_SYSTEM_ERROR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"
# include "classlib/utils/Error.h"
# include "classlib/utils/SysErr.h"
# include <string>

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Native system error. */
class SystemError : public Error
{
public:
    SystemError (int syserror, SysErr::Code portable = SysErr::ErrNone);
    SystemError (Error *next, int syserror, SysErr::Code portable = SysErr::ErrNone);
    // default destructor
    // default copy constructor
    // default assignment operator

    virtual std::string	explainSelf (void) const;
    virtual Error *	clone (void) const;
    virtual void	rethrow (void);

    int			errornum (void) const;
    SysErr::Code	portable (void) const;

    static std::string	errorString (int error);
    static SysErr::Code	translateFromErrno (int syserr);
    static SysErr::Code	translateFromHErrno (int syserr);

private:
    int			m_error;
    SysErr::Code	m_portable;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

inline int
SystemError::errornum (void) const
{ return m_error; }

inline SysErr::Code
SystemError::portable (void) const
{ return m_portable; }

} // namespace lat
#endif // CLASSLIB_SYSTEM_ERROR_H
