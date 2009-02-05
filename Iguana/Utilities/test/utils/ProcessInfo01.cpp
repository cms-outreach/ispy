#include "test/Test.h" 
#include "classlib/utils/ProcessInfo.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    std::cout << "process information\n"
	      << "  pid       : " << ProcessInfo::pid () << std::endl
	      << "  ppid      : " << ProcessInfo::ppid () << "\n\n";

    for (char **args = ProcessInfo::argv (); args && *args; ++args, ++argv)
	std::cout << "args <" << *args << ">; argv <" << *argv << ">\n";

    std::cout << "\nfull program name: " << ProcessInfo::fullProgramName () << "\n";
    std::cout << "short program name: " << ProcessInfo::shortProgramName () << "\n";

    return 0;
}
