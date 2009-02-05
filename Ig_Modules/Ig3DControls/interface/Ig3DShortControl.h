#ifndef IG_3D_CONTROLS_IG_3DSHORT_CONTROL_H
# define IG_3D_CONTROLS_IG_3DSHORT_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgControlCentre/interface/IgQtRangeControlShort.h"
# include "Ig_Modules/Ig3DControls/interface/Ig3DRangeControl.h"
# include <Inventor/fields/SoSFShort.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>


class IG_3D_CONTROLS_API Ig3DShortControl : public Ig3DRangeControl<SoSFShort, short, IgQtRangeControlShort>
{
    Q_OBJECT
public:
    Ig3DShortControl (IgControlCategory *pane, QString label,
		      int parts = IgQtRangeControl::ValuePart);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

public slots:
    void		setValue (short newval);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_CONTROLS_IG_3DSHORT_CONTROL_H
