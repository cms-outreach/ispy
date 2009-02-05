#include "test/Test.h" 
#include "classlib/utils/StringFormat.h"
#include "classlib/utils/Signal.h"
#include <iostream>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    std::cout << StringFormat ("Testing `%1%2'\n").arg (10.0, 4).arg ('%');
    return 0;
}
