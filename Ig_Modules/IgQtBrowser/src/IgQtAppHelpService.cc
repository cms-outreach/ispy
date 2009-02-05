#include "Ig_Modules/IgQtBrowser/interface/IgQtAppHelpService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppHelp.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppMenuService.h"
#include "Ig_Extensions/IgIcons/interface/iguana-small.xpm"
#include <qmenubar.h>
#include <qpopupmenu.h>

IG_DEFINE_STATE_ELEMENT (IgQtAppHelpService, "Services/Qt/App Help");

IgQtAppHelpService::IgQtAppHelpService (IgState *state,
			                QWidget *mainWindow)
    : m_state (state),
      m_mainWindow (mainWindow),
      m_about (new IgQtAppHelp (state, mainWindow))
{
    ASSERT (state);
    ASSERT (mainWindow);
    state->put (s_key, this);

    ASSERT (IgQtAppMenuService::get (state));
    QMenuBar	*menu = IgQtAppMenuService::get (state)->menuBar ();
    QPopupMenu	*help = new QPopupMenu (mainWindow);

    help->setCaption ("Help");
    help->insertTearOffHandle ();
    help->insertItem
	("IGUANA &Web site", m_about, SLOT (iguanaWebSite ()), Qt::Key_F1,
	 MENU_ID_HELP_IGUANA_WEB, IgQtAppMenuService::index
	 (help, MENU_ID_HELP_IGUANA_WEB));
    help->insertSeparator ();  
    help->insertItem
	("&Report Bug...", m_about, SLOT (reportBugDialog ()), 0,
	 MENU_ID_HELP_REPORT_BUG, IgQtAppMenuService::index
	 (help, MENU_ID_HELP_REPORT_BUG));
    help->insertSeparator ();  
    help->insertItem
	("What's &This", m_about, SLOT (whatsThis ()), Qt::SHIFT+Qt::Key_F1,
	 MENU_ID_HELP_WHATSTHIS, IgQtAppMenuService::index
	 (help, MENU_ID_HELP_WHATSTHIS));
    help->insertItem
	("Viewer &Buttons", m_about, SLOT (buttons ()), 0,
	 MENU_ID_HELP_BUTTONS, IgQtAppMenuService::index
	 (help, MENU_ID_HELP_BUTTONS));
    help->insertSeparator ();  
    help->insertItem
	("About Qt", m_about, SLOT (aboutQt ()), 0,
	 MENU_ID_HELP_ABOUT_QT, IgQtAppMenuService::index
	 (help, MENU_ID_HELP_ABOUT_QT));
    help->insertItem
	(QPixmap (iguana_small_xpm), "&About IGUANA", m_about, SLOT (about ()), 0,
	 MENU_ID_HELP_ABOUT, IgQtAppMenuService::index
	 (help, MENU_ID_HELP_ABOUT));

    menu->insertItem ("&Help", help, MENU_ID_HELP,
		      IgQtAppMenuService::index (menu, MENU_ID_HELP));
}

IgQtAppHelpService::~IgQtAppHelpService (void)
{
    ASSERT (m_state);
    ASSERT (m_mainWindow);
    m_state->detach (s_key);
}

