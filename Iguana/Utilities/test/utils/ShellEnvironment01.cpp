#include "test/Test.h" 
#include "classlib/utils/ShellEnvironment.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    ShellEnvironment env;
    std::string t;

    std::cout << "$PATH = " << (env.get ("PATH", t) ? t : "") << std::endl
	      << "have $SHELL? " << env.has ("SHELL") << std::endl
	      << "have $TEST? " << env.has ("TEST") << std::endl;

    env.unset ("TEST");
    std::cout << "have $TEST? " << env.get ("TEST", t) << std::endl
	      << "$TEST = " << t << std::endl;

    env.set ("TEST", "foo");
    std::cout << "have $TEST? " << env.has ("TEST") << std::endl
	      << "$TEST = " << (env.get ("TEST", t), t) << std::endl;

    return 0;
}
