//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/ISpyQueue.h"
#include <classlib/utils/Callback.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (ISpyQueue, "Services/Framework/Elements/EventQueue");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Construct the queue.  */
ISpyQueue::ISpyQueue (IgState *state)
    : QQueue<lat::Callback> (), 
      m_state (state)
{ ASSERT (m_state); m_state->put (s_key, this); }

/** Destroy the object and detach it from the state.  */
ISpyQueue::~ISpyQueue (void)
{ ASSERT (m_state); m_state->detach (s_key); }
