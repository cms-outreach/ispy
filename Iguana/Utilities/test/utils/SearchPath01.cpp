#include "test/Test.h" 
#include "classlib/utils/SearchPath.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/utils/Signal.h"
#include <algorithm>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <stdlib.h>

using namespace lat;

static void dosearch (const SearchPath &path, const std::string &name)
{
    SearchPath::const_iterator pos = path.search (name);
    if (pos == path.end ())
	std::cout << "`" << name << "' is not found";
    else
	std::cout << "`" << name << "' is in `" << *pos << "'";

    std::cout << " (expanded: `" << path.expand (name) << "')\n";
}

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);
    SearchPath path (getenv ("PATH"));

    std::cout << "$PATH is:\n";
    for (SearchPath::iterator i = path.begin (); i != path.end (); ++i)
	std::cout << "  " << *i << "\n";

    dosearch (path, "echo");
    dosearch (path, "gobbledygoo");
    return 0;
}
