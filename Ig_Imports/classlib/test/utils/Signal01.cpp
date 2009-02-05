#include "test/Test.h" 
#include "classlib/utils/Signal.h"
#include "classlib/utils/TimeInfo.h"
#include <iostream>
#include <cstdlib>

using namespace lat;

static void inthandler (int, siginfo_t *, void *)
{
    std::cout << "  Got SIGINT\n";
    TimeInfo::sleep (1);
    std::cout << "  Woke up\n";
}

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    std::cout << "Ignoring SIGINT\n";
    Signal::ignore (SIGINT);

    std::cout << "Raising SIGINT\n";
    Signal::raise (SIGINT);

    std::cout << "Handling SIGINT\n";
    Signal::handle (SIGINT, &inthandler);

    std::cout << "Raising SIGINT\n";
    Signal::raise (SIGINT);

    std::cout << "Blocking SIGINT\n";
    Signal::block (SIGINT, true);

    std::cout << "Raising SIGINT\n";
    Signal::raise (SIGINT);

    std::cout << "Pending SIGINT? = " << Signal::pending (SIGINT) << "\n";

    std::cout << "Unblocking SIGINT\n";
    Signal::block (SIGINT, false);
    
    std::cout << "Raising SIGINT\n";
    Signal::raise (SIGINT);

    return EXIT_SUCCESS;
}
