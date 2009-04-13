//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/Ig3DBaseModel.h"
#include "Iguana/QtGUI/interface/Ig3DBaseRep.h"
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoSeparator.h>
#include <classlib/utils/DebugAids.h>

Ig3DBaseModel::Ig3DBaseModel (IgState *state)
    : m_state (state),
      m_sceneGraph (new SoSelection),
      m_attachPoint (0)
{
    // ASSERT ("attachPoint is the scene graph or within it");
    m_sceneGraph->ref ();
    m_sceneGraph->setName ("IGUANA_SCENE_GRAPH_V2");
//     Ig3DBaseRep *rep = new Ig3DBaseRep (this, 0, 0, new SoSeparator);
//     ASSERT (m_attachPoint == rep);
}

Ig3DBaseModel::~Ig3DBaseModel (void)
{ m_sceneGraph->unref (); }

void
Ig3DBaseModel::changed (void)
{
    // FIXME: emit change ();
//     Ig3DBaseModelEvent event (this);
//     for (Listeners::iterator pos = m_listeners.begin ();
//          pos != m_listeners.end (); ++pos)
// 	(*pos) (event);
}

Ig3DBaseRep *
Ig3DBaseModel::lookup (SoGroup *node) const
{ ASSERT (false); return dynamic_cast<Ig3DBaseRep *> (node); }

void
Ig3DBaseModel::add (Ig3DBaseRep *rep)
{
    ASSERT (rep);
    
    if (! m_attachPoint)
    {
        // Initialising: creating the root rep.
	ASSERT (m_sceneGraph->getNumChildren () == 0);
	m_sceneGraph->addChild (m_attachPoint = rep);
    }
    else
    {
        ASSERT (m_attachPoint->findChild (rep) == -1);
	m_attachPoint->addChild (rep);
	changed ();
    }
}

void
Ig3DBaseModel::remove (Ig3DBaseRep *rep, bool search /* = false */)
{
    ASSERT (rep);
    // Be conservative -- the rep has no way of knowing it has been
    // added to a model or to a parent.
    int index;
    if ((index = m_attachPoint->findChild (rep)) != -1)
        m_attachPoint->removeChild (index);
    
    if (search)
    {
        // NB: The object rep can be attached in only one place.  We
        // end up here if the rep is being deleted via its object
	// going away and the rep doesn't know where it belongs to,
	// asking us to do the hard work.
	//
	// Find the parent.  Since this is a rep, the parent must
	// always be a rep (a separator) and not hidden inside
	// switches.  It might have been in the attach point but we
	// already removed that one, so we know the path must be at
	// least two levels.
	SoSearchAction searcher;
	searcher.setNode (rep);
	searcher.apply (m_attachPoint);

	if (searcher.getPath ())
	{
	    ASSERT (searcher.getPath ()->getLength () > 1);
	    SoNode *parent = searcher.getPath ()->getNodeFromTail (1);
	    ASSERT (parent && parent->isOfType(SoSeparator::getClassTypeId()));
            SoSeparator *p = static_cast<SoSeparator *> (parent);
	    p->removeChild (rep);
         }
    }
    
    changed ();
}

void
Ig3DBaseModel::change (Ig3DBaseRep *rep)
{
    ASSERT (rep);
    changed ();
}

SoGroup *
Ig3DBaseModel::sceneGraph (void) const
{ return m_sceneGraph; }

Ig3DBaseRep *
Ig3DBaseModel::attachPoint (void) const
{ return m_attachPoint; }

IgState *
Ig3DBaseModel::state (void) const
{ return m_state; }

SbString
Ig3DBaseModel::encode (const std::string &name)
{
    // Encode names so that they use valid OpenInventor characters and
    // thre rest is escaped as `_X<hex code>', and decode them when
    // reading in the files.
    static const char   hexdigits []     = "0123456789abcdef";
    std::string         result;

    if (! SbName::isBaseNameStartChar (name [0]))
    {
        result += '_';
    }
    
    for (std::string::size_type i = 0; i < name.size (); ++i)
	if (SbName::isBaseNameChar (name[i]))
	    result.append (1, name[i]);
	else if (i > 0 && SbName::isBaseNameStartChar (name[i]))
	    result.append (1, name[i]);
	else
	{
#           if UCHAR_MAX != 255 || CHAR_BIT != 8
#             error expected 8-bit characters
#           endif
	    result += '_';
	    result += 'X';
	    result += hexdigits [((unsigned char) name[i]>>4)&0xf];
	    result += hexdigits [(unsigned char) name[i]&0xf];
	}
    
    return result.c_str ();
}

std::string
Ig3DBaseModel::decode (const std::string &name)
{
    // Encode names so that they use valid OpenInventor characters and
    // thre rest is escaped as `_X<hex code>', and decode them when
    // reading in the files.
    static const char   hexdigits []     = "0123456789abcdef";
    std::string         result;
    const char          *first;
    const char          *second;
    
    for (std::string::size_type i = 0; i < name.size (); ++i)
        if (name[i] != '_'
	    || i > name.size () - 4
	    || name[i+1] != 'X'
	    || !(first = strchr (hexdigits, name[i+2]))
	    || !(second = strchr (hexdigits, name[i+3])))
	    result.append (1, name[i]);
	else
	{
#           if UCHAR_MAX != 255 || CHAR_BIT != 8
#             error expected 8-bit characters
#           endif
	    unsigned int code = ((unsigned) (first - hexdigits) << 4)
				+ ((unsigned) (second - hexdigits));
	    ASSERT (code <= UCHAR_MAX);
	    result += static_cast<char> (code);
	    i += 3;
	}

    return result;
}

std::string
Ig3DBaseModel::decode (const SbName &name)
{ return decode (std::string (name.getString ())); }

