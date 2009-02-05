#include "test/Test.h" 
#include "classlib/iotools/StdOutputStream.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    static const char	sample [] = "Hello world!\n";
    StdOutputStream	t (&std::cout);

    for (size_t i = 0; i < sizeof (sample)-1; ++i)
	t.write (sample [i]);

    return 0;
}
