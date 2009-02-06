//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/SubProcess.h"
#include "classlib/iobase/SubProcessError.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/utils/Signal.h"
#include "classlib/iobase/IOChannel.h"
#include "classlib/sysapi/SubProcess.h"
#ifndef _WIN32

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

pid_t
SubProcess::sysrun (const char **argz, unsigned flags,
		    IOChannel *input, IOChannel *output,
		    IOChannel *error, IOChannel *cleanup)
{
    // If "input" is specified, use it, otherwise use my stdin.
    int infd = input ? input->fd () : 0;

    // If "output" is specified, use it, otherwise use my stdout.
    int outfd = output ? output->fd () : 1;

    // If "error" is specified, use it, otherwise use my stderr.
    int errfd = error ? error->fd () : 2;

    // FIXME: Redirect other handles?
    switch (m_sub = fork ())
    {
    case -1:
	// Failed
	throw SubProcessError ("fork()", errno);

    case 0:
	// Child
	// FIXME: Optionally redirect non-piped other file handles
	// FIXME: Optionally close all other file descriptors?
	// FIXME: Optional environment cleanup?
	if (infd != 0)
	{
	    if (dup2 (infd, 0) == -1)
		_exit (255);

	    ::close (infd);
	}

	if (outfd != 1)
	{
	    if (dup2 (outfd, 1) == -1)
		_exit (255);

	    ::close  (outfd);
	}

	if (errfd != 2)
	{
	    if (dup2 (errfd, 2) == -1)
		_exit (255);

	    ::close  (errfd);
	}

	// Close other handles we no longer need.  If we are making a
	// pipe, this is the other end of the pipe the child won't
	// need.
	if (cleanup)
	    ::close (cleanup->fd ());

	// FIXME: propagate non-const to interface?
	((flags & Search) ? execv : execvp) (argz [0], (char **) argz);
	_exit (255);

    default:
	// Parent.  Close the child's descriptors, we don't need them
	// any more.
	if (input && ! (flags & NoCloseInput))
	    ::close (infd);

	if (output && ! (flags & NoCloseOutput))
	    ::close (outfd);

	if (error && ! (flags & NoCloseError))
	    ::close (errfd);
    }

    return m_sub;
}

pid_t
SubProcess::pid (void) const
{ return m_sub; }

bool
SubProcess::done (void)
{
    // If we've already waited and succeded, remember success.
    // (FIXME: Would rather not check this, now we can't detect an
    // inappropriate done()/wait() call!)
    if (m_sub == 0)
	return true;

    pid_t result;
    int status;
    if ((result = waitpid (m_sub, &status, WNOHANG)) < 0)
	throw SubProcessError ("waitpid()", errno);

    // waitpid() returns the pid or 0 if the child wasn not was available;
    // if it succeeded, act like wait and stash away the return code.
    if (result == m_sub)
    {
	m_status = status;
	m_sub = 0;
	return true;
    }
    else
	return false;
}

int
SubProcess::wait (void)
{
    // FIXME: Provide an interface to get wait4() resource info?
    // FIXME: wait with timeout?

    // If we've already been here, return the status we collected back then.
    if (m_sub == 0)
	return m_status;

    // Wait for the child.  If we get SIGCHLD (or an unblocked signal)
    // in the mean time, waitpid() will return EINTR, so ignore those.
    // FIXME? ASSERT (Signal::handler (SIGCHLD) != SIG_IGN);
    pid_t result;
    do
	result = waitpid (m_sub, &m_status, 0);
    while (result < 0 && errno == EINTR);

    if (result < 0)
	throw SubProcessError ("waitpid()", errno);

    ASSERT (result == m_sub);

    // Mark the child reaped and return the status.
    m_sub = 0;
    return m_status;
}

void
SubProcess::terminate (void)
{ kill (m_sub, SIGTERM); }

void
SubProcess::sysdetach (void)
{ m_sub = 0; }

bool
SubProcess::exitNormal (int waitcode)
{ return WIFEXITED (waitcode); }

bool
SubProcess::exitBySignal (int waitcode)
{ return WIFSIGNALED (waitcode); }

int
SubProcess::exitStatus (int waitcode)
{ return WEXITSTATUS (waitcode); }

int
SubProcess::exitSignal (int waitcode)
{ return WTERMSIG (waitcode); }

} // namespace lat
#endif // !_WIN32
