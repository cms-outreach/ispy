#include "test/Test.h" 
#include "classlib/iobase/IOSelector.h"
#include "classlib/iobase/IOSelectEvent.h"
#include "classlib/iobase/PipeCmd.h"
#include "classlib/utils/TimeInfo.h"
#include "classlib/utils/Signal.h"
#include "classlib/utils/DebugAids.h"
#include <iostream>
#include <iomanip>

using namespace lat;

static bool reader (IOSelectEvent *event)
{
    bool		done = false;
    unsigned char	buf [1024];
    IOSize		sz;

    do
	if ((sz = event->source->read (buf, sizeof (buf))))
	{
	    std::cout << "<" << std::flush;
	    std::cout.write ((char *) buf, sz);
	    std::cout << ">\n" << std::flush;
	}
	else
	    done = true;
    while (sz == sizeof (buf));

    return done;
}


int TEST(int argc, char **argv)
{
    Signal::handleFatal (argv[0]);

    if (argc == 1)
    {
	const char	*inargs [] = { argv [0], "input", 0 };
	PipeCmd		in (inargs, IOFlags::OpenRead);
	IOSelector	s;

	in.setBlocking (false);
	s.attach (&in, IORead, CreateHook (reader));
	while (s.dispatch ())
	    ;

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

