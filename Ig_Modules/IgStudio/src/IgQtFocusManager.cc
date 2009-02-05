//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgStudio/interface/IgQtFocusManager.h"
#include "Ig_Modules/IgStudio/interface/IgView.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppContextService.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgState.h"
#include <qwidget.h>
#undef emit // zap qt macro
#include <iomanip>
#include <map>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQtFocusManager::IgQtFocusManager (IgState *state, std::map<QWidget *, IgView *> *views)
    : m_state (state),
      m_views (views),
      m_oldView (0)
{    
}

void
IgQtFocusManager::windowActivated (QWidget *widget)
{    
    IgView *selectedView = (*m_views)[widget];
    IgState *newState = selectedView ? selectedView->state () : 0;
    IgState *oldState = m_oldView ? m_oldView->state () : 0;
    

    if (oldState)
    {
	IgQtAppContextService *contextService = IgQtAppContextService::get (oldState);
	contextService->focusOut ();
    }
    
    if (newState)
    {
	IgQtAppContextService *contextService = IgQtAppContextService::get (newState);
	contextService->focusIn ();    
    }
    
    m_oldView = selectedView;    
}
