#include "test/Test.h" 
#include "classlib/iobase/DirIterator.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <algorithm>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    std::cout << "scanning files in `.' using std::copy:\n";
    std::ostream_iterator<std::string> out (std::cout, "\n");
    std::copy (DirIterator ("."), DirIterator (), out);

    return EXIT_SUCCESS;
}
