//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig2DBrowser/interface/Ig2DBrowser.h"
#include "Ig_Modules/Ig2DBrowser/interface/xtypeinfo.h"
#include "Ig_Modules/Ig2DModel/interface/Ig2DModel.h"
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
DEFINE_IGUANA_PLUGIN (IgBrowserDB, Ig2DBrowser, Ig2DBrowser::catalogLabel ());

XTYPEINFO_DEF_1 (Ig2DBrowser, Ig3DBaseBrowser, false);
