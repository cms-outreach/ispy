#ifndef IG_3D_CONTROLS_IG_3DMFLOAT_CONTROL_H
# define IG_3D_CONTROLS_IG_3DMFLOAT_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgControlCentre/interface/IgQtRangeMControlFloat.h"
# include "Ig_Modules/Ig3DControls/interface/Ig3DRangeMControl.h"
# include <Inventor/fields/SoMFFloat.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>


class IG_3D_CONTROLS_API Ig3DMFloatControl : public Ig3DRangeMControl<SoMFFloat, float, IgQtRangeMControlFloat>
{
    Q_OBJECT
public:
    Ig3DMFloatControl (IgControlCategory *pane, QString label,
		      int parts = IgQtRangeMControl::ValuePart);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

public slots:
    void		setValue (unsigned int size, const float* newvals);
    void		setValue (const std::vector<float>& newvals);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_CONTROLS_IG_3DMFLOAT_CONTROL_H
