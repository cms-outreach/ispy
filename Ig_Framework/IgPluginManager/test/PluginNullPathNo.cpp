//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Framework/IgPluginManager/interface/IgPluginManager.h"
#include "classlib/utils/ShellEnvironment.h"
#include "classlib/iobase/SubProcess.h"
#include "classlib/utils/Signal.h"
#include "classlib/utils/Argz.h"
#include <cstdlib>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

using namespace lat;
int main (int argc, char **argv)
{
    Signal::handleFatal (argv [0]);

    if (argc == 1)
    {
	ShellEnvironment env;
	env.set ("IGUANA_PLUGINS", "");
	SubProcess sub (Argz (0+argv [0], "sub").argz ());
	return sub.wait ();
    }
    else if (argc == 2 && ! strcmp (argv [1], "sub"))
    {
	IgPluginManager::get ()->initialise ();
	return EXIT_SUCCESS;
    }
    else
	abort ();
}
