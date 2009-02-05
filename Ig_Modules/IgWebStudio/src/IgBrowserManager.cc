//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgWebStudio/interface/IgBrowserManager.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgState.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
IG_DEFINE_STATE_ELEMENT (IgBrowserManager, "Services/Browser Manager");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgBrowserManager::IgBrowserManager (IgState * state)
    : m_state (state)
{
    m_state->put (s_key, this);    
}

IgBrowserManager::~IgBrowserManager (void)
{
    m_state->detach (s_key);
}

void
IgBrowserManager::add (IgBrowser *browser, const std::string name)
{
    m_browsers.insert (MapEntry (name, browser));    
}
