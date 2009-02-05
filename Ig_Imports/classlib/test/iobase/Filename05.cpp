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

    Filename			pat ("[a-p]*test*");
    std::vector<Filename>	vec;

    pat.glob (vec, Filename::cwd ());
    std::cout << "\nglob (" << pat << "):\n";
    for (size_t i = 0; i < vec.size (); ++i)
	std::cout << "  " << vec [i] << "\n";

    // FIXME: remove
    // FIXME: rename
    // FIXME: move
    // FIXME: copy

    return 0;
}









