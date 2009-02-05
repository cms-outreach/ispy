#ifndef IG_TREE_MODEL_IG_SIMPLE_TREE_REP_H
# define IG_TREE_MODEL_IG_SIMPLE_TREE_REP_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgTreeModel/interface/IgTreeRep.h"
# include <vector>
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgTreeNodeIterator;
class IgTreeModel;
class IgTreeNode;
class IgTreePath;
class IgDatum;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_TREE_MODEL_API IgSimpleTreeRep : public IgTreeRep
{
public:
    IgSimpleTreeRep (IgTreeModel *model, IgTreeNode *parent,
		     IgDatum *payload = 0, bool allow_children = true);
    ~IgSimpleTreeRep (void);
    // default copy constructor
    // default assignment operator

    // FIXME: deep copy?

    // rep interface
    virtual IgRepContext *	context (void) const;
    virtual IgModel *		model (void) const;

    // node payload data
    virtual IgDatum *		payload (void) const;
    virtual void		payload (IgDatum *object);

    // traversal interface
    virtual IgTreeNode *	parent (void) const;
    virtual unsigned		children (void) const;
    virtual unsigned		child_index_of (const IgTreeNode *child) const;
    virtual IgTreeNode *	child (unsigned index) const;

    virtual bool		is_leaf (void) const;
    virtual bool		allow_children (void) const;

    // extended traversal
    virtual IgTreeNode *	previous (void) const; // in pre-order traversal order
    virtual IgTreeNode *	next (void) const;     // in pre-order traversal order

    virtual IgTreeNodeIterator	sibling_iterator (void) const;
    virtual IgTreeNodeIterator	siblings_begin (void) const;
    virtual IgTreeNodeIterator	siblings_end (void) const;
    virtual IgTreeNode *	sibling_next (void) const;
    virtual IgTreeNode *	sibling_previous (void) const;

    virtual IgTreeNodeIterator	children_begin (void) const;
    virtual IgTreeNodeIterator	children_end (void) const;

    virtual IgTreeNode *	child_after (const IgTreeNode *child) const;
    virtual IgTreeNode *	child_before (const IgTreeNode *child) const;
    virtual IgTreeNode *	child_first (void) const;
    virtual IgTreeNode *        child_last (void) const;

    // virtual IgTreeLeafIterator leaves_begin (void) const;
    // virtual IgTreeLeafIterator leaves_end (void) const;
    virtual IgTreeNode *	leaf_first (void) const;
    virtual IgTreeNode *	leaf_last (void) const;
    virtual IgTreeNode *	leaf_next (const IgTreeNode *leaf) const;
    virtual IgTreeNode *	leaf_previous (const IgTreeNode *leaf) const;

    // useful queries
    virtual unsigned		depth (void) const;
    virtual unsigned		level (void) const;
    virtual unsigned		leaves (void) const;
    virtual unsigned		siblings (void) const;
    
    virtual IgTreePath *	path (void) const;
    virtual IgTreeNode *	root (void) const;
    virtual IgTreeNode *	common_ancestor (const IgTreeNode *node) const;

    virtual bool		is_child_of (const IgTreeNode *other) const;
    virtual bool		is_ancestor_of (const IgTreeNode *other) const;
    virtual bool		is_descendant_of (const IgTreeNode *other) const;
    virtual bool		is_related (const IgTreeNode *other) const;
    virtual bool		is_sibling_of (const IgTreeNode *other) const;
    virtual bool		is_root (void) const;

    // node management interface
    virtual void		add (IgTreeNode *child);
    virtual void		insert (IgTreeNode *child, unsigned index);
    virtual void		remove (unsigned index);
    virtual void		remove (IgTreeNode *child);
    virtual void		clear (void);
    virtual void		detach (void);
    virtual void		reparent (IgTreeNode *to);

    virtual void		allow_children (bool allow = true);

    // FIXME: extended traversals: breadth-first, depth-first, pre-order, post-order, path-to-root?
    //        ==> more IgTreeNodeIterators?  more flexible IgTreeNodeIterator?

protected:
    virtual void		context (IgRepContext *context);

    virtual IgTreePath *	path_to_root (IgTreeNode *node, unsigned depth);

private:
    static unsigned		depth (const IgTreeNode *node);
    static unsigned		leaves (const IgTreeNode *node);

    IgTreeModel			*m_model;
    IgRepContext		*m_context;
    IgTreeNode			*m_parent;
    IgDatum			*m_payload;
    std::vector<IgTreeNode *>	m_children;
    bool			m_allow_children;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_TREE_MODEL_IG_SIMPLE_TREE_REP_H
