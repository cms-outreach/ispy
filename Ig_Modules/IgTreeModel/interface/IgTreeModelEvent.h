#ifndef IG_TREE_MODEL_IG_TREE_MODEL_EVENT_H
# define IG_TREE_MODEL_IG_TREE_MODEL_EVENT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgTreeModel/interface/config.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgTreeModel;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_TREE_MODEL_API IgTreeModelEvent
{
public:
    IgTreeModelEvent (IgTreeModel *source, IgTreePath *path);
    IgTreeModelEvent (IgTreeModel *source, IgTreePath *path,
		      const std::vector<unsigned> &indices,
		      const std::vector<IgTreeNode *> &children);
    // default copy constructor
    // default assignment operator
    // default destructor

    IgTreeModel *			source (void) const;
    const IgTreePath *			path (void) const;
    const std::vector<unsigned> &	indices (void) const;
    const std::vector<IgTreeNode *> &	children (void) const;

private:
    IgTreeModel *		m_source;
    IgTreePath *		m_path;
    std::vector<unsigned>	m_indices;
    std::vector<IgTreeNode *>	m_children;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

inline
IgTreeModelEvent::IgTreeModelEvent (IgTreeModel *source, IgTreePath *path)
    : m_source (source), m_path (path)
{}

inline
IgTreeModelEvent::IgTreeModelEvent (IgTreeModel *source, IgTreePath *path,
				    const std::vector<unsigned> &indices,
				    const std::vector<IgTreeNode *> &children)
    : m_source (source), m_path (path), m_indices (indices), m_children (children)
{}

inline IgTreeModel *
IgTreeModelEvent::source (void) const
{
    return m_source;
}

inline const IgTreePath *
IgTreeModelEvent::path (void) const
{
    return m_path;
}

inline const std::vector<unsigned> &
IgTreeModelEvent::indices (void) const
{
    return m_indices;
}

inline const std::vector<IgTreeNode *> &
IgTreeModelEvent::children (void) const
{
    return m_children;
}

#endif // IG_TREE_MODEL_IG_TREE_MODEL_EVENT_H
