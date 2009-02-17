//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoG4Box.h"
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
    IgSoG4Box::initClass ();

    SoTimeCounter *counter = new SoTimeCounter;
    counter->max = 50; 
    counter->frequency = 0.2;

    IgSoG4Box *obj = new IgSoG4Box;
    obj->dx = 100;
    obj->dy = 10;
    obj->dz = 50;
    obj->dz.connectFrom (&counter->output);
    root->addChild (obj);

    return "IGUANA: IgSoG4Box Test";
}
