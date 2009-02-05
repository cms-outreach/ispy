#include "test/Test.h" 
#include "classlib/iotools/StdStorage.h"
#include "classlib/utils/Signal.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstdlib>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    const char		data [] = "hello world\n";
    std::fstream	file ("test.out", std::fstream::out
			      | std::fstream::trunc);
    StdStorage		s (&file);

    s.write (data, sizeof (data)-1);
    return EXIT_SUCCESS;
}
