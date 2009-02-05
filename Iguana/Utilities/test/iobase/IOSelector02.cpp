#include "test/Test.h" 
#include "classlib/iobase/IOSelector.h"
#include "classlib/iobase/IOSelectEvent.h"
#include "classlib/iobase/PipeCmd.h"
#include "classlib/utils/TimeInfo.h"
#include "classlib/utils/Signal.h"
#include "classlib/utils/DebugAids.h"
#include <iostream>
#include <iomanip>
#include <cstdio>

using namespace lat;

class Test
{
    IOSelector		m_selector;
    PipeCmd		*m_input;
    PipeCmd		*m_output;
    std::string		m_buffer;
    bool		m_writing;

public:
    Test (PipeCmd *input, PipeCmd *output)
	: m_input (input),
	  m_output (output),
	  m_writing (false)
	{
	    m_input->setBlocking (false);
	    m_output->setBlocking (false);
	    m_selector.attach (m_input, IORead, CreateHook (this, &Test::reader));
	}

    int run (void) {
	// FIXME: check selector failure when we haven't flushed!
	// while (! m_done && m_selector->pump ()) ;
	while (m_selector.dispatch ())
	    ;

	m_input->close (); // FIXME: protection against already closed?
	m_output->close (); // FIXME: protection against already closed?
	return (m_input->wait () == EXIT_SUCCESS
		&& m_output->wait () == EXIT_SUCCESS
		? EXIT_SUCCESS : EXIT_FAILURE);
    }

    bool reader (IOSelectEvent *event) {
	bool	done = false;
	char	buf [1024];
	IOSize	sz;

	do
	    if ((sz = event->source->read (buf, sizeof (buf))))
	    {
	        std::cout << "<R:" << std::flush;
	        std::cout.write (buf, sz);
	        std::cout << ">" << std::flush;
	        m_buffer.append (buf, sz);
	    }
	    else
	        done = true;
        while (sz == sizeof (buf));

	if (m_buffer.size () && ! m_writing)
	{
	    m_selector.attach (m_output, IOWrite, CreateHook (this, &Test::writer));
	    m_writing = true;
	}

        return done;
    }

    bool writer (IOSelectEvent *event) {
	IOSize	sz;

	if ((sz = event->source->write (&m_buffer [0], m_buffer.size ())))
	{
	    std::cout << "<W:" << std::flush;
	    std::cout.write (&m_buffer [0], sz);
	    std::cout << ">" << std::flush;
	    m_buffer.erase (0, sz);
	}

	m_writing = m_buffer.size () > 0;
	return ! m_writing;
    }
};

int TEST(int argc, char **argv)
{
    Signal::handleFatal (argv[0]);

    if (argc == 1)
    {
	const char	*inargs [] = { argv [0], "input", 0 };
	const char	*outargs [] = { argv [0], "output", 0 };

	PipeCmd		in (inargs, IOFlags::OpenRead);
	PipeCmd		out (outargs, IOFlags::OpenWrite);
	Test		t (&in, &out);

	return t.run ();
    }
    else if (argc == 2 && ! strcmp (argv [1], "input"))
    {
	// Output with delay
	static const char message [] = "Hello world!";
	for (const char *s = message; *s; ++s)
	{
	    fputc (*s, stdout);
	    fflush (stdout);
	    TimeInfo::usleep (30000);
	}

	return EXIT_SUCCESS;
    }
    else if (argc == 2 && ! strcmp (argv [1], "output"))
    {
	// Input with delay
	int ch;
	while ((ch = fgetc (stdin)) != EOF)
	{
	    fputc (ch, stdout);
	    fflush (stdout);
	    TimeInfo::usleep (100000);
	}

	return EXIT_SUCCESS;
    }
    else
	abort ();

    return 0;
}
