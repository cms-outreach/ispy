#ifndef IG_3D_CONTROLS_IG_3DINT32_CONTROL_H
# define IG_3D_CONTROLS_IG_3DINT32_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgControlCentre/interface/IgQtRangeControlInt.h"
# include "Ig_Modules/Ig3DControls/interface/Ig3DRangeControl.h"
# include <Inventor/fields/SoSFInt32.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_3D_CONTROLS_API Ig3DInt32Control : public Ig3DRangeControl<SoSFInt32, int, IgQtRangeControlInt>
{
    Q_OBJECT
public:
    Ig3DInt32Control (IgControlCategory *pane, QString label,
		      int parts = IgQtRangeControl::ValuePart);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

public slots:
    void		setValue (int newval);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_CONTROLS_IG_3DINT32_CONTROL_H
