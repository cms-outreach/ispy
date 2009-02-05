//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Examples/IgHelloWorldView/interface/IgHelloWorldView.h"
#include "Ig_Examples/IgHelloWorldView/interface/IgHelloWorldButton.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtSiteStack.h"
#include "Ig_Modules/IgStudio/interface/IgPage.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgHelloWorldView::IgHelloWorldView (IgPage *page)
    :IgView (page)
{     
    IgHelloWorldButton *button = new IgHelloWorldButton (
      IgQtSite::selfFrom (page->workspace ())); 
    button->show ();    
}
