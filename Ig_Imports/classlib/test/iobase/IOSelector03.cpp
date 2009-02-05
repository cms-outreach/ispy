#include "test/Test.h" 
#include "classlib/iobase/IOSelector.h"
#include "classlib/iobase/PipeCmd.h"
#include "classlib/utils/TimeInfo.h"
#include "classlib/utils/Signal.h"
#include "classlib/utils/DebugAids.h"
#include <iostream>
#include <iomanip>

using namespace lat;

int TEST(int argc, char **argv)
{
    Signal::handleFatal (argv[0]);

    if (argc == 1)
    {
	const char	*inargs [] = { argv [0], "input", 0 };
	PipeCmd		in (inargs, IOFlags::OpenRead);
	bool		done = false;
	char		buf [1024];
	IOSize		sz;
	IOSelector	s;

	s.attach (&in, IORead);
	in.setBlocking (false);

	while (! done)
	{
	    // FIXME: s.dispatch ();
	    unsigned n = s.wait ();

	    done = (n == 0);
	    ASSERT (done || n == 1);

	    do
		if ((sz = in.read (buf, sizeof (buf))))
		{
		    std::cout << "<" << std::flush;
		    std::cout.write (buf, sz);
		    std::cout << ">\n" << std::flush;
		}
		else
		    done = true;
	    while (sz == sizeof (buf));
	}

	// close pipes gracefully (FIXME: .terminate () / .abort ()?)
	// FIXME: do this in PipeCmd destructor?
	in.close (); // FIXME: protection against already closed?
	return in.wait ();
    }
    else if (argc == 2 && ! strcmp (argv [1], "input"))
    {
	// Output with delay
	static const char message [] = "Hello world!";
	for (const char *s = message; *s; ++s)
	{
	    TimeInfo::sleep (1);
	    std::cout << *s;
	    if (*s >= 'l')
		std::cout << *++s;
	    std::cout.flush ();
	}

	return EXIT_SUCCESS;
    }
    else
	abort ();

    return 0;
}

