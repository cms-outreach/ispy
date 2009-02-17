//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoRotSolid.h"
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
    IgSoRotSolid::initClass ();

    SoMaterial *silicon = new SoMaterial;
    silicon->ambientColor.setValue (0.007, 0.002, 0.000);
    silicon->diffuseColor.setValue (0.314, 0.074, 0.000);
    silicon->specularColor.setValue (1.000, 1.000, 1.000);
    silicon->emissiveColor.setValue (0.867, 0.867, 0.337);
    silicon->shininess = 1;
    silicon->transparency = 0.5;
    root->addChild (silicon);


    IgSoRotSolid *obj = new IgSoRotSolid;
    
    SbVec2f polyline[] = 
	{
	    SbVec2f (0, -10),
	    SbVec2f (2, -10),
	    SbVec2f (2, 10),
	    SbVec2f (0, 10)    
	};
        
    obj->polygon.setValues(0, 4, polyline);
    obj->phiStart = 0;
    obj->phiDelta = 2 * M_PI;
    obj->smooth = true;
     
    assert (obj->getPart ("faces", TRUE));
    assert (obj->getPart ("lines", TRUE));
    root->addChild (obj);

    return "IGUANA: IgSoRotSolid02 Test";
}
