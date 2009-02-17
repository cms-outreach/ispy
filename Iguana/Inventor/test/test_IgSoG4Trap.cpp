//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoG4Trap.h"
#include "Iguana/Inventor/interface/IgSoCoordinateAxis.h"
#include "Iguana/Inventor/interface/IgSoAxis.h"
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
    IgSoG4Trap::initClass ();
    IgSoAxis::initClass ();
    IgSoCoordinateAxis::initClass ();

    IgSoCoordinateAxis *axis = new IgSoCoordinateAxis;
    axis->divisions = 5;
    axis->divisionLength = 50;
    root->addChild (axis);
  
    SoTimeCounter *counter = new SoTimeCounter;
    SoCalculator *calc = new SoCalculator;
    counter->max  = 360; 
    counter->step = 4;
    counter->frequency = 0.2;
    calc->a.connectFrom (&counter->output);
    calc->expression = "oa = a * M_PI / 180.0";

    IgSoG4Trap *obj = new IgSoG4Trap;
    obj->dz        = 200;
    obj->theta     = 1.0;
    obj->phi       = 0.0;
    obj->dy1       = 150;
    obj->dx1       = 125;
    obj->dx2       = 175;
    obj->dy2       = 165;
    obj->dx3       = 138;
    obj->dx4       = 193;
    obj->phi.connectFrom (&calc->oa);
    root->addChild (obj);

    return "IGUANA: IgSoG4Trap Test";
}
