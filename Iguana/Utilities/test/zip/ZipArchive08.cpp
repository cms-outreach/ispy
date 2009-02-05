#include "test/Test.h" 
#include "classlib/zip/ZipArchive.h"
#include "classlib/zip/ZipMember.h"
#include "classlib/iobase/File.h"
#include "classlib/iobase/Filename.h"
#include "classlib/utils/ShellEnvironment.h"
#include "classlib/iotools/InputStream.h"
#include "classlib/iotools/OutputStream.h"
#include "classlib/iotools/InputStreamBuf.h"
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
	std::cout << "*** READ " << (*i)->name ()
		  << " (" << (*i)->size (ZipMember::UNCOMPRESSED)
		  << "): " << std::flush;

	InputStream	*is = in.input (*i);
	InputStreamBuf	isbuf (is);
	std::istream	istd (&isbuf);
	std::string	word;

	while (istd >> word)
	    std::cout << "." << std::flush;

	std::cout << "\n";
	is->close ();
	delete is;
    }

    in.close ();
    return 0;
}
