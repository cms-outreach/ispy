//<<<<<< INCLUDES                                                       >>>>>>

#include <Iguana/Inventor/interface/IgBSPTree.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <iostream>
#include <vector>

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
    SoMaterial *silicon = new SoMaterial;
    silicon->ambientColor.setValue (0.007, 0.002, 0.000);
    silicon->diffuseColor.setValue (0.314, 0.074, 0.000);
    silicon->specularColor.setValue (1.000, 0.767, 0.000);
    silicon->emissiveColor.setValue (0.867, 0.867, 0.337);
    silicon->shininess = 0.048;
    silicon->transparency = 0.5;
    root->addChild (silicon);

    SoNode *obj = new SoCube;

    obj->ref ();
        
    IgBSPTree bspTree;
    
    bspTree.add (obj);

    IgBSPTriangle *triangle = new IgBSPTriangle (SbVec3f (-10, 20, 0), 
						 SbVec3f (-10, -10, 0), 
						 SbVec3f (20, -10, 0));
    
    IgBSPTriangle::List front;
    IgBSPTriangle::List back;
    
    bspTree.intersect (triangle, &front, &back);

    std::cerr << "Triangle count: " 
	      << bspTree.getTriangleCount () << std::endl;    
    std::cerr << "In front: " 
	      << front.size () << std::endl;
    std::cerr << "In back: "
	      << back.size () << std::endl;    

    SoFaceSet *faceSet = new SoFaceSet;    
    SoVertexProperty *property = new SoVertexProperty;
    
    property->vertex.setNum (front.size ()*3);
    
    int j = 0;
    
    for (IgBSPTriangle::List::iterator i = back.begin (); 
	 i != back.end (); 
	 i++)
    {
	property->vertex.setValues (j++, 1, &((*i)->v (0)));	
	property->vertex.setValues (j++, 1, &((*i)->v (1)));	
	property->vertex.setValues (j++, 1, &((*i)->v (2)));	
	std::cerr << "Triangle: " << std::endl;
	std::cerr << (*i)->v (0)[0] << " "
		  << (*i)->v (0)[1] << " "	
		  << (*i)->v (0)[2] << " "
		  << std::endl;
	std::cerr << (*i)->v (1)[0] << " "
		  << (*i)->v (1)[1] << " "	
		  << (*i)->v (1)[2] << " "
		  << std::endl;
	std::cerr << (*i)->v (2)[0] << " "
		  << (*i)->v (2)[1] << " "	
		  << (*i)->v (2)[2] << " "
		  << std::endl;
    }

    std::vector<int> vertexIndex (front.size (), 3);
    
    faceSet->numVertices.setValues (0, front.size (), &vertexIndex[0]);    
    faceSet->vertexProperty = property;
    root->addChild (faceSet);    
    SoDrawStyle *lines = new SoDrawStyle;
    lines->style = SoDrawStyle::LINES;

    root->addChild (lines);    
    root->addChild (obj);

    return "IGUANA: IgBSPTree Test";
}
