#include "test/Test.h" 
#include "classlib/zip/ZipInputStream.h"
#include "classlib/zip/ZipMember.h"
#include "classlib/zip/ZError.h"
#include "classlib/iobase/File.h"
#include "classlib/iobase/Filename.h"
#include "classlib/utils/ShellEnvironment.h"
#include "classlib/iotools/StorageInputStream.h"
#include "classlib/iotools/BufferInputStream.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv [0]);

    Filename inputFilename( Filename("test00.zip").substitute (ShellEnvironment ()));
    File		input (inputFilename);
    StorageInputStream	sinput (&input);
    BufferInputStream	binput (&sinput);
    ZipInputStream	t (&binput);
    ZipMember		*member;
    char		buf [1024];
    
    while ((member = t.nextMember ()))
    {
	std::cout << member->name ()
		  << " (" << member->size (ZipMember::COMPRESSED)
		  << "/" << member->size (ZipMember::UNCOMPRESSED)
		  << "): " << std::flush;

	while (t.read (buf, sizeof (buf)))
	    std::cout << "." << std::flush;

	std::cout << "\n";
    }

    return 0;
}
