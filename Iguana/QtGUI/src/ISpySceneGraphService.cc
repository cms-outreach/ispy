//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/ISpySceneGraphService.h"
#include <classlib/utils/DebugAids.h>
#include <Inventor/nodes/SoNodes.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (ISpySceneGraphService, "Services/Coin/Scene Graph");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

ISpySceneGraphService::ISpySceneGraphService (IgState *state, SoNode *sceneGraph, SoNode *overlaySceneGraph)
    : m_state (state),
      m_sceneGraph (sceneGraph),
      m_overlaySceneGraph (overlaySceneGraph)
{
    ASSERT (state);
    ASSERT (sceneGraph);
    ASSERT (overlaySceneGraph);
    m_sceneGraph->ref ();
    m_overlaySceneGraph->ref ();    
    state->put (s_key, this);
}

ISpySceneGraphService::~ISpySceneGraphService (void) 
{
    ASSERT (m_state);
    ASSERT (m_sceneGraph);
    ASSERT (m_overlaySceneGraph);
    m_sceneGraph->unref ();
    m_overlaySceneGraph->unref ();    
    m_state->detach (s_key);
}

SoNode *
ISpySceneGraphService::sceneGraph (void) 
{ return m_sceneGraph; }

SoNode *
ISpySceneGraphService::overlaySceneGraph (void) 
{ return m_overlaySceneGraph; }
