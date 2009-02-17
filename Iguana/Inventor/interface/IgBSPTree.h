#ifndef IG_OPEN_INVENTOR_IG_BSP_TREE_H
# define IG_OPEN_INVENTOR_IG_BSP_TREE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/config.h"
# include "Iguana/Inventor/interface/IgBSPTriangle.h"
# include "Iguana/Inventor/interface/IgBSPNode.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class SoNode;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_OPEN_INVENTOR_API IgBSPTree
{
public:
    enum 
    {
	COPLANAR_IN_FRONT,
	COPLANAR_IN_BACK,
	COPLANAR_IN_NODE
    };

    IgBSPTree (void);    
    ~IgBSPTree (void);    
    void add (SoNode *node);
    void addOnlyOneFace (SoNode *node);
    
    void intersect (IgBSPTriangle *triangle,     
		    IgBSPTriangle::List *frontTriangles,
		    IgBSPTriangle::List *backTriangles);

    void intersect (SoNode *node,     
		    IgBSPTriangle::List *frontTriangles,
		    IgBSPTriangle::List *backTriangles);
    
    int getTriangleCount (void);    
    
    /// @return true if the BSP-Tree is empty.
    bool empty (void);
    
private:
    IgBSPNode *m_rootNode;        
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_OPEN_INVENTOR_IG_BSP_TREE_H
