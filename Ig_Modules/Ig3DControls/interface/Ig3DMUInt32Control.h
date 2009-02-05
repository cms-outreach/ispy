#ifndef IG_3D_CONTROLS_IG_3DMUINT32_CONTROL_H
# define IG_3D_CONTROLS_IG_3DMUINT32_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgControlCentre/interface/IgQtRangeMControlUInt.h"
# include "Ig_Modules/Ig3DControls/interface/Ig3DRangeMControl.h"
# include <Inventor/fields/SoMFUInt32.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>


class IG_3D_CONTROLS_API Ig3DMUInt32Control : public Ig3DRangeMControl<SoMFUInt32, unsigned int, IgQtRangeMControlUInt>
{
    Q_OBJECT
public:
    Ig3DMUInt32Control (IgControlCategory *pane, QString label,
		      int parts = IgQtRangeMControl::ValuePart);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

public slots:
    void		setValue (unsigned int size, const unsigned int* newvals);
    void		setValue (const std::vector<unsigned int>& newvals);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_CONTROLS_IG_3DMUINT32_CONTROL_H
