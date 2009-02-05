#include "test/Test.h" 
#include "classlib/zip/ZipArchive.h"
#include "classlib/zip/ZipMember.h"
#include "classlib/iobase/File.h"
#include "classlib/iobase/Filename.h"
#include "classlib/utils/ShellEnvironment.h"
#include "classlib/iotools/InputStream.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv [0]);

    Filename inputFilename( Filename("test00.zip").substitute (ShellEnvironment ()));
    ZipArchive t (inputFilename, IOFlags::OpenRead);
    for (ZipArchive::Iterator i = t.begin (); i != t.end (); ++i)
    {
	std::cout << "*** BEGIN " << (*i)->name ()
		  << " (" << (*i)->size (ZipMember::COMPRESSED)
		  << "/" << (*i)->size (ZipMember::UNCOMPRESSED)
		  << ") ***\n";

	// FIXME: ZipMember should have streams configured by archive
	// (or input/output stream) and we should be able to read and
	// write through the member directly.
	InputStream	*is = t.input (*i);
	char		buf [1024];
	IOSize		n;

	while ((n = is->read (buf, sizeof (buf))))
	    std::cout.write (buf, n);

	is->close ();
	delete is;

	std::cout << "*** END\n";
    }

    return 0;
}
