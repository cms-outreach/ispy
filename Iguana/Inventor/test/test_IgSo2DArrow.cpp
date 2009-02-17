//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/test/test_main.h"
#include "Iguana/Inventor/interface/IgSo2DArrow.h"
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>

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
    IgSo2DArrow::initClass ();

    SoMaterial *bluePlastic = new SoMaterial;
    bluePlastic->emissiveColor.setValue (0, 0, 1);
    bluePlastic->transparency = .5;

    IgSo2DArrow *obj1 = new IgSo2DArrow;
    IgSo2DArrow *obj2 = new IgSo2DArrow;

    SoTransform	*trans = new SoTransform;
    trans->rotation.setValue (SbVec3f (0.0, 0.0, 1.0), 1.0);

    obj1->length.setValue (5.0);

    SoSeparator *sep = new SoSeparator;
    sep->addChild (trans);
    sep->addChild (bluePlastic);
    sep->addChild (obj2);

    root->addChild (obj1);
    root->addChild (sep); 

    return "IGUANA: IgSo2DArrow Test";
}
