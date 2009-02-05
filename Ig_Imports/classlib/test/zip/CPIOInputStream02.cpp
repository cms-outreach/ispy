#include "test/Test.h" 
#include "classlib/zip/CPIOInputStream.h"
#include "classlib/zip/CPIOMember.h"
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

    File		input ("test.cpio");
    StorageInputStream	sinput (&input);
    BufferInputStream	binput (&sinput);
    CPIOInputStream	t (&binput);
    CPIOMember		*member;
    
    while ((member = t.nextMember ()))
	std::cout << std::setw (8)
		  << member->size () << " "
		  << std::setw (7) << std::oct << member->mode () << std::dec << " "
		  << member->uid () << ":" << member->gid () << " "
		  << member->rdev () << ":" << member->dev () << ":" << member->ino () << " "
		  << member->nlink () << " "
	    // FIXME: mtime should be a Time
		  << Time (member->mtime(),0).format (true, " %Y-%m-%d %H:%M ")
		  << member->name () << std::endl;

    return 0;
}
