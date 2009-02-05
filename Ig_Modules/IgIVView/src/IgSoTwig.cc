//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgIVView/interface/IgSoTwig.h"
#include "Ig_Modules/Ig3DModel/interface/Ig3DModel.h"
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoClipPlane.h>
#include <classlib/utils/DebugAids.h>
#include <string>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgSoTwig::IgSoTwig (IgTwig *parent, const std::string &name,
		    bool selectable /* = false */,
		    bool traverse /* = true */,
		    bool visibility /* = true */,
		    bool cascade /* = true */)
    : IgSimpleTwig (parent, name, traverse, visibility, cascade),
      m_magic (0),
      m_node (0),
      m_selectable (selectable)
{
    m_magic = new SoGroup;
    m_magic->ref ();
    m_node = m_selectable ? new SoSelection : new SoSeparator;
    m_node->setName (Ig3DModel::encode (name));
    m_node->ref ();
}

IgSoTwig::IgSoTwig (const std::string &name,
		    bool selectable /* = false */,
		    bool traverse /* = true */,
		    bool visibility /* = true */,
		    bool cascade /* = true */)
    : IgSimpleTwig (0, name, traverse, visibility, cascade),
      m_magic (0),
      m_node (0),
      m_selectable (selectable)
{
    m_magic = new SoGroup;
    m_magic->ref ();
    m_node = m_selectable ? new SoSelection : new SoSeparator;
    m_node->setName (Ig3DModel::encode (name));
    m_node->ref ();
}

IgSoTwig::~IgSoTwig (void)
{
    ASSERT (m_magic);
    ASSERT (m_node);
    m_magic->unref ();
    m_node->unref ();
}

void
IgSoTwig::cleared (IgTwig *parent)
{
    // This works its way bottom up; our children have already been cleared.
    // So just blow away everything and recreate the tree.
    IgTwig::cleared (parent);

    node (m_selectable ? new SoSelection : new SoSeparator)
	->setName (Ig3DModel::encode (name ()));
    magic (new SoGroup);
}

void
IgSoTwig::destroy (void)
{
    magic ()->removeAllChildren ();
    node ()->removeAllChildren ();
    IgTwig::destroy ();
}

SoGroup *
IgSoTwig::node (void) const
{
    ASSERT (m_node);
    return m_node;
}

SoGroup *
IgSoTwig::node (SoGroup *node)
{
    ASSERT (m_node);
    ASSERT (node);

    if (node->getName () == "")
	node->setName (m_node->getName ());

    node->ref ();
    m_node->unref ();
    return m_node = node;
}

SoGroup *
IgSoTwig::magic (void) const
{
    ASSERT (m_magic);
    return m_magic;
}

SoGroup *
IgSoTwig::magic (SoGroup *magic)
{
    ASSERT (m_magic);
    ASSERT (magic);

    magic->ref ();
    m_magic->unref ();
    return m_magic = magic;
}

//////////////////////////////////////////////////////////////////////
IgSoTwig *
IgSoTwig::buildFrom (SoNode *node)
{
    if (node->isOfType (SoSelection::getClassTypeId ())
	&& node->getName () == "IGUANA_SCENE_GRAPH_V2")
	return buildFromReps (node);
    else if (node->isOfType (SoSeparator::getClassTypeId ())
	     && node->getName () == "Top")
	return buildFromVis (node, true);
    else if (node->isOfType (SoSelection::getClassTypeId ())
	     && ((SoSelection *) node)->getNumChildren ()
	     && ((SoSelection *) node)->getChild (0)->isOfType
	        (SoSeparator::getClassTypeId ())
	     && node->getName () == ""
	     && ((SoSelection *) node)->getChild (0)->getName () != "")
	return buildFromVis (node, true);
    else
	return buildFromOther (node);
}

IgSoTwig *
IgSoTwig::buildFromVis (SoNode *from, bool top)
{
    ASSERT (from && from->isOfType (SoGroup::getClassTypeId ()));
    SoGroup	*sep = static_cast<SoGroup *> (from);
    int		max = sep->getNumChildren ();
    int		i = 0;
    IgSoTwig	*twig;

    if (top
	&& from->isOfType (SoSeparator::getClassTypeId ())
	&& from->getName () == "Top")
    {
	twig = new IgSoTwig ("Root", false, true, true, true);

	// Pick magic stuff
	for ( ; i < max; ++i)
	{
	    SoNode *child = sep->getChild (i);
	    ASSERT (child);

	    if ((child->isOfType (SoGroup::getClassTypeId ())
	         && (child->getName () == "Default_Light_Group"
		     || child->getName () == "Default_Clip_Group"
		     || child->getName () == "Default_Animator_Group"))
	        || child->isOfType (SoClipPlane::getClassTypeId ()))
	    {
	        twig->magic ()->addChild (child);
	    }
	    else
	    {
		ASSERT (i == max-1);
		ASSERT (child->isOfType (SoSelection::getClassTypeId ()));
		ASSERT (child->getName () == "Scene");
		sep = static_cast<SoGroup *> (child);
		break;
	    }
	}
    }
    else if (top)
	twig = new IgSoTwig ("Root", false, true, true, true);
    else
    {
	twig = new IgSoTwig (Ig3DModel::decode (from->getName ()),
			     false, true, true, true);
	twig->node (sep);
    }

    // Insert nodes that look like twigs as sub-twigs into the parent;
    // then remove their SoNodes from the scene graph relationships in
    // order to make the twig their sole parent.  This will allow us
    // to use the twig's nodes directly as its 3D representation.
    std::vector<bool>	erase (sep->getNumChildren (), false);
    bool		sawcontent = false;

    for (int i = 0, max = sep->getNumChildren (); i < max; ++i)
    {
	SoNode *child = sep->getChild (i);
	ASSERT (child);

	if (child->isOfType (SoSeparator::getClassTypeId ())
	    && child->getName () != ""
	    && !sawcontent)
	{
	    twig->add (buildFromVis (child, false));
	    erase [i] = true;
	}
	else
	{
	    twig->node ()->addChild (child);
	    sawcontent = true;
	}
    }

    for (int i = sep->getNumChildren (); --i >= 0; )
	if (erase [i])
	    sep->removeChild (i);

    return twig;
}

IgSoTwig *
IgSoTwig::buildFromReps (SoNode *from)
{
    ASSERT (from);
    ASSERT (from->isOfType (SoGroup::getClassTypeId ()));
    SoGroup *sep = static_cast<SoGroup *> (from);
    IgSoTwig *twig;

    if (sep->isOfType (SoSelection::getClassTypeId ()))
    {
	// Create phantom top twig to contain nodes that should go to
	// the scene graph's top magic node.  The real twigs are the
	// children of the top twig.
	ASSERT (sep->getName () == "IGUANA_SCENE_GRAPH_V2");
	ASSERT (sep->getNumChildren () == 1);
	from = sep->getChild (0);
	ASSERT (from->isOfType (SoSeparator::getClassTypeId ()));
	ASSERT (from->getName () == "IGUANA_3D_REP");
	sep = static_cast<SoGroup *> (from);
	ASSERT (sep->getNumChildren () >= 2);
	ASSERT (sep->getChild (0)->isOfType (SoGroup::getClassTypeId ()));
	ASSERT (sep->getChild (1)->isOfType (SoGroup::getClassTypeId ()));
	ASSERT (((SoGroup *) sep->getChild (1))->getNumChildren () == 0);
	twig = new IgSoTwig ("Root", false, true, true, true);
	twig->magic (static_cast<SoGroup *> (sep->getChild (0)));
    }
    else
    {
  	ASSERT (sep->isOfType (SoSeparator::getClassTypeId ()));
  	ASSERT (sep->getName () == "IGUANA_3D_REP");
	ASSERT (sep->getNumChildren () >= 2);
	ASSERT (sep->getChild (0)->isOfType (SoGroup::getClassTypeId ()));
	ASSERT (sep->getChild (1)->isOfType (SoGroup::getClassTypeId ()));
	twig = new IgSoTwig (Ig3DModel::decode (sep->getChild (1)->getName ()),
			     false, true, true, true);
	twig->magic (static_cast<SoGroup *> (sep->getChild (0)));
	twig->node (static_cast<SoGroup *> (sep->getChild (1)));
    }

    for (int i = 2, max = sep->getNumChildren (); i < max; ++i)
    {
	SoNode *child = sep->getChild (i);
	ASSERT (child);
	ASSERT (child->isOfType (SoSeparator::getClassTypeId ()));
	SoGroup *rep = static_cast<SoGroup *> (child);
	ASSERT (rep->getName () == "IGUANA_3D_REP");
	ASSERT (rep->getNumChildren () >= 2);
	twig->add (buildFromReps (rep));
    }

    return twig;
}

IgSoTwig *
IgSoTwig::buildFromOther (SoNode *from)
{
    ASSERT (from);
    IgSoTwig *root = new IgSoTwig ("Root", false, true, true, true);
    IgSoTwig *content = new IgSoTwig ("Objects", false, true, true, true);
    root->add (content);
    content->node ()->addChild (from);

    return root;
}
