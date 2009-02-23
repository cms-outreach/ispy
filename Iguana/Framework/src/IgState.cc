//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Framework/interface/IgState.h"
#include "Iguana/Framework/interface/IgStateKey.h"
#include "Iguana/Framework/interface/IgStateElement.h"
#include "classlib/utils/DebugAids.h"
#include <algorithm>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Add a child to this state.  Invoked by the child constructor.  */
void
IgState::addChild (IgState *child)
{
    ASSERT (child);
    ASSERT (std::find (m_children.begin (), m_children.end (), child)
	    == m_children.end ());

    m_children.push_back (child);
}

/** Remove a child from this state.  Invoked by the child destructor.  */
void
IgState::detachChild (IgState *child)
{
    ASSERT (child);
    Children::iterator pos
	= std::find (m_children.begin (), m_children.end (), child);

    ASSERT (pos != m_children.end ());

    m_children.erase (pos);
}

//////////////////////////////////////////////////////////////////////
/** Construct a new state without any elements and attach it to @a parent. */
IgState::IgState (IgState *parent /* = 0 */)
    : m_parent (parent)
{ if (m_parent) m_parent->addChild (this); }

/** Destroy the state, all its sub-states and all elements recorded in it.  */
IgState::~IgState (void)
{
    if (m_parent)
	m_parent->detachChild (this);

    while (! m_children.empty ())
	delete m_children.front ();

    for (Elements::size_type i = 0; i < m_elements.size (); ++i)
	delete m_elements [i];
}

//////////////////////////////////////////////////////////////////////
/** Get the root of this state tree.  If this state has no #parent(),
    returns pointer to @c this.  */
IgState *
IgState::root (void) const
{
    IgState *s = const_cast<IgState *> (this);
    while (s->m_parent)
	s = s->m_parent;

    return s;
}

/** Get the parent of this state, or null if there is none.  */
IgState *
IgState::parent (void) const
{ return m_parent; }

/** Return the number of child states.  */
unsigned
IgState::children (void) const
{ return m_children.size (); }

/** Return the child state number @a index.  @a index must be less
    than #children().  Note that no meaning should be attached to the
    order of child states, they just happen to be indexed from zero to
    #children()-1.  */
IgState *
IgState::child (unsigned index) const
{ ASSERT (index < m_children.size ()); return m_children [index]; }

//////////////////////////////////////////////////////////////////////
/** Get an element at key @a index in this state.  Returns a pointer
    to the element in this or any of the parent states.  If no such
    element has been recorded, returns null.  If @a context is
    non-null, it will be set to point to the state in which the
    element was found (or null if no element is found).  */
IgStateElement *
IgState::get (IgStateKey index, IgState **context /* = 0 */) const
{
    IgState		*in = const_cast<IgState *> (this);
    IgStateElement	*element = 0;

    for ( ; in && ! element; in = in->parent ())
	if (index < in->m_elements.size ())
	    element = in->m_elements [index];

    if (context)
	*context = in;

    return element;
}

/** Add the element @a element to this state at key @a index.  The
    value will be set in this state.  If there is a different element
    at the same @a index, it will be destroyed.  @a element may be
    null, in which case the element is effectively removed from this
    state; the next lookup will then search for the element in the
    parent (i.e. it will not be remembered to be null in this
    state).  */
void
IgState::put (IgStateKey index, IgStateElement *element)
{
    if (index >= m_elements.size ())
    {
	if (! element)
	    return;
	else
	    m_elements.resize (index+1, 0);
    }

    if (m_elements [index] != element)
    {
	// Destructor must call detach()
	delete m_elements [index];
	ASSERT (m_elements [index] == 0);
    }

    m_elements [index] = element;
}

/** Erase any element at key @a index.  This is equivalent to
    #put(index,0). */
void
IgState::erase (IgStateKey index)
{ put (index, 0); }

/** Detach an element at key @a index.  This is equivalent to
    #put(index,0) except that it does not destroy the element, just
    erases any memory of it in the state.  This method should be
    called by element destructors.  */
void
IgState::detach (IgStateKey index)
{
    ASSERT (index <= m_elements.size ());
    ASSERT (m_elements [index]);
    m_elements [index] = 0;
}

/** Return the number of elements in this state.  This is not the
    actual elements in the vector, some of them might be 0 because
    of loading and un-loading of libraries. */
unsigned
IgState::elements (void) const
{ return m_elements.size (); }

/** Return the element state number @a index.  @a index must be less
    than #elements(). The element might be 0 as not all elements
    in this vector are initialized because of loading and un-loading
    of libraiies. */

IgStateElement *
IgState::element (unsigned index) const
{ ASSERT (index < m_elements.size ()); return m_elements [index]; }
