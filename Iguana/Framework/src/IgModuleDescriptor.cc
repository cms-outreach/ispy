//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Framework/interface/IgModuleDescriptor.h"
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

IgModuleDescriptor::IgModuleDescriptor (IgModuleDescriptor *parent /* = 0 */,
				        const std::string &t1 /* = "" */,
				        const std::string &t2 /* = "" */,
				        const std::string &t3 /* = "" */,
				        const std::string &t4 /* = "" */,
				        const std::string &t5 /* = "" */,
				        const std::string &t6 /* = "" */,
				        const std::string &t7 /* = "" */,
				        const std::string &t8 /* = "" */)
    : m_parent (parent)
{
    if (m_parent)
	m_parent->addChild (this);

    if (! t1.empty ())
	m_tokens.push_back (t1);

    if (! t2.empty ())
    {
	ASSERT (! t1.empty ());
	m_tokens.push_back (t2);
    }

    if (! t3.empty ())
    {
	ASSERT (! t2.empty ());
	m_tokens.push_back (t3);
    }

    if (! t4.empty ())
    {
	ASSERT (! t3.empty ());
	m_tokens.push_back (t4);
    }

    if (! t5.empty ())
    {
	ASSERT (! t4.empty ());
	m_tokens.push_back (t5);
    }

    if (! t6.empty ())
    {
	ASSERT (! t5.empty ());
	m_tokens.push_back (t6);
    }

    if (! t7.empty ())
    {
	ASSERT (! t7.empty ());
	m_tokens.push_back (t7);
    }

    if (! t8.empty ())
    {
	ASSERT (! t8.empty ());
	m_tokens.push_back (t8);
    }
}

IgModuleDescriptor::IgModuleDescriptor (IgModuleDescriptor *parent,
				        const std::vector<std::string> &tokens)
    : m_parent (parent),
      m_tokens (tokens)
{
    if (m_parent)
	m_parent->addChild (this);
}


IgModuleDescriptor::~IgModuleDescriptor (void)
{
    for (unsigned i = children (); i > 0; --i)
	delete m_children [i-1];
}

//////////////////////////////////////////////////////////////////////
IgModuleDescriptor *
IgModuleDescriptor::parent (void) const
{ return m_parent; }

unsigned
IgModuleDescriptor::children (void) const
{ return m_children.size (); }

IgModuleDescriptor *
IgModuleDescriptor::child (unsigned index) const
{ ASSERT (index < m_children.size ()); return m_children [index]; }

void
IgModuleDescriptor::addChild (IgModuleDescriptor *child)
{
    ASSERT (child);
    ASSERT (std::find (m_children.begin (), m_children.end (), child)
	    == m_children.end ());

    m_children.push_back (child);
}

//////////////////////////////////////////////////////////////////////
unsigned
IgModuleDescriptor::tokens (void) const
{ return m_tokens.size (); }

const std::string &
IgModuleDescriptor::token (unsigned index) const
{ ASSERT (index < m_tokens.size ()); return m_tokens [index]; }

void
IgModuleDescriptor::addToken (const std::string &token)
{ m_tokens.push_back (token); }

//////////////////////////////////////////////////////////////////////
void
IgModuleDescriptor::dup (IgModuleDescriptor *to) const
{
    IgModuleDescriptor *self = new IgModuleDescriptor (to, m_tokens);
    for (unsigned i = 0; i < children (); ++i)
	child (i)->dup (self);
}
