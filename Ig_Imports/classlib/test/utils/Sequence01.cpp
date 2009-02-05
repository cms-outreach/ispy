#include "test/Test.h" 
#include "classlib/utils/Sequence.h"
#include "classlib/utils/Signal.h"
#include <iostream>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    Sequence<int> s;
    std::cout << "s = [" << s.limits ().low ()
	      << ", " << s.limits ().high () << "]\n"
	      << "current = " << s.current () << "\n";

    s.next ();

    std::cout << "current = " << s.current () << "\n";
    return 0;
}
