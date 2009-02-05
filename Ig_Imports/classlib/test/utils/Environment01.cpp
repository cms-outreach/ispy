#include "test/Test.h" 
#include "classlib/utils/Environment.h"
#include "classlib/utils/Signal.h"

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);
    return 0;
}
