#include "test/Test.h" 
#include "classlib/zip/ZDecompressor.h"
#include "classlib/utils/Signal.h"

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv [0]);

    ZDecompressor t;
    return 0;
}
