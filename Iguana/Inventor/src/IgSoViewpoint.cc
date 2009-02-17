//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoViewpoint.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_NODE_SOURCE (IgSoViewpoint);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
IgSoViewpoint::initClass (void)
{
    SO_NODE_INIT_CLASS (IgSoViewpoint, SoNode, "Node");
}

IgSoViewpoint::IgSoViewpoint (void)
{
    SO_NODE_CONSTRUCTOR (IgSoViewpoint);
    SO_NODE_ADD_FIELD (position, (SbVec3f (0, 0, 1)));
    SO_NODE_ADD_FIELD (orientation, (SbRotation (SbVec3f (0, 0, 1), 0)));
    SO_NODE_ADD_FIELD (aspectRatio, (1.f));
    SO_NODE_ADD_FIELD (focalDistance, (5.f));
    SO_NODE_ADD_FIELD (heightAngle, (M_PI/4.f)); // 45°
    SO_NODE_ADD_FIELD (cameraType, (Perspective));
    SO_NODE_DEFINE_ENUM_VALUE (CameraType, Perspective);
    SO_NODE_DEFINE_ENUM_VALUE (CameraType, Orthographic);
    SO_NODE_SET_SF_ENUM_TYPE (cameraType, CameraType);
    SO_NODE_ADD_FIELD (nearDistance, (1.f));
    SO_NODE_ADD_FIELD (farDistance,  (1.f));
}
