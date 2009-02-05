#include "test/Test.h" 
#include "classlib/utils/Unwind.h"
#include "classlib/utils/Signal.h"

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    Unwind<int> p;
    return 0;
}
