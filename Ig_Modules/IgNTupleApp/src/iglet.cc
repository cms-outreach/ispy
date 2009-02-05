//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgNTupleApp/interface/IgNTupleAppMain.h"
#include "Ig_Modules/IgNTupleApp/interface/IgNTupleAppSetup.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgExtensionDB.h"
#undef emit
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
DEFINE_IGUANA_PLUGIN (IgExtensionDB, IgNTupleAppMain, IgNTupleAppMain::catalogLabel ());
DEFINE_IGUANA_PLUGIN (IgExtensionDB, IgNTupleAppSetup, IgNTupleAppSetup::catalogLabel ());

