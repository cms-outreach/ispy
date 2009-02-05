#include "test/Test.h" 
#include "classlib/iotools/FilterInputStream.h"
#include "classlib/iotools/IOChannelInputStream.h"
#include "classlib/iobase/PipeCmd.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    char		 buf [128];
    PipeCmd		 pipe ("echo 'Hello world!'", IOFlags::OpenRead);
    IOChannelInputStream in (&pipe);
    FilterInputStream	 s (&in);
    IOSize		 n = s.read (buf, sizeof (buf));

    std::cout.write (buf, n);
    return 0;
}
