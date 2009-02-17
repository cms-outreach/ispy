//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoJet.h"
#include <Inventor/engines/SoElapsedTime.h>
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
    IgSoJet::initClass ();

    IgSoJet *obj = new IgSoJet;
    obj->phi = 3 * M_PI / 4.0;
    obj->theta = M_PI / 6.0;
    obj->energy = 35.0;

    IgSoJet *obj2 = new IgSoJet;
    obj2->phi = 1.0;
    obj2->theta = 1.0;
    obj2->energy = 20.0;
    obj2->maxZ = 3.0;
    obj2->maxR = 1.0;

    SoElapsedTime *counter = new SoElapsedTime;
    counter->speed = 0.1;

    obj->phi.connectFrom (&counter->timeOut);
    obj->theta.connectFrom (&counter->timeOut);
    obj2->theta.connectFrom (&counter->timeOut);

    root->addChild (obj);
    root->addChild (obj2);

    return "IGUANA: IgSoJet Test";
}
