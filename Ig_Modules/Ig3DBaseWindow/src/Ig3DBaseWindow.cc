//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DBaseWindow/interface/Ig3DBaseWindow.h"
#include "Ig_Modules/IgStudio/interface/IgModelManager.h"
#include "Ig_Modules/IgStudio/interface/IgDocumentDataManager.h"
#include "Ig_Modules/Ig3DModel/interface/Ig3DModel.h"
#include "Ig_Modules/Ig3DSystem/interface/Ig3DSystem.h"
#include "Ig_Modules/IgStudio/interface/IgDocumentData.h"
#include "Ig_Modules/IgStudio/interface/IgPage.h"
#include "Ig_Modules/IgCommonData/interface/IgTwig.h"
#include "Ig_Modules/Ig3DBaseBrowser/interface/Ig3DBaseBrowser.h"
#include "Ig_Modules/Ig3DBaseWindow/interface/IgCommonViewPropertiesCategory.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtSite.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtSiteStack.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppMenuService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppWindowService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppContextService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppToolBarService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppImageService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtObjectMenuService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtObjectMenuMessage.h"
#include "Ig_Modules/IgControlCentre/interface/IgStudioCentre.h"
#include "Ig_Modules/IgControlCentre/interface/IgControlCentre.h"
#include "Ig_Extensions/IgQt/interface/IgQtWorkspace.h"
#include "Ig_Extensions/IgIcons/interface/IgImageIDs.h"
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qmenudata.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qwhatsthis.h>
#include <qbuttongroup.h>
#include <qmainwindow.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>


static void autoinit (IgState *state, IgSite *site)
{ Ig3DSystem::init (state, IgQtSite::selfFrom (site)->topLevelWidget ()); }

static Ig3DModel *getModelFromManager (IgPage *page)
{
    autoinit (page->state (), page->workspace ());
    
    IgModelManager *manager = IgModelManager::get (page->state ());
    if (!manager)
	manager = new IgModelManager (page->state ());
    
    Ig3DModel *model = 0;
    manager->lookup (page->state (), &model);
    return model;
}

Ig3DBaseWindow::Ig3DBaseWindow (IgPage *page)
    : IgView (page),
      m_site (page->workspace ()),
      m_browser (0)
{ 
     m_model = getModelFromManager (page);
     m_state = new IgState (IgDocumentDataManager::get(page->state ())->
                            lookup (page->state ()));
     m_centre = IgStudioCentre::get (m_state)->controlCentre ();
     
    // This is the context for this window 
    new IgQtAppContextService (m_state,
			       IgQtAppWindowService::get (m_state)->mainWindow ());    
}

void
Ig3DBaseWindow::watchBrowser (Ig3DBaseBrowser *browser)
{
    m_browser = browser;
    // Show myself and show all the document data (FIXME: select?).
    QString t (IgDocumentDataRoot::getCurrentRoot());
    t += QString(" ( ") + title ().c_str () + " )";
		    
    m_browser->getWidget ()->parentWidget ()->setCaption (t);
    setTitle (t.latin1 ());
    
    browse (IgDocumentData::get (m_state)->root ());
    // register the MDI window used by this view.
    page ()->registerView (dynamic_cast <IgView *> (this), 
			   m_browser->getWidget ()->parentWidget ());
}

void
Ig3DBaseWindow::initCategories (void)
{
    // Make sure this is called only once.
    // Create a 3D View Properties Category and add it in control
    // centre.
    
    m_viewPropertiesCategory = IgCommonViewPropertiesCategory::get (page ()->state ());
    if (!m_viewPropertiesCategory)
    {
	// Category is added to the global state.
	m_viewPropertiesCategory = 
	    new IgCommonViewPropertiesCategory (page ()->state ());
    }
    m_viewPropertiesCategory->registerBrowser (m_state, browser ());
}

void 
Ig3DBaseWindow::initMenu (void)
{
    ASSERT (m_state);    
    // Initialise menus. FIXME: share a global one with all browsers?
    // need to maintain a 3D context service...  This needs to come
    // after we've set scene graph so that camera exists.

    // FIXME! get main window from somewhere
    // FIXME: deal with absence of main window!

    ASSERT (IgQtAppMenuService::get (m_state));
    QMenuBar *bar = IgQtAppMenuService::get (m_state)->menuBar ();    
    
    IgQtAppMenuService *menuService = IgQtAppMenuService::get (m_state);

    QPopupMenu *menu = menuService->subMenu (Ig3DBaseBrowser::MENU_ID_3D);
    
    if (!menu)
    {
	menu = new QPopupMenu (m_browser->getShellWidget ());
	menu->setCaption ("View");
	menu->insertTearOffHandle ();
	bar->insertItem ("&View", menu, Ig3DBaseBrowser::MENU_ID_3D,
			 IgQtAppMenuService::index (bar, Ig3DBaseBrowser::MENU_ID_3D));
    }
    
    menu->setCheckable(true);
    // FIXME: also drag/drop!
    //menu->insertItem ("&Open...", this, SLOT(open()));
    
    if (IgQtAppMenuService::isIdThere (menu, Ig3DBaseBrowser::MENU_ID_3D_TOOLBAR) == -1)
    {
	// Miscellaneous controls
	menu->insertItem
	    ("&Tool Bar", 
	     this, SLOT (showToolbar ()), 0,
	     Ig3DBaseBrowser::MENU_ID_3D_TOOLBAR, 
	     IgQtAppMenuService::index (menu, Ig3DBaseBrowser::MENU_ID_3D_TOOLBAR));
	menu->setItemChecked (Ig3DBaseBrowser::MENU_ID_3D_TOOLBAR, true);
	menu->insertSeparator (IgQtAppMenuService::index (
	                        menu, Ig3DBaseBrowser::MENU_ID_3D_TOOLBAR_SEP));
        
	QToolBar *tb = IgQtAppToolBarService::get (m_state)
	   ->toolBar (TOOLBAR_ID_3D2D, "General graphics ops");
	tb->setCloseMode (QDockWindow::Always);
	
	connect (tb,   SIGNAL (visibilityChanged (bool)),
             this, SLOT (toolbarVisibilityChanged (bool)));
    }

    if (IgQtAppMenuService::isIdThere (menu, Ig3DBaseBrowser::MENU_ID_3D_VIEW_PROPERTIES) == -1)
    {
	// Miscellaneous controls
	menu->insertItem
	    ("&Print/View Settings...", 
	     this, SLOT (showViewProperties ()), 0,
	     Ig3DBaseBrowser::MENU_ID_3D_VIEW_PROPERTIES, 
	     IgQtAppMenuService::index (menu, Ig3DBaseBrowser::MENU_ID_3D_VIEW_PROPERTIES));
    }

    menu = menuService->subMenu (IgQtAppMenuService::MENU_ID_FILE);

    ASSERT (m_browser);
    
    IgQtAppMenuService::insertItem (menu,
				    "&Save As...",
				    m_browser,
				    SLOT(save()),
				    IgQtAppMenuService::MENU_ID_FILE_SAVE,
				    m_state);    

    IgQtAppMenuService::insertItem (menu,
				    "&Print As...", 
				    m_browser,
				    SLOT(print()),
				    IgQtAppMenuService::MENU_ID_FILE_PRINT,
				    m_state);

    // Register the message broadcasting only when the window is
    // focused, unregister on focus out.
    ASSERT (IgQtAppContextService::get (state ()));
    IgQtAppContextService *cs = IgQtAppContextService::get (state ());
    cs->addOnFocusIn (lat::CreateCallback (this, &Ig3DBaseWindow::focusIn));
    cs->addOnFocusOut (lat::CreateCallback (this, &Ig3DBaseWindow::focusOut));
}

void
Ig3DBaseWindow::viewingModeCallback (void)
{
    IgQtAppToolBarService *ts = IgQtAppToolBarService::get (state ());
    ASSERT (ts);
    
    QToolButton *vb = ts->getToolBarButton (TOOLBAR_ID_3D2D, TOOLBAR_ID_3D2D_VIEW);
    QToolButton *pb = ts->getToolBarButton (TOOLBAR_ID_3D2D, TOOLBAR_ID_3D2D_PICK);
    QToolButton *sb = ts->getToolBarButton (TOOLBAR_ID_3D2D, TOOLBAR_ID_3D2D_SEEK);
    
    bool view = m_browser->isViewing();
    vb->setOn(view);
    sb->setEnabled (view);
    pb->setOn(!view);
}

void
Ig3DBaseWindow::initToolBar (void)
{
    // This creates a toolbar with the general widgets.
    IgQtAppToolBarService *toolBarService = IgQtAppToolBarService::get (m_state);
    IgQtAppImageService *is = IgQtAppImageService::get (m_state);
    IgQtAppContextService *cs = IgQtAppContextService::get (state ());
    ASSERT (is);
    ASSERT (cs);
    
    QToolButton *button = 0;
    button = toolBarService->toolBarButton (m_state,
					    TOOLBAR_ID_3D2D,
					    TOOLBAR_ID_3D2D_SAVE_AS, 
					    *is->pixmapById (IgImageIDs::IMAGE_ID_SAVE_AS), 
					    "Save As",
					    m_browser,
					    SLOT (save ()));
    QWhatsThis::add (button, "Save scene graph.");
    
    button = toolBarService->toolBarButton (m_state,
					    TOOLBAR_ID_3D2D,
					    TOOLBAR_ID_3D2D_PRINT, 
					    *is->pixmapById (IgImageIDs::IMAGE_ID_PRINT), 
					    "Print scene graph",
					    m_browser,
					    SLOT (print ()));
    QWhatsThis::add (button, "Print scene graph.");

    button = toolBarService->toolBarButton (m_state,
					    TOOLBAR_ID_3D2D,
					    TOOLBAR_ID_3D2D_CONTROL_CENTRE, 
					    *is->pixmapById (IgImageIDs::IMAGE_ID_CONTROL_CENTRE), 
					    "Show Control Centre",
					    m_centre,
					    SLOT (show ()));
    QWhatsThis::add (button, "Show Control Centre. You can manipulate Clip "
                            "planes, slicers, lights, camera, view/print "
			    "setting via control centre.");

    button = toolBarService->toolBarButton (m_state,
					    TOOLBAR_ID_3D2D,
					    TOOLBAR_ID_3D2D_ZOOM_IN, 
					    *is->pixmapById (IgImageIDs::IMAGE_ID_ZOOM_PLUS), 
					    "Zoom in",
					    m_browser,
					    SLOT (zoomIn ()));
    QWhatsThis::add (button, "Zoom in to the scene graph");
    
    button = toolBarService->toolBarButton (m_state,
					    TOOLBAR_ID_3D2D,
					    TOOLBAR_ID_3D2D_ZOOM_OUT, 
					    *is->pixmapById (IgImageIDs::IMAGE_ID_ZOOM_MINUS),
					    "Zoom out",
					    m_browser,
					    SLOT (zoomOut ()));
    QWhatsThis::add (button, "Zoom out of the scene graph");
    
    button = toolBarService->toolBarButton (m_state,
					    TOOLBAR_ID_3D2D,
					    TOOLBAR_ID_3D2D_PICK,				 
					    *is->pixmapById (IgImageIDs::IMAGE_ID_PICK),
					    "Pick/Select mode",
					    this,
					    SLOT (pick ()));
    button->setToggleButton (true);
    button->setOn (false);
    QWhatsThis::add (button, "Switch to the object pick/select mode. "
		     "In this mode you can pick/select 3D objects in the "
		     "scene graph");
    
    button = toolBarService->toolBarButton (m_state,
					    TOOLBAR_ID_3D2D,
					    TOOLBAR_ID_3D2D_VIEW, 
					    *is->pixmapById (IgImageIDs::IMAGE_ID_VIEW),
					    "View mode",
					    this,
					    SLOT (view ()));
    button->setToggleButton (true);
    button->setOn (true);
    QWhatsThis::add (button, "Switch to the camera or viewer mode. "
		     "In this mode you can move/re-orient "
		     "the camera in 3D space.");
    
    button = toolBarService->toolBarButton (m_state,
					    TOOLBAR_ID_3D2D,
					    TOOLBAR_ID_3D2D_SEEK, 
					    *is->pixmapById (IgImageIDs::IMAGE_ID_SEEK),
					    "Seek mode",
					    m_browser,
					    SLOT (seek ()));
    QWhatsThis::add (button, "Seek towards an object. Click on the object and "
		     "camera will either jump (if Seet Time is 0) or "
		     "animate (otherwise) to this new position. Seek "
		     "time can be change via the View Setting control "
		     "category.");

    button = toolBarService->toolBarButton (m_state,
					    TOOLBAR_ID_3D2D,
					    TOOLBAR_ID_3D2D_HOME, 
					    *is->pixmapById (IgImageIDs::IMAGE_ID_HOME),
					    "Go to Home",
					    m_browser,
					    SLOT (resetToHomePosition ()));
    QWhatsThis::add (button, "Return camera to its home position or "
		     "initial one if no home position was set.");
    
    button = toolBarService->toolBarButton (m_state,
					    TOOLBAR_ID_3D2D,
					    TOOLBAR_ID_3D2D_SET_HOME,
					    *is->pixmapById (IgImageIDs::IMAGE_ID_SET_HOME),
					    "Set this as home",
					    m_browser,
					    SLOT (saveHomePosition ()));
    QWhatsThis::add (button, "Mark current camera position as Home. "
		     "You can click on Home button to come back "
		     "to this position.");
    
    button = toolBarService->toolBarButton (m_state,
					    TOOLBAR_ID_3D2D,
					    TOOLBAR_ID_3D2D_ALL, 
					    *is->pixmapById (IgImageIDs::IMAGE_ID_VIEW_ALL),
					    "View All scene",
					    m_browser,
					    SLOT (viewAll ()));
    QWhatsThis::add (button, "Adjust the view to include the entire secne graph.");
        
    button = toolBarService->toolBarButton (state (),
					    Ig3DBaseWindow::TOOLBAR_ID_3D2D,
					    Ig3DBaseWindow::TOOLBAR_ID_3D2D_YX_PLANE, 
					    *is->pixmapById (IgImageIDs::IMAGE_ID_YX_SMALL),
					    "YX plane",
					    browser (),
					    SLOT (viewPlaneZ ()));
    QWhatsThis::add (button, "Align camera along -ve Z-axis.");  

    button = toolBarService->toolBarButton (state (),
					    Ig3DBaseWindow::TOOLBAR_ID_3D2D,
					    Ig3DBaseWindow::TOOLBAR_ID_3D2D_XZ_PLANE, 
					    *is->pixmapById (IgImageIDs::IMAGE_ID_XZ_SMALL),
					    "XZ plane",
					    browser (),
					    SLOT (viewPlaneY ()));
    QWhatsThis::add (button, "Align camera along -ve Y-axis.");
    button = toolBarService->toolBarButton (state (),
					    Ig3DBaseWindow::TOOLBAR_ID_3D2D,
					    Ig3DBaseWindow::TOOLBAR_ID_3D2D_YZ_PLANE, 
					    *is->pixmapById (IgImageIDs::IMAGE_ID_YZ_SMALL),
					    "YZ plane",
					    browser (),
					    SLOT (viewPlaneX ()));
    QWhatsThis::add (button, "Align camera along +ve X-axis.");

    button = toolBarService->toolBarButton (m_state,
					    TOOLBAR_ID_3D2D,
					    TOOLBAR_ID_3D2D_AUTO_PRINT, 
					    *is->pixmapById (IgImageIDs::IMAGE_ID_AUTO_PRINT), 
					    "Auto Print",
					    m_browser,
					    SLOT (autoPrint ()));
    QWhatsThis::add (button, "Print JPEG.");

    cs->addOnFocusIn (lat::CreateCallback (this,
					   &Ig3DBaseWindow::viewingModeCallback));
}

void
Ig3DBaseWindow::showViewProperties (void)
{
    QString label (m_viewPropertiesCategory->catalogLabel ());
    m_centre->selectCategory (
	label.right (label.length ()- label.findRev ("/") - 1));
    m_centre->show ();
}

Ig3DBaseWindow::~Ig3DBaseWindow (void)
{
}

void
Ig3DBaseWindow::show (void)
{
    ASSERT (m_browser);
    m_browser->getWidget ()->parentWidget ()->show ();
    IgQtWorkspace *ws = dynamic_cast <IgQtWorkspace*>(IgQtSite::hostFrom (m_site));
    QWidget *pw = ws->activeWindow ()->parentWidget();
    if (!pw)
	pw = ws->activeWindow ();
      
    if (ws->windowList (QWorkspace::CreationOrder).count () == 1)
    {
	pw->resize (600, 600);
	pw->showMaximized ();
    }
    else if (ws)      
    {
	if (! pw->isMaximized ())
	    pw->setGeometry (0, 0, ws->width (), ws->height ());
    }
    else
	pw->showMaximized ();
}

void
Ig3DBaseWindow::hide (void)
{
    ASSERT (m_browser);
    m_browser->getWidget ()->parentWidget ()->hide ();
}

IgState *
Ig3DBaseWindow::state (void)
{
    return m_state;
}

Ig3DBaseBrowser *
Ig3DBaseWindow::browser (void)
{
    return m_browser;
}

Ig3DBaseModel *
Ig3DBaseWindow::model (void)
{
    return m_model;
}

IgControlCentre * 
Ig3DBaseWindow::centre  (void)
{ return m_centre; }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
Ig3DBaseWindow::browse (IgRepresentable *object)
{
    ASSERT (m_browser);
    // FIXME: This should be replaced with some sort of automatic
    // registration of models into the view/page/document to get all
    // this done more automatically.
    m_browser->browse (object);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
Ig3DBaseWindow::focusIn (void)
{ 
    ASSERT (IgQtObjectMenuService::get (state ()));
    ASSERT (m_browser);

    IgQtObjectMenuService::get (state ())
    	->add (lat::CreateCallback (m_browser, &Ig3DBaseBrowser::repMenu));
    m_browser->focusIn ();
    m_centre->updateOnWindowActive ();
    
}

void
Ig3DBaseWindow::focusOut (void)
{
    ASSERT (IgQtObjectMenuService::get (state ()));
    ASSERT (m_browser);
    
    IgQtObjectMenuService::get (state ())
    	->remove (lat::CreateCallback (m_browser, &Ig3DBaseBrowser::repMenu));
    m_browser->focusOut ();
}

void
Ig3DBaseWindow::pick (void)
{
    m_browser->pick ();
    viewingModeCallback ();
}

void
Ig3DBaseWindow::view (void)
{
    m_browser->view ();
    viewingModeCallback ();
}

void
Ig3DBaseWindow::showToolbar (void)
{
    QToolBar *tb = IgQtAppToolBarService::get (m_state)
                   ->toolBar (TOOLBAR_ID_3D2D, "");
    QPopupMenu *menu = IgQtAppMenuService::get (m_state)
		       ->subMenu (Ig3DBaseBrowser::MENU_ID_3D);
    ASSERT (menu);

    disconnect (tb,   SIGNAL (visibilityChanged (bool)),
                this, SLOT (toolbarVisibilityChanged (bool)));

    if (tb->isShown ())
	tb->hide ();
    else
	tb->show ();

    connect (tb,   SIGNAL (visibilityChanged (bool)),
             this, SLOT (toolbarVisibilityChanged (bool)));
    toolbarVisibilityChanged (tb->isShown ());
}

void
Ig3DBaseWindow::toolbarVisibilityChanged (bool visibility)
{
    QPopupMenu *menu = IgQtAppMenuService::get (m_state)
		       ->subMenu (Ig3DBaseBrowser::MENU_ID_3D);
    ASSERT (menu);
    menu->setItemChecked (Ig3DBaseBrowser::MENU_ID_3D_TOOLBAR, visibility);
}
