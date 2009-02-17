//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoPolyVol.h"
#include "Iguana/Inventor/interface/IgSoCoordinateAxis.h"
#include "Iguana/Inventor/interface/IgSoAxis.h"
#include "Inventor/engines/SoTimeCounter.h"
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
    IgSoShapeKit::initClass ();
    IgSoPolyVol::initClass ();
    IgSoAxis::initClass ();
    IgSoCoordinateAxis::initClass ();

    root->addChild (new IgSoCoordinateAxis);

    SoTimeCounter *counter = new SoTimeCounter;
    counter->max = 50; 
    counter->frequency = 0.2;

    SoMaterial *mat = new SoMaterial;
    mat->emissiveColor.setValue (1.0F, 0.F, 0.F);
    root->addChild (mat);

    IgSoPolyVol *obj = new IgSoPolyVol;
    obj->dZ.connectFrom (&counter->output);
    root->addChild (obj);

    return "IGUANA: IgSoPolyVol Test";
}
