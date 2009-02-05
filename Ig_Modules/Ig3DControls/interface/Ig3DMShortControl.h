#ifndef IG_3D_CONTROLS_IG_3DMSHORT_CONTROL_H
# define IG_3D_CONTROLS_IG_3DMSHORT_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgControlCentre/interface/IgQtRangeMControlShort.h"
# include "Ig_Modules/Ig3DControls/interface/Ig3DRangeMControl.h"
# include <Inventor/fields/SoMFShort.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>


class IG_3D_CONTROLS_API Ig3DMShortControl : public Ig3DRangeMControl<SoMFShort, short, IgQtRangeMControlShort>
{
    Q_OBJECT
public:
    Ig3DMShortControl (IgControlCategory *pane, QString label,
		      int parts = IgQtRangeMControl::ValuePart);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

public slots:
    void		setValue (unsigned int size, const short* newvals);
    void		setValue (const std::vector<short>& newvals);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_CONTROLS_IG_3DMSHORT_CONTROL_H
