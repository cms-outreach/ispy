//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoG4Torus.h"
#include "test_main.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

QString makeTest (SoSeparator *root)
{
    IgSoShapeKit::initClass();
    IgSoG4Torus::initClass ();

    IgSoG4Torus *obj = new IgSoG4Torus;
    obj->minRadius     = 0.75;
    obj->maxRadius     = 1.5;
    obj->phiStart      = M_PI;
    obj->phiDelta      = M_PI;
    root->addChild (obj);

    IgSoG4Torus *obj1 = new IgSoG4Torus;
    obj1->minRadius     = 2.75;
    obj1->maxRadius     = 3.5;
    obj1->phiStart      = M_PI;
    obj1->phiDelta      = M_PI;
    root->addChild (obj1);

    return "IGUANA: IgSoG4Torus Test";    
}
