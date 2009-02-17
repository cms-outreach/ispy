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
    
    for (int x = - SIZE; x <= SIZE ; x++)
    {
    	for (int y = -SIZE; y <= SIZE; y++)
	{
	    for (int z = -SIZE; z <= SIZE; z++)
	    {		
		SoSeparator *sep = new SoSeparator ();		
		SoTranslation *trans = new SoTranslation ();
		trans->translation.setValue ((float)x, (float)y, (float)z);		

		IgSoCube *obj = new IgSoCube;
		obj->width = 0.1+0.05*fabs(x);
		obj->thickness = 0.1+0.05*fabs(y);
		obj->length = 0.1+0.05*fabs(z);
		
		sep->addChild (trans);
		sep->addChild (obj);
		root->addChild (sep);		
	    }
	}
    }

    return "IGUANA: IgSoCube2 Test";
}
