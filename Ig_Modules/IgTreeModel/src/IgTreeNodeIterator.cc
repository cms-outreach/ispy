//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgTreeModel/interface/IgTreeNodeIterator.h"
#include "Ig_Modules/IgTreeModel/interface/IgTreeNode.h"
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

IgTreeNodeIterator::IgTreeNodeIterator (void)
    : m_parent (0), m_child_index (0)
{}

IgTreeNodeIterator::IgTreeNodeIterator (const IgTreeNode *node, unsigned index)
    : m_parent (node), m_child_index (index)
{}

bool
IgTreeNodeIterator::operator== (const IgTreeNodeIterator &x) const
{
    return m_parent == x.m_parent && m_child_index == x.m_child_index;
}

IgTreeNode *
IgTreeNodeIterator::operator* (void) const
{
    ASSERT (m_parent);
    return m_parent->child (m_child_index);
}

IgTreeNodeIterator &
IgTreeNodeIterator::operator++ (void)
{
    ASSERT (m_parent);
    ASSERT (m_child_index <= m_parent->children ());
    ++m_child_index;
    return *this;
}

IgTreeNodeIterator
IgTreeNodeIterator::operator++ (int)
{
    IgTreeNodeIterator tmp (*this);
    ++*this;
    return tmp;
}

IgTreeNodeIterator &
IgTreeNodeIterator::operator-- (void)
{
    ASSERT (m_parent);
    ASSERT (m_child_index <= m_parent->children ());
    ASSERT (m_child_index > 0);
    --m_child_index;
    return *this;
}

IgTreeNodeIterator
IgTreeNodeIterator::operator-- (int)
{
    IgTreeNodeIterator tmp (*this);
    --*this;
    return tmp;
}
