#include "test/Test.h" 
#include "classlib/iotools/StorageInputStream.h"
#include "classlib/iotools/MemoryStorage.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    static const char	sample [] = "Hello world!\n";
    MemoryStorage	mem (sample, sizeof (sample)-1);
    StorageInputStream	t (&mem);
    char		buf [3];
    IOSize		read;

    while ((read = t.read (buf, sizeof (buf))))
	std::cout.write (buf, read);

    return 0;
}
