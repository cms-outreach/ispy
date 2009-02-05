//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig2DWindow/interface/Ig2DWindow.h"
#include "Ig_Modules/Ig2DModel/interface/Ig2DModel.h"
#include "Ig_Modules/Ig3DSystem/interface/Ig3DSystem.h"
#include "Ig_Modules/IgStudio/interface/IgPage.h"
#include "Ig_Modules/IgStudio/interface/IgDocumentData.h"
#include "Ig_Modules/Ig2DBrowser/interface/Ig2DBrowser.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppMenuService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppContextService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtSiteStack.h"
#include "Ig_Modules/IgControlCentre/interface/IgControlCentre.h"
#include "Ig_Modules/IgControlCentre/interface/IgStudioCentre.h"
#include "Ig_Modules/Ig2DWindow/interface/Ig2DViewPropertiesCategory.h"
#include "Ig_Modules/IgStudio/interface/IgModelManager.h"
#include "Ig_Modules/IgStudio/interface/IgDocumentDataManager.h"

#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qmenudata.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

Ig2DWindow::Ig2DWindow (IgPage *page)
    : Ig3DBaseWindow (page),
      m_2dmodel (0)
{   
    static int lastWindowNumber = 0;
    IgModelManager *manager = IgModelManager::get (page->state ());
    if (!manager)
	manager = new IgModelManager (page->state ());
    manager->lookup (&m_2dmodel);
    if (!m_2dmodel)
    {
      IgDocumentDataManager* dm = IgDocumentDataManager::get(page->state ());
      m_2dmodel = new Ig2DModel (dm->lookup(0), model ());
      manager->add (dm->lookup(0), m_2dmodel);
    }
    
    QString t = "2D Window #" + QString::number (lastWindowNumber++);
    IgView::setTitle (t.ascii ());   
    
    watchBrowser (new Ig2DBrowser (state (), 
				   page->workspace ()->hostWithName (t), 
				   m_2dmodel));

    initCategories ();
    initMenu ();
    initToolBar ();    

    show ();
}

Ig2DWindow::~Ig2DWindow (void)
{
}

void
Ig2DWindow::initCategories (void)
{
    Ig3DBaseWindow::initCategories ();
    
    // Create a 2D Window Properties Category and add it in control
    // centre
    m_2DViewPropertiesCategory = Ig2DViewPropertiesCategory::get (page ()->state ());
    
    if (!m_2DViewPropertiesCategory)
    {
	m_2DViewPropertiesCategory = new Ig2DViewPropertiesCategory (
	    page ()->state (),
	    dynamic_cast <Ig2DBrowser *> (browser ()));
    }

    m_2DViewPropertiesCategory->registerBrowser (state (), browser ());    
}

void
Ig2DWindow::initMenu (void)
{
    Ig3DBaseWindow::initMenu ();    

    ASSERT (IgQtAppMenuService::get (state ()));
    
    // Get the menu as it has already been initialised in the
    // baseClass.
    QPopupMenu *menu = IgQtAppMenuService::get (state ())->subMenu
		       (Ig3DBaseBrowser::MENU_ID_3D);

    // Submenus
    if (IgQtAppMenuService::isIdThere (menu, Ig3DBaseBrowser::MENU_ID_3D_2D_VIEW_PROPERTIES) == -1)
    {
	menu->insertItem
	    ("2&D View properties...", this, SLOT (show2DViewProperties ()), 0,
	     Ig3DBaseBrowser::MENU_ID_3D_2D_VIEW_PROPERTIES, IgQtAppMenuService::index
	     (menu, Ig3DBaseBrowser::MENU_ID_3D_2D_VIEW_PROPERTIES));
    }
}

void
Ig2DWindow::initToolBar (void)
{
    Ig3DBaseWindow::initToolBar ();    
}


void
Ig2DWindow::show2DViewProperties (void)
{
    centre ()->selectCategory (m_2DViewPropertiesCategory);
    centre ()->show ();
}
