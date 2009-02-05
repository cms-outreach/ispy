#ifndef IG_USER_GUI_EXTENSION_IG_USER_GUI_H
# define IG_USER_GUI_EXTENSION_IG_USER_GUI_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Examples/IgUserGUIExtension/interface/config.h"
# include "Ig_Modules/IgStudio/interface/IgStudioSetupExtension.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;
class IgStudioDriver;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_USER_GUI_EXTENSION_API IgUserGUI : public IgStudioSetupExtension
{
    IG_DECLARE_STATE_ELEMENT (IgUserGUI);
public:
    IgUserGUI (IgState *state);
    ~IgUserGUI (void);
    // implicit copy constructor
    // implicit assignment operator

    virtual bool	setup (IgStudioDriver *into);

private:
    IgState		*m_state;
    IgStudioDriver	*m_studio;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_USER_GUI_EXTENSION_IG_USER_GUI_H
