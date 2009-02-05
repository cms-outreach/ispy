#include "test/Test.h" 
#include "classlib/zip/TarOutputStream.h"
#include "classlib/zip/TarInputStream.h"
#include "classlib/zip/BZIPOutputStream.h"
#include "classlib/zip/GZIPInputStream.h"
#include "classlib/zip/TarMember.h"
#include "classlib/iobase/File.h"
#include "classlib/iobase/Filename.h"
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

    Filename		fileName (Filename ("$SEAL/src/Foundation/classlib/zip/tests/testSealZip.tar.gz")
		        	 .substitute (ShellEnvironment ()));

    File		input (fileName);
    StorageInputStream	sinput (&input);
    BufferInputStream	binput (&sinput);
    GZIPInputStream	zinput (&binput);
    TarInputStream	tinput (&zinput);

    File		output ("test.tar.bz", IOFlags::OpenWrite
				| IOFlags::OpenCreate | IOFlags::OpenTruncate);
    StorageOutputStream	soutput (&output);
    BufferOutputStream	boutput (&soutput);
    BZIPOutputStream	zoutput (&boutput);
    TarOutputStream	t (&zoutput);

    TarMember		*inmember;
    char		buf [1024];
    IOSize		read;

    std::cout << "copying tar archive " << fileName << std::endl;
    while ((inmember = tinput.nextMember ()))
    {
	TarMember *outmember = new TarMember (*inmember);
	std::cout << "*** ADD " << outmember->name ()
		  << " (" << outmember->size () << ")\n";
	t.nextMember (outmember);
	while ((read = tinput.read (buf, sizeof (buf))))
	    t.write (buf, read);
	t.closeMember ();
    }

    t.close ();
    return 0;
}
