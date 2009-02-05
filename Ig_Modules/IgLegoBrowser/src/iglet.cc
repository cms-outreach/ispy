//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgLegoBrowser/interface/IgLegoBrowser.h"
#include "Ig_Modules/IgLegoBrowser/interface/xtypeinfo.h"
#include "Ig_Modules/IgLegoModel/interface/IgLegoModel.h"
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
DEFINE_IGUANA_PLUGIN (IgBrowserDB, IgLegoBrowser, IgLegoBrowser::catalogLabel ());

XTYPEINFO_DEF_1 (IgLegoBrowser, Ig3DBaseBrowser, false);
