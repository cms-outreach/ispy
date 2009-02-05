//<<<<<< INCLUDES                                                       >>>>>>

#include "test_main.h"
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoScale.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

#define SIZE 5

QString makeTest (SoSeparator *root)
{	
    //SoMaterial *mat = new SoMaterial ();
    //mat->diffuseColor.setValue(1.0,0,0);
    //root->addChild(mat);    

    SoCube *obj = new SoCube;
    obj->width     = 1;
    obj->height    = 1;
    obj->depth = 1;
    
    for (int x = -SIZE; x <= SIZE; x++)
    {
	for (int y = -SIZE; y <= SIZE; y++)
	{
	    for (int z = -SIZE; z <= SIZE; z++)
	    {		
		SoSeparator *sep = new SoSeparator;		
		SoTranslation *trans = new SoTranslation;
		SoScale *scale = new SoScale;
 		scale->scaleFactor.setValue (0.05*fabs(x)+0.1, 
 					     0.05*fabs(y)+0.1, 
 					     0.05*fabs(z)+0.1);		

		trans->translation.setValue ((float)x, (float)y, (float)z);
		sep->addChild (trans);
		sep->addChild (scale);
		sep->addChild (obj);
		root->addChild (sep);
	    }
	}
    }

    return "IGUANA: SoCube Test";
}
