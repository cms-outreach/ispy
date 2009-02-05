#include "test/Test.h" 
#include "classlib/zip/TarInputStream.h"
#include "classlib/zip/BZIPInputStream.h"
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

    File		input ("test.tar.bz");
    StorageInputStream	sinput (&input);
    BufferInputStream	binput (&sinput);
    BZIPInputStream	zinput (&binput);
    TarInputStream	t (&zinput);
    TarMember		*member;
    
    while ((member = t.nextMember ()))
	std::cout << std::setw (8)
		  << member->size () << " "
		  << (char) member->type () << " "
		  << std::setw (7) << std::oct << member->mode () << std::dec << " "
		  << member->uname () << ":"
		  << member->gname () << "/"
		  << member->uid () << ":"
		  << member->gid () << " "
	    // FIXME: mtime should be a Time
		  << Time (member->mtime(),0).format (true, " %Y-%m-%d %H:%M ")
		  << member->name () << std::endl;

    return 0;
}
