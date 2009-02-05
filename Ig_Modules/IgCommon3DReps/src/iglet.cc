//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Framework/IgObjectBrowser/interface/IgCapabilityDB.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgBrowserMethods.h"
#include "Ig_Framework/IgPluginManager/interface/IgModuleDef.h"
#include "Ig_Modules/IgCommonData/interface/IgTwig.h"
#include "Ig_Modules/Ig3DModel/interface/Ig3DModel.h"
#include "Ig_Modules/Ig2DModel/interface/Ig2DModel.h"
#include "Ig_Modules/IgLegoModel/interface/IgLegoModel.h"
#include "Ig_Modules/IgRPhiModel/interface/IgRPhiModel.h"
#include "Ig_Modules/IgRZModel/interface/IgRZModel.h"
#include "Ig_Modules/IgTextModel/interface/IgTextModel.h"

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
DEFINE_IG_PLUGIN_NAME (IgCapabilityDB, IgBrowserMethods::key
		       (typeid (IgTextModel).name (),
			typeid (IgTwig).name ()));
DEFINE_IG_PLUGIN_NAME (IgCapabilityDB, IgBrowserMethods::key
		       (typeid (Ig3DModel).name (),
			typeid (IgTwig).name ()));
DEFINE_IG_PLUGIN_NAME (IgCapabilityDB, IgBrowserMethods::key
		       (typeid (Ig2DModel).name (),
			typeid (IgTwig).name ()));
DEFINE_IG_PLUGIN_NAME (IgCapabilityDB, IgBrowserMethods::key
		       (typeid (IgLegoModel).name (),
			typeid (IgTwig).name ()));
DEFINE_IG_PLUGIN_NAME (IgCapabilityDB, IgBrowserMethods::key
		       (typeid (IgRZModel).name (),
			typeid (IgTwig).name ()));
DEFINE_IG_PLUGIN_NAME (IgCapabilityDB, IgBrowserMethods::key
		       (typeid (IgRPhiModel).name (),
			typeid (IgTwig).name ()));

