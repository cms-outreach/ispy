#ifndef IG_3D_CONTROLS_IG_3DVEC_2F_CONTROL_H
# define IG_3D_CONTROLS_IG_3DVEC_2F_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DControls/interface/Ig3DVectorBaseControl.h"
# include <Inventor/SbLinear.h>
# include <Inventor/fields/SoSFVec2f.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_3D_CONTROLS_API Ig3DVec2fControl : public Ig3DVectorBaseControl
{
    Q_OBJECT
public:
    Ig3DVec2fControl (IgControlCategory	*pane,
		      QString	      label,
		      QString         labels = "X:Y");
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    // Field and field value access
    SoSFVec2f *		field (void) const;
    void		setField (SoSFVec2f *field);
    SbVec2f		fieldValue (void) const;

    // Control value and change access
    float		xValue (void) const;
    float		yValue (void) const;
    SbVec2f		value (void) const;

    bool		isChanged (void) const;
    float		xChange (void) const;
    float		yChange (void) const;
    SbVec2f		change (void) const;

    // Access to the x,y,z control
    IgQtRangeControlFloat *	xControl (void) const;
    IgQtRangeControlFloat *	yControl (void) const;
    IgQtRangeControlFloat *	control  (void) const;
    
    using Ig3DVectorBaseControl::setValue;
    using Ig3DVectorBaseControl::value;
    using Ig3DVectorBaseControl::change;
    using Ig3DVectorBaseControl::control;
public slots:
    virtual void	setValue (const SbVec2f &vec);
    virtual void	setValue (float x, float y);

protected:
    virtual void	updateValue (void);
    virtual void	applyValue (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_CONTROLS_IG_3DVEC_2F_CONTROL_H
