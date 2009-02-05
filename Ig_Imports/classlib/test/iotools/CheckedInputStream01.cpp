#include "test/Test.h" 
#include "classlib/zip/CheckedInputStream.h"
#include "classlib/zip/CRC32.h"
#include "classlib/iotools/MemoryStorage.h"
#include "classlib/iotools/StorageInputStream.h"
#include "classlib/utils/Signal.h"
#include <iostream>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv [0]);

    static const char	sample [] = "Hello world!";
    MemoryStorage	mem (sample, sizeof (sample));
    StorageInputStream	minput (&mem);
    CheckedInputStream	t (&minput, new CRC32);
    char		buf [3];
    IOSize		read;

    while ((read = t.read (buf, sizeof (buf))))
	std::cout.write (buf, read);

    std::cout << "\nCRC-32 checksum is " << t.checksum ()->value () << "\n";
    return 0;
}
