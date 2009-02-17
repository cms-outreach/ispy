#ifndef IG_OPEN_INVENTOR_IG_BSP_NODE_H
# define IG_OPEN_INVENTOR_IG_BSP_NODE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/config.h"
# include "Iguana/Inventor/interface/IgBSPTriangle.h"
# include <Inventor/SbLinear.h>
# include <list>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_OPEN_INVENTOR_API IgBSPNode
{
public:
    // implicit copy constructor
    // implicit assignment operator
    IgBSPNode (IgBSPTriangle *triangle);
    IgBSPNode (const SbPlane &plane);
    ~IgBSPNode (void);

    
    void add (IgBSPTriangle *triangle,
	      bool addTriangle,
	      IgBSPTriangle::List *trianglesAddedInFront,
	      IgBSPTriangle::List *trianglesAddedInBack);    
    void add (const SbPlane &plane);    
    void add (IgBSPTriangle::List &list);
    

    const SbPlane &plane (void);
    
    int getTriangleCount (void);    

    IgBSPNode *&front (void);
    IgBSPNode *&back (void);
private:
    IgBSPTriangle::List	m_coplanarTriangles;
    IgBSPNode			*m_front;
    IgBSPNode			*m_back;    
    SbPlane			m_hyperPlane;
};


//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_OPEN_INVENTOR_IG_BSP_NODE_H
