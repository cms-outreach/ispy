#ifndef IG_GENERIC_DETECTOR_SETUP_IG_GD_SETUP_H
# define IG_GENERIC_DETECTOR_SETUP_IG_GD_SETUP_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Examples/IgGenericDetectorSetup/interface/config.h"
# include "Ig_Modules/IgStudio/interface/IgStudioSetupExtension.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;
class IgStudioDriver;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_GENERIC_DETECTOR_SETUP_API IgGDSetup : public IgStudioSetupExtension
{
    IG_DECLARE_STATE_ELEMENT (IgGDSetup);
public:
    IgGDSetup (IgState *state);
    ~IgGDSetup (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual bool	setup (IgStudioDriver *into);
    IgState *		state (void) const;
    IgStudioDriver *	studio (void) const;

private:
    IgState		*m_state;
    IgStudioDriver	*m_studio;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_GENERIC_DETECTOR_SETUP_IG_GD_SETUP_H
