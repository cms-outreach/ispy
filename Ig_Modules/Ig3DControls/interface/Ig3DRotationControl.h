#ifndef IG_3D_CONTROLS_IG_3DROTATION_CONTROL_H
# define IG_3D_CONTROLS_IG_3DROTATION_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DControls/interface/Ig3DVectorControl.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class SoSFRotation;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_3D_CONTROLS_API Ig3DRotationControl : public Ig3DVectorControl
{
    Q_OBJECT
public:
    Ig3DRotationControl (IgControlCategory *pane, QString label);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    // Field and field value access
    SoSFRotation *	field (void) const;
    void		setField (SoSFRotation *field);
    SbRotation		fieldValue (void) const;

    // Settings for ambiguities
    void		zForY90 (float degrees);
    void		yOver90 (bool value);

    // Utilities
    static float	deg2rad (float value);
    static float	rad2deg (float value);

    static SbRotation	rotate (const SbRotation &rot, const SbVec3f &angles);
    static SbVec3f	eulerXYZAngles (const SbRotation &from,
					bool		 yOver90 = false,
					float		 zForY90 = 0,
					float		 epsilon = .0001);

protected:
    virtual void	applyValue (void);
    virtual void	updateValue (void);

private:
    void		setup (IgQtRangeControlFloat *c, QPixmap rev, QPixmap fwd);

    bool		m_yOver90;
    float		m_zForY90;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_CONTROLS_IG_3DROTATION_CONTROL_H
