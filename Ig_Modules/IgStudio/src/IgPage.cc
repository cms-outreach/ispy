//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgStudio/interface/IgPage.h"
#include "Ig_Modules/IgStudio/interface/IgDocument.h"
#include "Ig_Modules/IgStudio/interface/IgView.h"
#include "Ig_Modules/IgStudio/interface/IgViewDB.h"
#include "Ig_Modules/IgStudio/interface/IgViewCreator.h"
#include "Ig_Modules/IgStudio/interface/IgQtFocusManager.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQDockAreaSite.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQDockWindowSite.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQVSplitterSite.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQWorkspaceSite.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQWidgetSite.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQWidgetStackSite.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppMenuService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtSite.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtSiteStack.h"
#include <qwidget.h>
#include <qlayout.h>
#include <qpopupmenu.h>
#include <qmenubar.h>

#undef emit // zap qt macro
#include <classlib/utils/DebugAids.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>


IgQtSiteStack *
createWorkspace (IgState *state, IgSite *site)
{
    return new IgQWorkspaceSite (state, site);    
}


IgPage::IgPage (IgDocument *doc)
    : IgQHSplitterSite (doc->state (), doc->window ()),
      m_document (doc),
      m_leftDockArea (new IgQDockAreaSite (doc->state (), this)),
      m_stackArea (new IgQWidgetStackSite (doc->state (), new IgQDockWindowSite (doc->state (), m_leftDockArea))),
      m_centralSplitter (new IgQVSplitterSite (doc->state (), this)),
      m_workspace (createWorkspace (doc->state (), m_centralSplitter)),
      m_bottomDockArea (new IgQDockAreaSite (doc->state (), m_centralSplitter)),
      m_rightDockArea (new IgQDockAreaSite (doc->state (), this)),
      m_numRegisteredWindows (0)
{
    // Construct a grid layout for holding contents
    // m_layout = new QGridLayout (self ());
    // m_layout->setAutoAdd (true);

    // FIXME: register in document
    // FIXME: read, write support
    ASSERT (doc);    
    doc->addPage (this);    
    
    IgQtAppMenuService *service = IgQtAppMenuService::get (doc->state ());
    
    QMenuBar *bar = service->menuBar ();    
    
    // Here we populate the file menu with the "New" entry to allow user to
    // create new views of the different models
    QPopupMenu *fileMenu = service->subMenu (IgQtAppMenuService::MENU_ID_FILE);
    
    ASSERT (fileMenu);
    
    QPopupMenu *newViewMenu = new QPopupMenu (bar);
    newViewMenu->setCaption ("New");
    fileMenu->insertItem ("&New", newViewMenu, MENU_ID_FILE_NEW,
			  IgQtAppMenuService::index (fileMenu, MENU_ID_FILE_NEW));
    fileMenu->insertSeparator (IgQtAppMenuService::index (fileMenu, MENU_ID_FILE_NEW) + 1 );
    newViewMenu->insertTearOffHandle ();
    
    IgViewDB *viewDB = IgViewDB::get ();
    ASSERT (viewDB);
    
    // FIXME: sometimes, especially after rebuilding, viewDB is
    // empty...We need to find some better way to get notified when a
    // new IgView plugin is loaded.
    for (IgViewDB::Iterator i = viewDB->begin ();
	 i != viewDB->end ();
	 i++)
    {	
	IgViewCreator *viewCreator = new IgViewCreator (i, this);
	newViewMenu->insertItem ((*i)->name (), 
				 viewCreator,
				 SLOT (createView ()),
				 0,
				 IgQtAppMenuService::index (newViewMenu,
							    MENU_ID_FILE_NEW));
    }
    
    // Here we create the "Window" menu in which the user can decide
    // to hide/show some utility views (for example the python shell
    // should go here...)
    //
    // FIXME: how do we distinguish between different kind of Views?
    // By having two, separate, plugin categories?
    
    QPopupMenu *windowMenu = new QPopupMenu (bar);
    windowMenu->setCaption ("Window");
    bar->insertItem ("&Window", windowMenu, MENU_ID_WINDOW,
		     IgQtAppMenuService::index (bar, MENU_ID_WINDOW));

    windowMenu->insertTearOffHandle ();
    windowMenu->insertItem ("&Tile", 
			    IgQtSite::selfFrom (m_workspace),
			    SLOT (tile ()),
			    0,
			    MENU_ID_WINDOW_TILE,
			    IgQtAppMenuService::index (bar, MENU_ID_WINDOW));
    windowMenu->insertItem ("&Cascade", 
			    IgQtSite::selfFrom (m_workspace),
			    SLOT (cascade ()),
			    0,
			    MENU_ID_WINDOW_CASCADE,
			    IgQtAppMenuService::index (bar, MENU_ID_WINDOW));

    windowMenu->insertSeparator ();
		
    // Get notified by window selection changes
    //    connect (IgQtSite::selfFrom (m_workspace), SIGNAL (windowActivated (QWidget *)),
    //	     this, SLOT (windowActivated (QWidget *)));
    IgQtFocusManager *focusManager = new IgQtFocusManager (state (), &m_views);
    
    QObject::connect (IgQtSite::selfFrom (m_workspace),
		      SIGNAL (windowActivated (QWidget *)),
		      focusManager,
		      SLOT (windowActivated (QWidget *)));
    
    self ()->show ();
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
IgState *
IgPage::state (void) const
{ return m_document->state (); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
IgQtSiteStack *
IgPage::workspace (void)
{
    return m_workspace;
}

IgQtSite *
IgPage::leftDockArea (void)
{
    return m_leftDockArea;
}

IgQtSite *
IgPage::rightDockArea (void)
{
    return m_rightDockArea;
}

IgQtSite *
IgPage::bottomDockArea (void)
{
    return m_bottomDockArea;
}

IgQtSiteStack *
IgPage::stackArea (void)
{
    return m_stackArea;
}

void
IgPage::registerView (IgView *view, QWidget *selectableWidget)
{
    ASSERT (m_views[selectableWidget] == 0);
    m_views[selectableWidget] = view;
    
    // FIXME: maybe we should also introduce different kind of views,
    // so that a view can have a title, without having to be
    // registered in the window menu.
    if (view->title () != "")
    {
	IgQtAppMenuService *ms = IgQtAppMenuService::get (state ());
    
	QPopupMenu *menu = ms->subMenu (MENU_ID_WINDOW);
	int itemID = MENU_ID_WINDOW_LIST_OFFSET + m_numRegisteredWindows++;
    
	menu->insertItem (view->title (), 
			  selectableWidget,
			  SLOT (show ()),
			  0,
			  itemID,
			  IgQtAppMenuService::index (menu,
			    MENU_ID_WINDOW_LIST_OFFSET));
	menu->connectItem (itemID, selectableWidget, SLOT (setFocus()));
    }
}
