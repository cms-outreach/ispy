//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DWindow/interface/Ig3DWindow.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgSite.h"
#include "Ig_Modules/Ig3DModel/interface/Ig3DModel.h"
#include "Ig_Modules/Ig3DBaseModel/interface/Ig3DBaseModel.h"
#include "Ig_Modules/Ig3DSystem/interface/Ig3DSystem.h"
#include "Ig_Modules/IgStudio/interface/IgDocumentData.h"
#include "Ig_Modules/IgStudio/interface/IgPage.h"
#include "Ig_Modules/IgCommonData/interface/IgTwig.h"
#include "Ig_Modules/Ig3DBrowser/interface/Ig3DBrowser.h"
#include "Ig_Modules/Ig3DCategories/interface/Ig3DLightsCategory.h"
#include "Ig_Modules/Ig3DCategories/interface/Ig3DSlicersCategory.h"
#include "Ig_Modules/Ig3DCategories/interface/Ig3DClipsCategory.h"
#include "Ig_Modules/Ig3DCategories/interface/Ig3DAnimsCategory.h"
#include "Ig_Modules/Ig3DCategories/interface/Ig3DNodeCategory.h"
#include "Ig_Modules/Ig3DCategories/interface/Ig3DViewpointsCategory.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtSite.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtSiteStack.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppContextService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppMenuService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtObjectMenuService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtObjectMenuMessage.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtObjectMenu.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepSet.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepContext.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtCallbacks.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppToolBarService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppImageService.h"
#include "Ig_Modules/IgControlCentre/interface/IgStudioCentre.h"
#include "Ig_Modules/IgControlCentre/interface/IgControlCentre.h"
#include "Ig_Modules/Ig3DBaseModel/interface/Ig3DBaseRep.h"
#include "Ig_Extensions/IgIcons/interface/IgImageIDs.h"
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <classlib/utils/Callback.h>
#include <qpopupmenu.h>
#include <qmenudata.h>
#include <qmenubar.h>
#include <qtoolbutton.h>
#include <qtoolbar.h>
#include <qwhatsthis.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

Ig3DWindow::Ig3DWindow (IgPage *page)
    : Ig3DBaseWindow (page),
      m_slicersCategory (0),
      m_clipsCategory (0),
      m_lightsCategory (0),
      m_materialsCategory (0),
      m_animsCategory (0),
      m_viewpointsCategory (0),
      m_nodeCategory (0)
{
    static int lastWindowNumber = 0;
    QString t = "3D Window #" + QString::number (lastWindowNumber++);
    setTitle (t.ascii ());

    Ig3DBrowser *b = new Ig3DBrowser (state (), 
				      page->workspace ()->hostWithName (t), 
				      dynamic_cast<Ig3DModel *> (model ()));

    watchBrowser (b);

    // Show myself and show all the document data (FIXME: select?).    
    initCategories ();
    // Make sure that this is called AFTER we set up a browser for this window.
    initMenu ();    
    initToolBar ();

    show ();
}

Ig3DWindow::~Ig3DWindow (void)
{
}

void
Ig3DWindow::initCategories (void)
{
    Ig3DBaseWindow::initCategories ();
    
    //Create a Slicer Category and add it in control centre
    m_lightsCategory = Ig3DLightsCategory::get (page ()->state ());
    if (!m_lightsCategory)
    {
	m_lightsCategory = new Ig3DLightsCategory (page ()->state ());
    }    
    
    //Create a Slicer Category and add it in control centre
    m_slicersCategory = Ig3DSlicersCategory::get (page ()->state ());
    if (!m_slicersCategory)
    {
    	m_slicersCategory = new Ig3DSlicersCategory (page ()->state (),
						     browser ());
    }

    //Create the Clipplane category and add it in control centre
    m_clipsCategory = Ig3DClipsCategory::get (page ()->state ());
    if (!m_clipsCategory)
    {
	m_clipsCategory = new Ig3DClipsCategory (page ()->state (),
						 browser ());
    }    

    //Create the Animator category and add it in control centre

    m_animsCategory = Ig3DAnimsCategory::get (page ()->state ());
    if (!m_animsCategory)
    {
	m_animsCategory = new Ig3DAnimsCategory (page ()->state (),
						 browser ());
    }
    
    // Create the View Points category and add it in control centre
    m_viewpointsCategory = Ig3DViewpointsCategory::get (page ()->state ());
    if (!m_viewpointsCategory)
    {
	m_viewpointsCategory = new Ig3DViewpointsCategory(page ()->state (),
							  dynamic_cast<Ig3DBrowser *> (browser ()));
    }

    // Create the category to control shapes
    m_nodeCategory = Ig3DNodeCategory::get (page ()->state ());
    if (!m_nodeCategory)
    {
	m_nodeCategory = new Ig3DNodeCategory(page ()->state (), browser ());
    }

    m_lightsCategory->registerBrowser (state (), browser ());
    m_slicersCategory->registerBrowser (state (), browser ());
    m_clipsCategory->registerBrowser (state (), browser ());
    m_animsCategory->registerBrowser (state (), browser ());
    m_viewpointsCategory->registerBrowser (state (), browser ());
    m_nodeCategory->registerBrowser (state (), browser ());
}

void
Ig3DWindow::initMenu (void)
{
    // NB: We need to make sure that the popup menu is already there.
    // This is done by calling the base class method.
    Ig3DBaseWindow::initMenu ();
    
    ASSERT (IgQtAppMenuService::get (state ()));

    // Get the menu as it has already been initialised in the
    // baseClass.
    QPopupMenu *menu = IgQtAppMenuService::get (state ())->
		       subMenu (Ig3DBaseBrowser::MENU_ID_3D);

    // Submenus. First we check if the first item of the submenu is
    // already there, if it is, we don't create any item, as we as
    // assume that all of them are already there. Viceversa, we create
    // all of them if the first one is not found.
    if (IgQtAppMenuService::isIdThere (menu, Ig3DBaseBrowser::MENU_ID_3D_Z_SLICE) == -1)
    {
	menu->insertItem
	    ("&Slicers...", this, SLOT (showSlicers ()), 0,
	     Ig3DBaseBrowser::MENU_ID_3D_Z_SLICE, IgQtAppMenuService::index
	     (menu, Ig3DBaseBrowser::MENU_ID_3D_Z_SLICE));

	menu->insertItem
	    ("&Clips...", this, SLOT (showClips ()), 0,
	     Ig3DBaseBrowser::MENU_ID_3D_CLIPS, IgQtAppMenuService::index
	     (menu, Ig3DBaseBrowser::MENU_ID_3D_CLIPS));

	menu->insertItem
	    ("&Lights...", this, SLOT (showLights ()), 0,
	     Ig3DBaseBrowser::MENU_ID_3D_LIGHTS, IgQtAppMenuService::index
	     (menu, Ig3DBaseBrowser::MENU_ID_3D_LIGHTS));

	menu->insertItem
	    ("&Animations...", this, SLOT (showAnims ()), 0,
	     Ig3DBaseBrowser::MENU_ID_3D_ANIMS, IgQtAppMenuService::index
	     (menu, Ig3DBaseBrowser::MENU_ID_3D_ANIMS));

	menu->insertItem
	    ("&Viewpoints...", this, SLOT (showViewpoints ()), 0,
	     Ig3DBaseBrowser::MENU_ID_3D_VIEWPOINTS, IgQtAppMenuService::index
	     (menu, Ig3DBaseBrowser::MENU_ID_3D_VIEWPOINTS));
	
	menu->insertItem
	    ("&Node Manipulator...", this, SLOT (showNodeManip ()), 0,
	     Ig3DBaseBrowser::MENU_ID_3D_NODEMANIP, IgQtAppMenuService::index
	     (menu, Ig3DBaseBrowser::MENU_ID_3D_NODEMANIP));
    }    
}

void
Ig3DWindow::focusIn (void)
{
    Ig3DBaseWindow::focusIn ();
    
    ASSERT (IgQtObjectMenuService::get (state ()));
    IgQtObjectMenuService::get (state ())
	->add (lat::CreateCallback (this, &Ig3DWindow::repMenu));
}

void
Ig3DWindow::focusOut (void)
{
    Ig3DBaseWindow::focusOut ();

    ASSERT (IgQtObjectMenuService::get (state ()));
    IgQtObjectMenuService::get (state ())
	->remove (lat::CreateCallback (this, &Ig3DWindow::repMenu));
}

static void
feedbackButtonCallback (Ig3DBaseBrowser *browser, QToolButton *button)
{
    button->setOn (browser->isFeedbackVisible ());    
}

static void
gridButtonCallback (Ig3DBaseBrowser *browser, QToolButton *button)
{
    button->setOn (browser->isGridVisible ());    
}

static void
orthogonalButtonCallback (Ig3DBaseBrowser *browser, QToolButton *button)
{
    SoCamera *camera = browser->getCamera ();
    if (camera->getTypeId ()==SoOrthographicCamera::getClassTypeId ())
    {
	button->setOn (true);    
    }else
    {
	button->setOn (false);	
    }
}

static void
qtSetOff (QToolButton *button)
{
    button->setOn (false);    
}

void
Ig3DWindow::cameraToggled (void)
{
    IgQtAppToolBarService *tb = IgQtAppToolBarService::get (state ());
    orthogonalButtonCallback(browser (),
                             tb->getToolBarButton(
				 Ig3DBaseWindow::TOOLBAR_ID_3D2D,
				 Ig3DBaseWindow::TOOLBAR_ID_3D2D_ORTHO));
}

void
Ig3DWindow::initToolBar (void)
{
    Ig3DBaseWindow::initToolBar ();
    // This creates a toolbar with the general widgets.
    IgQtAppToolBarService *toolBarService = IgQtAppToolBarService::get (state ());
    IgQtAppImageService *is = IgQtAppImageService::get (state ());
    IgQtAppContextService *cs = IgQtAppContextService::get (state ());

    ASSERT (is);

    QToolButton *button = 0;
    
    button = toolBarService->toolBarButton (state (),
					    Ig3DBaseWindow::TOOLBAR_ID_3D2D,
					    Ig3DBaseWindow::TOOLBAR_ID_3D2D_GRID, 
					    *is->pixmapById (IgImageIDs::IMAGE_ID_GRID),
					    "Show/Hide Grid",
					    browser (),
					    SLOT (setGridVisibility (bool)),
					    SIGNAL (toggled (bool)));
    button->setToggleButton (true); 
    QWhatsThis::add (button, "Enable/disable the grid ont he scene graph.");
    cs->addOnFocusIn (lat::CreateCallback (&gridButtonCallback, 
					   browser (),
					   button));
    cs->addOnFocusOut (lat::CreateCallback (&qtSetOff,
					    button));

    button = toolBarService->toolBarButton (state (),
					    Ig3DBaseWindow::TOOLBAR_ID_3D2D,
					    Ig3DBaseWindow::TOOLBAR_ID_3D2D_SHOW_AXIS,
					    *is->pixmapById (IgImageIDs::IMAGE_ID_YX_SMALL_COL), 
					    "Show/Hide Axis",
					    browser (),
					    SLOT (setFeedbackVisibility (bool)),
					    SIGNAL (toggled (bool)));
    button->setToggleButton (true);    
    QWhatsThis::add (button, "Show/Hide the axis on the 3D scene.");
    cs->addOnFocusIn (lat::CreateCallback (&feedbackButtonCallback, 
					   browser (),
					   button));
    cs->addOnFocusOut (lat::CreateCallback (&qtSetOff,
					    button));

    button = toolBarService->toolBarButton (state (),
					    Ig3DBaseWindow::TOOLBAR_ID_3D2D,
					    Ig3DBaseWindow::TOOLBAR_ID_3D2D_INVERT, 
					    *is->pixmapById (IgImageIDs::IMAGE_ID_INVERSE),
					    "Invert normals",
					    browser (),
					    SLOT (invertCamera ()));
    QWhatsThis::add (button, "Invert the camera orientation and "
		     "position through the origin.");

    // FIXME: this needs to be a togglable button. Create a service
    // for doing so.
    QIconSet iconSet(*is->pixmapById (IgImageIDs::IMAGE_ID_PERSPECTIVE));
    iconSet.setPixmap (*is->pixmapById (IgImageIDs::IMAGE_ID_ORTHO), QIconSet::Automatic,
                       QIconSet::Normal, QIconSet::On);

    button = toolBarService->toolBarButton (state (),
					    Ig3DBaseWindow::TOOLBAR_ID_3D2D,
					    Ig3DBaseWindow::TOOLBAR_ID_3D2D_ORTHO, 
					    iconSet,
					    "Change projection",
					    browser (),
					    SLOT (toggleCameraType ()));

    button->setToggleButton (true);
    QWhatsThis::add (button, "Set the camera projection by toggling "
		     "between the perspective and "
		     "orthographic modes.");
    cs->addOnFocusIn (lat::CreateCallback (&orthogonalButtonCallback, 
					   browser (), 
					   button));
    cs->addOnFocusOut (lat::CreateCallback (&qtSetOff,
					    button));
    connect (browser (), SIGNAL (cameraToggled (void)),
             this,       SLOT   (cameraToggled (void)));
    
    button = toolBarService->toolBarButton (state (),
					    Ig3DBaseWindow::TOOLBAR_ID_3D2D,
					    Ig3DBaseWindow::TOOLBAR_ID_3D2D_PRINT_DEFAULT_VIEWS,
					    *is->pixmapById (IgImageIDs::IMAGE_ID_PRINT_DEFAULT_VIEWS),
					    "Print the default views",
					    this,
					    SLOT (printViews ()));
    QWhatsThis::add (button, "Print the default views.");
}

void
Ig3DWindow::showClips (void)
{
    centre ()->selectCategory (m_clipsCategory);
    centre ()->show ();
}

void
Ig3DWindow::showSlicers (void)
{
    centre ()->selectCategory (m_slicersCategory);
    centre ()->show ();
}

void
Ig3DWindow::showLights (void)
{
    centre ()->selectCategory (m_lightsCategory);
    centre ()->show ();
}

void
Ig3DWindow::showAnims (void)
{
    centre ()->selectCategory (m_animsCategory);
    centre ()->show ();
}

void
Ig3DWindow::showViewpoints (void)
{
    centre ()->selectCategory (m_viewpointsCategory);
    centre ()->show ();
}

void
Ig3DWindow::showNodeManip (void)
{
    centre ()->selectCategory (m_nodeCategory);
    centre ()->show ();
}

//////////////////////////////////////////////////////////////////////
void
Ig3DWindow::repMenu (IgQtObjectMenuMessage message)
{
    static const IgQtObjectMenu::ItemDef defs [] = {
	{ -1, Ig3DBaseBrowser::MENU_3D_OPS, 0, 0, -1 },
	{ -1, Ig3DBaseBrowser::MENU_SLICE,  "Slice", SLOT(repSlice()), -1 },
	{ -1, Ig3DBaseBrowser::MENU_CLIP,   "Clip", SLOT(repClip()), -1 },
    };
    static const int	ndefs = sizeof (defs)/sizeof (defs [0]);

    IgRepresentable	*object = message.object ();

    IgRep *m_menuRep = dynamic_cast<IgRep *>
		       (IgRepSet::lookup (object, model (), false));

    // An irrelevant object or already added item, remove our items.
    message.menu ()->removeFromDefs (defs, ndefs);
    if (m_menuRep)
	// Add our items if they aren't there yet.
	message.menu ()->insertFromDefs (this, defs, ndefs);
}

//////////////////////////////////////////////////////////////////////
void
Ig3DWindow::repSlice (void)
{
    ASSERT (browser ()->menuRep ());
    ASSERT (m_slicersCategory);
    m_slicersCategory->makeOne (browser ()->menuRep ());
}

void
Ig3DWindow::repClip (void)
{
    ASSERT (browser ()->menuRep ());
    ASSERT (m_clipsCategory);
    m_clipsCategory->makeOne (browser ()->menuRep ());
}

//////////////////////////////////////////////////////////////////////
void
Ig3DWindow::printViews (void)
{
    static SoNode * node = 0;
    if (! node)
    {
	node = browser ()->openNode (Ig3DBaseModel::encode
				     ("Default View Points").getString (),
				     "Select viewpoints to load",
				     browser ()->getWidget ()->parentWidget ()); 
    }
    
    if (node)
    {
        ASSERT (node->isOfType (SoGroup::getClassTypeId ()));
	SoGroup *views = static_cast<SoGroup *> (node);
	IgSoViewpoint *currentView = new IgSoViewpoint ();
	Ig3DViewpointsCategory::setViewpoint (browser (), currentView);
	for (int i = 1, n = views->getNumChildren (); i < n; ++i)
        {
	    ASSERT (views->getChild (i)->isOfType (IgSoViewpoint::getClassTypeId ()));
            IgSoViewpoint *obj = static_cast<IgSoViewpoint *>(views->getChild (i));
	    Ig3DViewpointsCategory::setCamera (browser (), obj);
	    QString filename = obj->getName ().getString ();
	    
	    browser ()->printVector (filename + ".pdf", "pdf", 2);
	    browser ()->printBitmap (filename + ".jpg",
	                            SoOffscreenRenderer::getScreenPixelsPerInch (),
	    			    300, "jpg");
        }
	Ig3DViewpointsCategory::setCamera (browser (), currentView);
    }
}
