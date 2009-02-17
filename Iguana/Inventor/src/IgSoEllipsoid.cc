//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoEllipsoid.h"
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoTranslation.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_KIT_SOURCE (IgSoEllipsoid);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
IgSoEllipsoid::initClass (void)
{ SO_KIT_INIT_CLASS (IgSoEllipsoid, IgSoShapeKit, "IgSoShapeKit"); }

IgSoEllipsoid::IgSoEllipsoid (void)
{
    SO_KIT_CONSTRUCTOR (IgSoEllipsoid);
    SO_KIT_ADD_FIELD (eigenvalues, (1.0,1.0,1.0));
    SO_KIT_ADD_FIELD (rotation,    (SbVec3f (0,0,1),0));
    SO_KIT_ADD_FIELD (center,      (0,0,0));

    SO_KIT_ADD_CATALOG_ENTRY (trans,  SoTranslation, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (rot,    SoRotation, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (scale,  SoScale, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (sphere, SoSphere, FALSE, separator,\x0, TRUE);
    SO_KIT_INIT_INSTANCE ();
    setUpConnections (true, true);
}

void
IgSoEllipsoid::refresh (void)
{
    SoTranslation    *trans  = new SoTranslation;
    SoRotation       *rot    = new SoRotation;
    SoScale          *scale  = new SoScale;
    SoSphere         *sphere = new SoSphere;

    rot->rotation = rotation;
    trans->translation = center;
    scale->scaleFactor = eigenvalues;

    setPart ("rot",    rot);
    setPart ("trans",  trans);
    setPart ("scale",  scale);
    setPart ("sphere", sphere);
}
