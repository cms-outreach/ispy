//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/View/interface/IViewQWindowService.h"
#include <classlib/utils/DebugAids.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IViewQWindowService, "Services/Qt/Main Window");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

using namespace SIM::Coin3D::Quarter;

IViewQWindowService::IViewQWindowService (IgState *state, QWidget *mainWindow, QuarterWidget *viewer,
					  SoRenderManager::Superimposition *super)
    : m_state (state),
      m_mainWindow (mainWindow),
      m_viewer (viewer),
      m_super (super)
{
    ASSERT (state);
    ASSERT (mainWindow);
    ASSERT (viewer);
    ASSERT (super);
    state->put (s_key, this);
}

IViewQWindowService::~IViewQWindowService (void)
{
    ASSERT (m_state);
    ASSERT (m_mainWindow);
    ASSERT (m_viewer);
    m_state->detach (s_key);
}

QWidget *
IViewQWindowService::mainWindow (void)
{ return m_mainWindow; }

QuarterWidget *
IViewQWindowService::viewer (void)
{ return m_viewer; }

SoRenderManager::Superimposition *
IViewQWindowService::super (void)
{ return m_super; }

void
IViewQWindowService::super (SoRenderManager::Superimposition *super)
{
    m_viewer->getSoRenderManager()->removeSuperimposition (m_super);
    m_super = super; 
}
