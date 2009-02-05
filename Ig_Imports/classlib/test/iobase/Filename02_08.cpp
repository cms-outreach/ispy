#include "test/Test.h" 
#include "Filename02-00.h"

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    Filename x ("$PWD/bar.txt");
    dumpl ("base", x);
    dumpl ("expanded", x.expand ());
    dumpl ("substituted", x.substitute (ShellEnvironment ()));
    return 0;
}
