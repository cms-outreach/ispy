//<<<<<< INCLUDES                                                       >>>>>>

#include "PluginFactoryArgsType.h"
#include "PluginFactoryArgs.h"
#include "Ig_Framework/IgPluginManager/interface/IgModuleDef.h"
#include <iostream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

static bool onload (void) {
    std::cout << "args test library was loaded\n";
    return true;
}

extern "C" void IGUANA_CAPABILITIES (const char **&names, int &)
{ static const char *entries [] = { "Sample", 0 }; names = entries; }

DEFINE_IGUANA_MODULE ();
DEFINE_IGUANA_PLUGIN (TestPluginFactoryArgs0, TestPluginType0, "Sample");
DEFINE_IGUANA_PLUGIN (TestPluginFactoryArgs1, TestPluginType1, "Sample");
DEFINE_IGUANA_PLUGIN (TestPluginFactoryArgs2, TestPluginType2, "Sample");
DEFINE_IGUANA_PLUGIN (TestPluginFactoryArgs3, TestPluginType3, "Sample");
DEFINE_IGUANA_PLUGIN (TestPluginFactoryArgs4, TestPluginType4, "Sample");

static bool autoload = onload ();
