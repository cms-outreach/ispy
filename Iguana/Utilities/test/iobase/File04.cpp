#include "test/Test.h" 
#include "classlib/iobase/File.h"
#include "classlib/iobase/Filename.h"
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

    f.write (data, sizeof (data)-1);

    std::cout << "position = " << f.position () << std::endl
	      << "size = " << f.size () << std::endl
	      << "eof = " << f.eof () << std::endl;
    
    f.position (0);
    
    std::cout << "position = " << f.position () << std::endl
	      << "size = " << f.size () << std::endl
	      << "eof = " << f.eof () << std::endl;
    
    f.close ();
    return 0;
}
