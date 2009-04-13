//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/ISpyContent.h"
#include "Iguana/QtGUI/interface/ISpyQueueProcessor.h"
#include "Iguana/QtGUI/interface/ISpyQueue.h"
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

ISpyContent::ISpyContent (IgState *state, IgStateKey key,
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
	if (ISpyQueueProcessor::get (m_state))
	    ISpyQueue::get (m_state)->enqueue (action);
	else
	    action ();
}

ISpyContent::ISpyContent (IgState *state, IgStateKey key,
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
	if (ISpyQueueProcessor::get (m_state))
	    ISpyQueue::get (m_state)->enqueue (eventAction);
	else
	    eventAction ();
}
 
/** Destruct the data proxy.  Detaches itself from the state and
    deregisters pending initialisation actions if any.  */
ISpyContent::~ISpyContent (void)
{ ASSERT (m_state); m_state->detach (m_key); }
