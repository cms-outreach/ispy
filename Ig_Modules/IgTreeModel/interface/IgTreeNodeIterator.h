#ifndef IG_TREE_MODEL_IG_TREE_NODE_ITERATOR_H
# define IG_TREE_MODEL_IG_TREE_NODE_ITERATOR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgTreeModel/interface/config.h"
# include <iterator>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgTreeNode;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IgTreeNodeIterator
{
    // FIXME: iterator inheritance: bidirectional_iterator
    // FIXME: not assignable, reference is not a reference type!
public:
    typedef std::bidirectional_iterator_tag	iterator_category;
    typedef IgTreeNode *			value_type;
    typedef ptrdiff_t				distance_type;
    typedef IgTreeNode **			pointer;  // FIXME? needed?
    typedef IgTreeNode *			reference;

    IgTreeNodeIterator (void);
    IgTreeNodeIterator (const IgTreeNode *node, unsigned index);
    // default constructor
    // default copy constructor
    // default assignment operator

    bool		operator== (const IgTreeNodeIterator &x) const;

    IgTreeNode *	operator* (void) const;
    IgTreeNodeIterator &operator++ (void);
    IgTreeNodeIterator	operator++ (int);
    IgTreeNodeIterator &operator-- (void);
    IgTreeNodeIterator	operator-- (int);

private:
    const IgTreeNode	*m_parent;
    unsigned		m_child_index;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_TREE_MODEL_IG_TREE_NODE_ITERATOR_H
