//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoCoordinateAxis.h"
#include "Iguana/Inventor/interface/IgSoAxis.h"
#include <Inventor/engines/SoElapsedTime.h>
#include <Inventor/engines/SoTimeCounter.h>
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
    IgSoAxis::initClass ();
    IgSoCoordinateAxis::initClass ();

    SoElapsedTime *time = new SoElapsedTime;
    time->speed = 0.1;

    SoTimeCounter *counter = new SoTimeCounter;
    counter->step = 1;
    counter->min = 0;
    counter->max = 10;
    counter->frequency = 0.02;

    IgSoCoordinateAxis *obj = new IgSoCoordinateAxis;
    obj->divisions = 3;
    obj->divisionLength = 0.2;
    obj->divisions.connectFrom (&counter->output);
    obj->divisionLength.connectFrom (&time->timeOut);

    root->addChild (obj);

    return "IGUANA: IgSoCoordinateAxis Test";
}
