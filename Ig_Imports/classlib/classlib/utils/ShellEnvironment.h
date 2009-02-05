#ifndef CLASSLIB_SHELL_ENVIRONMENT_H
# define CLASSLIB_SHELL_ENVIRONMENT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/utils/Environment.h"
# include <string>

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Process shell environment.  */
class ShellEnvironment : public Environment
{
public:
    using Environment::get;
    virtual bool		has (const std::string &name) const;
    virtual std::string		get (const std::string &name,
		    		     bool *have = 0) const;
    virtual void		unset (const std::string &name);
    virtual void		set (const std::string &name,
				     const std::string &value);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_SHELL_ENVIRONMENT_H
