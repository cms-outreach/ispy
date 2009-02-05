#include "test/Test.h" 
#include "classlib/iotools/MemoryStorage.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    MemoryStorage s;
    return 0;
}
