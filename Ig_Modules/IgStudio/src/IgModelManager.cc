//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgStudio/interface/IgModelManager.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgModel.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
IG_DEFINE_STATE_ELEMENT (IgModelManager, "Services/Model Manager");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgModelManager::IgModelManager (IgState *state)
    : m_state (state)
{ ASSERT (m_state); m_state->put (s_key, this); }

IgModelManager::~IgModelManager (void)
{
    m_state->detach (s_key);
    for (ModelList::iterator i = m_models.begin ();
	 i != m_models.end ();
	 i++)
        for (std::vector<IgModel *>::iterator j = i->second.begin ();
             j != i->second.end (); j++)
	    delete (*j);
}

IgState *
IgModelManager::state (void)
{ return m_state; }

std::vector<std::string>
IgModelManager::roots (void)
{
    std::vector<std::string> names;
	    
    for (ModelList::iterator i = m_models.begin ();
	 i != m_models.end (); i++)
        names.push_back (i->first);
	    
    return names;
}

void
IgModelManager::add (IgState *state, IgModel* model, std::string name /*= ""*/)
{
    ASSERT (model);
    
    if (name.size () == 0)
        name = IgDocumentDataRoot::getCurrentRoot ();
    
    IgDocumentDataManager::get (state ? state : m_state)->
        lookup(state, name);
    
    m_models[name].push_back (model);
}
