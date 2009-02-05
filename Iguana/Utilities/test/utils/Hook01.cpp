#include "test/Test.h" 
#include "classlib/utils/Hook.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>

using namespace lat;

static int hfunc (int x, int y)
{ std::cout << "hfunc " << x << ", " << y << std::endl; return 3; }

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    Hook<int> h;
    std::cout << "h defined? " << h << std::endl;

    h = CreateHook (&hfunc, 1, 2);
    std::cout << "h defined? " << h << std::endl;
    int n = h ();
    std::cout << "h = " << n << std::endl;

    return 0;
}
