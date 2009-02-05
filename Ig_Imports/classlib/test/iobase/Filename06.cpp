#include "test/Test.h" 
#include "classlib/iobase/Filename.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    // FIXME: status, setStatus, times

    // << "\ndrive           = " << x.drive ()
    // << "\nshare           = " << x.share ()

    // FIXME: append
	
    // FIXME: findBackupName

    // FIXME: remove
    // FIXME: rename
    // FIXME: move
    // FIXME: copy

    return 0;
}
