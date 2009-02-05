#include "test/Test.h" 
#include "classlib/iobase/InetServerSocket.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    InetServerSocket s (InetAddress (5678));
    return 0;
}

