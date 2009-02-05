#include "test/Test.h" 
#include "classlib/iotools/StdioStorage.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    const char		data [] = "hello world\n";
    StdioStorage	s ("test.out", "w");

    s.write (data, sizeof (data)-1);
    s.close ();
    return EXIT_SUCCESS;
}
