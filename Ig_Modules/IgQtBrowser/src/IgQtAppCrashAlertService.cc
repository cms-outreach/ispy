//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtBrowser/interface/IgQtAppCrashAlertService.h"
#include <classlib/utils/DebugAids.h>
#include <classlib/utils/Signal.h>
#include <algorithm>
#include <qwidget.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgQtAppCrashAlertService, "Services/Qt/Crash Alert");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQtAppCrashAlertService::IgQtAppCrashAlertService (IgState *state,
						    QWidget *mainWindow)
    : m_state (state),
      m_mainWindow (mainWindow),
      m_popup (0)
{
    ASSERT (state);
    ASSERT (mainWindow);

    m_state->put (s_key, this);

    // FIXME: Install crash handlers, main loop return etc.  For more
    // comments, see comments in beginning of `iguana'.

    // FIXME: Hook into some existing base application crash service
    // already installed by `iguana'.  The unwind mechanism should
    // come from there, and we only put up an alert here.

    // FIXME: Create a popup dialog for the crash alert.  On crash,
    // just bring it up.
}

IgQtAppCrashAlertService::~IgQtAppCrashAlertService (void)
{
    ASSERT (m_state);
    ASSERT (m_mainWindow);
    // ASSERT (m_popup);
    m_state->detach (s_key);
    delete m_popup;
}

bool
IgQtAppCrashAlertService::hasCrashed (void)
{
    return lat::Signal::crashed ();
}

void
IgQtAppCrashAlertService::addUnwind (lat::Callback action)
{
    m_unwinds.push_front (action);
}

void
IgQtAppCrashAlertService::removeUnwind (lat::Callback action)
{
    m_unwinds.erase (std::find (m_unwinds.begin (), m_unwinds.end (), action));
}
