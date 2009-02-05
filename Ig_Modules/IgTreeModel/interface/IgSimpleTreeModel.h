#ifndef IG_TREE_MODEL_IG_SIMPLE_TREE_MODEL_H
# define IG_TREE_MODEL_IG_SIMPLE_TREE_MODEL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgTreeModel/interface/IgTreeModel.h"
# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgTreeNode;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_TREE_MODEL_API IgSimpleTreeModel : public IgTreeModel
{
public:
    typedef std::vector<Listener> Listeners;

    IgSimpleTreeModel (IgTreeNode *root = 0);
    // default copy constructor
    // default destructor
    // default assignment operator

    virtual void		listen (EventType event, const Listener &listener);
    virtual void		unlisten (EventType event, const Listener &listener);
    virtual void		changed (const IgTreePath *path, IgTreeNode *value);

    virtual const Listeners &	listeners (EventType type) const;
    virtual void		node_changed (IgTreeNode *node);
    virtual void		nodes_changed (IgTreeNode *node, const std::vector<unsigned> &indices);
    virtual void		nodes_inserted (IgTreeNode *node, const std::vector<unsigned> &indices);
    virtual void		nodes_removed (IgTreeNode *node, const std::vector<unsigned> &indices,
					       const std::vector<IgTreeNode *> &removed_children);
    virtual void		structure_changed (IgTreeNode *node);

    virtual IgTreeNode *	root (void) const;
    virtual void		root (IgTreeNode *new_root);
    virtual IgTreePath *	path_to_root (IgTreeNode *node);

    // ask_allow_children?

protected:
    virtual void		trigger_nodes_changed (/* FIXME */);
    virtual void		trigger_nodes_inserted (/* FIXME */);
    virtual void		trigger_nodes_removed (/* FIXME */);
    virtual void		trigger_structure_changed (/* FIXME */);

    virtual IgTreePath *	path_to_root (IgTreeNode *node, unsigned depth);

private:
    IgTreeNode *		m_root;

    Listeners			m_nodes_changed_listeners;
    Listeners			m_nodes_inserted_listeners;
    Listeners			m_nodes_removed_listeners;
    Listeners			m_structure_changed_listeners;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_TREE_MODEL_IG_SIMPLE_TREE_MODEL_H
