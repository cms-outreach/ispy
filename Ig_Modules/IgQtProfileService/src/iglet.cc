//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtProfileService/interface/IgProfile.h"
#include "Ig_Modules/IgQtProfileService/interface/IgQtProfileExtension.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgExtensionDB.h"
#include "Ig_Framework/IgPluginManager/interface/IgModuleDef.h"
#include <classlib/utils/DebugAids.h>

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
DEFINE_IGUANA_PLUGIN (IgExtensionDB, IgProfile, IgProfile::catalogLabel ());
DEFINE_IGUANA_PLUGIN (IgExtensionDB, IgQtProfileExtension, IgQtProfileExtension::catalogLabel ());
