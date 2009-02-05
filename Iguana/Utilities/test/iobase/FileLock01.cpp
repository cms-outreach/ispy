#include "test/Test.h" 
#include "classlib/iobase/File.h"
#include "classlib/iobase/FileLock.h"
#include "classlib/iobase/Filename.h"
#include "classlib/utils/Signal.h"
#include "classlib/utils/DebugAids.h"
#include <iostream>
#include <iomanip>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    FileLock l;
    return 0;
}
