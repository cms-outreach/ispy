#include "test/Test.h" 
#include "classlib/zip/CheckedOutputStream.h"
#include "classlib/zip/CRC32.h"
#include "classlib/iotools/StdOutputStream.h"
#include "classlib/utils/Signal.h"
#include <iostream>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv [0]);

    static const char	sample [] = "Hello world!";
    StdOutputStream	soutput (&std::cout);
    CheckedOutputStream	t (&soutput, new CRC32);

    for (size_t i = 0; i < sizeof (sample); ++i)
	t.write (sample [i]);

    std::cout << "\nCRC-32 checksum is " << t.checksum ()->value () << "\n";
    return 0;
}
