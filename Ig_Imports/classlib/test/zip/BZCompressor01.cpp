#include "test/Test.h" 
#include "classlib/zip/BZCompressor.h"
#include "classlib/utils/Signal.h"

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv [0]);

    BZCompressor t;
    return 0;
}
