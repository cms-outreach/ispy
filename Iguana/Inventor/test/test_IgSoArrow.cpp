//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/test/test_main.h"
#include "Iguana/Inventor/interface/IgSoArrow.h"
#include <Inventor/engines/SoElapsedTime.h>
#include <Inventor/engines/SoCalculator.h>

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
    IgSoArrow::initClass ();

    SoMaterial *bluePlastic = new SoMaterial;
    bluePlastic->emissiveColor.setValue (0, 0, 1);
    bluePlastic->transparency = .5;

    SoElapsedTime *counter = new SoElapsedTime;
    SoCalculator *calc = new SoCalculator;
    counter->speed = 3;
    calc->a.connectFrom (&counter->timeOut);
    calc->expression.set1Value (0, "oA = vec3f( -.5 + -.5 * sin( a ), 0, 0 )");
    calc->expression.set1Value (1, "oB = vec3f( .5 + .5 * sin( a ), 0, 0)");
  
    IgSoArrow *obj1 = new IgSoArrow;
    IgSoArrow *obj2 = new IgSoArrow;

    obj1->tail.setValue (-5, 0, 0);
    obj1->tip.setValue (0, 0, 0);
    obj1->tip.connectFrom (&calc->oA);

    obj2->tail.setValue (5, 0, 0);
    obj2->tip.setValue (0, 0, 0);
    obj2->tip.connectFrom (&calc->oB);

    root->addChild (obj1);
    root->addChild (bluePlastic);
    root->addChild (obj2);

    return "IGUANA: IgSoArrow Test";
}
