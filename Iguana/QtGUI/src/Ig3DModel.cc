//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/Ig3DModel.h"
#include "Iguana/QtGUI/interface/Ig3DModelEvent.h"
#include "Iguana/QtGUI/interface/Ig3DRep.h"
#include <QGraphicsScene>
#include <QString>
#include <classlib/utils/DebugAids.h>
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

Ig3DModel::Ig3DModel (IgState	*state)
    : m_state (state),
      m_scene (new QGraphicsScene),
      m_attachPoint (0)
{
    m_scene->setObjectName (QString ("IGUANA_SCENE_GRAPH_V2"));
    Ig3DRep *rep = new Ig3DRep (this, 0, 0, new QGraphicsItemGroup);
    ASSERT (m_attachPoint == rep);
}

Ig3DModel::~Ig3DModel (void)
{ 
    m_scene->clear ();
}

//////////////////////////////////////////////////////////////////////
IgState *
Ig3DModel::state (void) const
{ return m_state; }

//////////////////////////////////////////////////////////////////////
void
Ig3DModel::listen (EventType /* event */, const Listener &listener)
{ m_listeners.push_back (listener); }

void
Ig3DModel::unlisten (EventType /* event */, const Listener &listener)
{
    Listeners::iterator pos = std::find (m_listeners.begin (),
					 m_listeners.end(),
					 listener);
    ASSERT (pos != m_listeners.end ());
    m_listeners.erase (pos);
}

void
Ig3DModel::changed (void)
{
    Ig3DModelEvent event (this);
    for (Listeners::iterator pos = m_listeners.begin ();
	 pos != m_listeners.end (); ++pos)
	(*pos) (event);
}

//////////////////////////////////////////////////////////////////////
Ig3DRep *
Ig3DModel::lookup (QGraphicsItemGroup *node) const
{ ASSERT (false); return dynamic_cast<Ig3DRep *> (node); }

//////////////////////////////////////////////////////////////////////
void
Ig3DModel::add (Ig3DRep *rep)
{
    ASSERT (rep);

    if (! m_attachPoint)
    {
	// Initialising: creating the root rep.
	ASSERT (m_scene->children ().size () == 0);
	m_scene->addItem (m_attachPoint = rep);
    }
    else
    {
	// FIXME ASSERT (m_attachPoint->findChild (rep) == -1);
	// m_attachPoint->addItem (rep);
	changed ();
    }
}

void
Ig3DModel::remove (Ig3DRep *rep, bool search /* = false */)
{
    ASSERT (rep);
    // Be conservative -- the rep has no way of knowing it has been
    // added to a model or to a parent.
    int index;
//     if ((index = m_attachPoint->findChild (rep)) != -1)
// 	m_attachPoint->removeChild (index);

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
	// SoSearchAction searcher;
// 	searcher.setNode (rep);
// 	searcher.apply (m_attachPoint);

// 	if (searcher.getPath ())
// 	{
// 	    ASSERT (searcher.getPath ()->getLength () > 1);
// 	    SoNode *parent = searcher.getPath ()->getNodeFromTail (1);
// 	    ASSERT (parent && parent->isOfType(SoSeparator::getClassTypeId()));
// 	    SoSeparator *p = static_cast<SoSeparator *> (parent);
// 	    p->removeChild (rep);
// 	}
    }

    changed ();
}

void
Ig3DModel::change (Ig3DRep *rep)
{
    ASSERT (rep);
    changed ();
}

//////////////////////////////////////////////////////////////////////
QGraphicsScene *
Ig3DModel::scene (void) const
{ return m_scene; }

Ig3DRep *
Ig3DModel::attachPoint (void) const
{ return m_attachPoint; }

//////////////////////////////////////////////////////////////////////
QString
Ig3DModel::encode (const std::string &name)
{
    // Encode names so that they use valid OpenInventor characters and
    // thre rest is escaped as `_X<hex code>', and decode them when
    // reading in the files.
    static const char	hexdigits []     = "0123456789abcdef";
    std::string		result;

//     if (! SbName::isBaseNameStartChar (name [0]))
//     {
// 	result += '_';
//     }
	
//     for (std::string::size_type i = 0; i < name.size (); ++i)
// 	if (SbName::isBaseNameChar (name[i]))
// 	    result.append (1, name[i]);
// 	else if (i > 0 && SbName::isBaseNameStartChar (name[i]))
// 	    result.append (1, name[i]);
// 	else
// 	{
// #	    if UCHAR_MAX != 255 || CHAR_BIT != 8
// #	      error expected 8-bit characters
// #	    endif
// 	    result += '_';
// 	    result += 'X';
// 	    result += hexdigits [((unsigned char) name[i]>>4)&0xf];
// 	    result += hexdigits [(unsigned char) name[i]&0xf];
// 	}

    return result.c_str ();
}

std::string
Ig3DModel::decode (const std::string &name)
{
    // Encode names so that they use valid OpenInventor characters and
    // thre rest is escaped as `_X<hex code>', and decode them when
    // reading in the files.
    static const char	hexdigits []     = "0123456789abcdef";
    std::string		result;
    const char		*first;
    const char		*second;

    for (std::string::size_type i = 0; i < name.size (); ++i)
	if (name[i] != '_'
	    || i > name.size () - 4
	    || name[i+1] != 'X'
	    || !(first = strchr (hexdigits, name[i+2]))
	    || !(second = strchr (hexdigits, name[i+3])))
	    result.append (1, name[i]);
	else
	{
#	    if UCHAR_MAX != 255 || CHAR_BIT != 8
#	      error expected 8-bit characters
#	    endif
	    unsigned int code = ((unsigned) (first - hexdigits) << 4)
				+ ((unsigned) (second - hexdigits));
	    ASSERT (code <= UCHAR_MAX);
	    result += static_cast<char> (code);
	    i += 3;
	}

    return result;
}

std::string
Ig3DModel::decode (const QString &name)
{ return decode (std::string (name.toStdString ())); }
