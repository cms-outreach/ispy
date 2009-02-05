#include "test/Test.h" 
#include "classlib/iobase/Filename.h"
#include "classlib/iobase/TempFile.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    std::cout <<   "tmpdir          = " << TempFile::tempdir ()
	      << "\ncwd             = " << Filename::cwd ()
	      << "\nnull            = " << Filename::null ()
	      << "\n";

    return 0;
}
