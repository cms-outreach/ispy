#include "test/Test.h" 
#include "classlib/zip/CPIOOutputStream.h"
#include "classlib/zip/CPIOInputStream.h"
#include "classlib/zip/CPIOMember.h"
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

    Filename		fileName (Filename ("$SEAL/src/Foundation/classlib/zip/tests/testSealZip.cpio")
		        	 .substitute (ShellEnvironment ()));

    File		input (fileName);
    StorageInputStream	sinput (&input);
    BufferInputStream	binput (&sinput);
    CPIOInputStream	tinput (&binput);

    File		output ("test.cpio", IOFlags::OpenWrite
				| IOFlags::OpenCreate | IOFlags::OpenTruncate);
    StorageOutputStream	soutput (&output);
    BufferOutputStream	boutput (&soutput);
    CPIOOutputStream	t (&boutput);

    CPIOMember		*inmember;
    char		buf [1024];
    IOSize		read;

    std::cout << "copying cpio archive " << fileName << std::endl;
    while ((inmember = tinput.nextMember ()))
    {
	CPIOMember *outmember = new CPIOMember (*inmember);
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
