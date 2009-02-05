#include "test/Test.h" 
#include "classlib/iobase/DirIterator.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <algorithm>

using namespace lat;

static void dump_file (const std::string &file)
{
    std::cout << file << "\n";
}

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    std::cout << "scanning files in `.' using std::for_each:\n";
    std::for_each (DirIterator ("."), DirIterator (), dump_file);

    return EXIT_SUCCESS;
}
