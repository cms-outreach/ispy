#include "test/Test.h" 
#include "classlib/iotools/FilterOutputStream.h"
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
    FilterOutputStream	  s (&out);

    s.write (buf, sizeof (buf)-1);
    s.close ();
    return 0;
}
