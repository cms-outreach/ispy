#ifndef IG_3D_CONTROLS_IG_3DUSHORT_CONTROL_H
# define IG_3D_CONTROLS_IG_3DUSHORT_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgControlCentre/interface/IgQtRangeControlUShort.h"
# include "Ig_Modules/Ig3DControls/interface/Ig3DRangeControl.h"
# include <Inventor/fields/SoSFUShort.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>


class IG_3D_CONTROLS_API Ig3DUShortControl : public Ig3DRangeControl<SoSFUShort, unsigned short, IgQtRangeControlUShort>
{
    Q_OBJECT
public:
    Ig3DUShortControl (IgControlCategory *pane, QString label,
		      int parts = IgQtRangeControl::ValuePart);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

public slots:
    void		setValue (unsigned short newval);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_CONTROLS_IG_3DUSHORT_CONTROL_H
