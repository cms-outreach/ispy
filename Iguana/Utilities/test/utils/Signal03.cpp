#include "test/Test.h" 
#include "classlib/utils/Signal.h"
#include "classlib/utils/TimeInfo.h"
#include "classlib/iobase/SubProcess.h"
#include "classlib/utils/Argz.h"
#include <iostream>
#include <cstdlib>

using namespace lat;

static void inthandler (int n, siginfo_t *, void *)
{
    std::cout << "** child: got " << n << ", sleeping\n";
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

	TimeInfo::msleep (500);

	std::cout << "main: sending SIGINT\n";
	Signal::kill (s.pid (), SIGINT);

	TimeInfo::msleep (300);

	std::cout << "main: sending SIGTERM\n";
	Signal::kill (s.pid (), SIGTERM);

	TimeInfo::msleep (2500);

	std::cout << "main: sending SIGUSR1\n";
	Signal::kill (s.pid (), SIGUSR1);

	std::cout << "main: sending SIGINT\n";
	Signal::kill (s.pid (), SIGINT);

	TimeInfo::msleep (2500);

	std::cout << "main: sending SIGUSR1\n";
	Signal::kill (s.pid (), SIGUSR1);

	return s.wait ();
    }
    else if (! strcmp (argv [1], "sub"))
    {
	std::cout << "* child: handling SIGINT + SIGTERM with all signals unblocked\n";
	sigset_t nix;
	sigemptyset (&nix);
	Signal::handle (SIGINT, &inthandler, &nix);
	Signal::handle (SIGTERM, &inthandler, &nix);

	TimeInfo::msleep (1000);

	std::cout << "* child: blocking SIGINT\n";
	Signal::block (SIGINT, true);

	std::cout << "* child: waiting for USR1 with default USR1 handler\n";
	Signal::wait (SIGUSR1);
	std::cout << "* child: got USR1\n";

	TimeInfo::msleep (1000);

	std::cout << "* child: pending SIGINT? = " << Signal::pending (SIGINT) << "\n";
	std::cout << "* child: unblocking SIGINT\n";
	Signal::block (SIGINT, false);

	std::cout << "* child: waiting for USR1 with USR1 ignored\n";
	Signal::ignore (SIGUSR1);
	bool gotit = Signal::wait (SIGUSR1, 0, 1000);
	std::cout << "* child: got USR1? " << gotit << "\n";
    }
    else
	abort ();

    return EXIT_SUCCESS;
}
