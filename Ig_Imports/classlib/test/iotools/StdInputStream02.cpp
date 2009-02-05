#include "test/Test.h" 
#include "classlib/iotools/StdInputStream.h"
#include "classlib/utils/TimeInfo.h"
#include "classlib/utils/Signal.h"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace lat;
static const char *testText = "Hello, this is a test of SealIoTools::StdinputStream\n"
			      "If you see this message the test is OK !\n";

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    // Output with delay (FIXME: line buffering?)
    std::istringstream	input (testText);
    StdInputStream	s (&input);
    char		data [2];

    while (IOSize n = s.read (data, sizeof (data)))
    {
	std::cout.write (data, n);
	std::cout.flush ();
	TimeInfo::msleep (11);
    }

    return EXIT_SUCCESS;
}
