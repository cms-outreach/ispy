#ifndef IG_3D_CONTROLS_IG_3DMINT32_CONTROL_H
# define IG_3D_CONTROLS_IG_3DMINT32_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgControlCentre/interface/IgQtRangeMControlInt.h"
# include "Ig_Modules/Ig3DControls/interface/Ig3DRangeMControl.h"
# include <Inventor/fields/SoMFInt32.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>


class IG_3D_CONTROLS_API Ig3DMInt32Control : public Ig3DRangeMControl<SoMFInt32, int, IgQtRangeMControlInt>
{
    Q_OBJECT
public:
    Ig3DMInt32Control (IgControlCategory *pane, QString label,
		      int parts = IgQtRangeMControl::ValuePart);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

public slots:
    void		setValue (unsigned int size, const int* newvals);
    void		setValue (const std::vector<int>& newvals);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_CONTROLS_IG_3DMINT32_CONTROL_H
