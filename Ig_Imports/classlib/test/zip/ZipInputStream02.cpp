#include "test/Test.h" 
#include "classlib/zip/ZipInputStream.h"
#include "classlib/zip/ZipMember.h"
#include "classlib/utils/ShellEnvironment.h"
#include "classlib/iobase/File.h"
#include "classlib/iobase/FileMapping.h"
#include "classlib/iobase/Filename.h"
#include "classlib/iotools/MemoryStorage.h"
#include "classlib/iotools/StorageInputStream.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv [0]);

    Filename inputFilename( Filename("test00.zip").substitute (ShellEnvironment ()));
    File    *f  = new File(inputFilename);
    FileMapping		m (f);
    MemoryStorage	ms (m.map ());
    StorageInputStream	sinput (&ms);
    ZipInputStream	t (&sinput);
    ZipMember		*member;
    
    while ((member = t.nextMember ()))
	std::cout << std::setw (7)
		  << member->size (ZipMember::UNCOMPRESSED) << " "
		  << member->method () << ":" << member->level () << " "
		  << std::setw (7)
		  << member->size (ZipMember::COMPRESSED) << " "
		  << std::setw (3)
		  << (! member->size (ZipMember::UNCOMPRESSED) ? 0
		      : int (100. - 100. * member->size (ZipMember::COMPRESSED)
			     / member->size (ZipMember::UNCOMPRESSED) + .5))
		  << "% "
		  << member->time ().format (true, "  %Y-%m-%d %H:%M:%S ")
		  << std::hex << std::setw (8) << member->crc32 () << std::dec
		  << " "
		  << member->name () << "\n";

    return 0;
}
