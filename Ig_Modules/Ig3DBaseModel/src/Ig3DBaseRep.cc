//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DBaseModel/interface/Ig3DBaseRep.h"
#include "Ig_Modules/Ig3DBaseModel/interface/Ig3DBaseModel.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepContext.h"
#include <Inventor/nodes/SoSeparator.h>
#include <classlib/utils/DebugAids.h>
#include <algorithm>
#include <cstdlib>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Checked conversion of the node to a rep; use this whenever you
    wanted to do a @c dynamic_cast of a node to a rep.  Required for
    TGS brain-deadness, they refuse to build Inventor libraries for
    MSVC++ with RTTI, so we need another mechanism.  This one avoids
    having to declare Ig3DBaseRep as a node type, but has the drawback in
    that it has no way of verifying whether the separator really is a
    rep or not -- we just hope it is.  */
Ig3DBaseRep *
Ig3DBaseRep::asRep (SoNode *node)
{
    return (node
	    && node->isOfType (SoSeparator::getClassTypeId ())
	    && node->getName () == "IGUANA_3D_REP"
	    ? static_cast<Ig3DBaseRep *> (node) : 0);
}

// Hidden layout magic:
//  - 1st child: `magic' prefix group (not a separator, allows magic
//    children like clip planes, cameras and lights to be inserted)
//  - 2nd child: `node' group (separator or switch holding the nodes
//    of this rep)
//  - 3rd.. Nth: child reps.
//
// The rep itself is hidden from the client (it is a separator node,
// but clients must never access it as such).  We give it a magic name
// recognisable for reading in .iv files.  The parent and children are
// always 3D reps, even though we add them as plain Inventor nodes.
// Again, that is not known to the clients.
//
// Note that we do not remember our parent, but we do remember the
// model.  Worst come to worst, the model can always be searched for
// this one, as this must never appear more than once in the scene
// graph.  Moreover, since all my parents must be 3D reps, they by
// definition are separators, so I can't be hiding inside mysterious
// switch structures or such.

Ig3DBaseRep::Ig3DBaseRep (Ig3DBaseModel *model,
		  Ig3DBaseRep *parent,
		  Ig3DBaseRep *prev,
		  SoGroup *node)
    : m_context (0),
      m_model (model)
{
    ASSERT (m_model);
    ASSERT (node);

    // Ref myself -- I'll be pointed to from outside!
    ref ();

    // Give myself a magic recognisable name.
    setName ("IGUANA_3D_REP");

    // Turn off render caching in intermediate nodes
    renderCaching = OFF;

    // Add the magic group.
    ASSERT (getNumChildren () == 0);
    addChild (new SoGroup);

    // Add the node.
    // ASSERT (node is a separator or switch, not a group!)
    addChild (node);

    // Tell parent/model; this will broadcast a model change.
    ASSERT (! prev || parent);
    if (parent)
    {
	// If `prev' was given, insert right after it; otherwise make
	// this the first child (but skip the two non-rep children!).
	ASSERT (parent->findChild (this) == -1);
	ASSERT (parent->getNumChildren () >= 2);
	ASSERT (! prev || parent->findChild (prev) != -1);
	int index = prev ? parent->findChild (prev)+1 : 2;
	if (index < parent->getNumChildren ())
	    parent->insertChild (this, index);
	else
	    parent->addChild (this);

	m_model->change (parent);
    }
    else
	m_model->add (this);
}

Ig3DBaseRep::~Ig3DBaseRep (void)
{
    // The destruction may come upon me in two ways: the object is
    // going away and the context is deleting me, or `zap' unref'd
    // myself (called from the parent).  In the former case Inventor
    // reference count may not yet be zero and I have to remove myself
    // from the scene graph.  Note however that I can never be deleted
    // by Inventor -- I ref'ed myself in the constructor.  The only I
    // can die is via `zap', which is only called here, meaning that
    // either myself or one of my ancestors had its context deleted.
    // However if my context *was* deleted, I must not delete it
    // again; if so, it has already set its rep to null, which we use
    // as a clue.

    // FIXME: `destroy()' does some useful work in Inventor, and it
    // isn't getting called for reps getting deleted via context
    // deletion.  Is that a problem?

    if (getRefCount () > 0)
	// NB: This cannot kill us again since the constructor ref'ed
	// myself, even though all Inventor references will go away.
	zap (true, false);

    // Make sure `zap' has ran and cleaned up everything.
    ASSERT (getRefCount () == 0);
    ASSERT (! m_model);

    // We should have no rep children now, `zap' removed them.  My
    // parent class takes care of deleting the remaining children.
    ASSERT (getNumChildren () == 2);

    // Make the context go away if we are not being deleted by it.
    ASSERT (m_context);
    if (m_context->rep ())
    {
	ASSERT (m_context->rep () == this);
	m_context->erase (false);
	delete m_context;
    }
}

void
Ig3DBaseRep::zap (bool search /* = false */, bool kill /* = true */)
{
    // Kill rep children.
    ASSERT (getNumChildren () >= 2);
    for (int i = getNumChildren () - 1; i >= 2; --i)
    {
	SoNode *node = getChild (i);
	ASSERT (node && node->isOfType (Ig3DBaseRep::getClassTypeId ()));

	// Get it off the list; since we ref reps in the constructor,
	// this isn't fatal...
	removeChild (i);

	// ... but this is -- blast it away.
	static_cast<Ig3DBaseRep *> (node)->zap ();
    }

    // Make sure model knows I am gone.  If I am not being zapped by
    // my parent, `search' will be true and the model will make sure
    // no references remain to me.
    m_model->remove (this, search);
    m_model = 0;

    // ... and make the fatal unref (unless we are being called from
    // the destructor, in which case just let it finish its job).
    ASSERT (getRefCount () == 1);
    if (kill)
	unref ();
    else
	unrefNoDelete ();
}

void
Ig3DBaseRep::context (IgRepContext *context)
{
    ASSERT (! m_context);
    ASSERT (context);
    m_context = context;
}

//////////////////////////////////////////////////////////////////////
IgRepContext *
Ig3DBaseRep::context (void) const
{ return m_context; }

IgModel *
Ig3DBaseRep::model (void) const
{ return m_model; }

//////////////////////////////////////////////////////////////////////
int
Ig3DBaseRep::children (void) const
{
    ASSERT (getNumChildren () >= 2);
    return getNumChildren () - 2;
}

Ig3DBaseRep *
Ig3DBaseRep::child (int i) const
{
    ASSERT (i >= 0);
    ASSERT (getNumChildren () >= 2);
    ASSERT (i + 2 < getNumChildren ());
    ASSERT (getChild (i+2)
	    && getChild (i+2)->isOfType (Ig3DBaseRep::getClassTypeId ()));

    return static_cast<Ig3DBaseRep *> (getChild (i+2));
}

Ig3DBaseRep *
Ig3DBaseRep::child (SbString name) const
{
    for (int i = 0; i < children (); ++i)
    {
	Ig3DBaseRep *c = child (i);
	if (c->node ()->getName () == name.getString())
	    return c;
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////
SoGroup *
Ig3DBaseRep::magic (void) const
{
    ASSERT (getNumChildren () >= 2);
    ASSERT (getChild (0) && getChild (0)->isOfType(SoGroup::getClassTypeId()));
    return static_cast<SoGroup *> (getChild (0));
}

SoGroup *
Ig3DBaseRep::magic (SoGroup *node)
{
    ASSERT (getNumChildren () >= 2);
    ASSERT (getChild (0) && getChild (0)->isOfType(SoGroup::getClassTypeId()));
    ASSERT (node);
    replaceChild (0, node);
    m_model->change (this);
    return node;
}

SoNode *
Ig3DBaseRep::findMagic (SbName name) const
{
    for (int j = 0; j < magic ()->getNumChildren (); j++)
    {
	SoNode *node = magic ()->getChild (j);
	if (node->getName () == name)
	    return node;	
    }	
    return 0;
}

SoGroup *
Ig3DBaseRep::node (void) const
{
    ASSERT (getNumChildren () >= 2);
    ASSERT (getChild (1) && getChild (1)->isOfType(SoGroup::getClassTypeId()));
    return static_cast<SoGroup *> (getChild (1));
}

SoGroup *
Ig3DBaseRep::node (SoGroup *node)
{
    ASSERT (getNumChildren () >= 2);
    ASSERT (getChild (1) && getChild (1)->isOfType(SoGroup::getClassTypeId()));
    ASSERT (node);
    replaceChild (1, node);
    m_model->change (this);
    return node;
}

void
Ig3DBaseRep::clear (void)
{
    ASSERT (node ());
    node ()->removeAllChildren ();
}
