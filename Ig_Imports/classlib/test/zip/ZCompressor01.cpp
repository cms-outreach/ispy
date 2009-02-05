#include "test/Test.h" 
#include "classlib/zip/ZCompressor.h"
#include "classlib/utils/Signal.h"

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv [0]);

    ZCompressor t;
    return 0;
}
