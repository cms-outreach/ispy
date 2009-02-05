#ifndef IG_USER_GUI_EXTENSION_IG_USER_GUI_H
# define IG_USER_GUI_EXTENSION_IG_USER_GUI_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtProfileService/interface/config.h"
# include "Ig_Modules/IgStudio/interface/IgStudioSetupExtension.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;
class IgStudioDriver;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>


/** This creates a new Studio Extension with the profiler GUI. 
 */
class IG_QT_PROFILE_SERVICE_API IgProfile : public IgStudioSetupExtension
{
    IG_DECLARE_STATE_ELEMENT (IgProfile);
public:
    IgProfile (IgState *state);
    ~IgProfile (void);
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
