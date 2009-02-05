#include "test/Test.h" 
#include "classlib/zip/BZDecompressor.h"
#include "classlib/utils/Signal.h"

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv [0]);

    BZDecompressor t;
    return 0;
}
