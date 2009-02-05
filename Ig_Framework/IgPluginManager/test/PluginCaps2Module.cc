//<<<<<< INCLUDES                                                       >>>>>>

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

// Define normal entry point but nothing in it, and then capability entry.
DEFINE_IGUANA_MODULE ();

extern "C" void
IGUANA_CAPABILITIES (const char **&names, int &n)
{
    static const char *caps [] = {
	"IGUANA/Test Capabilities/Zoo",
	"IGUANA/Test Capabilities/Goo",
	"IGUANA/Test Capabilities/Bar" // invisible!
    };

    n = 2;
    names = caps;
}
