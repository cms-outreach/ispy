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

    static const char aaa [] = "a/a/a/a/a/a/a/a/a";
    Filename n (Filename::thisdir ().asDirectory ());
    TempFile::dir (n);
    Filename sub (n, aaa);
    Filename::makedir (sub, 0755, true);
    std::cout << "temporary dir = " << n << std::endl
	      << "sub = " << sub << std::endl
	      << "exists? " << sub.exists () << std::endl
	      << "is dir? " << sub.isDirectory () << std::endl;
    // << "mode? " << n.status ().m_mode << std::endl;
    Filename::remove (n, true);
    return 0;
}
