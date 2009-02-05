#ifndef IG_3D_CONTROLS_IG_3DVEC_4F_CONTROL_H
# define IG_3D_CONTROLS_IG_3DVEC_4F_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DControls/interface/Ig3DVectorBaseControl.h"
# include <Inventor/SbLinear.h>
# include <Inventor/fields/SoSFVec4f.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_3D_CONTROLS_API Ig3DVec4fControl : public Ig3DVectorBaseControl
{
    Q_OBJECT
public:
    Ig3DVec4fControl (IgControlCategory	*pane,
		      QString		label,
		      QString		labels = "X:Y:Z:W");
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    // Field and field value access
    SoSFVec4f *		field (void) const;
    void		setField (SoSFVec4f *field);
    SbVec4f		fieldValue (void) const;

    // Control value and change access
    float		xValue (void) const;
    float		yValue (void) const;
    float		zValue (void) const;
    float		wValue (void) const;
    SbVec4f		value (void) const;

    bool		isChanged (void) const;
    float		xChange (void) const;
    float		yChange (void) const;
    float		zChange (void) const;
    float		wChange (void) const;
    SbVec4f		change (void) const;

    // Access to the x,y,z control
    IgQtRangeControlFloat *	xControl (void) const;
    IgQtRangeControlFloat *	yControl (void) const;
    IgQtRangeControlFloat *	zControl (void) const;
    IgQtRangeControlFloat *	wControl (void) const;
    IgQtRangeControlFloat *	control  (void) const;
    
    using Ig3DVectorBaseControl::setValue;
    using Ig3DVectorBaseControl::value;
    using Ig3DVectorBaseControl::change;
    using Ig3DVectorBaseControl::control;
public slots:
    virtual void	setValue (const SbVec4f &vec);
    virtual void	setValue (float x, float y, float z, float w);

protected:
    virtual void	updateValue (void);
    virtual void	applyValue (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_CONTROLS_IG_3DVEC_4F_CONTROL_H
