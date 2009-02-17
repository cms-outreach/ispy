//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoCalHit.h"
#include "Inventor/nodes/SoShapeHints.h"
#include "Inventor/nodes/SoIndexedFaceSet.h"
#include "test_main.h"
#include <vector>

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
    IgSoCalHit::initClass ();

    std::vector<SbVec3f> pts(8);
    pts	[0] = SbVec3f (0, 0, 0); pts [4] = SbVec3f (0, 0, 1);
    pts	[1] = SbVec3f (1, 0, 0); pts [5] = SbVec3f (1, 0, 1);
    pts	[2] = SbVec3f (1, 1, 0); pts [6] = SbVec3f (1, 1, 1);
    pts	[3] = SbVec3f (0, 1, 0); pts [7] = SbVec3f (0, 1, 1);

    IgSoCalHit *obj = new IgSoCalHit;
    obj->energy = 10;
    obj->vertices.setValues (0, 8, &pts[0]);
    obj->orderedRGBA.set1Value (0, SbColor (0.5,0.5,0.5).getPackedValue ());
    obj->orderedRGBA.set1Value (1, SbColor (0.5,1.0,0.5).getPackedValue ());
    obj->orderedRGBA.set1Value (2, SbColor (0.5,0.5,1.0).getPackedValue ());
    obj->minimumEnergy = 0.2;
    obj->drawShape = false;

    // SoShapeHints *hints = new SoShapeHints;
    // hints->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
    // hints->shapeType = SoShapeHints::SOLID;
    // obj->setPart("shapeHints",hints);
    root->addChild (obj);

    return "IGUANA: IgSoCalHit Test";
}
