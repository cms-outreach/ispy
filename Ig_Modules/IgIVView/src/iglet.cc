//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgIVView/interface/IgIVView.h"
#include "Ig_Modules/IgStudio/interface/IgViewDB.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgBrowserMethods.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgCapabilityDB.h"
#include "Ig_Framework/IgPluginManager/interface/IgModuleDef.h"
#include "Ig_Modules/Ig3DModel/interface/Ig3DModel.h"
#include "Ig_Modules/IgIVView/interface/IgSoTwig.h"

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
DEFINE_IGUANA_PLUGIN (IgViewDB, IgIVView, "Open Inventor View");
DEFINE_IG_PLUGIN_NAME (IgCapabilityDB, IgBrowserMethods::key
		       (typeid (Ig3DModel).name (),
			typeid (IgSoTwig).name ()));

