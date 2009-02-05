#include "classlib/utils/SearchPath.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/utils/Signal.h"
#include <algorithm>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <stdlib.h>

using namespace lat;

int main (int, char **argv)
{
    Signal::handleFatal (argv[0]);
    SearchPath path ("");

    std::cout << "Empty path has " << path.size () << " elements\n";
    return 0;
}
