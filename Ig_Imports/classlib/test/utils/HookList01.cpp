#include "test/Test.h" 
#include "classlib/utils/HookList.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>

using namespace lat;

static bool hfunc1 (int x, int y)
{ std::cout << "hfunc1 " << x << ", " << y << std::endl; return false; }

static bool hfunc2 (const char *foo)
{ std::cout << "hfunc2 " << (foo ? foo : "NONE!") << std::endl; return !foo; }

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    HookList h;
    h.push_back (CreateHook (&hfunc1, 1, 2));
    h.push_back (CreateHook (&hfunc2, (const char *) "yo!"));
    h.push_back (CreateHook (&hfunc2, (const char *) 0));
    h.push_back (CreateHook (&hfunc2, (const char *) "you won't see this!"));
    h.call ();
    return 0;
}
