#include "test/Test.h" 
#include "classlib/iobase/File.h"
#include "classlib/iobase/FileMapping.h"
#include "classlib/iobase/Filename.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    File	*f = new File ("map.out",
			       IOFlags::OpenRead
			       | IOFlags::OpenWrite
			       | IOFlags::OpenCreate);
    f->write ("Hello world", 11);

    FileMapping	fmap (f);
    IOBuffer	section = fmap.map ();

    std::cout << "data = <";
    std::cout.write ((char *) section.data (), section.size ());
    std::cout << ">\n";

    return 0;
}
