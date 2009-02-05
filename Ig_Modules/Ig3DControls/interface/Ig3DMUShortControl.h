#ifndef IG_3D_CONTROLS_IG_3DMUSHORT_CONTROL_H
# define IG_3D_CONTROLS_IG_3DMUSHORT_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgControlCentre/interface/IgQtRangeMControlUShort.h"
# include "Ig_Modules/Ig3DControls/interface/Ig3DRangeMControl.h"
# include <Inventor/fields/SoMFUShort.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>


class IG_3D_CONTROLS_API Ig3DMUShortControl : public Ig3DRangeMControl<SoMFUShort, unsigned short, IgQtRangeMControlUShort>
{
    Q_OBJECT
public:
    Ig3DMUShortControl (IgControlCategory *pane, QString label,
		      int parts = IgQtRangeMControl::ValuePart);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

public slots:
    void		setValue (unsigned int size, const unsigned short* newvals);
    void		setValue (const std::vector<unsigned short>& newvals);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_CONTROLS_IG_3DMUSHORT_CONTROL_H
