#include "test/Test.h" 
#include "classlib/iobase/File.h"
#include "classlib/iobase/Filename.h"
#include "classlib/utils/Time.h"
#include "classlib/utils/Signal.h"
#include "classlib/utils/DebugAids.h"
#include <iostream>
#include <iomanip>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    const char	data [] = "hello world\n";
    File	f ("test.out", IOFlags::OpenWrite | IOFlags::OpenCreate);
    Time	ctime, mtime, atime;

    f.write (data, sizeof (data)-1);
    f.times (&ctime, &mtime, &atime);

    std::cout << "ctime = " << ctime << std::endl
	      << "mtime = " << mtime << std::endl
	      << "atime = " << atime << std::endl;

    f.close ();
    return 0;
}
