#include "test/Test.h" 
#include "classlib/utils/Signal.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/utils/Log.h"
#include "classlib/utils/Error.h"
#include "classlib/iobase/LocalServerSocket.h"
#include "classlib/iobase/Filename.h"
#include <iostream>
#include <cerrno>
#include <cstdlib>

using namespace lat;

static char			*sockname = 0;
static LocalServerSocket	*sock = 0;

static void cleanup (void)
{
    try
    {
	if (sock)
	{
	    sock->close ();
	    delete sock;
	    sock = 0;
	}

	if (sockname)
	{
	    Filename::remove (sockname, false, true);
	    sockname = 0;
	}
    }
    catch (...)
    {
    }
}

static void interrupt (int, siginfo_t *, void *)
{
    cleanup ();
    std::cout << "\nInterrupted, exiting...\n";
    exit (0);
}

int TEST(int argc, char **argv)
{
    Signal::handleFatal (argv[0]);

    // Check parameters
    if (argc != 2)
    {
	// Print usage but don't make it an error to be qmtest happy
	std::cout << "usage: " << argv [0] << " socket-name\n";
	return EXIT_SUCCESS;
    }
    sockname = argv [1];

    if (Filename (sockname).exists ())
    {
	std::cerr << sockname << ": already exists\n";
	return EXIT_FAILURE;
    }

    Signal::handle (SIGINT, &interrupt);

    try
    {
	sock = new LocalServerSocket (sockname);
    }
    catch (Error &e)
    {
	std::cerr << argv [0] << ": error: " << e.explain () << std::endl;
	cleanup ();
	return EXIT_FAILURE;
    }

    while (true)
    {
	Socket *data = 0;
	try
	{
	    data = sock->accept ();
	}
	catch (Error &e)
	{
	    std::cout << "error: " << e.explain () << std::endl;
	    delete data;
	    cleanup ();
	    return EXIT_FAILURE;
	}

	LocalAddress peer = ((LocalSocket *) data)->peername ();
	if (strlen (peer.path ()))
	    std::cout << "connection established from `" << peer.path () << "'\n";
	else
	    std::cout << "connection established\n"; 
	std::cout.flush ();

	char	buffer [512];
	IOSize	n;

	while (true)
	{
	    try
	    {
		n = data->read (buffer, sizeof (buffer));
	    }
	    catch (Error &e)
	    {
		std::cout << "error: " << e.explain () << std::endl;
		delete data;
		break;
	    }

	    if (n == 0)
	    {
		std::cout << "connection closed\n";
		std::cout.flush ();
		data->close ();
		delete data;
		break;
	    }
	    else
		std::cout.write (buffer, n).flush ();
	}
    }

    std::cerr << "\n\noops\n"; // What happened? We shouldn't get here.
    return 1;
}
