#ifndef IG_OPEN_INVENTOR_IG_SO_VIEWPOINT_H
# define IG_OPEN_INVENTOR_IG_SO_VIEWPOINT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/config.h"
# include <Inventor/nodes/SoNode.h>
# include <Inventor/nodes/SoSubNode.h>
# include <Inventor/fields/SoSFVec3f.h>
# include <Inventor/fields/SoSFRotation.h>
# include <Inventor/fields/SoSFFloat.h>
# include <Inventor/fields/SoSFEnum.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif // WIN32

class IG_OPEN_INVENTOR_API IgSoViewpoint : public SoNode
{
    SO_NODE_HEADER (IgSoViewpoint);
public:
    enum CameraType {
        Perspective = 1,
	Orthographic= 2};

    SoSFVec3f		position;
    SoSFRotation	orientation;
    SoSFFloat		aspectRatio;
    SoSFFloat		focalDistance;
    SoSFFloat		heightAngle;
    SoSFEnum		cameraType;

    // SoSFEnum		viewportMapping;
    SoSFFloat	nearDistance;
    SoSFFloat	farDistance;

    static void		initClass (void);

    IgSoViewpoint (void);
};
#ifdef WIN32
#include <SoWinEnterScope.h>
#endif // WIN32

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_OPEN_INVENTOR_IG_SO_VIEWPOINT_H
