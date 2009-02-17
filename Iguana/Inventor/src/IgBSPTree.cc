//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgBSPTree.h"
#include <iostream>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/nodes/SoNode.h>


//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

// FIXME: shall we remove this?

static void
nodeToList (void * userData, 
	    SoCallbackAction *action, 
	    const SoPrimitiveVertex * v1, 
	    const SoPrimitiveVertex * v2, 
	    const SoPrimitiveVertex * v3)
{
    // Get the points and transform them to world coordinates.
    const SbVec3f vtx[] = { v1->getPoint(), 
			    v2->getPoint(), 
			    v3->getPoint() };
    const SbMatrix mm = action->getModelMatrix ();
    SbVec3f vx[3];
    for (int j = 0; j < 3; j++) { mm.multVecMatrix (vtx[j], vx[j]); }

    IgBSPTriangle *triangle = 0;
        
    if (action->getVertexOrdering () == SoShapeHints::CLOCKWISE)
    {
	triangle = new IgBSPTriangle (vx[1], vx[0], vx[2]);
    }
    else
    {
	triangle = new IgBSPTriangle (vx[0], vx[1], vx[2]);
    }
    
    IgBSPTriangle::List *list = 
	static_cast<IgBSPTriangle::List *> (userData);
    
    list->push_back (triangle);
}

IgBSPTree::IgBSPTree (void)
    :m_rootNode (0)
{
}

IgBSPTree::~IgBSPTree (void)
{
    if (m_rootNode)
	delete m_rootNode;
}

void 
IgBSPTree::addOnlyOneFace (SoNode */*node*/)
{
    //    SoCallbackAction action;
    //action.addTriangleCallback (SoNode::getClassTypeId (), 
    //				faceToBSP,
    //				(void *) &m_rootNode);
    //    action.apply (node);
    //std::cerr << "Number of triangles in the BSP tree: " 
    // << getTriangleCount () << std::endl;    
}

int level = 0;

SbVec3f
calculateBarycenter (IgBSPTriangle::List &triangleList)
{
    SbVec3f result (0, 0, 0);
    assert (triangleList.begin () != triangleList.end ());
    
    IgBSPTriangle::List::iterator end (triangleList.end ());
    
    for (IgBSPTriangle::List::iterator i = triangleList.begin ();
	 i != end;
	 i++)
    {      
	result += (*i)->barycenter ();	
    }

    result /= (float) triangleList.size ();
    return result;    
}

float
evaluatePlane (SbPlane &plane, IgBSPTriangle::List &triangleList)
{
    SbVec3f normal = plane.getNormal ();
    assert (normal [0] || normal [1] || normal [2]);
    
    float frontTriangles = 0;
    float backTriangles = 0;
    float coplanarTriangles = 0;
    float totalTriangles = 0;
    
    assert (triangleList.begin () != triangleList.end ());

    IgBSPTriangle::List::iterator end (triangleList.end ());
    
    for (IgBSPTriangle::List::iterator i = triangleList.begin ();
	 i != end;
	 i++)
    {
	totalTriangles++;
	
	int coplanarMask = (*i)->evaluateCoplanarMask (plane);
	int intersectionMask = (*i)->evaluateInHalfPlaneMask (plane);

	if (IgBSPTriangle::isInFront (coplanarMask, 
				      intersectionMask))
	    frontTriangles++;
	else if (IgBSPTriangle::isInBack (coplanarMask, 
					  intersectionMask))
	    backTriangles++;
	else if (IgBSPTriangle::areCoplanar (coplanarMask))
	    coplanarTriangles++;	
    }    

    if (totalTriangles == 0)
	return 0;    

    float splittingFactor = (frontTriangles 
			     + backTriangles 
			     + coplanarTriangles) / totalTriangles;

    float balancingFactor;
    
    if (frontTriangles+backTriangles)
	balancingFactor = 1 - fabs ((frontTriangles - backTriangles) /
				    (frontTriangles + backTriangles));
    else
	balancingFactor = 0;
    

    float coplanarFactor = coplanarTriangles / totalTriangles;

    float total = 0.4 * splittingFactor 
		  + 0.4 * balancingFactor 
		  + 0.2 * coplanarFactor;     
    
    return total;    
}

void 
debug (const IgBSPTriangle::List &triangleList)
{
    for (int i = 0; i < level; i++)
	std::cerr << " ";
    std::cerr << "Num of triangles: " << triangleList.size () << std::endl;
}

void 
addBestPlane (IgBSPNode *&rootNode, 
	      IgBSPTriangle::List &triangleList)
{    
    //    debug (triangleList);
    
    level++;    

    if (triangleList.size ())
    {
	
	//Calculate the best plane possible for this set of points
	//
	// The position of the plane is given by the average of all the
	// barycenters.  
	SbVec3f planePos = calculateBarycenter (triangleList);
	// Evaluate which of the three cardinal planes passing for
	// planePos is the best suited for doing the subdivision.
	SbPlane bestPlane (SbVec3f (1,0,0), 0);
	float bestPlaneScore = 0;
    
	for (int j = 0; j < 3; j++)
	{
	    SbVec3f planeNormal (0, 0, 0);
	    planeNormal[j] = 1;
	    SbPlane plane = SbPlane (planeNormal, planePos);
	
	    float planeScore = evaluatePlane (plane, triangleList);
	
	    if (bestPlaneScore < planeScore)
	    {
		bestPlane = plane;
		bestPlaneScore = planeScore;	    
	    }	
	}
    
	if (triangleList.size () > 10
	    && level < 10
	    && bestPlaneScore > 0.7)
	{
	    // Add the plane, making sure that a BSP node exist.
	    assert (rootNode == 0);
	    rootNode = new IgBSPNode (bestPlane);
    
	    // Generate a list of triangles that should go in front and that
	    // of those which should go in the back.  We include also the
	    // triangles originating from the split against the cutting plane
    
	    IgBSPTriangle::List frontList;
	    IgBSPTriangle::List backList;
	    IgBSPTriangle::List::iterator end (triangleList.end ());
	    
	    for (IgBSPTriangle::List::iterator i = triangleList.begin ();
		 i != end;
		 i++)
	    {
		int coplanarMask = (*i)->evaluateCoplanarMask (bestPlane);
		int intersectionMask = (*i)->evaluateInHalfPlaneMask (bestPlane);
	
		(*i)->ref ();
		if (IgBSPTriangle::areIntersecting (coplanarMask, intersectionMask))
		{
		    // If we are intersecting, splits the triangles, adds the
		    // results 
		    IgBSPTriangle::List splittedList;

		    (*i)->split (bestPlane, 
				 splittedList, 
				 coplanarMask, 
				 intersectionMask);
				 
		    (*i)->unref ();
		    IgBSPTriangle::List::iterator end (splittedList.end ());
		    for (IgBSPTriangle::List::iterator j = splittedList.begin ();
			 j != end;
			 j++)
		    {
			int cMask = (*j)->evaluateCoplanarMask (bestPlane);
			int iMask = (*j)->evaluateInHalfPlaneMask (bestPlane);
		
			if (IgBSPTriangle::isInFront (cMask, iMask))
			{
			    (*j)->ref ();
			    frontList.push_back ((*j));
			}
			else if (IgBSPTriangle::isInBack (cMask, iMask))
			{
			    (*j)->ref ();
			    backList.push_back ((*j));
			}
		    }
		    IgBSPTriangle::unrefAndDeleteTriangles (splittedList);	
		}
		else if (IgBSPTriangle::isInFront (coplanarMask, intersectionMask))
		    frontList.push_back ((*i));	    
		else if (IgBSPTriangle::isInBack (coplanarMask, intersectionMask))
		    backList.push_back ((*i));
		else
		    rootNode->add ((*i), true, 0, 0);
		// FIXME: handle the case in which they are coplanar...
	    }

	    // Keep on going in subdividing.
	    if (frontList.begin () != frontList.end ())
		addBestPlane (rootNode->front (), frontList);
	    if (backList.begin () != backList.end ())
		addBestPlane (rootNode->back (), backList);
            IgBSPTriangle::unrefAndDeleteTriangles (frontList);
	    IgBSPTriangle::unrefAndDeleteTriangles (backList);
	}
	else
	{
	    // If there are less then 50 triangles in the left or we have
	    // already too much recursion, start to add triangles in the
	    // usual way.  
	    //
	    // FIXME: start with the biggest one (shall we sort the list?).
	    if (triangleList.begin () != triangleList.end ())
	    {
		if (rootNode == 0)
		{
		    IgBSPTriangle *t = *triangleList.begin ();
		    rootNode = new IgBSPNode (SbPlane (t->v (0), t->v (1), t->v (2)));
		}
		rootNode->add (triangleList);
	    }
	}
    }
    
    level--;	
    return;
}

void
IgBSPTree::add (SoNode *node)
{
    IgBSPTriangle::List triangleList;
    
    SoCallbackAction action;
    action.addTriangleCallback (SoNode::getClassTypeId (), 
				nodeToList, 
				(void *) &triangleList);
    action.apply (node);    
    
    // Add the best splitting planes for this set of triangles.
    //    addBestPlane (m_rootNode, triangleList);

    //    if (triangleList.begin () != triangleList.end ())
    // {
    //	if (m_rootNode == 0)
    //	{
    //	    m_rootNode = new IgBSPNode ((*triangleList.begin ()));
    //	}
    //	m_rootNode->add (triangleList);    
    //}
    
    addBestPlane (m_rootNode, triangleList);
    IgBSPTriangle::unrefAndDeleteTriangles (triangleList);
}

void
IgBSPTree::intersect (IgBSPTriangle *triangle,     
		      IgBSPTriangle::List *frontTriangles,
		      IgBSPTriangle::List *backTriangles)
{
    if (!empty ())
    {
	// WARNING: add takes ownerwhip of triangle, so such a pointer
	// could be not valid anymore after this call.
	m_rootNode->add (triangle, 
			 false, 
			 frontTriangles, 
			 backTriangles);    
    }
    else 
    {
	//	std::cerr << "IgBSPTree::intersect : doing intersection against empty tree!" 
	//	  << std::endl;	
    }
}

void
IgBSPTree::intersect (SoNode *node,     
		      IgBSPTriangle::List *frontTriangles,
		      IgBSPTriangle::List *backTriangles)
{
    IgBSPTriangle::List triangleList;
    
    SoCallbackAction action;
    action.addTriangleCallback (SoNode::getClassTypeId (), 
				nodeToList, 
				(void *) &triangleList);
    action.apply (node);

    IgBSPTriangle::List::iterator end (triangleList.end ());
    
    for (IgBSPTriangle::List::iterator i = triangleList.begin ();
	 i != end;
	 i++)
    {
	intersect ((*i), frontTriangles, backTriangles);	
    }
    IgBSPTriangle::unrefAndDeleteTriangles (triangleList);  
}

int
IgBSPTree::getTriangleCount (void)
{
    //return m_rootNode ? m_rootNode->getTriangleCount () : -1;    
    return 0;    
}

bool
IgBSPTree::empty (void)
{ return m_rootNode == 0; }
