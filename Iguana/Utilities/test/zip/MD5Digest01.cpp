#include "test/Test.h" 
#include "classlib/zip/MD5Digest.h"
#include "classlib/utils/Signal.h"
#include <iostream>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv [0]);

    static const char	sample [] = "Hello world!";
    MD5Digest		t;

    t.update (sample, sizeof (sample));
    std::cout << "MD5 for <" << sample << "> is "
	      << t.format () << "\n";
    return 0;
}
