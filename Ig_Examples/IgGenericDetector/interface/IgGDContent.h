#ifndef IG_GENERIC_DETECTOR_DEMO_IG_GD_CONTECT_H
# define IG_GENERIC_DETECTOR_DEMO_IG_GD_CONTECT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Framework/IgObjectBrowser/interface/IgExtension.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS  	 		                        >>>>>>
class IgState;

class IgGDContent : public IgExtension
{
    IG_DECLARE_STATE_ELEMENT (IgGDContent);
public:
     IgGDContent(IgState *state);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    
private:
    IgState		*m_state;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_GENERIC_DETECTOR_DEMO_IG_GD_CONTECT_H
