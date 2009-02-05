//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgTreeModel/interface/IgSimpleTreeRep.h"
#include "Ig_Modules/IgTreeModel/interface/IgTreeModel.h"
#include "Ig_Modules/IgTreeModel/interface/IgTreeNodeIterator.h"
#include <classlib/utils/DebugAids.h>
#include <algorithm>
#include <utility>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgSimpleTreeRep::IgSimpleTreeRep (IgTreeModel *model,
				  IgTreeNode *parent,
				  IgDatum *payload /*=0*/,
				  bool allow_children /*=true*/)
    : m_model (model),
      m_context (0),
      m_parent (parent),
      m_payload (payload),
      m_allow_children (allow_children)
{}

IgSimpleTreeRep::~IgSimpleTreeRep (void)
{
    // FIXME: delete children?
    // FIXME: delete m_payload;
}

//////////////////////////////////////////////////////////////////////
void
IgSimpleTreeRep::context (IgRepContext *context)
{
    m_context = context;
}

IgRepContext *
IgSimpleTreeRep::context (void) const
{
    return m_context;
}

IgModel *
IgSimpleTreeRep::model (void) const
{
    return m_model;
}

//////////////////////////////////////////////////////////////////////
IgDatum *
IgSimpleTreeRep::payload (void) const
{
    return m_payload;
}

void
IgSimpleTreeRep::payload (IgDatum *payload)
{
    // FIXME: delete m_payload;
    m_payload = payload;
}

//////////////////////////////////////////////////////////////////////
IgTreeNode *
IgSimpleTreeRep::parent (void) const
{
    return m_parent;
}

unsigned
IgSimpleTreeRep::children (void) const
{
    return m_children.size ();
}

// FIXME: This needs to be outside functions due to mutually incompatible
// bugs in GCC 3.1 and Solaris CC 5.3.  GCC can't grok the first form in
// a function-local scope but does accept a "using namespace std::rel_ops;".
// However, Solaris CC doesn't find the operator with the latter.
#if HAVE_NAMESPACE_STD_REL_OPS
    using std::rel_ops::operator!=;
#elif HAVE_REL_OPS_IN_STD
    using std::operator!=;
#endif

unsigned
IgSimpleTreeRep::child_index_of (const IgTreeNode *node) const
{
#if HAVE_NAMESPACE_STD_REL_OPS
    // FIXME (see above): using std::rel_ops::operator!=;
#elif HAVE_REL_OPS_IN_STD
    // FIXME (see above): using std::operator!=;
#endif

    ASSERT (! m_children.empty ());

    std::vector<IgTreeNode *>::const_iterator pos
	= std::find (m_children.begin (), m_children.end (), node);

    ASSERT (pos != m_children.end ());
    return m_children.end () - pos;
}

IgTreeNode *
IgSimpleTreeRep::child (unsigned index) const
{
    ASSERT (index < m_children.size ());
    return m_children [index];
}

bool
IgSimpleTreeRep::is_leaf (void) const
{
    return m_children.empty ();
}

bool
IgSimpleTreeRep::allow_children (void) const
{
    return m_allow_children;
}

void
IgSimpleTreeRep::allow_children (bool allow /* = true */)
{
    m_allow_children = allow;
}

//////////////////////////////////////////////////////////////////////
IgTreeNode *
IgSimpleTreeRep::previous (void) const
{
    // FIXME
    ASSERT (false);
    return 0;
}

IgTreeNode *
IgSimpleTreeRep::next (void) const
{
    // FIXME
    ASSERT (false);
    return 0;
}

IgTreeNodeIterator
IgSimpleTreeRep::sibling_iterator (void) const
{
    ASSERT (m_parent);
    return IgTreeNodeIterator (m_parent, m_parent->child_index_of (this));
}

IgTreeNodeIterator
IgSimpleTreeRep::siblings_begin (void) const
{
    ASSERT (m_parent);
    return IgTreeNodeIterator (m_parent, 0);
}

IgTreeNodeIterator
IgSimpleTreeRep::siblings_end (void) const
{
    ASSERT (m_parent);
    return IgTreeNodeIterator (m_parent, m_parent->children());
}

IgTreeNode *
IgSimpleTreeRep::sibling_next (void) const
{
    IgTreeNodeIterator self = sibling_iterator ();
    return *++self;
}

IgTreeNode *
IgSimpleTreeRep::sibling_previous (void) const
{
    IgTreeNodeIterator self = sibling_iterator ();
    return *--self;
}

IgTreeNodeIterator
IgSimpleTreeRep::children_begin (void) const
{
    return IgTreeNodeIterator (this, 0);
}

IgTreeNodeIterator
IgSimpleTreeRep::children_end (void) const
{
    return IgTreeNodeIterator (this, m_children.size ());
}

IgTreeNode *
IgSimpleTreeRep::child_after (const IgTreeNode *child) const
{
    unsigned index = child_index_of (child);
    ASSERT (index + 1 < m_children.size ());
    return m_children [index + 1];
}

IgTreeNode *
IgSimpleTreeRep::child_before (const IgTreeNode *child) const
{
    unsigned index = child_index_of (child);
    ASSERT (index > 0);
    return m_children [index - 1];
}

IgTreeNode *
IgSimpleTreeRep::child_first (void) const
{
    ASSERT (! m_children.empty ());
    return m_children.front ();
}

IgTreeNode *
IgSimpleTreeRep::child_last (void) const
{
    ASSERT (! m_children.empty ());
    return m_children.back ();
}

// IgTreeLeafIterator
// IgSimpleTreeRep::leaves_begin (void) const
// {}

// IgTreeLeafIterator
// IgSimpleTreeRep::leaves_end (void) const
// {}

IgTreeNode *
IgSimpleTreeRep::leaf_first (void) const
{
    // FIXME
    ASSERT (false);
    return 0;
}

IgTreeNode *
IgSimpleTreeRep::leaf_last (void) const
{
    // FIXME
    ASSERT (false);
    return 0;
}

IgTreeNode *
IgSimpleTreeRep::leaf_next (const IgTreeNode * /*leaf*/) const
{
    // FIXME
    ASSERT (false);
    return 0;
}

IgTreeNode *
IgSimpleTreeRep::leaf_previous (const IgTreeNode * /*leaf*/) const
{
    // FIXME
    ASSERT (false);
    return 0;
}

//////////////////////////////////////////////////////////////////////
unsigned
IgSimpleTreeRep::depth (const IgTreeNode *node)
{
#if HAVE_NAMESPACE_STD_REL_OPS
    // FIXME (see above): using std::rel_ops::operator!=;
#elif HAVE_REL_OPS_IN_STD
    // FIXME (see above): using std::operator!=;
#endif

    unsigned d = 0;
    IgTreeNodeIterator child (node, 0);
    IgTreeNodeIterator end (node, node->children());
    for ( ; child != end; ++child) {
	unsigned branch_depth = depth (*child) + 1;
        d = (branch_depth > d ? branch_depth : d);
    }
    return d;
}

unsigned
IgSimpleTreeRep::depth (void) const
{
    return depth (this);
}

unsigned
IgSimpleTreeRep::level (void) const
{
    unsigned level = 0;
    for (IgTreeNode *node = m_parent; node; ++level, node = node->parent ())
	;
    return level;
}

unsigned
IgSimpleTreeRep::leaves (const IgTreeNode *node)
{
#if HAVE_NAMESPACE_STD_REL_OPS
    // FIXME (see above): using std::rel_ops::operator!=;
#elif HAVE_REL_OPS_IN_STD
    // FIXME (see above): using std::operator!=;
#endif

    if (node->is_leaf ())
	return 1;

    unsigned n = 0;
    IgTreeNodeIterator child (node, 0);
    IgTreeNodeIterator end (node, node->children());
    for ( ; child != end; ++child)
	n += leaves (*child);

    return n;
}

unsigned
IgSimpleTreeRep::leaves (void) const
{
    return leaves (this);
}

unsigned
IgSimpleTreeRep::siblings (void) const
{
    ASSERT (m_parent);
    return m_parent->children ();
}

IgTreePath *
IgSimpleTreeRep::path (void) const
{
    // FIXME
    ASSERT (false);
    return 0;
}

IgTreeNode *
IgSimpleTreeRep::root (void) const
{
    IgTreeNode *node = const_cast<IgSimpleTreeRep *> (this);
    for ( ; node->parent (); node = node->parent ())
	;
    return node;
}

IgTreeNode *
IgSimpleTreeRep::common_ancestor (const IgTreeNode * /*node*/) const
{
    // FIXME
    ASSERT (false);
    return 0;
}

bool
IgSimpleTreeRep::is_child_of (const IgTreeNode *other) const
{
    ASSERT (other);
    IgTreeNode *node;
    for (node = m_parent; node && node != other; node = node->parent ())
	;

    return node == other;
}

bool
IgSimpleTreeRep::is_ancestor_of (const IgTreeNode *other) const
{
    ASSERT (other);
    const IgTreeNode *node;
    for (node = other; node && node != this; node = node->parent ())
	;

    return node == this;
}

bool
IgSimpleTreeRep::is_descendant_of (const IgTreeNode *other) const
{
    ASSERT (other);
    const IgTreeNode *node;
    for (node = m_parent; node && node != other; node = node->parent ())
	;
    return node == other;
}

bool
IgSimpleTreeRep::is_related (const IgTreeNode *other) const
{
    ASSERT (other);
    const IgTreeNode *node;
    for (node = other; node->parent (); node = node->parent ())
	;
    return root () == node;
}

bool
IgSimpleTreeRep::is_sibling_of (const IgTreeNode *other) const
{
    ASSERT (other);
    return parent () == other->parent ();
}

bool
IgSimpleTreeRep::is_root (void) const
{
    return root () != 0;
}

//////////////////////////////////////////////////////////////////////
void
IgSimpleTreeRep::add (IgTreeNode *child)
{
    ASSERT (m_allow_children);
    ASSERT (child);

    m_children.push_back (child);
}

void
IgSimpleTreeRep::insert (IgTreeNode *child, unsigned index)
{
    ASSERT (m_allow_children);
    ASSERT (child);
    ASSERT (index <= m_children.size());

    m_children.insert (m_children.begin () + index, child);
}

void
IgSimpleTreeRep::remove (unsigned index)
{
    ASSERT (m_allow_children);
    ASSERT (index < m_children.size ());

    m_children.erase (m_children.begin () + index);
}

void
IgSimpleTreeRep::remove (IgTreeNode *child)
{
    ASSERT (m_allow_children);
    ASSERT (child);
    remove (child_index_of (child));
}

void
IgSimpleTreeRep::clear (void)
{
    ASSERT (m_allow_children);
    m_children.clear ();
}

void
IgSimpleTreeRep::detach (void)
{
    // FIXME: how can we do this?  the parent may not be a simple tree rep!
    // maybe add another IgChangeableTreeNode?
    if (m_parent) {
	// m_parent->remove (this);
	ASSERT (false);
	m_parent = 0;
    }
}

void
IgSimpleTreeRep::reparent (IgTreeNode *to)
{
    // FIXME: how can we do this?  the parent may not be a simple tree rep!
    // maybe add another IgChangeableTreeNode?
    ASSERT (to);
    detach ();
    ASSERT (false);
    // to->add (this);
}

IgTreePath *
IgSimpleTreeRep::path_to_root (IgTreeNode * /*node*/, unsigned /*depth*/)
{
    // FIXME
    ASSERT (false);
    return 0;
}
