#ifndef IG_3D_CONTROLS_IG_3DVEC_3F_CONTROL_H
# define IG_3D_CONTROLS_IG_3DVEC_3F_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DControls/interface/Ig3DVectorControl.h"
#include <Inventor/fields/SoSFVec3f.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_3D_CONTROLS_API Ig3DVec3fControl : public Ig3DVectorControl
{
    Q_OBJECT
public:
    Ig3DVec3fControl (IgControlCategory *pane,
                      QString 		label,
		      Style		style = PositionStyle);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    // Field and field value access
    SoSFVec3f *		field (void) const;
    void		setField (SoSFVec3f *field);
    SbVec3f		fieldValue (void) const;

protected:
    virtual void	updateValue (void);
    virtual void	applyValue (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_CONTROLS_IG_3DVEC_3F_CONTROL_H
