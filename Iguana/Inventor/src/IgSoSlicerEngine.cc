//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoSlicerEngine.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_ENGINE_SOURCE (IgSoSlicerEngine);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
IgSoSlicerEngine::initClass (void)
{
    SO_ENGINE_INIT_CLASS (IgSoSlicerEngine, SoEngine, "Engine");
}

IgSoSlicerEngine::IgSoSlicerEngine (void)
{
    SO_ENGINE_CONSTRUCTOR (IgSoSlicerEngine);
    SO_ENGINE_ADD_INPUT (thickness, (.5));
    SO_ENGINE_ADD_INPUT (plane, (SbPlane (SbVec3f (0, 0, -1), 0)));
    SO_ENGINE_ADD_OUTPUT (frontPlane, SoSFPlane);
    SO_ENGINE_ADD_OUTPUT (backPlane, SoSFPlane);
}

IgSoSlicerEngine::~IgSoSlicerEngine (void)
{}

void
IgSoSlicerEngine::evaluate (void)
{
    SbPlane	p (plane.getValue ());
    SbVec3f	n (p.getNormal ());
    float	d1 = p.getDistanceFromOrigin ();
    float	d2 = thickness.getValue ()/2.0f;

    SO_ENGINE_OUTPUT (frontPlane, SoSFPlane, 
                      setValue (SbPlane (n, d1 - d2)));
    SO_ENGINE_OUTPUT (backPlane, SoSFPlane, 
                      setValue (SbPlane (n * -1, -d1 - d2)));
}
