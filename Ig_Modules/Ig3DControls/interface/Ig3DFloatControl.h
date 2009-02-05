#ifndef IG_3D_CONTROLS_IG_3DFLOAT_CONTROL_H
# define IG_3D_CONTROLS_IG_3DFLOAT_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgControlCentre/interface/IgQtRangeControlFloat.h"
# include "Ig_Modules/Ig3DControls/interface/Ig3DRangeControl.h"
# include <Inventor/fields/SoSFFloat.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>


class IG_3D_CONTROLS_API Ig3DFloatControl : public Ig3DRangeControl<SoSFFloat, float, IgQtRangeControlFloat>
{
    Q_OBJECT
public:
    Ig3DFloatControl (IgControlCategory *pane, QString label,
		      int parts = IgQtRangeControl::ValuePart);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

public slots:
    void		setValue (float newval);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_CONTROLS_IG_3DFLOAT_CONTROL_H
