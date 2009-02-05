#include "test/Test.h" 
#include "classlib/utils/Regexp.h"
#include "classlib/utils/StringFormat.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/utils/Signal.h"
#include <iostream>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    VERIFY(StringFormat ("Testing `%1'").arg (10)
	   .value() == "Testing `10'");

    VERIFY(StringFormat ("Testing `%1'").arg (10, 4)
	   .value() == "Testing `  10'");

    VERIFY(StringFormat ("Testing `%1'").arg (10, -4)
	   .value() == "Testing `10  '");

    VERIFY(StringFormat ("Testing `%1'").arg (10, 0, 8)
	   .value() == "Testing `12'");

    VERIFY(StringFormat ("Testing `%1'").arg (10, 0, 16)
	   .value() == "Testing `a'");

    VERIFY(StringFormat ("Testing `%1'").arg (10, -4, 16)
	   .value() == "Testing `a   '");

    VERIFY(StringFormat ("Testing `%1%2'\n").arg ("10").arg ('%')
	   .value() == "Testing `10%'\n");

    VERIFY(StringFormat ("Testing `%1%2'\n").arg ("10", 4).arg ('%')
	   .value() == "Testing `  10%'\n");

    VERIFY(StringFormat ("Testing `%1%2'\n").arg ("10", -4).arg ('%')
	   .value() == "Testing `10  %'\n");

    VERIFY(StringFormat ("Testing `%1%2'\n").arg (10.0).arg ('%')
	   .value() == "Testing `10%'\n");

    VERIFY(StringFormat ("Testing `%1%2'\n").arg (10.0, 4).arg ('%')
	   .value() == "Testing `  10%'\n");

    VERIFY(StringFormat ("Testing `%1%2'\n").arg (10.0, -4).arg ('%')
	   .value() == "Testing `10  %'\n");

    VERIFY(StringFormat ("Testing `%1%2'\n").arg (10.0, 0, 'f', 2).arg ('%')
	   .value() == "Testing `10.00%'\n");

    VERIFY(StringFormat ("Testing `%1%2'\n").arg (10.0, 0, 'f', 2).arg ('%')
	   .value() == "Testing `10.00%'\n");

    VERIFY(StringFormat ("Testing `%1%2'\n").arg (10.0, 0, 'f', 2).arg ('%')
	   .value() == "Testing `10.00%'\n");

    VERIFY(StringFormat ("").arg (10)
	   .value() == "10");

    return 0;
}
