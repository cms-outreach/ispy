//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSo3DErrorBar.h"
#include <Inventor/nodes/SoMaterial.h>
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
    IgSo3DErrorBar::initClass ();

    SbVec3f		point (.5, .5, .5);
    SbVec3f		error (.2, .3, .4);
    IgSo3DErrorBar	*object = new IgSo3DErrorBar;

    object->u1.setValue (point [0]-error [0], point [1], point [2]);
    object->u2.setValue (point [0]+error [0], point [1], point [2]);
    object->v1.setValue (point [0], point [1]-error [1], point [2]);
    object->v2.setValue (point [0], point [1]+error [1], point [2]);
    object->w1.setValue (point [0], point [1], point [2]-error[2]);
    object->w2.setValue (point [0], point [1], point [2]+error[2]);
    object->lineWidth.setValue (1.0f);

    root->addChild (object);

    return "IGUANA: IgSo3DErrorBar Test";
}
