#include "test/Test.h" 
#include "classlib/utils/Log.h"
#include "classlib/utils/Signal.h"
#include <ostream>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);
    LOG (2, trace, LFgeneral, "foo\n");
    LOG (2, trace, LFgeneral, "test manips" << std::endl);
    return 0;
}
