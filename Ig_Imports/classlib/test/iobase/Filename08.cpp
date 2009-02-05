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

    Filename n (Filename::thisdir ().asDirectory ());
    TempFile::dir (n);

    std::cout << "temporary dir = " << n << std::endl
	      << "exists? " << n.exists () << std::endl
	      << "is dir? " << n.isDirectory () << std::endl;
    // << "mode? " << n.status ().m_mode << std::endl;
    Filename::remove (n);
    return 0;
}
