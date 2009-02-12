//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Framework/interface/IgTwig.h"
#include "Iguana/Framework/interface/IgDatum.h"
#include <classlib/utils/MapIterator.h>
#include <classlib/utils/DebugAids.h>
#include <classlib/utils/StringOps.h>
#include <algorithm>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>

const char IgTwig::s_name[]	= "Name";
const char IgTwig::s_traverse[]	= "Traverse";
const char IgTwig::s_visible[]	= "Visible";
const char IgTwig::s_cascade[]	= "Cascade";

//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgTwig::IgTwig (IgTwig *parent /* = 0 */)
    : m_parent (parent)
{ if (m_parent) m_parent->add (this); }

// Removed in parent.
IgTwig::~IgTwig (void)
{}

IgTwig *
IgTwig::root (void) const
{ return m_parent ? m_parent->root () : const_cast<IgTwig *> (this); }

IgTwig *
IgTwig::parent (void) const
{ return m_parent; }

unsigned
IgTwig::children (void)
{ return 0; }

IgTwig *
IgTwig::child (unsigned /* index */)
{ ASSERT (false); return 0; }

unsigned
IgTwig::indexOf (const IgTwig * /* p */) const
{ ASSERT (false); return 0; } 

bool
IgTwig::expand (void)
{ return true; }

void
IgTwig::add (IgTwig *child)
{ ASSERT (child); ASSERT (false); }

void
IgTwig::added (IgTwig *parent)
{ ASSERT (parent); m_parent = parent; }

void
IgTwig::remove (IgTwig *child)
{ ASSERT (child); ASSERT (false); }

void
IgTwig::removed (IgTwig *parent)
{ ASSERT (m_parent == parent); m_parent = 0; }

void
IgTwig::destroy (void)
{}

void
IgTwig::clear (void)
{}

std::string
IgTwig::fullName (void)
{
    IgTwig *currentTwig = this;
    std::string result = "/"+currentTwig->name ();
    while (currentTwig->parent ())
    {
	result = "/" + currentTwig->parent ()->name () + result;	
	currentTwig = currentTwig->parent ();
    }
    return result;    
}


void
IgTwig::cleared (IgTwig *parent)
{ ASSERT (m_parent == parent); }

void
IgTwig::attributes (Attributes &attrs)
{
    attrs [s_name]
	= GetSetPair (lat::CreateCallback (this, &IgTwig::getNameAttribute),
		      lat::CreateCallback (this, &IgTwig::setNameAttribute));
    
    attrs [s_traverse]
	= GetSetPair (lat::CreateCallback (this, &IgTwig::getTraverseAttribute),
		      lat::CreateCallback (this, &IgTwig::setTraverseAttribute));
    
    attrs [s_visible]
	= GetSetPair (lat::CreateCallback (this, &IgTwig::getVisibleAttribute),
		      lat::CreateCallback (this, &IgTwig::setVisibleAttribute));
    
    attrs [s_cascade]
	= GetSetPair (lat::CreateCallback (this, &IgTwig::getCascadeAttribute),
		      lat::CreateCallback (this, &IgTwig::setCascadeAttribute));
}

void
IgTwig::getNameAttribute (IgDatum *&result)
{
    ASSERT (! result);
    result = new IgStringDatum (name ());
}

void
IgTwig::setNameAttribute (const IgDatum *value)
{
    ASSERT (value);
    ASSERT (dynamic_cast<const IgStringDatum *> (value));
    name (dynamic_cast<const IgStringDatum *> (value)->value ());
}

void
IgTwig::getTraverseAttribute (IgDatum *&result)
{
    ASSERT (! result);
    result = new IgBoolDatum (selfTraverse ());
}

void
IgTwig::setTraverseAttribute (const IgDatum *value)
{
    ASSERT (value);
    ASSERT (dynamic_cast<const IgBoolDatum *> (value));
    selfTraverse (dynamic_cast<const IgBoolDatum *> (value)->value ());
}

void
IgTwig::getVisibleAttribute (IgDatum *&result)
{
    ASSERT (! result);
    result = new IgBoolDatum (selfVisible ());
}

void
IgTwig::setVisibleAttribute (const IgDatum *value)
{
    ASSERT (value);
    ASSERT (dynamic_cast<const IgBoolDatum *> (value));
    selfVisible (dynamic_cast<const IgBoolDatum *> (value)->value ());
}

void
IgTwig::getCascadeAttribute (IgDatum *&result)
{
    ASSERT (! result);
    result = new IgBoolDatum (cascade ());
}

void
IgTwig::setCascadeAttribute (const IgDatum *value)
{
    ASSERT (value);
    ASSERT (dynamic_cast<const IgBoolDatum *> (value));
    cascade (dynamic_cast<const IgBoolDatum *> (value)->value ());
}

bool
IgTwig::traverse (void) const
{
    return selfTraverse () && (! parent () || parent ()->traverse ());
}

bool
IgTwig::visible (void) const
{
    return selfVisible () && (! parent () || parent ()->traverse ());
}

IgTwig *
IgTwig::lookup (const std::string &path)
{
    IgTwig *currentTwig = this;

    using namespace lat;
    
    StringList stringList = StringOps::split (path, "/", 
					      StringOps::KeepLeadingEmpty);

    for (StringList::iterator i = stringList.begin ();
	 i != stringList.end ();
	 i++)
    {
	std::string &pathUnit = *i;
     
	if (pathUnit == "")
	{
	    while (currentTwig->parent () != 0)
	    {
		currentTwig = currentTwig->parent ();	    
	    }
	}
	else if (pathUnit == "..")
	{
	    if (currentTwig->parent ());
	    {
		currentTwig = currentTwig->parent ();
	    }	    
	}
	else
	{
	    bool notDone = true;	    
	    for (unsigned int i = 0;
		 notDone && i < currentTwig->children ();
		 i++)
	    {
		IgTwig *twig = currentTwig->child (i);
		if (twig->name () == pathUnit)
		{
		    currentTwig = twig;
		    notDone = false;		    
		} 
	    }
	}	
    }    
    return currentTwig;    
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
IgCompoundTwig::IgCompoundTwig (IgTwig *parent /* = 0 */,
				unsigned sizeEstimate /* = 0 */)
    : IgTwig (parent),
      m_flags (0)
{ m_children.reserve (sizeEstimate); }

IgCompoundTwig::~IgCompoundTwig (void)
{ destroy (); } 

unsigned
IgCompoundTwig::children (void)
{ return m_children.size (); }

IgTwig *
IgCompoundTwig::child (unsigned index)
{
    ASSERT (index < m_children.size ());
    ASSERT (m_children [index]);

    return m_children [index];
}



unsigned
IgCompoundTwig::indexOf (const IgTwig *p) const
{
    ASSERT (flag (EXPANDED));

    for (unsigned i = 0; i < m_children.size (); ++i)
	if (m_children [i] == p)
	    return i;

    ASSERT (false);
    return 0;
}

bool
IgCompoundTwig::expand (void)
{
    bool expanded = flag (EXPANDED);
    flag (EXPANDED, true);
    return expanded;
}

void
IgCompoundTwig::add (IgTwig *child)
{
    ASSERT (child);
    flag (EXPANDED, true);
    m_children.push_back (child);
    child->added (this);
}

void
IgCompoundTwig::remove (IgTwig *child)
{
    ASSERT (flag (EXPANDED));

    std::vector<IgTwig *>::iterator pos
	= std::find (m_children.begin (), m_children.end (), child);
    ASSERT (pos != m_children.end ());
    m_children.erase (pos);
    child->removed (this);
    // FIXME?: child->destroy (); delete child;
}

void
IgCompoundTwig::destroy (void)
{
    std::vector<IgTwig *>::iterator pos = m_children.begin ();
    for ( ; pos != m_children.end (); ++pos)
    {
	(*pos)->destroy ();
	delete *pos;
    }

    m_children.clear ();
}


void
IgCompoundTwig::clear (void)
{
    std::vector<IgTwig *>::iterator pos = m_children.begin ();
    for ( ; pos != m_children.end (); ++pos)
    {
	(*pos)->clear ();
	(*pos)->cleared (this);
    }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
IgSimpleTwig::IgSimpleTwig (IgTwig *parent, const std::string &name /* = "" */,
			    bool traverse /* = true */, bool visible /* = true */,
			    bool cascade /* = true */)
    : IgCompoundTwig (parent),
      m_name (name)
{
    flag (TRAVERSE, traverse);
    flag (VISIBLE, visible);
    flag (CASCADE, cascade);
}

IgSimpleTwig::IgSimpleTwig (const std::string &name,
			    bool traverse /* = true */, bool visible /* = true */,
			    bool cascade /* = true */)
    : IgCompoundTwig (0),
      m_name (name)
{
    flag (TRAVERSE, traverse);
    flag (VISIBLE, visible);
    flag (CASCADE, cascade);
}

std::string
IgSimpleTwig::name (void) const
{ return m_name; }

void
IgSimpleTwig::name (const std::string &name)
{ m_name = name; }

bool
IgSimpleTwig::selfTraverse (void) const
{ return flag (TRAVERSE); }

void
IgSimpleTwig::selfTraverse (bool value)
{ flag (TRAVERSE, value); }

bool
IgSimpleTwig::selfVisible (void) const
{ return flag (VISIBLE); }

void
IgSimpleTwig::selfVisible (bool value)
{ flag (VISIBLE, value); }

bool
IgSimpleTwig::cascade (void) const
{ return flag (CASCADE); }

void
IgSimpleTwig::cascade (bool value)
{ flag (CASCADE, value); }


    
