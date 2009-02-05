//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtProfileService/interface/IgQtProfileExtension.h"
#include "Ig_Modules/IgQtProfileService/interface/IgQtProfileService.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfMainWindow.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfConfiguration.h"
#include "Ig_Modules/IgQtProfileService/interface/IgQtProfileInfo.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppMenuService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppDebugService.h"
#include "Ig_Modules/IgStudio/interface/IgStudioDriver.h"

#include <qwidget.h>
#include <qmainwindow.h>
#include <qpopupmenu.h>
#include <iostream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

/*In order to have the extension loaded at startup, it is important that it has
  such a name.
  Extensions/Qt GUI/
*/
IG_DEFINE_STATE_ELEMENT (IgQtProfileExtension, "Extensions/Profiler/OProfile");
 

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

//using namespace boost::python;


IgQtProfileExtension::IgQtProfileExtension (IgState *state)
    :m_state(state),
     m_profileInfo(0)
{
    ASSERT(m_state);

    QPopupMenu *profile = IgQtAppMenuService::get(state)->subMenu(IgQtAppDebugService::MENU_ID_DEBUG);
    profile->connectItem (IgQtAppDebugService::MENU_ID_DEBUG_PROFILER, this, SLOT(showProfiler()));
    showProfiler();
}

IgQtProfileExtension::~IgQtProfileExtension (void)
{ ASSERT (m_state); }

void 
IgQtProfileExtension::showProfiler(void)
{
    if(m_profileInfo == 0 ) m_profileInfo = new IgQtProfileInfo(m_state);
    else m_profileInfo->show();  
}
