//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/test/test_main.h"
#include "Iguana/Inventor/interface/IgSoSimpleTrajectory.h"
#include <Inventor/nodes/SoDrawStyle.h>

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
    IgSoSimpleTrajectory::initClass ();
    
    SoMaterial *material = new SoMaterial;
    material->ambientColor.setValue  (0.000, 0.000, 0.000); 
    material->diffuseColor.setValue  (1.000, 1.000, 0.000);  
    material->emissiveColor.setValue (1.000, 1.000, 0.000); 
    material->specularColor.setValue (0.000, 0.000, 0.000); 
    material->shininess = 0.0;
    material->transparency = 0.0;

    IgSoSimpleTrajectory *obj = new IgSoSimpleTrajectory;
    SbVec3f pnt [] = {SbVec3f(0, 0, 0), SbVec3f(0, 0, 1), SbVec3f(0, 1, 2), SbVec3f(1, 2, 3)};

    obj->controlPoints.setValues (0, 4, pnt);
    obj->markerPoints.setValues (0, 4, pnt);
    obj->linePattern = 0x0f0f;

    root->addChild (material);
    root->addChild (obj);

    //    delete obj;
    IgSoSimpleTrajectory *obj2 = new IgSoSimpleTrajectory;
    SbVec3f pnt1 [] = {SbVec3f(1, 3, 2), SbVec3f(0, 1, 0), SbVec3f(3, 0, 2), SbVec3f(1, 2, 1)};

    obj2->controlPoints.setValues (0, 4, pnt1);
    obj2->markerPoints.setValues (0, 4, pnt1);
    obj2->linePattern = 0x0fff;
    obj2->lineWidth = 4.0;

    root->addChild(obj2);

    return "IGUANA: IgSoSimpleTrajectory test";
}
    
