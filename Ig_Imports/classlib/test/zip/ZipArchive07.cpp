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
    for (ZipArchive::Iterator i = in.begin (); i != in.end (); ++i)
    {
	std::cout << "*** READ STORED " << (*i)->name ()
		  << " (" << (*i)->size (ZipMember::UNCOMPRESSED)
		  << ")\n" << std::flush;

	Storage *s = in.openStored (*i);
	char	buf [1024];
	IOSize	n;

	s->position (-33, Storage::END);
	while ((n = s->read (buf, sizeof (buf))))
	    std::cout.write (buf, n);

	std::cout << "\n";
	delete s;
    }

    in.close ();
    return 0;
}
