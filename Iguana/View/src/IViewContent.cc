//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/View/interface/IViewContent.h"
#include "Iguana/View/interface/IViewQueueProcessor.h"
#include "Iguana/View/interface/IViewQueue.h"
#include <classlib/utils/Callback.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IViewContent::IViewContent (IgState *state, IgStateKey key,
			    Type which, lat::Callback action)
    : m_state (state),
      m_key (key)
{
    ASSERT (m_state);
    m_state->put (m_key, this);
    
    if (which == MAIN_THREAD && action)
	action ();
    else if (which == EVENT_THREAD && action)
        // Schedule the initialisation as appropriate
	if (IViewQueueProcessor::get (m_state))
	    IViewQueue::get (m_state)->enqueue (action);
	else
	    action ();
}

IViewContent::IViewContent (IgState *state, IgStateKey key,
			    lat::Callback mainAction, lat::Callback eventAction)
    : m_state (state),
      m_key (key)
{
    ASSERT (m_state);
    m_state->put (m_key, this);
    
    if (mainAction)
	mainAction ();
    if (eventAction)
	// Schedule the initialisation as appropriate
	if (IViewQueueProcessor::get (m_state))
	    IViewQueue::get (m_state)->enqueue (eventAction);
	else
	    eventAction ();
}
 
/** Destruct the data proxy.  Detaches itself from the state and
    deregisters pending initialisation actions if any.  */
IViewContent::~IViewContent (void)
{ ASSERT (m_state); m_state->detach (m_key); }
