#ifndef IG_CONTROL_CENTRE_IG_STUDIO_CENTRE_H
# define IG_CONTROL_CENTRE_IG_STUDIO_CENTRE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgControlCentre/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgExtension.h"
# include "Ig_Modules/IgQtBrowser/interface/IgQtAppActiveService.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgControlCentre;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_CONTROL_CENTRE_API IgStudioCentre : public IgExtension
{
    IG_DECLARE_STATE_ELEMENT (IgStudioCentre);
public:
    static const int	MENU_ID_CONTROL_CENTRE = 100; // FIXME: not right place.

    IgStudioCentre (IgState *state);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    
    IgControlCentre* controlCentre (void);
    void	     appActive (IgQtAppActiveMessage message);
    
private:
    IgState		*m_state;
    IgControlCentre	*m_centre;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_CONTROL_CENTRE_IG_STUDIO_CENTRE_H
