#ifndef IG_SO_VIEWER_APP_DRIVER_IG_SO_VIEWER_APP_DRIVER_H
# define IG_SO_VIEWER_APP_DRIVER_IG_SO_VIEWER_APP_DRIVER_H

//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgSoViewerAppDriver/interface/config.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgDriver.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_SO_VIEWER_APP_DRIVER_API IgSoViewerAppDriver : public IgDriver
{
public:
    IgSoViewerAppDriver (IgState *state);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual int		run (void);

    static const char *		catalogLabel (void);

private:
    IgState		*m_state;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_SO_VIEWER_APP_DRIVER_IG_SO_VIEWER_APP_DRIVER_H
