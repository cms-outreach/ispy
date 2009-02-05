#ifndef IG_3D_CONTROLS_IG_3DCAMERA_POS_CONTROL_H
# define IG_3D_CONTROLS_IG_3DCAMERA_POS_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DControls/interface/Ig3DVec3fControl.h"
# include "Ig_Modules/Ig3DControls/interface/Ig3DCameraUtils.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_3D_CONTROLS_API Ig3DCameraPosControl
    : public Ig3DVec3fControl, public Ig3DCameraUtils
{
public:
    Ig3DCameraPosControl (IgControlCategory *pane, QString label);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_CONTROLS_IG_3DCAMERA_POS_CONTROL_H
