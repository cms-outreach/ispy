//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgSocketDriver/interface/IgSessionManager.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgState.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
IG_DEFINE_STATE_ELEMENT (IgSessionManager, "Session Manager");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgSessionManager::IgSessionManager (IgState *state)
    : m_state (state)
{
    m_state->put (s_key, this);   
}

IgSessionManager::~IgSessionManager (void)
{
    m_state->detach (s_key);    
}

void
IgSessionManager::setSession (const std::string &idCookie, 
			      IgState *state)
{
    if (m_stateMap.find (idCookie) == m_stateMap.end ())
    {
	m_stateMap.insert (StateMapElement (idCookie, state));	
    }
    else
    {
	m_stateMap [idCookie] = state;	
    }    
}

IgState *
IgSessionManager::getSession (const std::string &idCookie)
{
    if (m_stateMap.find (idCookie) != m_stateMap.end ())
    {
	return m_stateMap[idCookie];	
    }
    return 0;        
}

IgState *
IgSessionManager::createSession (const std::string &idCookie,
				 IgState *parent)
{
    IgState *state = new IgState (parent);
    m_stateMap.insert (StateMapElement (idCookie, state));
    return state;    
}

