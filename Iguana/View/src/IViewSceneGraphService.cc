//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/View/interface/IViewSceneGraphService.h"
#include <classlib/utils/DebugAids.h>
#include <Inventor/nodes/SoNodes.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IViewSceneGraphService, "Services/Coin/Scene Graph");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IViewSceneGraphService::IViewSceneGraphService (IgState *state, SoNode *sceneGraph, SoNode *overlaySceneGraph)
    : m_state (state),
      m_sceneGraph (sceneGraph),
      m_overlaySceneGraph (overlaySceneGraph)
{
    ASSERT (state);
    ASSERT (sceneGraph);
    ASSERT (overlaySceneGraph);
    state->put (s_key, this);
}

IViewSceneGraphService::~IViewSceneGraphService (void) 
{
    ASSERT (m_state);
    ASSERT (m_sceneGraph);
    ASSERT (m_overlaySceneGraph);
    m_sceneGraph->unref ();
    m_overlaySceneGraph->unref ();
    m_state->detach (s_key);
}

SoNode *
IViewSceneGraphService::sceneGraph (void) 
{ return m_sceneGraph; }

SoNode *
IViewSceneGraphService::overlaySceneGraph (void) 
{ return m_overlaySceneGraph; }
