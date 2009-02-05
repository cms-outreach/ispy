//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtBrowser/interface/IgQtAppContextService.h"
#include <classlib/utils/DebugAids.h>
#include <qwidget.h>
#include <qtabwidget.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgQtAppContextService, "Services/Qt/App Context");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQtAppContextService::IgQtAppContextService (IgState *state,
					      QWidget *mainWindow)
    : m_state (state),
      m_mainWindow (mainWindow)
{
    ASSERT (state);
    ASSERT (mainWindow);
    state->put (s_key, this);
}

IgQtAppContextService::~IgQtAppContextService (void)
{
    ASSERT (m_state);
    ASSERT (m_mainWindow);
    m_state->detach (s_key);
}

void
IgQtAppContextService::add (ConnectionCallback connection, 
			    void *data, bool enable /*=true*/)
{
    lat::Callback focusIn;
    lat::Callback focusOut;
    focusIn = lat::CreateCallback (connection, data, enable);
    focusOut = lat::CreateCallback (connection, data, false);

    m_focusInCallbacks.push_back (focusIn);
    m_focusOutCallbacks.push_back (focusOut);    
}


void
IgQtAppContextService::addOnFocusIn (lat::CallbackRep *callback)
{
    m_focusInCallbacks.push_back (lat::Callback (callback));    
}

void
IgQtAppContextService::addOnFocusOut (lat::CallbackRep *callback)
{
    m_focusOutCallbacks.push_back (lat::Callback (callback));    
}
				     
// FIXME: rather than doing focusIn focusOut we should have a
// changeFocus which does both and does it up to the common state
// level, rather than always up to the to of the state tree.

void
IgQtAppContextService::focusIn (void)
{
    if (m_state->parent ())
    {
	IgQtAppContextService *service;
	service = IgQtAppContextService::get (m_state->parent ());	
	if (service)
	    service->focusIn ();	
    }    
    
    //FIXME: use LOG macro for logging.
    //std::cerr << "Focusing in of state " << m_state << std::endl;    
    for (CallbackList::iterator i = m_focusInCallbacks.begin ();
	 i != m_focusInCallbacks.end ();
	 i++)
    {
	(*i) ();	
    }    
}

void
IgQtAppContextService::focusOut (void)
{
    //FIXME: use LOG macro for logging.
    //std::cerr << "Focusing out of state " << m_state << std::endl;    
    for (CallbackList::iterator i = m_focusOutCallbacks.begin ();
	 i != m_focusOutCallbacks.end ();
	 i++)
    {
	(*i) ();
    }    
    
    if (m_state->parent ())
    {
	IgQtAppContextService *service;
	service = IgQtAppContextService::get (m_state->parent ());	
	if (service)
	    service->focusOut ();
    }    
}
