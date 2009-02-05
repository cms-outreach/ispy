#include "test/Test.h" 
#include "classlib/zip/SHA1Digest.h"
#include "classlib/utils/Signal.h"
#include <iostream>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv [0]);

    static const char	sample [] = "Hello world!";
    SHA1Digest		t;

    t.update (sample, sizeof (sample));
    std::cout << "SHA1 for <" << sample << "> is "
	      << t.format () << "\n";
    return 0;
}
