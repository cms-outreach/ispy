#include "test/Test.h" 
#include "classlib/zip/ZipUtils.h"
#include "classlib/utils/Signal.h"

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv [0]);

    ZipUtils();
    return 0;
}
