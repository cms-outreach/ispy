#include "test/Test.h" 
#include "Filename02-00.h"

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    Filename x (".");
    dump ("base", x);
    dump ("expanded", x.expand ());
    dump ("substituted", x.substitute (ShellEnvironment ()));
    return 0;
}
