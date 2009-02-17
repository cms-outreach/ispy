//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSurfaceOperation.h"
#include "Iguana/Inventor/interface/IgBSPTriangle.h"
#include "Iguana/Inventor/interface/IgBSPTree.h"
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoShape.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/SbLinear.h>
#include <cassert>
#include <map>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>

struct SbVec3fPtrComparison
{
    bool operator()(const SbVec3f &a, const SbVec3f &b)
	{
	    float diffX = a[0] - b[0];
	    float diffY = a[1] - b[1];
	    float diffZ = a[2] - b[2];

	    if (fabs (diffX) < MIN_POINT_DISTANCE)
		if (fabs (diffY) < MIN_POINT_DISTANCE)
		    if (fabs (diffZ) < MIN_POINT_DISTANCE)
			return false;
		    else return diffZ < 0;
		else return diffY < 0;
	    else return diffX < 0;	    
	}    
};


static SoIndexedFaceSet *
listToNode (IgBSPTriangle::List *list)
{
    SoIndexedFaceSet *indexedFaceSet = new SoIndexedFaceSet;
    SoVertexProperty *vertexProperty = new SoVertexProperty;
	    
    int lastIndex = 0;
    int j = 0;
    
    std::map <const SbVec3f, int, SbVec3fPtrComparison> indicesMap;
    
    int vertexIndices[4] = {0, 0, 0, -1};
    IgBSPTriangle::List::iterator end (list->end ());
    for (IgBSPTriangle::List::iterator i = list->begin ();
	i != end;
	i++)
    {
	// Updates the vertexMap with the vertices from the new
	// triangle.
	for (int k = 0; k < 3; k++)
	{
	    const SbVec3f &vertex = (*i)->v (k);
	    if (indicesMap.find (vertex) == indicesMap.end ())
	    {
		vertexProperty->vertex.setValues (lastIndex, 1, &vertex);
		indicesMap[vertex] = lastIndex++;
	    }
	    vertexIndices[k] = indicesMap[vertex];	    
	}
    
	indexedFaceSet->coordIndex.setValues (j, 4, vertexIndices);
	j += 4;    
    }

    //    std::cerr << "Num of vertexes: " << lastIndex
    //	      << "Num of faces: " << j/4 << std::endl;
    
    indexedFaceSet->vertexProperty = vertexProperty;
	    
    return indexedFaceSet;
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgSurfaceOperation::IgSurfaceOperation (int operation)
    : m_operation (operation)
{}
    
void
IgSurfaceOperation::addNode (SoNode *node)
{
    m_nodes.push_back (node);
}

SoNode *
IgSurfaceOperation::apply (void)
{
    assert ("Multiple operand operations not supported yet" == 0);
    
    return 0;	    
}

SoNode *
IgSurfaceOperation::apply (SoNode *a, SoNode *b)
{
    IgBSPTree tree;
    
    // WARNING: the triangles in these list are valid until the BSP
    // tree is deleted.
    IgBSPTriangle::List trianglesInFront;
    IgBSPTriangle::List trianglesInBack;
    SoIndexedFaceSet *node = 0;

    switch (m_operation)
    {
    case INTERSECTION:
	assert ("operation not supported" == 0);	
	break;

    case SUBTRACTION:
	assert ("operation not supported" == 0);	
	break;
	
    case UNION:
	assert ("operation not supported" == 0);	
	break;			

    case SURFACE_1_IN_2:
	tree.addOnlyOneFace (a);	
	tree.add (b);
	tree.intersect (a, 0, &trianglesInBack);    
	node = listToNode (&trianglesInBack);
	break;

    case SURFACE_2_IN_1:
	tree.addOnlyOneFace (b);	
	tree.add (a);
	tree.intersect (b, 0, &trianglesInBack);    
	node = listToNode (&trianglesInBack);
	break;
	
    case SURFACE_1_OUT_2:
	tree.addOnlyOneFace (a);	
	tree.add (b);
	tree.intersect (a, &trianglesInFront, 0);    
	node = listToNode (&trianglesInFront);
	break;

    case SURFACE_2_OUT_1:
	tree.addOnlyOneFace (b);	
	tree.add (a);
	tree.intersect (b, &trianglesInFront, 0);    
	node = listToNode (&trianglesInFront);
	break;
    }
    IgBSPTriangle::unrefAndDeleteTriangles (trianglesInFront);
    IgBSPTriangle::unrefAndDeleteTriangles (trianglesInBack);
        
    return node;
}
