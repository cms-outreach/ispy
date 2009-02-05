//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Examples/IgOpenInventorDemo/interface/IgOIVDemoExtension.h"
#include "Ig_Examples/IgOpenInventorDemo/interface/IgOIVDemo.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgExtensionDB.h"
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
DEFINE_IGUANA_PLUGIN (IgExtensionDB, IgOIVDemoExtension, IgOIVDemoExtension::catalogLabel ());
