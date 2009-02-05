#include "test/Test.h" 
#include "classlib/iotools/PushBackInputStream.h"
#include "classlib/iotools/IOChannelInputStream.h"
#include "classlib/iobase/PipeCmd.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    char		 buf [3];
    PipeCmd		 pipe ("echo \"Hello world!\"", IOFlags::OpenRead);
    IOChannelInputStream in (&pipe);
    PushBackInputStream  s (&in);
    int			 pos = 0;

    while (IOSize n = s.read (buf, sizeof (buf)))
    {
	std::cout.write (buf, n);
	if ((pos += n) > 5 && pos < 10)
	    s.unread (buf, n);
    }
    return 0;
}
