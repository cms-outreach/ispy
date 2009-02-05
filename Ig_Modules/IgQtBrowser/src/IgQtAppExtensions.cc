#include "Ig_Modules/IgQtBrowser/interface/IgQtAppExtensions.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppWindowService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppLoopService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppMenuService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppToolBarService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppStatusBarService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppContextService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppDebugService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppHelpService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppCrashAlertService.h"

// Install common Qt main window services:
//   - main loop service
//   - main menu service
//   - tool bar service
//   - status bar service
//   - current user attention context
//   - debugging menu (debugging builds only)
//   - help menu
//   - crash alert service  (<-- Rob's great name invention!)

void
IgQtAppExtensions::install (IgState *state, QWidget *mainWindow)
{
    new IgQtAppWindowService (state, mainWindow);
    new IgQtAppLoopService (state);
    new IgQtAppMenuService (state, mainWindow);
    new IgQtAppToolBarService (state, mainWindow);
    new IgQtAppStatusBarService (state, mainWindow);
    new IgQtAppContextService (state, mainWindow);
    new IgQtAppDebugService (state, mainWindow);
    new IgQtAppHelpService (state, mainWindow);
    new IgQtAppCrashAlertService (state, mainWindow);
}
