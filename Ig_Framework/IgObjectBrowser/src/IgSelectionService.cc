//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Framework/IgObjectBrowser/interface/IgSelectionService.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgSelectionMessage.h"
#include "classlib/utils/DebugAids.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgSelectionService, "Services/Message/Selection");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgSelectionService::IgSelectionService (IgState *state)
    : m_state (state)
{ ASSERT (m_state); m_state->put (s_key, this); }
