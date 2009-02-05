#include "test/Test.h" 
#include "classlib/zip/TarOutputStream.h"
#include "classlib/zip/TarInputStream.h"
#include "classlib/zip/GZIPOutputStream.h"
#include "classlib/zip/GZIPInputStream.h"
#include "classlib/zip/TarMember.h"
#include "classlib/iobase/File.h"
#include "classlib/iobase/Filename.h"
#include "classlib/iobase/DirIterator.h"
#include "classlib/utils/ShellEnvironment.h"
#include "classlib/iotools/StorageOutputStream.h"
#include "classlib/iotools/StorageInputStream.h"
#include "classlib/iotools/BufferOutputStream.h"
#include "classlib/iotools/BufferInputStream.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>

using namespace lat;
int TEST(int, char **argv)
{
    Signal::handleFatal (argv [0]);

    Filename		dirName (Filename ("$SEAL/src/Foundation/classlib/zip/tests")
		        	 .substitute (ShellEnvironment ()));
    File		output ("test.tar.gz", IOFlags::OpenWrite
				| IOFlags::OpenCreate | IOFlags::OpenTruncate);
    StorageOutputStream	soutput (&output);
    BufferOutputStream	boutput (&soutput);
    GZIPOutputStream	zoutput (&boutput);
    TarOutputStream	t (&zoutput);

    std::cout << "creating compressed tar archive from directory " << dirName << std::endl; 
    for (DirIterator item (dirName), last; item != last; ++item)
    { 
	// Pick normal .cpp files only
	Filename fname (dirName, *item); 
        if (! fname.isRegular () || fname.extension () != "cpp")
	    continue;

	// Open input
	File		input (fname);
	char		buf [1024];
	IOSize		read;

	// Initialise new member
	TarMember	*outmember = new TarMember (*item);
	outmember->size (input.size ());
	std::cout << "*** ADD " << outmember->name ()
		       << " (" << outmember->size () << ")\n";

	// Write the member into the archive
	t.nextMember (outmember);
	while ((read = input.read (buf, sizeof (buf))))
	    t.write (buf, read);
	t.closeMember ();
    }

    t.close ();
    return 0;
}
