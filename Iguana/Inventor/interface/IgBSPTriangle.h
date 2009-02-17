#ifndef IG_OPEN_INVENTOR_IG_BSP_TRIANGLE_H
# define IG_OPEN_INVENTOR_IG_BSP_TRIANGLE_H

//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/config.h"
#include <Inventor/SbLinear.h>
#include <list>
#include <iosfwd>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_OPEN_INVENTOR_API IgBSPTriangle
{
public:
    struct LessArea
    {
	bool operator ()(IgBSPTriangle *a, IgBSPTriangle *b);	
    };

    struct MoreArea
    {
	bool operator ()(IgBSPTriangle *a, IgBSPTriangle *b);	
    };
    
    
    enum 
    { 
	POINT_A = 1,
	POINT_B = 2,
	POINT_C = 4,
	ALL_POINTS = 7
    };    
    
    typedef std::list<IgBSPTriangle *> List;
    IgBSPTriangle (const SbVec3f &a, const SbVec3f &b, const SbVec3f &c);
    IgBSPTriangle (const IgBSPTriangle &t);

    int evaluateCoplanarMask (const SbPlane &plane);    
    int evaluateInHalfPlaneMask (const SbPlane &plane);
    float area (void);
    
    static bool areCoplanar (int coplanarMask);
    static bool isInFront (int coplanarMask, int inHalfPlaneMask);
    static bool isInBack (int coplanarMask, int inHalfPlaneMask);
    static bool areIntersecting (int coplanarMask, int inHalfPlaneMask);
    static void largestOnTop (List &triangleList);
    
    const SbVec3f	&v (int n) const;
    bool		good (void) const;    
    SbVec3f		barycenter (void) const;    
    void 		split (const SbPlane &plane, 
			       IgBSPTriangle::List &result,
			       int coplanarMask,
			       int inFrontMask);    

    void 		ref (void);
    void		unref (void);    
    static void         unrefAndDeleteTriangles(IgBSPTriangle::List &triangles);
    static void         unrefAndDeleteTriangle(IgBSPTriangle *triangle);
protected:
    ~IgBSPTriangle (void);    
private:
    SbVec3f	m_vertex[3];    
    unsigned int	m_refCounter;    
};

std::ostream &
operator<< (std::ostream &stream, const IgBSPTriangle &triangle);

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_OPEN_INVENTOR_IG_BSP_TRIANGLE_H
