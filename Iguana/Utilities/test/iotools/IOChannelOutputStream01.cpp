#include "test/Test.h" 
#include "classlib/iotools/IOChannelOutputStream.h"
#include "classlib/iobase/PipeCmd.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    char		  buf [] = "Hello world!\n";
    PipeCmd		  pipe ("od -c", IOFlags::OpenWrite);
    IOChannelOutputStream out (&pipe);

    out.write (buf, sizeof (buf)-1);
    out.close ();
    return pipe.wait ();
}
