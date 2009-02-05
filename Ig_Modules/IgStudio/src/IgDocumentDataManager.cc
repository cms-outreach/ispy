//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgStudio/interface/IgDocumentDataManager.h"
#include "Ig_Modules/IgStudio/interface/IgDocumentData.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgDocumentDataRoot.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
IG_DEFINE_STATE_ELEMENT (IgDocumentDataManager, "Services/Document Data Manager");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgDocumentDataManager::IgDocumentDataManager (IgState *state)
    : m_state (state)
{ ASSERT (m_state); m_state->put (s_key, this); }

IgDocumentDataManager::~IgDocumentDataManager (void)
{ m_state->detach (s_key); }

IgState *
IgDocumentDataManager::state (void)
{ return m_state; }

std::vector<std::string>
IgDocumentDataManager::roots (void)
{
    std::vector<std::string> names;
	    
    for (DocDataMap::iterator i = m_docData.begin ();
	 i != m_docData.end (); i++)
        names.push_back (i->first);
	    
    return names;
}

IgState*
IgDocumentDataManager::lookup (IgState* state /*=0*/, std::string root /*=""*/)
{
    if (root.size () == 0)
      root = IgDocumentDataRoot::getCurrentRoot ();
      
    DocDataMap::iterator i = m_docData .find (root);
    
    if (i != m_docData.end ())
       return i->second;
    else if (state)
    {
        m_docData [root] = state;
	IgDocumentData::get (state)->root (root, true);
    }
    return state;
}
