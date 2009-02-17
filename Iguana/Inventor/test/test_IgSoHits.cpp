//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoHits.h"
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
    IgSoHits::initClass ();

    // Make the IgSoHits
    SbVec3f points [5] = {
	SbVec3f	(0.0, 0.0, 0.0),
	SbVec3f	(1.0, 0.0, 0.0),
	SbVec3f	(0.0, 1.0, 0.0),
	SbVec3f	(0.0, 0.0, 1.0),
	SbVec3f	(1.0, 1.0, 1.0)
    };
    IgSoHits *obj = new IgSoHits;
    obj->initXYZ (5, points);
    root->addChild (obj);

    return "IGUANA: IgSoHits Test";
}
