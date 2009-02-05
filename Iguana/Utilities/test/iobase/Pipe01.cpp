#include "test/Test.h" 
#include "classlib/iobase/Pipe.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    Pipe p;
    return 0;
}

