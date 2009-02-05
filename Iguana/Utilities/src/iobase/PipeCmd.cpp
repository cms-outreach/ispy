//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/PipeCmd.h"
#include "classlib/utils/Argz.h"
#include "classlib/iobase/Pipe.h"
#include "classlib/iobase/SubProcess.h"
#include "classlib/iobase/IOError.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/utils/Argz.h"
#include "classlib/sysapi/PipeCmd.h"
#include <algorithm>

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

// FIXME: block SIGPIPE while reading/writing from a pipe and then
// look for EPIPE error?

PipeCmd::PipeCmd (void)
    : m_pipe (0),
      m_sub (0)
{}

PipeCmd::~PipeCmd (void)
{
    delete m_sub;
    delete m_pipe;
}

PipeCmd::PipeCmd (const char **argz, unsigned mode)
    : m_pipe (0),
      m_sub (0)     
{ open (argz, mode); }

PipeCmd::PipeCmd (const std::string &command, unsigned mode)
    : m_pipe (0),
      m_sub (0)
{ open (command, mode); }

void
PipeCmd::open (const char **argz, unsigned mode)
{
    // Check that the command is not empty
    ASSERT (argz);
    ASSERT (*argz);
    ASSERT (**argz);

    // Get a file descriptor pair for the subprocess.
    ASSERT (! m_pipe);
    ASSERT (! m_sub);

    m_pipe = new Pipe;
    ASSERT (m_pipe->source ()->fd () != IOFD_INVALID);
    ASSERT (m_pipe->sink ()->fd () != IOFD_INVALID);

    IOFD parentfd;
    unsigned flags = SubProcess::One;
    if (mode == IOFlags::OpenRead)
    {
	flags |= SubProcess::Read;
	parentfd = m_pipe->source ()->fd ();
    }
    else
    {
	flags |= SubProcess::Write;
	parentfd = m_pipe->sink ()->fd ();
    }

    m_sub = new SubProcess (argz, flags, m_pipe);
    fd (parentfd);
}

void
PipeCmd::open (const std::string &command, unsigned mode)
{
    // Check that the command is not empty
    ASSERT (! command.empty ());
    ASSERT (command.find_first_not_of (' ') != std::string::npos);
    ASSERT (command.find_first_not_of ('\t') != std::string::npos);
    ASSERT (command.find_first_not_of ('\n') != std::string::npos);

#ifdef _WIN32
    // On windows, parse into components, SubProcess will smash back
    // into a single, properly qouted string.  (Ugh.)
    Argz cmd (command);
    const char **argz = cmd.argz ();
#else
    // On unix-like systems, pass the entire command to shell
    const char *argz [] = { "sh", "-c", command.c_str (), 0 };
#endif

    open (argz, mode);
}

int
PipeCmd::wait (void)
{ return m_sub->wait (); }

} // namespace lat
