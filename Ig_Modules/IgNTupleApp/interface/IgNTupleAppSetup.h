#ifndef IG_NTUPLE_APP_IG_NTUPLE_APP_SETUP_H
# define IG_NTUPLE_APP_IG_NTUPLE_APP_SETUP_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgNTupleApp/interface/config.h"
# include "Ig_Modules/IgStudio/interface/IgStudioSetupExtension.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgState;
class IgStudioDriver;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_NTUPLE_APP_API IgNTupleAppSetup : public IgStudioSetupExtension
{
    IG_DECLARE_STATE_ELEMENT (IgNTupleAppSetup);    
public:
    IgNTupleAppSetup (IgState *state);
    ~IgNTupleAppSetup (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    bool		setup (IgStudioDriver *into);
    
    IgState *		state (void) const;
    IgStudioDriver *	studio (void) const;
    
private:
    IgState *		m_state;
    IgStudioDriver *	m_studio;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_NTUPLE_APP_IG_NTUPLE_APP_SETUP_H
