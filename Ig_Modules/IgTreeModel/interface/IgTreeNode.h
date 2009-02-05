#ifndef IG_TREE_MODEL_IG_TREE_NODE_H
# define IG_TREE_MODEL_IG_TREE_NODE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgTreeModel/interface/config.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_TREE_MODEL_API IgTreeNode
{
public:
    virtual ~IgTreeNode (void);
    // default constructor
    // default copy constructor
    // default assignment operator

    virtual IgTreeNode *	parent (void) const = 0;
    virtual unsigned		children (void) const = 0;
    virtual unsigned		child_index_of (const IgTreeNode *child) const = 0;
    virtual IgTreeNode *	child (unsigned index) const = 0;

    virtual bool		is_leaf (void) const = 0;

    virtual bool		allow_children (void) const = 0;

    // FIXME: iteration? just use IgTreeNodeIterator?
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_TREE_MODEL_IG_TREE_NODE_H
