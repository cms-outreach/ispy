#include "test/Test.h" 
#include "classlib/iobase/DirIterator.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/utils/Signal.h"
#include <iostream>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    std::cout << "scanning files in `.' using for loop:\n";
    DirIterator first (".");
    DirIterator last;
    for ( ; first != last; ++first)
	std::cout << *first << '\n';

    return EXIT_SUCCESS;
}
