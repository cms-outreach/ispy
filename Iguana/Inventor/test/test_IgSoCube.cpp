//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoCube.h"
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
    IgSoCube::initClass ();

    SoMaterial *silicon = new SoMaterial;
    silicon->ambientColor.setValue (0.007, 0.002, 0.000);
    silicon->diffuseColor.setValue (0.314, 0.074, 0.000);
    silicon->specularColor.setValue(1.000, 0.767, 0.000);
    silicon->emissiveColor.setValue(0.867, 0.867, 0.337);
    silicon->shininess = 0.048;
    silicon->transparency = 0.5;
    root->addChild (silicon);

    IgSoCube *obj = new IgSoCube;
    obj->width     = 2.0;
    obj->length    = 6.0;
    obj->thickness = 1.0;
    
    root->addChild (obj);

    return "IGUANA: IgSoCube Test";
}
