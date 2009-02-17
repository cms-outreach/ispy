#ifndef IG_OPEN_INVENTOR_IG_SURFACE_OPERATION_H
# define IG_OPEN_INVENTOR_IG_SURFACE_OPERATION_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/config.h"
# include <list>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class SoNode;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

#define MIN_POINT_DISTANCE 0.0000001

class IgSurfaceOperation
{
public:
    enum OPERATION { INTERSECTION,
		     SUBTRACTION,
		     UNION,
		     SURFACE_1_IN_2,		  
		     SURFACE_2_IN_1,		  
		     SURFACE_1_OUT_2,		  
		     SURFACE_2_OUT_1		  
    };
        
    IgSurfaceOperation (int operatingMode);
    void addNode (SoNode *node);
    SoNode *apply (void);  
    SoNode *apply (SoNode *a, SoNode *b);    

private:
    typedef std::list <SoNode *> NodeList;
    typedef std::list<SoNode *>::const_iterator NodeIterator;    
    
    NodeList m_nodes;
    int m_operation;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_OPEN_INVENTOR_IG_SURFACE_OPERATION_H
