//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoCircleArc.h"
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
    IgSoCircleArc::initClass();

    IgSoCircleArc *obj = new IgSoCircleArc;
    obj->center     = SbVec2f (0, 1);
    obj->radius     = 0.75;
    obj->sweepAngle = 120;

    root->addChild (obj);

    return "IGUANA: IgSoCircleArc Test";
}
