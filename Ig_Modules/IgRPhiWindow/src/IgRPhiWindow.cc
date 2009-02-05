//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgRPhiWindow/interface/IgRPhiWindow.h"
#include "Ig_Modules/IgRPhiModel/interface/IgRPhiModel.h"
#include "Ig_Modules/Ig3DBaseModel/interface/Ig3DBaseRep.h"
#include "Ig_Modules/Ig3DSystem/interface/Ig3DSystem.h"
#include "Ig_Modules/IgStudio/interface/IgPage.h"
#include "Ig_Modules/IgStudio/interface/IgDocumentData.h"
#include "Ig_Modules/IgRPhiBrowser/interface/IgRPhiBrowser.h"
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
#include "Ig_Modules/IgRPhiWindow/interface/IgRPhiViewPropertiesCategory.h"
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

IgRPhiWindow::IgRPhiWindow (IgPage *page)
    : Ig3DBaseWindow (page),
      m_rphiViewPropertiesCategory (0),
      m_lightsCategory (0)
{   
    static int lastWindowNumber = 0;    
    IgRPhiModel *rphiModel = 0;
    IgModelManager *manager = IgModelManager::get (page->state ());
    if (!manager)
	manager = new IgModelManager (page->state ());
    manager->lookup (&rphiModel);
    if (!rphiModel)
    {
      IgDocumentDataManager* dm = IgDocumentDataManager::get(page->state ());
      rphiModel = new  IgRPhiModel (dm->lookup(0));
      manager->add (dm->lookup(0), rphiModel);
    }
    
    QString t = "RPhi Window #" + QString::number (lastWindowNumber++);
    IgView::setTitle (t.ascii ());   
    
    watchBrowser (new IgRPhiBrowser (state (), 
				   page->workspace ()->hostWithName (t), 
				   rphiModel));

    initCategories ();
    initMenu ();
    initToolBar ();    

    show ();
}

IgRPhiWindow::~IgRPhiWindow (void)
{
}

void
IgRPhiWindow::initCategories (void)
{
    Ig3DBaseWindow::initCategories ();
    
    //Create a Lights Category and add it in control centre
    m_lightsCategory = Ig3DLightsCategory::get (page ()->state ());
    if (!m_lightsCategory)
    {
	m_lightsCategory = new Ig3DLightsCategory (page ()->state ());
    }    
    m_lightsCategory->registerBrowser (state (), browser ());

    // Create a RPhi Window Properties Category and add it in control
    // centre
    m_rphiViewPropertiesCategory = IgRPhiViewPropertiesCategory::get (page ()->state ());
    
    if (!m_rphiViewPropertiesCategory)
    {
	m_rphiViewPropertiesCategory = new IgRPhiViewPropertiesCategory (
	    page ()->state (),
	    dynamic_cast <IgRPhiBrowser *> (browser ()));
    }

    m_rphiViewPropertiesCategory->registerBrowser (state (),
          dynamic_cast <IgRPhiBrowser *> (browser ()));    
}

void
IgRPhiWindow::initMenu (void)
{
    Ig3DBaseWindow::initMenu ();    

    ASSERT (IgQtAppMenuService::get (state ()));
    
    // Get the menu as it has already been initialised in the
    // baseClass.
    QPopupMenu *menu = IgQtAppMenuService::get (state ())->subMenu
		       (Ig3DBaseBrowser::MENU_ID_3D);

    // Submenus
    if (IgQtAppMenuService::isIdThere (menu, Ig3DBaseBrowser::MENU_ID_3D_RPHI_VIEW_PROPERTIES) == -1)
    {
	menu->insertItem
	    ("R-Phi View properties...", this, SLOT (showRPhiViewProperties ()), 0,
	     Ig3DBaseBrowser::MENU_ID_3D_RPHI_VIEW_PROPERTIES, IgQtAppMenuService::index
	     (menu, Ig3DBaseBrowser::MENU_ID_3D_RPHI_VIEW_PROPERTIES));
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
IgRPhiWindow::focusIn (void)
{
    Ig3DBaseWindow::focusIn ();
    
    ASSERT (IgQtObjectMenuService::get (state ()));
    IgQtObjectMenuService::get (state ())
	->add (lat::CreateCallback (this, &IgRPhiWindow::repMenu));
}

void
IgRPhiWindow::focusOut (void)
{
    Ig3DBaseWindow::focusOut ();

    ASSERT (IgQtObjectMenuService::get (state ()));
    IgQtObjectMenuService::get (state ())
	->remove (lat::CreateCallback (this, &IgRPhiWindow::repMenu));
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
qtSetOff (QToolButton *button)
{
    button->setOn (false);    
}

typedef void (*FeedbackButtonCallback) (IgRPhiBrowser *browser, QToolButton *button);

void
IgRPhiWindow::initToolBar (void)
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
					    "Show Axis",
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
}

void
IgRPhiWindow::showRPhiViewProperties (void)
{
    centre ()->selectCategory (m_rphiViewPropertiesCategory);
    centre ()->show ();
}

void
IgRPhiWindow::showLights (void)
{
    centre ()->selectCategory (m_lightsCategory);
    centre ()->show ();
}

//////////////////////////////////////////////////////////////////////
void
IgRPhiWindow::repMenu (IgQtObjectMenuMessage message)
{
    static const IgQtObjectMenu::ItemDef defs [] = {
	{ -1, Ig3DBaseBrowser::MENU_3D_OPS, 0, 0, -1 }
    };
    static const int	ndefs = sizeof (defs) / sizeof (defs [0]);

    IgRepresentable	*object = message.object ();

    IgRep *m_menuRep = dynamic_cast<IgRep *>
		(IgRepSet::lookup (object, model (), false));

    // An irrelevant object or already added item, remove our items.
    message.menu ()->removeFromDefs (defs, ndefs);
    if (m_menuRep)
	// Add our items if they aren't there yet.
	message.menu ()->insertFromDefs (this, defs, ndefs);
}
