//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgTreeModel/interface/IgSimpleTreeModel.h"
#include "Ig_Modules/IgTreeModel/interface/IgSimpleTreeRep.h"
#include <classlib/utils/DebugAids.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgSimpleTreeModel::IgSimpleTreeModel (IgTreeNode *root /* = 0 */)
    : m_root (root ? root : new IgSimpleTreeRep (this, 0))
{}

IgTreeNode *
IgSimpleTreeModel::root (void) const
{
    ASSERT (m_root);
    return m_root;
}

void
IgSimpleTreeModel::listen (EventType /* event*/, const Listener & /*listener*/)
{ ASSERT (false); }

void
IgSimpleTreeModel::unlisten (EventType /*event*/, const Listener & /*listener*/)
{ ASSERT (false); }

void
IgSimpleTreeModel::changed (const IgTreePath * /*path*/, IgTreeNode * /*value*/)
{ ASSERT (false); }

const IgSimpleTreeModel::Listeners &
IgSimpleTreeModel::listeners (EventType type) const
{
    switch (type) 
    {
    case NodesChanged:		return m_nodes_changed_listeners;
    case NodesInserted:		return m_nodes_inserted_listeners;
    case NodesRemoved:		return m_nodes_removed_listeners;
    case StructureChanged:	return m_structure_changed_listeners;
    default:			ASSERT (false); return listeners (NodesChanged);
    }
}

void
IgSimpleTreeModel::node_changed (IgTreeNode * /*node*/)
{
    ASSERT (false);
}

void
IgSimpleTreeModel::nodes_changed (IgTreeNode * /*node*/,
				  const std::vector<unsigned> & /*indices*/)
{
    ASSERT (false);
}

void
IgSimpleTreeModel::nodes_inserted (IgTreeNode * /*node*/,
				   const std::vector<unsigned> & /*indices*/)
{
    ASSERT (false);
}

void
IgSimpleTreeModel::nodes_removed (IgTreeNode * /*node*/,
				  const std::vector<unsigned> & /*indices*/,
				  const std::vector<IgTreeNode *> & /*removed_children*/)
{
    ASSERT (false);
}

void
IgSimpleTreeModel::structure_changed (IgTreeNode * /*node*/)
{
    ASSERT (false);
}

void
IgSimpleTreeModel::root (IgTreeNode * /*new_root*/)
{
    ASSERT (false);
}

IgTreePath *
IgSimpleTreeModel::path_to_root (IgTreeNode * /*node*/)
{
    ASSERT (false);
    return 0;
}

void
IgSimpleTreeModel::trigger_nodes_changed (/* FIXME */)
{
    ASSERT (false);
}

void
IgSimpleTreeModel::trigger_nodes_inserted (/* FIXME */)
{
    ASSERT (false);
}

void
IgSimpleTreeModel::trigger_nodes_removed (/* FIXME */)
{
    ASSERT (false);
}

void
IgSimpleTreeModel::trigger_structure_changed (/* FIXME */)
{
    ASSERT (false);
}

IgTreePath *
IgSimpleTreeModel::path_to_root (IgTreeNode * /*node*/, unsigned /*depth*/)
{
    ASSERT (false);
    return 0;
}



