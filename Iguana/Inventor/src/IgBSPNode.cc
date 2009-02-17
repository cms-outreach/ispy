//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgBSPNode.h"
#include <cassert>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>

static SbVec3f
pointOnPlane (const SbPlane &plane)
{
    SbVec3f planeNormal = plane.getNormal ();    
    return plane.getDistanceFromOrigin () * planeNormal;    
}

static bool
doPlaneIntersect (const SbPlane &plane1, const SbPlane &plane2)
{
    if (plane1 == plane2)
	return true;    

    SbVec3f n1 = plane1.getNormal ();
    SbVec3f n2 = plane2.getNormal ();
    
    if (n1 == n2 
	|| n1 == -n2)
    {
	// If the normals are the same, or opposite, they are parallel
	// and therefore don't intersect
	return false;	
    }
    
    return true;
}




//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgBSPNode::IgBSPNode (IgBSPTriangle *triangle)
    :m_front (0),
     m_back (0),
     m_hyperPlane (triangle->v (0), triangle->v (1), triangle->v (2))
{
    m_coplanarTriangles.push_back (triangle);
    triangle->ref ();
}

IgBSPNode::IgBSPNode (const SbPlane &plane)
    :m_front (0),
     m_back (0),
     m_hyperPlane (plane)
{
}

IgBSPNode::~IgBSPNode (void)
{
    if (m_front)
	delete m_front;	
    if (m_back)
	delete m_back;
    IgBSPTriangle::unrefAndDeleteTriangles (m_coplanarTriangles);
}


SbVec3f 
getFaceNormal (IgBSPTriangle &triangle)
{
    SbVec3f side1 = triangle.v (1) - triangle.v (0);
    SbVec3f side2 = triangle.v (2) - triangle.v (0);
    side1.normalize ();
    side2.normalize ();
    return side1.cross (side2);    
}


void
addTriangleInFront (IgBSPNode 			*&node, 
		    IgBSPTriangle	 	*triangle,
		    bool			addTriangle,
		    IgBSPTriangle::List 	*trianglesAddedInFront,
		    IgBSPTriangle::List 	*trianglesAddedInBack)
{
    // If the triangle is in front either pass it down the front
    // branch (if it exists) or create a new front branch.
    //
    // The node is actually added only if addTriangle is true.
    //
    // If the list of triangles added in front is there, the
    // triangle is also added there.

    if (node)
    {
	// if node is there, we navigate further.
	node->add (triangle, 
		   addTriangle,
		   trianglesAddedInFront, 
		   trianglesAddedInBack);
    }		
    else	
    {	    
	if (addTriangle)
	    node = new IgBSPNode (triangle);		    
	if (trianglesAddedInFront != 0)
	{
	    triangle->ref ();
	    trianglesAddedInFront->push_back (triangle);	    
	}
    }	
}

void
addTriangleInBack (IgBSPNode 			*&node, 
		   IgBSPTriangle 		*triangle,
		   bool				addTriangle,
		   IgBSPTriangle::List 	*trianglesAddedInFront,
		   IgBSPTriangle::List 	*trianglesAddedInBack)
{
    // If the back branch exists make the triangle go down it,
    // otherwise, create a new back branch, using the triangle as
    // initialization.
    //
    // The node is actually added only if addTriangle is true
    //
    // If the list of triangles added in back is there, the
    // triangle is also added there.

    assert (triangle);	
    
    if (node)
    {
	// if node is there, we navigate further.
	node->add (triangle, 
		   addTriangle,
		   trianglesAddedInFront, 
		   trianglesAddedInBack);
    }		
    else	
    {	    
	if (addTriangle)
	{  
	    node = new IgBSPNode (triangle);
	}
	if (trianglesAddedInBack != 0)
	{
	    triangle->ref ();
	    trianglesAddedInBack->push_back (triangle);	    
	}
    }	
}

void
IgBSPNode::add (IgBSPTriangle	 	*triangle,
		bool			addTriangle,
		IgBSPTriangle::List 	*trianglesAddedInFront,
		IgBSPTriangle::List 	*trianglesAddedInBack)
{
    assert (triangle);

    int coplanarMask = triangle->evaluateCoplanarMask (m_hyperPlane);
    int inHalfPlaneMask = triangle->evaluateInHalfPlaneMask (m_hyperPlane);

    if (IgBSPTriangle::areCoplanar (coplanarMask))
    {

	// If they are coplanar, we check how the normals of the
	// triangle and the plane relate to each other.
	//
	// If dot (n1, n2) > 0 (which means that the two are pointing
	// in the same direction) we add the node in the back.  
	//
	// If dot (n1, n2) < 0 (which means that the two are pointing
	// in opposite directions) we add the node in front.


	if (addTriangle)
	{
	    SbVec3f n1 = getFaceNormal (*triangle);
	    SbVec3f n2 = m_hyperPlane.getNormal ();
	    if (n1.dot (n2) < 0)
	    {
		return addTriangleInFront (m_front,
					   triangle,
					   addTriangle,
					   trianglesAddedInFront,
					   trianglesAddedInBack);
	    }
	    else
	    {
		return addTriangleInBack (m_back,
					  triangle,
					  addTriangle,
					  trianglesAddedInFront,
					  trianglesAddedInBack);
	    }
	}
	else
	{
	    // If we are not adding triangles to the tree, but only
	    // navigating
	    
	    return addTriangleInBack (m_front,
				      triangle,
				      addTriangle,
				      trianglesAddedInFront,
				      trianglesAddedInBack);
	}	
    }
    else if (IgBSPTriangle::areIntersecting (coplanarMask, inHalfPlaneMask))
    {
	// If a triangle is intersecting the hyperplane, split it in
	// several sub-triangles part of which will be put in the
	// front node, others in the back node.  
	//
	// If those node do not exist, create them, using one of the
	// triangles in the respective lists to define the hyperplane,
	// and add all the rest as coplanar.
 
	IgBSPTriangle::List splittedTriangles;
	triangle->split (m_hyperPlane, 
			 splittedTriangles, 
			 coplanarMask, 
			 inHalfPlaneMask);

	splittedTriangles.sort (IgBSPTriangle::LessArea ());	

	// If addTriangle is true, the BSPNode has the ownership of
	// the triangles being added, so we are free to delete those
	// who need to be splitted as they will never end-up in the
	// tree.
	//
	// FIXME: what happens in addTriangle is false? Do I still
	// need to delete them? The first time I split a triangle, in
	// theory, I shouldn't because the triangle being splitted is
	// owned by whom is checking it against the BSP tree, but
	// subsequent splits, should be deleted, as they are just
	// temporary triangles.

	IgBSPTriangle::List::iterator end (splittedTriangles.end ());
	
	for (IgBSPTriangle::List::iterator i = splittedTriangles.begin ();
	     i != end;
	     i++)
	{	
	    add (*i,
		 addTriangle,
		 trianglesAddedInFront, 
		 trianglesAddedInBack);
	}
	IgBSPTriangle::unrefAndDeleteTriangles(splittedTriangles);
    }
    else if (IgBSPTriangle::isInFront (coplanarMask, inHalfPlaneMask))
    {
	addTriangleInFront (m_front,
			    triangle,
			    addTriangle,
			    trianglesAddedInFront, 
			    trianglesAddedInBack);
    }
    else 
    {
	// If we are here, it means that the triangle is in the back
	// of the current plane. 
	addTriangleInBack (m_back,
			   triangle,
			   addTriangle,
			   trianglesAddedInFront, 
			   trianglesAddedInBack);
    }
}


void
IgBSPNode::add (IgBSPTriangle::List &list)
{
    list.sort (IgBSPTriangle::LessArea ());

    IgBSPTriangle::List::iterator end (list.end ());
    
    for (IgBSPTriangle::List::iterator i = list.begin ();
	 i != end;
	 i++)
    {
	add ((*i), true, 0, 0);	
    }	 
}


void
addPlane (IgBSPNode *&treeNode, const SbPlane &plane)
{
    if (treeNode)
    {
	// If it is not a leaf, continue the navigation process.
	treeNode->add (plane);	
    }
    else
    {
	treeNode = new IgBSPNode (plane);	
    }    
}

const SbPlane &
IgBSPNode::plane (void)
{
    return m_hyperPlane;    
}


void
IgBSPNode::add (const SbPlane &plane)
{
    if (this->plane () == plane)
    {	
    }    
    else if (doPlaneIntersect (this->plane (), plane))
    {
	// The two planes intersect, add the plane on both sides.
	addPlane (m_back, plane);	
	addPlane (m_front, plane);	
    }
    else
    {
	// The two planes are parallel, add only in front or in the
	// back accordingly
	if (this->plane ().isInHalfSpace (pointOnPlane (plane)))
	{
	    addPlane (m_front, plane);	    
	}
	else
	{
	    addPlane (m_back, plane);	    
	}		
    }
}


int
IgBSPNode::getTriangleCount (void)
{
    int count = m_coplanarTriangles.size ();
    count += m_front ? m_front->getTriangleCount () : 0;
    count += m_back ? m_back->getTriangleCount () : 0;    
    return count;    
}

IgBSPNode *&
IgBSPNode::front (void)
{ return m_front; }

IgBSPNode *&
IgBSPNode::back (void)
{ return m_back; }
