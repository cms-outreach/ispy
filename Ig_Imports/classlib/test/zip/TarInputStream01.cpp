#include "test/Test.h" 
#include "classlib/zip/TarInputStream.h"
#include "classlib/zip/GZIPInputStream.h"
#include "classlib/zip/TarMember.h"
#include "classlib/iobase/File.h"
#include "classlib/iobase/Filename.h"
#include "classlib/iotools/StorageInputStream.h"
#include "classlib/iotools/BufferInputStream.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv [0]);

    File		input ("test.tar.gz");
    StorageInputStream	sinput (&input);
    BufferInputStream	binput (&sinput);
    GZIPInputStream	zinput (&binput);
    TarInputStream	t (&zinput);
    TarMember		*member;
    char		buf [1024];
    IOSize		read;

    while ((member = t.nextMember ()))
    {
	std::cout << "*** BEGIN " << member->name ()
		  << " (" << member->size () << ") ***\n";

	while ((read = t.read (buf, sizeof (buf))))
	    std::cout.write (buf, read);

	std::cout << "*** END\n";
    }

    return 0;
}
