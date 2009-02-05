#include "test/Test.h" 
#include "classlib/zip/CRC32.h"
#include "classlib/utils/Signal.h"
#include <iostream>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv [0]);


    static const char	sample [] = "Hello world!";
    CRC32		t;

    t.update (sample, sizeof (sample));
    std::cout << "CRC-32 checksum for <" << sample << "> is "
	      << t.value () << "\n";
    return 0;
}
