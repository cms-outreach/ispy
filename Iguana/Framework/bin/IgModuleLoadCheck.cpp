//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/utils/SharedLibrary.h"
#include "classlib/utils/ShellEnvironment.h"
#include "classlib/utils/Signal.h"
#include "classlib/utils/Error.h"
#include <iostream>
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
    for (int i = 1; i < argc; ++i)
    {
	SharedLibrary	*lib = 0;
	const char	*step = 0;

	try
	{
	    step = "load";
	    lib = SharedLibrary::load (argv [i]);

	    step = "unload";
	    if (lib && ! ShellEnvironment ().has ("IGUANA_KEEP_MODULES"))
		lib->release ();
	}
	catch (Error &e)
	{
	    std::cerr << argv [i]
		      << ": " << step << " failed:\n  "
		      << e.explain () << "\n";

	    return EXIT_FAILURE;
	}
    }

    return EXIT_SUCCESS;
}

