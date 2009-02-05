#include "test/Test.h" 
#include "classlib/iotools/PipedInputStream.h"
#include "classlib/iotools/PipedOutputStream.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    // FIXME: This should use threads or something, this has a dead-lock
    // danger.  So write only little bits of data, at least on unixen
    // that will always work.
    static const char	sample [] = "Hello World!";
    PipedOutputStream	out;
    PipedInputStream	in (&out);
    char		buf [3];
    IOSize		n;

    out.write (sample, sizeof (sample)-1);
    out.close ();

    while ((n = in.read (buf, sizeof (buf))))
    {
	std::cout << "<";
	std::cout.write (buf, n);
	std::cout << ">" << std::flush;
    }

    std::cout << std::endl;
    return 0;
}
