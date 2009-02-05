#include "test/Test.h" 
#include "classlib/utils/Signal.h"
#include "classlib/utils/TimeInfo.h"
#include "classlib/iobase/SubProcess.h"
#include "classlib/utils/Argz.h"
#include <iostream>
#include <cstdlib>

using namespace lat;

static void inthandler (int, siginfo_t *, void *)
{
    std::cout << "** child: got SIGINT, sleeping\n";
    TimeInfo::sleep (1);
    std::cout << "** child: woke up\n";
}

int TEST(int argc, char **argv)
{
    Signal::handleFatal (argv[0]);

    if (argc == 1)
    {
	// NB: See SubProcessXX.cxx for the "0 + argv[0]" magic.
	std::cout << "main: starting subprocess\n";
	SubProcess s (Argz (0 + argv [0], "sub").argz ());

	TimeInfo::msleep (100);

	std::cout << "main: sending SIGINT\n";
	Signal::kill (s.pid (), SIGINT);

	TimeInfo::msleep (300);

	std::cout << "main: sending SIGINT\n";
	Signal::kill (s.pid (), SIGINT);

	TimeInfo::msleep (300);

	std::cout << "main: sending SIGINT\n";
	Signal::kill (s.pid (), SIGINT);

	TimeInfo::msleep (300);

	std::cout << "main: sending SIGINT\n";
	Signal::kill (s.pid (), SIGINT);

	TimeInfo::msleep (2000);

	std::cout << "main: sending SIGINT\n";
	Signal::kill (s.pid (), SIGINT);

	return s.wait ();
    }
    else if (! strcmp (argv [1], "sub"))
    {
	std::cout << "* child: ignoring SIGINT\n";
	Signal::ignore (SIGINT);

	TimeInfo::msleep (300);

	std::cout << "* child: handling SIGINT\n";
	Signal::handle (SIGINT, &inthandler);

	TimeInfo::msleep (300);

	std::cout << "* child: blocking SIGINT\n";
	Signal::block (SIGINT, true);

	TimeInfo::msleep (1000);

	std::cout << "* child: pending SIGINT? = " << Signal::pending (SIGINT) << "\n";
	std::cout << "* child: unblocking SIGINT\n";
	Signal::block (SIGINT, false);
    }
    else
	abort ();

    return EXIT_SUCCESS;
}
