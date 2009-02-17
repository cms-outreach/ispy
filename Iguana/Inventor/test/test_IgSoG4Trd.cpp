//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoG4Trd.h"
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
    IgSoG4Trd::initClass ();

    SoTimeCounter *counter = new SoTimeCounter;
    counter->min = 50; 
    counter->max = 450; 
    counter->frequency = 0.1;

    IgSoG4Trd *obj = new IgSoG4Trd;
    obj->dx1      = 450;
    obj->dy1      = 450;
    obj->dz       = 100;
    obj->dx2      =  50;
    obj->dy2      =  50;
    obj->dx1.connectFrom (&counter->output);
    obj->dy1.connectFrom (&counter->output);
    root->addChild (obj);

    return "IGUANA: IgSoG4Trd Test";
}
