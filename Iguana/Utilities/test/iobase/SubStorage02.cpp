#include "test/Test.h" 
#include "classlib/iotools/SubStorage.h"
#include "classlib/iotools/MemoryStorage.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    MemoryStorage base;
    for (int i = 0; i < 1000; ++i)
	base.write ('a' + i % 27);

    base.rewind ();

    SubStorage	s (&base, 100, 100);
    char	buf [3];
    IOSize	read;

    s.position (71, Storage::SET);
    while ((read = s.read (buf, sizeof (buf))))
	std::cout.write (buf, read);

    return EXIT_SUCCESS;
}
