//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoSphereHit.h"
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoTransform.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_KIT_SOURCE (IgSoSphereHit);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
IgSoSphereHit::initClass (void)
{ SO_KIT_INIT_CLASS (IgSoSphereHit, IgSoShapeKit, "IgSoShapeKit"); }

IgSoSphereHit::IgSoSphereHit (void)
{
    SO_KIT_CONSTRUCTOR (IgSoSphereHit);
    SO_KIT_ADD_FIELD (scale,  (1.0));
    SO_KIT_ADD_FIELD (energy, (1.0));
    SO_KIT_ADD_FIELD (centre, (0.0, 0.0, 0.0));
    SO_KIT_ADD_CATALOG_ENTRY (transform, SoTransform, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (sphere, SoSphere, FALSE, separator,\x0, TRUE);
    SO_KIT_INIT_INSTANCE ();
    setUpConnections (true, true);
}

void
IgSoSphereHit::refresh (void)
{
    if (energy.getValue () < 0)
    {	
	setPart ("transform", NULL);
	setPart ("sphere", NULL);
	return;
    }
    

    SoTransform	*transform = new SoTransform;
    SoSphere	*sphere = new SoSphere;

    // 2D projected area scales with energy, centred at the requested position
    sphere->radius = sqrt (energy.getValue ()) * scale.getValue ();
    transform->translation = centre;

    setPart ("transform", transform);
    setPart ("sphere", sphere);
}
