#include "test/Test.h" 
#include "classlib/iobase/SubProcess.h"
#include "classlib/iobase/Filename.h"
#include "classlib/iobase/File.h"
#include "classlib/utils/Argz.h"
#include "classlib/utils/TimeInfo.h"
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
	IOChannel *nil = 0;
	File nul(Filename::null(), IOFlags::OpenRead | IOFlags::OpenWrite);
	SubProcess s1 (Argz (0 + argv [0], "output").argz (),
		       SubProcess::First | SubProcess::NoCloseError,
		       nil, nil, &nul);
	SubProcess s2 (Argz (0 + argv [0], "input").argz (),
		       SubProcess::Last | SubProcess::NoCloseError,
		       &s1, nil, &nul);

	TimeInfo::msleep (80);
	std::cout << "main, s1 ready = " << s1.done () << std::endl;
	std::cout << "main, s2 ready = " << s2.done () << std::endl;

	TimeInfo::msleep (4000);
	std::cout << "main, s1 ready = " << s1.done () << std::endl;
	std::cout << "main, s2 ready = " << s2.done () << std::endl;

	int val = s1.wait ();
	std::cout << "first child returned " << val << std::endl
		  << " normal? " << SubProcess::exitNormal (val) << "\n"
		  << " signal? " << SubProcess::exitBySignal (val) << "\n"
		  << " status  " << SubProcess::exitStatus (val) << "\n"
		  << " signr   " << SubProcess::exitSignal (val) << "\n";

	val = s2.wait ();
	std::cout << "last child returned " << val << std::endl
		  << " normal? " << SubProcess::exitNormal (val) << "\n"
		  << " signal? " << SubProcess::exitBySignal (val) << "\n"
		  << " status  " << SubProcess::exitStatus (val) << "\n"
		  << " signr   " << SubProcess::exitSignal (val) << "\n";
    }
    else if (argc == 2 && ! strcmp (argv [1], "output"))
    {
	// Output with delay
	int n = 0;
	static const char message []
	    = "Hello world!\nHello world!\n"
	    "1234567890abcdefghijkmnopqrstuvwxyz\n"
	    "easldjslk lasjd laks jd!\nH";
	for (const char *s = message; *s; ++s, ++n)
	{
	    TimeInfo::msleep (37);
	    std::cout << *s;
	    if ((*s & 1) && s[1])
		std::cout << *++s;
	    std::cout.flush ();
	    if (n % 5 == 0)
	        std::cerr << '#' << std::flush;
	}
    }
    else if (argc == 2 && ! strcmp (argv [1], "input"))
    {
	// Input with delay
	int n = 0;
	char ch;
	while (! std::cin.get (ch).eof ())
	{
	    std::cout << ch << std::flush;
	    TimeInfo::msleep (11);
	    if (++n % 5 == 0)
	        std::cerr << '@' << std::flush;
	}
    }
    else
	abort ();
    
    return EXIT_SUCCESS;
}
