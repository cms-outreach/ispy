//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtBrowser/interface/IgQtAppLoopService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtScheduledEventExecutor.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtScheduledEvent.h"
#include <classlib/utils/DebugAids.h>
#include <qapplication.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgQtAppLoopService, "Services/Qt/Main Loop");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQtAppLoopService::IgQtAppLoopService (IgState *state)
    : m_state (state),
      m_run (lat::CreateCallback (&defaultRunAction)),
      m_running (false),
      m_executor (new IgQtScheduledEventExecutor ())
{
    ASSERT (state);
    state->put (s_key, this);
}

IgQtAppLoopService::~IgQtAppLoopService (void)
{
    ASSERT (m_state);
    m_state->detach (s_key);
    m_executor->deleteLater ();    
}

void
IgQtAppLoopService::prepare (lat::Callback action)
{
    ASSERT (action);
    m_preparations.push_back (action);
}

void
IgQtAppLoopService::schedule (lat::Callback action)
{
    ASSERT (action);
    QEvent *event = new IgQtScheduledEvent (action);
    QApplication::postEvent (m_executor, event);    
}



void
IgQtAppLoopService::post (lat::Callback action)
{
    ASSERT (action);
    m_post.push_back (action);
}

void
IgQtAppLoopService::recover (lat::Callback action)
{
    ASSERT (action);
    m_recoveries.push_back (action);
}

void
IgQtAppLoopService::override (lat::Callback action)
{
    ASSERT (action);
    m_run = action;
}

void
IgQtAppLoopService::run (void)
{
    doPrepare ();
    doRun ();
    doPost ();
}

bool
IgQtAppLoopService::running (void)
{
    return m_running;
}

void
IgQtAppLoopService::resume (void)
{
    // FIXME: Or siglongjmp?  See doRun for comments.
    longjmp (m_return, 1);
}

void
IgQtAppLoopService::doPrepare (void)
{
    std::list<lat::Callback>::iterator i = m_preparations.begin ();
    for ( ; i != m_preparations.end (); ++i)
	(*i) ();
}

void
IgQtAppLoopService::doRun (void)
{
    // FIXME: Use sigsetjmp instead of setjmp to restore signal
    // blocking masks?  But then we'll have to know about all that
    // stuff...  but then again, we'll have to know about restoring
    // signal masks otherwise.  See DDD for code.
    if (setjmp (m_return))
    {
	m_running = false;
	// FIXME: do recoveries as detailed in `iguana'
	doResume ();
    }

    ASSERT (m_run);
    m_running = true;
    m_run ();
}

void
IgQtAppLoopService::doResume (void)
{
    std::list<lat::Callback>::iterator i = m_recoveries.begin ();
    for ( ; i != m_recoveries.end (); ++i)
	(*i) ();
}

void
IgQtAppLoopService::doPost (void)
{
    std::list<lat::Callback>::iterator i = m_post.begin ();
    for ( ; i != m_post.end (); ++i)
	(*i) ();
}

void
IgQtAppLoopService::defaultRunAction (void)
{
    qApp->exec ();
}
