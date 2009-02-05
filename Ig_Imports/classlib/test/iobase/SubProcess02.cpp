#include "test/Test.h" 
#include "classlib/iobase/SubProcess.h"
#include "classlib/utils/Argz.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>

using namespace lat;

int TEST(int argc, char **argv)
{
    Signal::handleFatal (argv[0]);

    if (argc == 1)
    {
	// NB: The "0 + argv [0]" below is purely for disambiguation
	// for GCC 2.95.2.  It appears to parse the s1 declaration as
	// a function instead of object constructor and fails to
	// backtrack as specified in the standard.  Any number of
	// things can be done to it (e.g. cast), but it must be done
	// to make "argv" argument to Argz appear as an expression,
	// anything else is too late.
	std::cout << "main running\n" << std::flush;
	SubProcess s1 (Argz (0 + argv [0], "sub").argz ());
	int val = s1.wait ();
	std::cout << "child returned " << val << std::endl
		  << " normal? " << SubProcess::exitNormal (val) << "\n"
		  << " signal? " << SubProcess::exitBySignal (val) << "\n"
		  << " status  " << SubProcess::exitStatus (val) << "\n"
		  << " signr   " << SubProcess::exitSignal (val) << "\n";
    }
    else
	abort ();

    return EXIT_SUCCESS;
}
