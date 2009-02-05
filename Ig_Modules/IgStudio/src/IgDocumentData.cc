//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgStudio/interface/IgDocumentData.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgDocumentDataRoot.h"
#include "Ig_Modules/IgCommonData/interface/IgTwig.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgDocumentData, "Services/Studio/Document Data");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgDocumentData::IgDocumentData (IgState *state, IgDocument *owner)
    : m_state (state),
      m_document (owner)
{
    ASSERT (m_state);
    // FIXME: temporarly removing the check, until IgDocument returns
    // Qt free.
    // ASSERT (m_document);
    m_state->put (s_key, this);
}

IgDocumentData::~IgDocumentData (void)
{
    ASSERT (m_state);
    //FIXME: assertion not needed.
    //ASSERT (m_document);

    for (TwigMap::iterator i = m_rootMap.begin ();
	 i != m_rootMap.end ();
	 i++)
    {	
	delete i->second;
    }
    
    m_state->detach (s_key);
}

IgDocument *
IgDocumentData::document (void)
{
    ASSERT (m_state);
    ASSERT (m_document);

    return m_document;
}


IgSimpleTwig *
IgDocumentData::root (const std::string &name /*= ""*/, bool create)
{
    ASSERT (m_state);
    // FIXME: needs proper fixing. Document should be Qt-less and also
    // the web application should create one.    
    // ASSERT (m_document);
    
    std::string n = name;
    if (n.size () == 0)
	n = IgDocumentDataRoot::getCurrentRoot ();

    TwigMap::iterator i = m_rootMap.find (n);
    
    if (i == m_rootMap.end ())
    {
	IgSimpleTwig *twig = 0;
	
	if (create)
	{
	    twig = new IgSimpleTwig ("Objects");
	    m_rootMap[n] = twig;
	}
	return twig;
    }
    return i->second;
}

void
IgDocumentData::root (IgSimpleTwig *twig, 
		      const std::string &rootName /*= ""*/)
{
    ASSERT (twig);
    std::string n = rootName;
    if (n.size () == 0)
	n = IgDocumentDataRoot::getCurrentRoot ();
    m_rootMap[n] = twig; 
}

IgSimpleTwig *
IgDocumentData::add (const std::string &name, const std::string &rootName)
{
    // FIXME: Create subtree paths (breaking at `/' letters)?
    // FIXME: Notify document about changes?
    ASSERT (m_state);
    // NOTE: assertion not needed.
    //ASSERT (m_document);

    return new IgSimpleTwig (root (rootName, true), name);
}

IgTwig *
IgDocumentData::find (const std::string &name, const std::string &rootName)
{
    // FIXME: Search subtree path (breaking at `/' letters)?
    // FIXME: Notify document about changes?
    // FIXME: Provide special root twig that always notifies
    //  document about changes and add a protocol for twigs
    //  to notify their parents when they add children.
    ASSERT (m_state);
    // NOTE: Assertion not needed?
    //ASSERT (m_document);
    IgSimpleTwig *parent = root (rootName, true);
    for (unsigned i = 0; i < parent->children (); ++i)
    {
	IgTwig *child = parent->child (i);
	if (child->name () == name)
	    return child;
    }
    return 0;
}

IgDocumentData::TwigMap::const_iterator
IgDocumentData::begin (void)
{
    return m_rootMap.begin ();    
}

IgDocumentData::TwigMap::const_iterator
IgDocumentData::end (void)
{
    return m_rootMap.end ();
}
