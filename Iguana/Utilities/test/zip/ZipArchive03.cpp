#include "test/Test.h" 
#include "classlib/zip/ZipArchive.h"
#include "classlib/zip/ZipMember.h"
#include "classlib/iobase/File.h"
#include "classlib/iobase/Filename.h"
#include "classlib/utils/ShellEnvironment.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>

using namespace lat;

static int sizeRatio (ZipMember *m)
{
    if (m->size (ZipMember::UNCOMPRESSED) == 0)
	return 0;

    return int (100. - (100. * m->size (ZipMember::COMPRESSED)
			/ m->size (ZipMember::UNCOMPRESSED))
		+ .5);
}

int TEST(int, char **argv)
{
    Signal::handleFatal (argv [0]);

    Filename inputFilename( Filename("test00.zip").substitute (ShellEnvironment ()));
    ZipArchive t (inputFilename, IOFlags::OpenRead);
    for (ZipArchive::Iterator i = t.begin (); i != t.end (); ++i)
	std::cout << std::setw (7) << (*i)->size (ZipMember::UNCOMPRESSED)
		  << " " << (*i)->method () << ":" << (*i)->level () << " "
		  << std::setw (7) << (*i)->size (ZipMember::COMPRESSED) << " "
		  << std::setw (3) << sizeRatio (*i) << "% "
		  << (*i)->time ().format (true, "  %Y-%m-%d %H:%M:%S ")
		  << std::hex << std::setw (8) << (*i)->crc32 () << std::dec
		  << " " << (*i)->name () << "\n";

    return 0;
}
