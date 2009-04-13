//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/ISpyQueueProcessor.h"
#include "Iguana/QtGUI/interface/ISpyQueue.h"
#include <classlib/utils/Signal.h>
#include <classlib/utils/Callback.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (ISpyQueueProcessor,
			 "Services/Framework/Elements/QueueProcessor");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Construct the visualisation event processing handler.  */
ISpyQueueProcessor::ISpyQueueProcessor (IgState *state)
    : m_state (state),
      m_done (false),
      m_dispatcher (new ISpyEventDispatcher ())
{
    ASSERT (m_state); m_state->put (s_key, this); 
}

/** Destroy the object and detach from the state.  */
ISpyQueueProcessor::~ISpyQueueProcessor (void)
{ ASSERT (m_state); m_state->detach (s_key); }

ISpyQueueProcessor::ISpyEventDispatcher
ISpyQueueProcessor::eventDispatcher (void)
{
    return *m_dispatcher;
}

void
ISpyQueueProcessor::process (void)
{
    lat::Signal::handleFatal ("iguana");

    // Pull items off the queue and execute them until we are told to
    // stop.  Abort throws an exception so we don't need to care.
    ISpyQueue *q = ISpyQueue::get (m_state);
    ASSERT (q);

    for (m_done = false; ! m_done;)
    {
	lat::Callback action (q->dequeue ());
	action ();
    }
}

void
ISpyQueueProcessor::scheduleAbort (void)
{
    ASSERT (ISpyQueue::get (m_state));
    ISpyQueue::get (m_state)
	->enqueue (lat::CreateCallback (this, &ISpyQueueProcessor::doAbort));
}

void
ISpyQueueProcessor::scheduleNextEvent (void)
{
    ASSERT (ISpyQueue::get (m_state));
    ISpyQueue::get (m_state)
	->enqueue (lat::CreateCallback (this, &ISpyQueueProcessor::doNextEvent));
}

void
ISpyQueueProcessor::doAbort (void)
{ 
    //throw FIXME::SomeExceptionTypeHere (); 
}

void
ISpyQueueProcessor::doNextEvent (void)
{
    ISpyEventMessage event;
    
    eventDispatcher ().broadcast (event);
    m_done = true;
}
