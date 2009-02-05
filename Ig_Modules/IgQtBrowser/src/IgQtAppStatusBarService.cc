//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtBrowser/interface/IgQtAppStatusBarService.h"
#include <qapplication.h>
#include <qmainwindow.h>
#include <qstatusbar.h>
#undef emit // zap qt macro
#include <classlib/utils/DebugAids.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgQtAppStatusBarService, "Services/Qt/Status Bar");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQtAppStatusBarService::IgQtAppStatusBarService (IgState *state,
						  QWidget *mainWindow)
    : m_state (state),
      m_mainWindow (dynamic_cast<QMainWindow *> (mainWindow))
{
    ASSERT (state);
    ASSERT (mainWindow);
    ASSERT (m_mainWindow);
    state->put (s_key, this);

    QStatusBar *sbar = m_mainWindow->statusBar ();
    sbar->show ();
}

IgQtAppStatusBarService::~IgQtAppStatusBarService (void)
{
    ASSERT (m_state);
    ASSERT (m_mainWindow);
    m_state->detach (s_key);
}

void
IgQtAppStatusBarService::setMessage (QString message)
{
    m_mainWindow->statusBar ()->message (message);
    qApp->syncX ();
}

void
IgQtAppStatusBarService::setMessage (const std::string &message)
{
    m_mainWindow->statusBar ()->message (message.c_str ());
    qApp->syncX ();
}

void
IgQtAppStatusBarService::setMessage (const char *message)
{
    m_mainWindow->statusBar ()->message (message);
    qApp->syncX ();
}
