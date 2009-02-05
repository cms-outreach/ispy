#include "test/Test.h" 
#include "classlib/utils/DebugAids.h"
#include "classlib/utils/Signal.h"

using namespace lat;

static void h (void);
static void g (void);
static void f (void);

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    f ();
    return 0;
}

void f (void)
{
    g ();
}

void g (void)
{
    h ();
}


void h (void)
{
    ASSERT (false);
}

