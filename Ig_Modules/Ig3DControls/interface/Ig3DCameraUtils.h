#ifndef IG_3D_CONTROLS_IG_3DCAMERA_UTILS_H
# define IG_3D_CONTROLS_IG_3DCAMERA_UTILS_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DControls/interface/config.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class SoSFRotation;
class SoSFVec3f;
class SoSFFloat;
class SbVec3f;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_3D_CONTROLS_API Ig3DCameraUtils
{
public:
    Ig3DCameraUtils (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    void	setCamera (SoSFRotation *orientation,
		    	   SoSFVec3f *position,
			   SoSFFloat *focalDistance);

    SbVec3f	getFocalPoint (void) const;
    void	setFocalPoint (const SbVec3f &focalPoint);

private:
    SoSFRotation*m_orientation;
    SoSFVec3f	*m_position;
    SoSFFloat	*m_focalDistance;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_CONTROLS_IG_3DCAMERA_UTILS_H
