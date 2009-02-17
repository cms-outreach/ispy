//<<<<<< INCLUDES                                                       >>>>>>

#include <Iguana/Inventor/interface/IgSoCube.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoMaterial.h>
#include <cmath>
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

#define SIZE 5

QString makeTest (SoSeparator *root)
{	
    IgSoShapeKit::initClass ();
    IgSoCube::initClass ();

    IgSoCube *obj = new IgSoCube;
    obj->width = 1;
    obj->thickness = 1;
    obj->length = 1;
    
    for (int x = - SIZE; x <= SIZE ; x++)
    {
    	for (int y = -SIZE; y <= SIZE; y++)
	{
	    for (int z = -SIZE; z <= SIZE; z++)
	    {		
		SoSeparator *sep = new SoSeparator;		
		SoTranslation *trans = new SoTranslation;
		trans->translation.setValue ((float)x, (float)y, (float)z);		

		SoScale *scale = new SoScale;
 		scale->scaleFactor.setValue (0.05*fabs(x)+0.1, 
 					     0.05*fabs(y)+0.1, 
 					     0.05*fabs(z)+0.1);		


		sep->addChild (trans);
		sep->addChild (scale);		
		sep->addChild (obj);
		root->addChild (sep);		
	    }
	}
    }    	

    return "IGUANA: IgSoCube3 Test";
}
