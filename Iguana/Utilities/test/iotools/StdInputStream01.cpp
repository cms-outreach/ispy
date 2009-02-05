#include "test/Test.h" 
#include "classlib/iotools/StdInputStream.h"
#include "classlib/iobase/PipeCmd.h"
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
	static const char sample [] = "Hello world!\n";
	const char *outargs [] = { argv [0], "output", 0 };
	PipeCmd out (outargs, IOFlags::OpenWrite);
	out.write (sample, sizeof (sample)-1);
	out.close ();
	return out.wait ();
    }
    else if (argc == 2 && ! strcmp (argv [1], "output"))
    {
	// Output with delay (FIXME: line buffering?)
	StdInputStream	s (&std::cin);
	char		data [2];

	while (IOSize n = s.read (data, sizeof (data)))
	{
	    std::cout.write (data, n);
	    std::cout.flush ();
	    TimeInfo::msleep (11);
	}

	return EXIT_SUCCESS;
    }
    else
	abort ();

    return EXIT_SUCCESS;
}
