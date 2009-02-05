//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtProfileService/interface/IgQtProfileService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppMenuService.h"

#include "Ig_Modules/IgQtProfileService/src/IgOProfMainWindow.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfConfiguration.h"

#include <classlib/utils/DebugAids.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qapplication.h>

#include <iostream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgQtProfileService, "Services/Qt/Profile");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQtProfileService::IgQtProfileService (IgState *state,
					QMenuBar *menuBar,
					QWidget *parent,
					QStatusBar *statusBar,
					bool inDialog)
    :m_state (state),
     m_menuBar (menuBar),
     m_parent (parent),
     m_statusBar(statusBar),
     m_config( IgOProfConfigurationSingleton::instance() ),
     m_profiler (new IgOProfMainWindow (m_menuBar, 
					m_parent, 
					m_statusBar, 
					inDialog))
{	
    ASSERT (state);
    state->put (s_key, this);
    m_profiler->createGUI (! m_profiler->init ());	
}

IgQtProfileService::~IgQtProfileService (void)
{
    ASSERT (m_state);
    ASSERT (m_parent);
    m_state->detach (s_key);
}
