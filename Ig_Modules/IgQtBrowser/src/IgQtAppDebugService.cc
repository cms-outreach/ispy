//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtBrowser/interface/IgQtAppDebugService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppDebug.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppMenuService.h"
#include <classlib/utils/DebugAids.h>
#include <qmenubar.h>
#include <qpopupmenu.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgQtAppDebugService, "Services/Qt/App Debug");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQtAppDebugService::IgQtAppDebugService (IgState *state,
					  QWidget *mainWindow)
    : m_state (state),
      m_mainWindow (mainWindow),
      m_info (new IgQtAppDebug (state, mainWindow))
{
    ASSERT (state);
    ASSERT (mainWindow);
    state->put (s_key, this);

    ASSERT (IgQtAppMenuService::get (state));
    QMenuBar	*menu = IgQtAppMenuService::get (state)->menuBar ();
    QPopupMenu	*debug = new QPopupMenu (mainWindow);

    debug->setCaption ("Debug");
    debug->insertTearOffHandle ();
    debug->insertItem
	("Show &Resources", m_info, SLOT(resources()), 0,
	 MENU_ID_DEBUG_RESOURCES, IgQtAppMenuService::index
	 (debug, MENU_ID_DEBUG_RESOURCES));
    debug->insertItem
	("Show &Libraries", m_info, SLOT(libraries()), 0,
	 MENU_ID_DEBUG_LIBRARIES, IgQtAppMenuService::index
	 (debug, MENU_ID_DEBUG_LIBRARIES));
    debug->insertItem
	("Show &Plugins", m_info, SLOT(plugins()), 0,
	 MENU_ID_DEBUG_PLUGINS, IgQtAppMenuService::index
	 (debug, MENU_ID_DEBUG_PLUGINS));
    debug->insertItem
        ("&Show State Tree", m_info, SLOT(dumpState()), 0,
         MENU_ID_DEBUG_DUMPSTATE,  IgQtAppMenuService::index
         (debug, MENU_ID_DEBUG_DUMPSTATE));
    // debug->insertItem
    //     ("Show &State", m_info, SLOT(state()), 0,
    //	    MENU_ID_DEBUG_STATE, IgQtAppMenuService::index
    //      (debug, MENU_ID_DEBUG_STATE));
    // debug->insertItem
    //     ("Show &Pools", this, SLOT(pools()), 0,
    //      MENU_ID_DEBUG_POOLS,  IgQtAppMenuService::index
    //      (debug, MENU_ID_DEBUG_POOLS));
    // debug->insertItem
    //     ("Show &Crash", this, SLOT(crash()), 0,
    //      MENU_ID_DEBUG_CRASH,  IgQtAppMenuService::index
    //      (debug, MENU_ID_DEBUG_CRASH));
    debug->insertItem
        ("&Debug Me Now", m_info, SLOT(debugme()), 0,
         MENU_ID_DEBUG_DEBUGME,  IgQtAppMenuService::index
         (debug, MENU_ID_DEBUG_DEBUGME));

    debug->insertSeparator (IgQtAppMenuService::index 
                            (debug, MENU_ID_DEBUG_PROFILER_SEP));

    debug->insertItem
        ("Show &Profiler", m_info, SLOT(showProfiler()), 0,
         MENU_ID_DEBUG_PROFILER,  IgQtAppMenuService::index
         (debug, MENU_ID_DEBUG_PROFILER));
    
    menu->insertItem ("&Debug", debug, MENU_ID_DEBUG,
		      IgQtAppMenuService::index (menu, MENU_ID_DEBUG));
}

IgQtAppDebugService::~IgQtAppDebugService (void)
{
    ASSERT (m_state);
    ASSERT (m_mainWindow);
    m_state->detach (s_key);
}
