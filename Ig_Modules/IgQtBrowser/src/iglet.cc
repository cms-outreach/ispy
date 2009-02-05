//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtBrowser/interface/IgQtSite.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQWidgetSite.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQMainWindowSite.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQSplitterSite.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQHSplitterSite.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQVSplitterSite.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQWorkspaceSite.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQDockAreaSite.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQDockWindowSite.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgSiteDB.h"
#include "Ig_Framework/IgPluginManager/interface/IgModuleDef.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

DEFINE_IGUANA_MODULE ();
DEFINE_IGUANA_PLUGIN (IgSiteDB, IgQWidgetSite, "QWidget");
DEFINE_IGUANA_PLUGIN (IgSiteDB, IgQMainWindowSite, "QMainWindow");
DEFINE_IGUANA_PLUGIN (IgSiteDB, IgQSplitterSite, "QSplitter");
DEFINE_IGUANA_PLUGIN (IgSiteDB, IgQHSplitterSite, "QHSplitter");
DEFINE_IGUANA_PLUGIN (IgSiteDB, IgQVSplitterSite, "QVSplitter");
DEFINE_IGUANA_PLUGIN (IgSiteDB, IgQWidgetSite, "QWorkspace");
DEFINE_IGUANA_PLUGIN (IgSiteDB, IgQDockAreaSite, "QDockArea");
DEFINE_IGUANA_PLUGIN (IgSiteDB, IgQDockAreaSite, "QDockWindow");
