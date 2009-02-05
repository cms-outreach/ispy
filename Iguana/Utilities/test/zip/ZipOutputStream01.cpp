#include "test/Test.h" 
#include "classlib/zip/ZipOutputStream.h"
#include "classlib/zip/ZipInputStream.h"
#include "classlib/zip/ZipMember.h"
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

    Filename		fileName (Filename ("$SEAL/src/Foundation/classlib/zip/tests/testSealZip.zip")
		        	 .substitute (ShellEnvironment ()));

    File		input (fileName);
    StorageInputStream	sinput (&input);
    BufferInputStream	binput (&sinput);
    ZipInputStream	tinput (&binput);

    File		output ("test01.zip", IOFlags::OpenWrite
				| IOFlags::OpenCreate | IOFlags::OpenTruncate);
    StorageOutputStream	soutput (&output);
    BufferOutputStream	boutput (&soutput);
    ZipOutputStream	t (&boutput);

    ZipMember		*inmember;
    char		buf [1024];
    IOSize		read;

    std::vector<ZipMember *> outmembers; // FIXME: Stream should maintain...
    std::vector<ZipMember *> inmembers;  // FIXME: Stream should maintain...

    std::cout << "copying zip archive " << fileName << std::endl;
    while ((inmember = tinput.nextMember ()))
    {
	ZipMember *outmember = new ZipMember (*inmember);
	inmembers.push_back (inmember);
	outmembers.push_back (outmember);

	std::cout << "*** ADD " << outmember->name ()
		  << " (" << outmember->size (ZipMember::UNCOMPRESSED) << ")\n";
	t.nextMember (outmember);
	while ((read = tinput.read (buf, sizeof (buf))))
	    t.write (buf, read);
	t.closeMember ();
    }

    // FIXME: This should really be simpler to do, but at the moment
    //   that's difficult to achieve.  The input stream won't know
    //   the central directory extra data until it's seen it, and
    //   we can't share the members since the compression settings
    //   aren't unambiguous so the compressed size might change.
    //   The output side might change various fields and cause the
    //   input side to bail out when it gets to the central dir.
    for (size_t i = 0; i < inmembers.size (); ++i)
    {
	void		*extra = 0;
	IOSize		len = 0;
	inmembers [i]->extra (ZipMember::CENTRAL, &extra, &len);
	outmembers [i]->extra (ZipMember::CENTRAL, extra, len);
    }

    t.close ();
    return 0;
}
