//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/View/interface/IViewQueue.h"
#include <classlib/utils/Callback.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IViewQueue, "Services/Framework/Elements/EventQueue");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Construct the queue.  */
IViewQueue::IViewQueue (IgState *state)
    : QQueue<lat::Callback> (), 
      m_state (state)
{ ASSERT (m_state); m_state->put (s_key, this); }

/** Destroy the object and detach it from the state.  */
IViewQueue::~IViewQueue (void)
{ ASSERT (m_state); m_state->detach (s_key); }
