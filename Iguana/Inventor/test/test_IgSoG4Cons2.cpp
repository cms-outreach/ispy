//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoRotSolid.h"
#include <Inventor/engines/SoTimeCounter.h>
#include <Inventor/engines/SoCalculator.h>
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
    IgSoRotSolid::initClass ();

    SoTimeCounter *counter = new SoTimeCounter;
    SoCalculator *calc = new SoCalculator;
    counter->max  = 360;
    counter->step = 10;
    counter->frequency = .05;
    calc->a.connectFrom (&counter->output);
    calc->expression = "oa = a * M_PI / 180.0";
  
    IgSoRotSolid *obj = new IgSoRotSolid;
    obj->makeCons (0, 1.0, 4.5, 5.0, 1.0);
    obj->phiDelta = M_PI;
    obj->phiDelta.connectFrom (&calc->oa);
    root->addChild (obj);

    return "IGUANA: IgSoG4Cons2 Test";
}
