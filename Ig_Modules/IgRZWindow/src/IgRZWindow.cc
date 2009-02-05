//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgRZWindow/interface/IgRZWindow.h"
#include "Ig_Modules/IgRZModel/interface/IgRZModel.h"
#include "Ig_Modules/Ig3DBaseModel/interface/Ig3DBaseRep.h"
#include "Ig_Modules/Ig3DSystem/interface/Ig3DSystem.h"
#include "Ig_Modules/IgStudio/interface/IgPage.h"
#include "Ig_Modules/IgStudio/interface/IgDocumentData.h"
#include "Ig_Modules/IgRZBrowser/interface/IgRZBrowser.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppImageService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppContextService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppMenuService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppToolBarService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtObjectMenu.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtObjectMenuMessage.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtObjectMenuService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtSiteStack.h"
#include "Ig_Modules/IgControlCentre/interface/IgControlCentre.h"
#include "Ig_Modules/IgControlCentre/interface/IgStudioCentre.h"
#include "Ig_Modules/IgRZWindow/interface/IgRZViewPropertiesCategory.h"
#include "Ig_Modules/Ig3DCategories/interface/Ig3DLightsCategory.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepSet.h"
#include "Ig_Extensions/IgIcons/interface/IgImageIDs.h"
#include "Ig_Modules/IgStudio/interface/IgModelManager.h"
#include "Ig_Modules/IgStudio/interface/IgDocumentDataManager.h"
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoCamera.h>

#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qmenudata.h>
#include <qtoolbutton.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgRZWindow::IgRZWindow (IgPage *page)
    : Ig3DBaseWindow (page),
      m_rzViewPropertiesCategory (0),
      m_lightsCategory (0)
{   
    static int lastWindowNumber = 0;    
    IgRZModel *rzModel = 0;
    IgModelManager *manager = IgModelManager::get (page->state ());
    if (!manager)
	manager = new IgModelManager (page->state ());
    manager->lookup (&rzModel);
    if (!rzModel)
    {
      IgDocumentDataManager* dm = IgDocumentDataManager::get(page->state ());
      rzModel = new  IgRZModel (dm->lookup(0));
      manager->add (dm->lookup(0), rzModel);
    }
    
    QString t = "RZ Window #" + QString::number (lastWindowNumber++);
    IgView::setTitle (t.ascii ());   
    
    watchBrowser (new IgRZBrowser (state (), 
				   page->workspace ()->hostWithName (t), 
				   rzModel));

    initCategories ();
    initMenu ();
    initToolBar ();    

    show ();
}

IgRZWindow::~IgRZWindow (void)
{
}

void
IgRZWindow::initCategories (void)
{
    Ig3DBaseWindow::initCategories ();
    
    //Create a Lights Category and add it in control centre
    m_lightsCategory = Ig3DLightsCategory::get (page ()->state ());
    if (!m_lightsCategory)
    {
	m_lightsCategory = new Ig3DLightsCategory (page ()->state ());
    }    
/*    m_lightsCategory = IgRZLightsCategory::get (page ()->state ());
    if (!m_lightsCategory)
    {
	m_lightsCategory = new IgRZLightsCategory (page ()->state ());
    }    
*/    
    m_lightsCategory->registerBrowser (state (), browser ());

    // Create a RZ Window Properties Category and add it in control
    // centre
    m_rzViewPropertiesCategory = IgRZViewPropertiesCategory::get (page ()->state ());
    
    if (!m_rzViewPropertiesCategory)
    {
	m_rzViewPropertiesCategory = new IgRZViewPropertiesCategory (
	    page ()->state (),
	    dynamic_cast <IgRZBrowser *> (browser ()));
    }

    m_rzViewPropertiesCategory->registerBrowser (state (),
          dynamic_cast <IgRZBrowser *> (browser ()));    
}

void
IgRZWindow::initMenu (void)
{
    Ig3DBaseWindow::initMenu ();    

    ASSERT (IgQtAppMenuService::get (state ()));
    
    // Get the menu as it has already been initialised in the
    // baseClass.
    QPopupMenu *menu = IgQtAppMenuService::get (state ())->subMenu
		       (Ig3DBaseBrowser::MENU_ID_3D);

    // Submenus
    if (IgQtAppMenuService::isIdThere (menu, Ig3DBaseBrowser::MENU_ID_3D_RZ_VIEW_PROPERTIES) == -1)
    {
	menu->insertItem
	    ("RZ View properties...", this, SLOT (showRZViewProperties ()), 0,
	     Ig3DBaseBrowser::MENU_ID_3D_RZ_VIEW_PROPERTIES, IgQtAppMenuService::index
	     (menu, Ig3DBaseBrowser::MENU_ID_3D_RZ_VIEW_PROPERTIES));
    }
    if (IgQtAppMenuService::isIdThere (menu, Ig3DBaseBrowser::MENU_ID_3D_Z_SLICE) == -1)
    {
	menu->insertItem
	    ("&Lights...", this, SLOT (showLights ()), 0,
	     Ig3DBaseBrowser::MENU_ID_3D_LIGHTS, IgQtAppMenuService::index
	     (menu, Ig3DBaseBrowser::MENU_ID_3D_LIGHTS));
    }    
}

void
IgRZWindow::focusIn (void)
{
    Ig3DBaseWindow::focusIn ();
    
    ASSERT (IgQtObjectMenuService::get (state ()));
    IgQtObjectMenuService::get (state ())
	->add (lat::CreateCallback (this, &IgRZWindow::repMenu));
}

void
IgRZWindow::focusOut (void)
{
    Ig3DBaseWindow::focusOut ();

    ASSERT (IgQtObjectMenuService::get (state ()));
    IgQtObjectMenuService::get (state ())
	->remove (lat::CreateCallback (this, &IgRZWindow::repMenu));
}

static void
feedbackButtonCallback (Ig3DBaseBrowser *browser, QToolButton *button)
{
    button->setOn (browser->isFeedbackVisible ());    
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

typedef void (*FeedbackButtonCallback) (IgRZBrowser *browser, QToolButton *button);

void
IgRZWindow::initToolBar (void)
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
					    Ig3DBaseWindow::TOOLBAR_ID_3D2D_SHOW_AXIS,
					    *is->pixmapById (IgImageIDs::IMAGE_ID_YX_SMALL_COL), 
					    "Show Axis",
					    browser (),
					    SLOT (setFeedbackVisibility (bool)),
					    SIGNAL (toggled (bool)));
    button->setToggleButton (true);    

    toolBarService->toolBarButton (state (),
				   Ig3DBaseWindow::TOOLBAR_ID_3D2D,
				   Ig3DBaseWindow::TOOLBAR_ID_3D2D_INVERT, 
				   *is->pixmapById (IgImageIDs::IMAGE_ID_INVERSE),
				   "Invert normals",
				   browser (),
				   SLOT (invertCamera ()));

    // FIXME: this needs to be a togglable button. Create a service
    // for doing so.
    QIconSet iconSet(*is->pixmapById (IgImageIDs::IMAGE_ID_PERSPECTIVE));
    iconSet.setPixmap (*is->pixmapById (IgImageIDs::IMAGE_ID_ORTHO), QIconSet::Automatic,
                       QIconSet::Normal, QIconSet::On);
    QToolButton *cpButton 
	= toolBarService->toolBarButton (state (),
					 Ig3DBaseWindow::TOOLBAR_ID_3D2D,
					 Ig3DBaseWindow::TOOLBAR_ID_3D2D_ORTHO, 
					 //*is->pixmapById (IgImageIDs::IMAGE_ID_ORTHO),
					 iconSet,
					 "Change projection",
					 browser (),
					 SLOT (toggleCameraType ()));

    cpButton->setToggleButton (true);
    
    cs->addOnFocusIn (lat::CreateCallback (&feedbackButtonCallback, 
					   browser (), 
					   button));    
    cs->addOnFocusOut (lat::CreateCallback (&qtSetOff,
					    button));

    cs->addOnFocusIn (lat::CreateCallback (&orthogonalButtonCallback, 
					   browser (), 
					   cpButton));
    cs->addOnFocusOut (lat::CreateCallback (&qtSetOff,
					    cpButton));    
}

void
IgRZWindow::showRZViewProperties (void)
{
    centre ()->selectCategory (m_rzViewPropertiesCategory);
    centre ()->show ();
}

void
IgRZWindow::showLights (void)
{
    centre ()->selectCategory (m_lightsCategory);
    centre ()->show ();
}

//////////////////////////////////////////////////////////////////////
void
IgRZWindow::repMenu (IgQtObjectMenuMessage message)
{
    static const IgQtObjectMenu::ItemDef defs [] = {
	{ -1, Ig3DBaseBrowser::MENU_3D_OPS, 0, 0, -1 }
	//	{ -1, IgRZWindow::MENU_RZ_MANIP, "Manipulate", SLOT(repNode ()), -1 },
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
