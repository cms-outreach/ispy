#ifndef IG_OBJECT_BROWSER_IG_DRIVER_H
# define IG_OBJECT_BROWSER_IG_DRIVER_H

//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Framework/IgObjectBrowser/interface/config.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Base class for application drivers.

    This is the base class for application drivers.  The inheriting
    class must implement the #run method to do whatever it means to
    "run" this application.  The driver will normally be created
    through a #IgDriverFactory, which passes the constructor the
    global #IgState object as an argument.  */
class IG_OBJECT_BROWSER_API IgDriver
{
public:
    virtual ~IgDriver (void);

    /** Run the application.

	@return Program exit code, like @c main would.  */
    virtual int		run (void) = 0;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_OBJECT_BROWSER_IG_DRIVER_H
