//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtPython/interface/IgPythonView.h"
#include "Ig_Modules/IgQtPython/interface/IgQtPythonShell.h"
#include "Ig_Modules/IgPython/interface/IgPythonState.h"
#include "Ig_Modules/IgPython/interface/IgPythonService.h"
#include "Ig_Modules/IgStudio/interface/IgPage.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQWidgetSite.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQDockWindowSite.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppMenuService.h"
#include "Ig_Modules/IgQtPython/interface/IgQtPythonLoadMenu.h"
#include <classlib/utils/DebugAids.h>
#include <qwidget.h>
#include <qmenubar.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
IgQtSite		*IgPythonView::m_site = 0;
IgQtPythonShell 	*IgPythonView::m_shell = 0;    

//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgPythonView::IgPythonView (IgPage *page)
    : IgView (page)
{
    if (!m_site)
    {
	m_site = new IgQDockWindowSite (page->state (), 
					page->bottomDockArea ());
	
	IgPythonService *ps = IgPythonService::get (page->state ());
	if (!ps)
	{
	    ps = new IgPythonService (page->state ());
	}
	
	IgPythonStream *stream = ps->pythonStream ();
	
	m_shell = new IgQtPythonShell (IgQtSite::hostFrom (m_site),
				       stream);	
	IgQtSite::host (m_site, m_shell);

	// FIXME: argh! this is **evil**, but a proper solution would
	// require to split up IgPython or move it to IgObjectBrowser.
	IgPythonState::set (page->state ());
	
	// Creating menu for the scripts.
        m_loadMenu = new IgQtPythonLoadMenu (page->state ());

	IgQtAppMenuService *menuService = 
	    IgQtAppMenuService::get (page->state ());
	ASSERT (menuService);

	QMenuBar *bar = IgQtAppMenuService::get (page->state ())->menuBar ();    
	ASSERT (bar);
	
	QPopupMenu *menu = menuService->subMenu (IgPythonView::MENU_ID_SCRIPTS);    

	if (!menu)
	{
	    menu = new QPopupMenu (bar);
	    menu->setCaption ("Scripts");
	    menu->insertTearOffHandle ();
	    bar->insertItem ("&Scripts", menu, IgPythonView::MENU_ID_SCRIPTS,
			     IgQtAppMenuService::index (bar, 
							IgPythonView::MENU_ID_SCRIPTS));
	}

	if (IgQtAppMenuService::isIdThere (menu, 
					   IgPythonView::MENU_ID_SCRIPTS_LOAD) == -1)
	{
	    // Miscellaneous controls
	    menu->insertItem
		("&Load and execute", 
		 m_loadMenu, SLOT (slotPythonLoader ()), 0,
		 IgPythonView::MENU_ID_SCRIPTS_LOAD, 
		 IgQtAppMenuService::index (menu, 
					    IgPythonView::MENU_ID_SCRIPTS_LOAD));
	}
    }
}
