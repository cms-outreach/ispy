//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/View/interface/IViewQueueProcessor.h"
#include "Iguana/View/interface/IViewQueue.h"
#include <classlib/utils/Signal.h>
#include <classlib/utils/Callback.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IViewQueueProcessor,
			 "Services/Framework/Elements/QueueProcessor");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Construct the visualisation event processing handler.  */
IViewQueueProcessor::IViewQueueProcessor (IgState *state)
    : m_state (state),
      m_done (false),
      m_dispatcher (new IViewEventDispatcher ())
{
    ASSERT (m_state); m_state->put (s_key, this); 
}

/** Destroy the object and detach from the state.  */
IViewQueueProcessor::~IViewQueueProcessor (void)
{ ASSERT (m_state); m_state->detach (s_key); }

IViewQueueProcessor::IViewEventDispatcher
IViewQueueProcessor::eventDispatcher (void)
{
    return *m_dispatcher;
}

void
IViewQueueProcessor::process (void)
{
    lat::Signal::handleFatal ("iguana");

    // Pull items off the queue and execute them until we are told to
    // stop.  Abort throws an exception so we don't need to care.
    IViewQueue *q = IViewQueue::get (m_state);
    ASSERT (q);

    for (m_done = false; ! m_done;)
    {
	lat::Callback action (q->dequeue ());
	action ();
    }
}

void
IViewQueueProcessor::scheduleAbort (void)
{
    ASSERT (IViewQueue::get (m_state));
    IViewQueue::get (m_state)
	->enqueue (lat::CreateCallback (this, &IViewQueueProcessor::doAbort));
}

void
IViewQueueProcessor::scheduleNextEvent (void)
{
    ASSERT (IViewQueue::get (m_state));
    IViewQueue::get (m_state)
	->enqueue (lat::CreateCallback (this, &IViewQueueProcessor::doNextEvent));
}

void
IViewQueueProcessor::doAbort (void)
{ 
    //throw FIXME::SomeExceptionTypeHere (); 
}

void
IViewQueueProcessor::doNextEvent (void)
{
    IViewEventMessage event;
    
    eventDispatcher ().broadcast (event);
    m_done = true;
}
