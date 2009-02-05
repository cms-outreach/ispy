//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgRZBrowser/interface/IgRZBrowser.h"
#include "Ig_Modules/IgRZBrowser/interface/xtypeinfo.h"
#include "Ig_Modules/IgRZModel/interface/IgRZModel.h"
#include "Ig_Modules/Ig3DBaseBrowser/interface/xtypeinfo.h"
#include "Ig_Framework/IgObjectBrowser/interface/xtypeinfo.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgBrowserDB.h"
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
DEFINE_IGUANA_PLUGIN (IgBrowserDB, IgRZBrowser, IgRZBrowser::catalogLabel ());

XTYPEINFO_DEF_1 (IgRZBrowser, Ig3DBaseBrowser, false);
