#ifndef IG_TREE_MODEL_IG_TREE_REP_H
# define IG_TREE_MODEL_IG_TREE_REP_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgTreeModel/interface/IgTreeNode.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgRep.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_TREE_MODEL_API IgTreeRep : public IgRep, public IgTreeNode
{
public:
    ~IgTreeRep (void);
    // default constructor
    // default copy constructor
    // default assignment operator
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_TREE_MODEL_IG_TREE_REP_H
