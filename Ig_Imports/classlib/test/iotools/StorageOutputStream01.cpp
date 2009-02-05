#include "test/Test.h" 
#include "classlib/iotools/StorageOutputStream.h"
#include "classlib/iotools/MemoryStorage.h"
#include "classlib/utils/TimeInfo.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    static const char	sample [] = "Hello world!\n";
    MemoryStorage	mem;
    StorageOutputStream	s (&mem);
    s.write (sample, sizeof (sample)-1);
    s.close ();

    mem.rewind ();
    while (! mem.eof ())
    {
	std::cout << (char) ((Storage &) mem).read () << std::flush;
	TimeInfo::msleep (42);
    }

    return EXIT_SUCCESS;
}
