//<<<<<< INCLUDES                                                       >>>>>>

#include "SealBase/SharedLibrary.h"
#include "SealBase/ShellEnvironment.h"
#include "SealBase/Signal.h"
#include "SealBase/Error.h"
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

using namespace seal;
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
	    if (lib && ! ShellEnvironment ().has ("SEAL_KEEP_MODULES"))
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

