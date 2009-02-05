#include "test/Test.h" 
#include "classlib/iobase/File.h"
#include "classlib/iobase/FileMapping.h"
#include "classlib/iobase/Filename.h"
#include "classlib/utils/SystemInfo.h"
#include "classlib/utils/Signal.h"
#include "classlib/utils/DebugAids.h"
#include <iostream>
#include <iomanip>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    IOSize	n = SystemInfo::pagesize () * 10;
    FileMapping	null (n);
    IOBuffer	sect = null.map (FileMapping::ProtRead
				 | FileMapping::ProtWrite,
				 FileMapping::MapPrivate);
    char	*data = (char *) sect.data ();

    ASSERT (sect.size () == n);

    for (IOSize i = 0; i < sect.size (); ++i)
    {
	ASSERT (data [i] == 0);
	data [i] = '0' + i % 10;
    }

    return 0;
}

