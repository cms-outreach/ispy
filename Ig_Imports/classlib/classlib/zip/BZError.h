#ifndef CLASSLIB_BZ_ERROR_H
# define CLASSLIB_BZ_ERROR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"
# include "classlib/utils/Error.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** BZLIB error indication.  */
class BZError : public Error
{
public:
    BZError (int code);
    // implicit destructor
    // implicit copy constructor
    // implicit assignment operator

    virtual int		code (void) const;
    virtual std::string	explainSelf (void) const;
    virtual Error *	clone (void) const;
    virtual void	rethrow (void);

private:
    int			m_code;		//< BZLIB error code
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_BZ_ERROR_H
