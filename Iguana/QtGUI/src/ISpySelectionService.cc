//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/ISpySelectionService.h"
#include <classlib/utils/DebugAids.h>
#include <Inventor/nodes/SoNodes.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (ISpySelectionService, "Services/Coin/Selection");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

ISpySelectionService::ISpySelectionService (IgState *state, SoNode *selection)
    : m_state (state),
      m_selection (selection)
{
    ASSERT (state);
    ASSERT (selection);

    m_selection->ref ();    
    state->put (s_key, this);
}

ISpySelectionService::~ISpySelectionService (void) 
{
    ASSERT (m_state);
    ASSERT (m_selection);
    m_selection->unref ();
    m_state->detach (s_key);
}

SoNode *
ISpySelectionService::selection (void) 
{ return m_selection; }
