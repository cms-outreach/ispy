#include "test/Test.h" 
#include "classlib/zip/ZipArchive.h"
#include "classlib/zip/ZipMember.h"
#include "classlib/iobase/File.h"
#include "classlib/iobase/Filename.h"
#include "classlib/utils/ShellEnvironment.h"
#include "classlib/iotools/InputStream.h"
#include "classlib/iotools/OutputStream.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv [0]);

    Filename inputFilename( Filename("test00.zip").substitute (ShellEnvironment ()));
    ZipArchive in (inputFilename, IOFlags::OpenRead);

    Filename outputFilename( Filename("test2.zip").substitute (ShellEnvironment ()));
    ZipArchive t  (outputFilename, IOFlags::OpenWrite
		   | IOFlags::OpenCreate | IOFlags::OpenTruncate);

    for (ZipArchive::Iterator i = in.begin (); i != in.end (); ++i)
    {
	// Unlike with Zip*Stream, we have direct access to both
	// the local and local extra data immediately since the
	// archive has random access.  This copies both the basic
	// data and the global data.
	ZipMember *copy = new ZipMember (**i);

	std::cout << "*** ADD " << copy->name ()
		  << " (" << copy->size (ZipMember::UNCOMPRESSED)
		  << ")\n" << std::flush;

	// FIXME: ZipMember should have streams configured by archive
	// (or input/output stream) and we should be able to read and
	// write through the member directly.
	InputStream	*is = in.input (*i);
	OutputStream	*os = t.output (copy);
	char		buf [1024];
	IOSize		n;

	while ((n = is->read (buf, sizeof (buf))))
	    os->write (buf, n);

	is->close ();
	os->close ();
	delete is;
	delete os;
    }

    t.close ();
    return 0;
}
