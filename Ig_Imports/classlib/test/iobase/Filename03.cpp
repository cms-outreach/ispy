#include "test/Test.h" 
#include "classlib/iobase/Filename.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>
#include <vector>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    Filename			prefix ("r");
    std::vector<Filename>	vec;
    bool			exact;

    prefix.complete (vec, Filename::cwd ());
    std::cout << "\ncomplete (" << prefix << "):\n";
    for (size_t i = 0; i < vec.size (); ++i)
	std::cout << "  " << vec [i] << "\n";

    if (prefix.complete (Filename::cwd (), &exact))
    {
	std::cout << " ==> " << prefix
		  << (exact ? " (exact)\n" : "\n");
    }
    else
	std::cout << " ... no match\n";

    return 0;
}
