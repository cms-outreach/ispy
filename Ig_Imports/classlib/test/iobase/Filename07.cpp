#include "test/Test.h" 
#include "classlib/iobase/Filename.h"
#include "classlib/iobase/TempFile.h"
#include "classlib/iobase/File.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    static const char	sample [] = "Hello World\n";
    Filename		n (Filename::thisdir ().asDirectory ());
    File		*f = TempFile::file (n);

    std::cout << "temporary file = " << n << std::endl;
    f->write (sample, sizeof (sample)-1);
    f->close ();
    Filename::remove (n);
    return 0;
}
