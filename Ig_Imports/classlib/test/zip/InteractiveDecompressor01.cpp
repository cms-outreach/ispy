#include "test/Test.h" 
#include "classlib/zip/InteractiveDecompressor.h"
#include "classlib/zip/ZDecompressor.h"
#include "classlib/iobase/PipeCmd.h"
#include "classlib/iobase/IOSelector.h"
#include "classlib/iotools/IOChannelInputStream.h"
#include "classlib/iotools/IOChannelOutputStream.h"
#include "classlib/iotools/StorageOutputStream.h"
#include "classlib/iotools/MemoryStorage.h"
#include "classlib/zip/ZOutputStream.h"
#include "classlib/utils/TimeInfo.h"
#include "classlib/utils/Signal.h"
#include "classlib/utils/DebugAids.h"
#include <iostream>
#include <cstdio>

using namespace lat;

class Test : public InteractiveDecompressor
{
    IOSelector		*m_selector;
    IOChannel		*m_ichannel;
    IOChannel		*m_ochannel;
    bool		m_done;

public:
    Test (IOChannel *input, IOChannel *output)
	: InteractiveDecompressor (new ZDecompressor),
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
	InteractiveDecompressor::waitForInput ();
	m_selector->attach (m_ichannel, IORead,
			    CreateHook (this, &Test::onInput));
    }

    virtual void waitForOutput (void) {
	std::cout << '+' << std::flush;
	InteractiveDecompressor::waitForOutput ();
	m_selector->attach (m_ochannel, IOWrite,
			    CreateHook (this, &Test::onOutput));
    }

    virtual bool flush (void) {
	// FIXME: trailer?
	if (InteractiveDecompressor::flush ())
	    return true;

	m_done = true;
	return false;
    }
};

int TEST(int argc, char **argv)
{
    Signal::handleFatal (argv [0]);

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
	// Compress some sample data
	static const char	sample [] = "Hello world!";
	MemoryStorage		mem;
	StorageOutputStream	moutput (&mem);
	ZOutputStream		zoutput (&moutput);
	zoutput.write (sample, sizeof (sample)-1);
	zoutput.close ();

	// Output it with delay
	mem.rewind ();
	while (! mem.eof ())
	{
	    std::cout << (char) ((Storage &) mem).read () << std::flush;
	    TimeInfo::msleep (42);
	}

	return EXIT_SUCCESS;
    }
    else if (argc == 2 && ! strcmp (argv [1], "output"))
    {
	// Input with delay
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
	    TimeInfo::msleep (17);
	}

	return EXIT_SUCCESS;
    }
    else
	abort ();

    return 0;
}
