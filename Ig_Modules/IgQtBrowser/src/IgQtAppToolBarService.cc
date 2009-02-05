//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtBrowser/interface/IgQtAppToolBarService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppContextService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtCallbacks.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgState.h"
#include <classlib/utils/DebugAids.h>
#include <qtoolbutton.h>
#include <qmainwindow.h>
#include <qtoolbar.h>
#include <qiconset.h>
#include <qtooltip.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgQtAppToolBarService, "Services/Qt/Tool Bar");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQtAppToolBarService::IgQtAppToolBarService (IgState *state,
					      QWidget *mainWindow)
    : m_state (state),
      m_mainWindow (mainWindow)
{
    ASSERT (state);
    ASSERT (mainWindow);
    state->put (s_key, this);
}

IgQtAppToolBarService::~IgQtAppToolBarService (void)
{
    ASSERT (m_state);
    ASSERT (m_mainWindow);
    m_state->detach (s_key);
}

QToolBar *
IgQtAppToolBarService::toolBar (const int id,
				const std::string &label)
{
    if (m_toolBarMap[id] == 0)
    {
	QToolBar *bar = new QToolBar (static_cast<QMainWindow *>(m_mainWindow),
				      label.c_str ());
	bar->setCaption (label.c_str ());
	m_toolBarMap[id] = bar;	
    }
    return m_toolBarMap[id];    
}



QToolButton *
IgQtAppToolBarService::toolBarButton (IgState *state,
				      const int barId,
				      const int buttonId,
				      const QIconSet &iconSet,
				      const std::string &name,
				      QObject *target,
				      const char *slot,
				      const char *signal)
{
    std::string realSignal (signal);
    std::string realSlot (slot);
    
    if (realSignal == "")
      realSignal  = SIGNAL (clicked ());

    // FIXME: toolbar must already exists otherwise the name is set to
    // ""    
    QToolBar *toolBar = this->toolBar (barId, std::string (""));

    // FIXME: Ugly way of having unique ids...  
    QToolButton *button = 0;
    if (m_toolButtonMap[barId * 10000 + buttonId] == 0)
    {
	button = new QToolButton (toolBar);
	button->setIconSet (iconSet);
	
	m_toolButtonMap[barId * 10000 + buttonId] = button;
	QToolTip::add (button, name.c_str ());
    }
    else
      button = m_toolButtonMap[barId * 10000 + buttonId];
    
    IgQtAppContextService *service = IgQtAppContextService::get (state);
    if (realSlot != "")
    {
	service->add (qtConnectionCallback, 
		      createQtConnection (button, 
					  realSignal.c_str (), 
					  target,
					  slot));
        service->add (qtShowHideCallback, button); 
    }
    else
      service->add (qtShowHideCallback, button, false); 

    return button;
}

QToolButton *
IgQtAppToolBarService::getToolBarButton (const int barId,
				         const int buttonId)
{
    if (m_toolButtonMap.find(barId * 10000 + buttonId) !=
        m_toolButtonMap.end ())
      return m_toolButtonMap [barId * 10000 + buttonId];
    else
      return 0;
}
