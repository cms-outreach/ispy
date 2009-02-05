#include "test/Test.h" 
#include "classlib/utils/UUID.h"
#include "classlib/utils/Signal.h"
#include <iostream>

using namespace lat;
typedef lat::UUID TestUUID;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);
    std::cout << TestUUID () << std::endl;
    return 0;
}
