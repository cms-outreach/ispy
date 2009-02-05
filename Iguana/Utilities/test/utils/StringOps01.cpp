#include "test/Test.h" 
#include "classlib/utils/StringOps.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/utils/Signal.h"
#include <iostream>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);
    
    {
	StringList l;
	l.push_back ("abc");
	l.push_back ("123");
	VERIFY(StringOps::join (l, ":") == "abc:123");
    }
    
    return 0;
}
