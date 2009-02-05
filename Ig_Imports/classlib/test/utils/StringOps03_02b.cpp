#include "test/Test.h" 
#include "classlib/utils/StringOps.h"
#include "classlib/utils/Signal.h"
#include <iostream>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    std::cout << StringOps::join
	(StringOps::split (":abc:123", ':', StringOps::TrimEmpty), "\n")
	      << "\n";
    return 0;
}
