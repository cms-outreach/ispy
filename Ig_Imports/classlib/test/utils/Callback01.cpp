#include "test/Test.h" 
#include "classlib/utils/Callback.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>

using namespace lat;

static void cbfunc (int x, int y)
{ std::cout << "cbfunc " << x << ", " << y << std::endl; }


int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    Callback c;
    std::cout << "c defined? " << c << std::endl;

    c = CreateCallback (&cbfunc, 1, 2);
    std::cout << "c defined? " << c << std::endl;
    c ();

    return 0;
}

