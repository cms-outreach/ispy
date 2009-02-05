#ifndef IG_USER_GUI_DRIVER_IG_GUI_TEST_H
# define IG_USER_GUI_DRIVER_IG_GUI_TEST_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Examples/IgUserGUIDriver/interface/config.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgDriver.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;
class IgSite;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_USER_GUI_DRIVER_API IgGUITest : public IgDriver
{
public:
    IgGUITest (IgState *state);
    ~IgGUITest (void);
    // implicit copy constructor
    // implicit assignment operator

    virtual int		run (void);

    static const char *	catalogLabel (void);

private:
    IgState		*m_state;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_USER_GUI_DRIVER_IG_GUI_TEST_H
