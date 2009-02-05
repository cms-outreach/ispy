#include "test/Test.h" 
#include "classlib/zip/InteractiveCompressor.h"
#include "classlib/zip/ZCompressor.h"
#include "classlib/iobase/PipeCmd.h"
#include "classlib/iobase/IOSelector.h"
#include "classlib/iotools/IOChannelInputStream.h"
#include "classlib/iotools/IOChannelOutputStream.h"
#include "classlib/utils/TimeInfo.h"
#include "classlib/utils/Signal.h"
#include "classlib/utils/DebugAids.h"
#include <iostream>
#include <iomanip>

using namespace lat;

class Test : public InteractiveCompressor
{
    IOSelector		*m_selector;
    IOChannel		*m_ichannel;
    IOChannel		*m_ochannel;
    bool		m_done;

public:
    Test (IOChannel *input, IOChannel *output)
	: InteractiveCompressor (new ZCompressor),
	  m_selector (new IOSelector),
	  m_ichannel (input),
	  m_ochannel (output),
	  m_done (false)
	{
	    m_ichannel->setBlocking (false);
	    m_ochannel->setBlocking (false);
	    setInputStream (new IOChannelInputStream (input));
	    setOutputStream (new IOChannelOutputStream (output));
	}

    int run (void) {
	begin ();
	m_done = false;
	// FIXME: check selector failure when we haven't flushed!
	while (! m_done && m_selector->pump ())
	    ;

	return EXIT_SUCCESS;
    }

protected:
    bool onInput (IOSelectEvent *) { doInput (); return true; }   //Single-shot
    bool onOutput (IOSelectEvent *) { doOutput (); return true; } //Single-shot

    virtual void waitForInput (void) {
	std::cout << '-' << std::flush;
	InteractiveCompressor::waitForInput ();
	m_selector->attach (m_ichannel, IORead,
			    CreateHook (this, &Test::onInput));
    }

    virtual void waitForOutput (void) {
	std::cout << '+' << std::flush;
	InteractiveCompressor::waitForOutput ();
	m_selector->attach (m_ochannel, IOWrite,
			    CreateHook (this, &Test::onOutput));
    }

    virtual bool flush (void) {
	// FIXME: trailer?
	if (! InteractiveCompressor::flush ())
	    return false;

	m_done = true;
	return true;
    }
};

int TEST(int argc, char **argv)
{
    Signal::handleFatal (argv [0]);

    std::cout << argc << std::endl;

    if (argc == 1)
    {
	const char *inargs [] = { argv [0], "input", 0 };
	const char *outargs [] = { argv [0], "output", 0 };

	PipeCmd in (inargs, IOFlags::OpenRead);
	PipeCmd out (outargs, IOFlags::OpenWrite);
	Test t (&in, &out);

	int ret = t.run ();

	// close pipes gracefully (FIXME: .terminate () / .abort ()?)
	// FIXME: do this in PipeCmd destructor?
	in.close (); // FIXME: protection against already closed?
	out.close (); // FIXME: protection against already closed?

	in.wait ();
	out.wait ();

	return ret;
    }
    else if (argc == 2 && ! strcmp (argv [1], "input"))
    {
	// Output with delay
	static const char message []
	    = "Hello world!\nHello world!\n"
	    "1234567890abcdefghijkmnopqrstuvwxyz\n"
	    "easldjslk lasjd laks jd!\nH"
	    "^%@#&!@*^#(*@!)#*_(!@# @_(# _@\n"
	    ":LUAJF  WQNR)(#@*#$&@)! #@I#@!#&0972\n"
	    "^%@#&!@*^#(*@!)#*_(!@# @_(# _@\n"
	    "82973 2k 23kj 213 2 032i39218 321983029 82098283\n"
	    "1234567890abcdefghijkmn.l;sm m-3wqm; d.f m.dmopqrstuvwxyz\n"
	    "Hello world!\nHello world!\nalksd sae-0912 3 0912"
	    ":LUAJF  WQNR)(#@*#$&@)! #@I#@!#&0972\n"
	    "easldjslk lasjd ald jLKJAKLSDJ Slaks jd!\nH"
	    "82973 2k 23kj _())_(*@(!)*# @!213 2 0382098283\n"
	    "^%@#&!@*^#(*@!)#*_(!@# @_(# _@\n"
	    ":LUAJF  aljsdksja WQNR)(#@*#$&@)! #@I#@!#&0972\n"
	    "1234567890abcdefgh*&#@(*!@ lajsd lsjijkmnopqrstuvwxyz\n"
	    "easldjslk lasjd laks jd! ';qwnwpnoe0q98n029\nH";
	for (const char *s = message; *s; ++s)
	{
	    TimeInfo::msleep (37);
	    std::cout << *s;

	    if (*s & 1)
		std::cout << *++s;
	    std::cout.flush ();
	}

	return EXIT_SUCCESS;
    }
    else if (argc == 2 && ! strcmp (argv [1], "output"))
    {
	// Input with delay (FIXME: line buffering?)
#if HAVE_NEW_IOSTREAMS
	std::cin.rdbuf ()->pubsetbuf (0, 0);
#else
	std::cin.rdbuf ()->setbuf (0, 0);
#endif
	while (! std::cin.eof ())
	{
	    char ch;
	    std::cin >> ch;
	    std::cout << ch << std::flush;
	    TimeInfo::msleep (11);
	}

	return EXIT_SUCCESS;
    }
    else
	abort ();

    return 0;
}
