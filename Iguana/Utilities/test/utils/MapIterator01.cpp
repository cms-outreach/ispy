#include "test/Test.h" 
#include "classlib/utils/MapIterator.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>
#include <iterator>
#include <algorithm>
#include <string>
#include <map>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);
    typedef std::map<int, std::string> TestMap;
    TestMap m;
    m [0] = "foo";
    m [-1] = "bar";
    m [2] = "zoo";
    m [-2] = "goo";

    std::copy (MapValueIterator<TestMap> (m.begin ()),
	       MapValueIterator<TestMap> (m.end ()),
	       std::ostream_iterator<std::string> (std::cout, "\n"));

    return 0;
}

