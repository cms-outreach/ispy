#ifndef IG_3D_CONTROLS_IG_3DCAMERA_ROT_CONTROL_H
# define IG_3D_CONTROLS_IG_3DCAMERA_ROT_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DControls/interface/Ig3DRotationControl.h"
# include "Ig_Modules/Ig3DControls/interface/Ig3DCameraUtils.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_3D_CONTROLS_API Ig3DCameraRotControl
    : public Ig3DRotationControl, public Ig3DCameraUtils
{
public:
    Ig3DCameraRotControl (IgControlCategory *pane, QString label);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

protected:
    virtual void	applyValue (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_CONTROLS_IG_3DCAMERA_ROT_CONTROL_H
