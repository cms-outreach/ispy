//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgStudio/interface/IgMDIView.h"
#include "Ig_Modules/IgStudio/interface/IgViewDB.h"
#include "Ig_Modules/IgStudio/interface/IgPage.h"
#include "Ig_Modules/IgStudio/interface/IgView.h"
#include "Ig_Modules/IgStudio/interface/IgDocumentData.h"
#include "Ig_Modules/IgCommonData/interface/IgTwig.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQWorkspaceSite.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQDockWindowSite.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgBrowser.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgBrowserDB.h"
#include <qwidget.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgMDIView::IgMDIView (IgPage *page, const char *window /*= 0*/)
{
    IgBrowser *twig = IgBrowserDB::get()->create ("Twig", page->state (),
                                page->stackArea ()->hostWithName ("Main model"));
    IgBrowser *text = IgBrowserDB::get()->create ("Text", page->state (),
                                new IgQDockWindowSite (page->state (),
				                       page->leftDockArea ()));
    
    IgQtSite::selfFrom (page)->show ();
    
    QString name ("3D Window");
    if (window)
      name = window;
    IgBrowser *browser = dynamic_cast<IgBrowser*>(
                        IgViewDB::get()->create (name.latin1 (), page ));
    
    IgRepresentable *obj = IgDocumentData::get (page->state ())->root ();
    browser->browse (obj);
    twig->browse (obj);
    text->browse (obj);
}
