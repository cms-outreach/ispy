//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Extensions/IgOpenInventor/interface/IgBSPTriangle.h"
#include <iostream>
#include <assert.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

std::ostream &
operator<< (std::ostream &stream, const IgBSPTriangle &triangle)
{
    stream << "Triangle:" << std::endl;    
    stream << triangle.v (0)[0] << " " 
	   << triangle.v (0)[1] << " " 
	   << triangle.v (0)[2] << " " << std::endl;    
    stream << triangle.v (1)[0] << " " 
	   << triangle.v (1)[1] << " " 
	   << triangle.v (1)[2] << " " << std::endl;    
    stream << triangle.v (2)[0] << " " 
	   << triangle.v (2)[1] << " " 
	   << triangle.v (2)[2] << " " << std::endl;    
    return stream;    
}


IgBSPTriangle::IgBSPTriangle (const SbVec3f &a, 
			      const SbVec3f &b, 
			      const SbVec3f &c)
    :m_refCounter (1)
{
    m_vertex[0] = a;
    m_vertex[1] = b;
    m_vertex[2] = c;
}

IgBSPTriangle::IgBSPTriangle (const IgBSPTriangle &t)
    :m_refCounter (1)
{
    m_vertex[0] = t.v (0);
    m_vertex[1] = t.v (1);
    m_vertex[2] = t.v (2);
}

const SbVec3f &
IgBSPTriangle::v (int n) const
{
    return m_vertex[n];    
}

bool
IgBSPTriangle::good (void) const
{
     SbVec3f side1 = v (1) - v (0);
     SbVec3f side2 = v (2) - v (0);
     side1.normalize ();
     side2.normalize ();
     
     SbVec3f norm = side1.cross (side2);
     
     return norm.length () > 0.000001;     
}

SbVec3f
IgBSPTriangle::barycenter (void) const
{
    SbVec3f t = v (0) + v (1) + v (2);
    t /= 3;

    return t;    
}

static float
getDistance (const SbPlane &plane, const SbVec3f &point)
{
    // With TGS, we have to calculate the distance by hand.  This is
    // done by sutracting the point position vector with a vector
    // pointing to whatever point on the plane and then getting the
    // projection of the resulting vector along the vector normal to
    // the plane.
    SbVec3f planeNormal = plane.getNormal ();    
    SbVec3f planePoint = plane.getDistanceFromOrigin () * planeNormal;    
    SbVec3f t = point - planePoint;
    return fabs (t.dot (planeNormal));    
}

#define MIN_DISTANCE 0.00001

static bool
isOnPlane (const SbPlane &plane, const SbVec3f &point)
{
    return getDistance (plane, point) < MIN_DISTANCE;
}


int
IgBSPTriangle::evaluateCoplanarMask (const SbPlane &plane)
{
    // This returns a mask which has bit n set to 1 if point n is 
    // on the plane.
    return ((int) isOnPlane (plane, v (0)))
	| (((int) isOnPlane (plane, v (1))) << 1)
	| (((int) isOnPlane (plane, v (2))) << 2);
}

int
IgBSPTriangle::evaluateInHalfPlaneMask (const SbPlane &plane)
{
    // This returns a mask which has bit n set to 1 if point n is in
    // front of the plane.
    return ((int) plane.isInHalfSpace (v (0)))
	| (((int) plane.isInHalfSpace (v (1))) << 1)
	| (((int) plane.isInHalfSpace (v (2))) << 2);
}


bool
IgBSPTriangle::areCoplanar (int coplanarMask)
{
    return coplanarMask == IgBSPTriangle::ALL_POINTS;    
}

bool 
IgBSPTriangle::isInFront (int coplanarMask, int inHalfPlaneMask)
{
    return (coplanarMask | inHalfPlaneMask) == IgBSPTriangle::ALL_POINTS;    
}

bool
IgBSPTriangle::isInBack (int coplanarMask, int inHalfPlaneMask)
{
    // Check if all the points of a triangle are in the back of the
    // plane (or at most they touch it).  
    //
    // First create a mask with bit n set to 1 for point n being in
    // the back. This is done by doing a not operation on the
    // isInFrontMask, and then masking out the 3 significant bit.
    // Than mask out coplanar points by doing an or of the mask of
    // coplanar points with the mask of points in the back.  If all
    // the points are in the back, the result should be all the bits
    // set to 1.
    int isInBackMask = inHalfPlaneMask ^ IgBSPTriangle::ALL_POINTS;    
    return (coplanarMask | isInBackMask) == IgBSPTriangle::ALL_POINTS;    
}

bool
IgBSPTriangle::areIntersecting (int coplanarMask, int inHalfPlaneMask)
{
    return !isInFront (coplanarMask,  inHalfPlaneMask) 
	&& !isInBack (coplanarMask,  inHalfPlaneMask);    
}

#define NEXT_VERTEX(v) (v + 1) % 3
#define PREV_VERTEX(v) (v + 2) % 3

void
IgBSPTriangle::split (const SbPlane &plane, 
		      IgBSPTriangle::List &result,
		      int coplanarMask,
		      int inFrontMask)
{
    // This splits a triangle which is intersecting the plane passed.
    
    // First of all we handle the special case in which one of the
    // points is actually on the plane.
    // Two triangles results in this case.
    int cv; // index of the vertex which is coplanar to the cutting plane.
    if (coplanarMask == IgBSPTriangle::POINT_A) cv = 0;
    else if (coplanarMask == IgBSPTriangle::POINT_B) cv = 1;
    else if (coplanarMask == IgBSPTriangle::POINT_C) cv = 2;
    else cv = -1; 

    if (cv >= 0)
    {
	int cvn = NEXT_VERTEX(cv); // index of the point which is after the
	// coplanar point in counterclockwise ordering.
	int cvp = PREV_VERTEX(cv); // index of the point which is before the
	// coplanar point in counterclockwise ordering.

	SbVec3f intersectionPoint;
	plane.intersect (SbLine (v (cvn), v (cvp)), 
			 intersectionPoint);	
    
	IgBSPTriangle *t1 = new IgBSPTriangle (v (cv),
					       v (cvn),
					       intersectionPoint);
	IgBSPTriangle *t2 = new IgBSPTriangle (v (cv),
					       intersectionPoint,
					       v (cvp));
		
	if (t1->good ())
	    result.push_back (t1);
	else
	    delete t1;
	
	if (t2->good ())
	    result.push_back (t2);
	else
	    delete t2;
	
	return;
    }
    
    // Now we handle the case in which none of the vertices are
    // coplanar with the plane.

    // index of the vertex which is alone on one side of the plane.
    int av = 0; 
    // mask with bit n set to 1 if point n is behind the plane.
    int inBackMask = (inFrontMask ^ IgBSPTriangle::ALL_POINTS);
    
    assert (inFrontMask != IgBSPTriangle::ALL_POINTS);
    assert (inBackMask  != IgBSPTriangle::ALL_POINTS);
    
    if ((inFrontMask == IgBSPTriangle::POINT_A) 
	|| (inBackMask == IgBSPTriangle::POINT_A)) av = 0;
    else if ((inFrontMask == IgBSPTriangle::POINT_B) 
	     || (inBackMask == IgBSPTriangle::POINT_B)) av = 1;
    else if ((inFrontMask == IgBSPTriangle::POINT_C) 
	     || (inBackMask == IgBSPTriangle::POINT_C)) av = 2;
    else 
	assert ("split: This should never be reached!" == 0);
    
    // index of the vertex which is after the one
    // alone on one side of the plane in
    // counterclockwise ordering.
    int avn = NEXT_VERTEX(av); 
    // index of the vertex which is before the one
    // alone on one side of the plane in
    // counterclockwise ordering.
    int avp = PREV_VERTEX(av);     

    SbVec3f intersectionWithNext;
    SbVec3f intersectionWithPrev;
    
    plane.intersect (SbLine (v (av), v (avn)),
		     intersectionWithNext);
    plane.intersect (SbLine (v (avp), v (av)),
		     intersectionWithPrev);

    IgBSPTriangle *t1 = new IgBSPTriangle (v (av),
					   intersectionWithNext,
					   intersectionWithPrev);
    IgBSPTriangle *t2 = new IgBSPTriangle (intersectionWithNext,
					   v (avn),
					   v (avp));
    IgBSPTriangle *t3 = new IgBSPTriangle (v (avp),
					   intersectionWithPrev,
					   intersectionWithNext);

    if (t1->good ())
        result.push_back (t1);
    else
	delete t1;
    
    if (t2->good ())
        result.push_back (t2);
    else
	delete t2;
    
    if (t3->good ())
        result.push_back (t3);
    else
	delete t3;
}

float
IgBSPTriangle::area (void)
{
    SbVec3f side1 = v (0) - v (1);
    SbVec3f side2 = v (0) - v (2);
    return fabs (side1.cross (side2).length ());    
}


void
IgBSPTriangle::largestOnTop (List &triangleList)
{
    List::iterator biggest;
    float maxArea;
    List::iterator end (triangleList.end ());

    for (List::iterator i = triangleList.begin ();
	 i != end;
	 i++)
    {
	float currentArea = (*i)->area ();
	
	if ( maxArea < currentArea)
	{
	    maxArea = currentArea;	    
	    biggest = i;	    
	}
    }

    triangleList.splice (triangleList.begin (),
			 triangleList,
			 biggest);    
}

IgBSPTriangle::~IgBSPTriangle (void)
{    
}

void
IgBSPTriangle::ref (void)
{
    m_refCounter++;    
}

void
IgBSPTriangle::unref (void)
{
    assert (m_refCounter >= 1);
    m_refCounter--;
}

bool
IgBSPTriangle::LessArea::operator() (IgBSPTriangle *a, IgBSPTriangle *b)
{
    if (a->area () < b->area ())
	return true;
    else
	return false;    
}

bool
IgBSPTriangle::MoreArea::operator() (IgBSPTriangle *a, IgBSPTriangle *b)
{
    if (a->area () > b->area ())
	return true;
    else
	return false;    
}

void
IgBSPTriangle::unrefAndDeleteTriangles (IgBSPTriangle::List &triangles)
{
    List::iterator i = triangles.begin ();
    List::iterator end (triangles.begin ());
    while ( i != end)
    {
      (*i)->unref ();
      if ((*i)->m_refCounter == 0)
      {
        delete (*i);
	List::iterator j = i;
	i++;
	triangles.erase (j);
      }
      else
        i++;
    }
}

void
IgBSPTriangle::unrefAndDeleteTriangle (IgBSPTriangle *triangle)
{
    triangle->unref ();
    if (triangle->m_refCounter == 0)
      delete triangle;
}
