//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/SubProcess.h"
#include "classlib/iobase/SubProcessError.h"
#include "classlib/utils/Argz.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/utils/Signal.h"
#include "classlib/iobase/IOChannel.h"
#include "classlib/iobase/IOError.h"
#include "classlib/sysapi/SubProcess.h"
#ifdef _WIN32

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
    // Windows wants full command.  (FIXME: quoting?)
    std::string command = Argz (argz).quote ();

    // Save the current STDIN/STDOUT, to be restored later.
    HANDLE oldstdin = GetStdHandle (STD_INPUT_HANDLE);
    HANDLE oldstdout = GetStdHandle (STD_OUTPUT_HANDLE);
    HANDLE oldstderr = GetStdHandle (STD_ERROR_HANDLE);

    // If "input" is specified, use it, otherwise use my stdin.
    HANDLE infd = input ? input->fd () : oldstdin;

    // If "output" is specified, use it, otherwise use my stdout.
    HANDLE outfd = output ? output->fd () : oldstdout;

    // If "error" is specified, use it, otherwise use my stderr.
    HANDLE errfd = error ? error->fd () : oldstderr;

    // Create non-inheritable duplicates of our ends of the
    // redirections; incopy and outcopy will be our file descriptors
    // if all goes well.  (FIXME: This will turn inheritable handles
    // uninheritable -- we are not checking if we need to redirect,
    // we just always duplicate.  Is this what we want?)
    HANDLE incopy;
    HANDLE outcopy;
    HANDLE errcopy;

    if (! DuplicateHandle (GetCurrentProcess (), infd,
			   GetCurrentProcess (), &incopy,
			   0, FALSE, DUPLICATE_SAME_ACCESS))
	throw IOError ("DuplicateHandle()", GetLastError ());

    if (! DuplicateHandle (GetCurrentProcess (), outfd,
			   GetCurrentProcess (), &outcopy,
			   0, FALSE, DUPLICATE_SAME_ACCESS))
    {
	int errcode = GetLastError ();
	CloseHandle (incopy);
	throw IOError ("DuplicateHandle()", errcode);
    }

    if (! DuplicateHandle (GetCurrentProcess (), errfd,
			   GetCurrentProcess (), &errcopy,
			   0, FALSE, DUPLICATE_SAME_ACCESS))
    {
	int errcode = GetLastError ();
	CloseHandle (incopy);
	CloseHandle (outcopy);
	throw IOError ("DuplicateHandle()", errcode);
    }

    // Redirect my STDIN/STDOUT for the child to inherit them.
    if (! SetStdHandle (STD_INPUT_HANDLE, infd)
	|| ! SetStdHandle (STD_OUTPUT_HANDLE, outfd)
	|| ! SetStdHandle (STD_ERROR_HANDLE, errfd))
    {
	int errcode = GetLastError ();
	SetStdHandle (STD_INPUT_HANDLE, oldstdin);
	SetStdHandle (STD_OUTPUT_HANDLE, oldstdout);
	SetStdHandle (STD_ERROR_HANDLE, oldstderr);
	CloseHandle (incopy);
	CloseHandle (outcopy);
	CloseHandle (errcopy);
	throw IOError ("SetStdHandle()", errcode);
    }

    // Close the inheritable handles.  If NoClose* flags are given,
    // this will leave them open in the child as well, but that's just
    // too bad; there's no way we can leave them open in only one, and
    // the caller in the parent expects to find them open on return.
    if (! (flags & NoCloseInput))
	CloseHandle (infd);

    if (! (flags & NoCloseOutput))
	CloseHandle (outfd);

    if (! (flags & NoCloseError))
	CloseHandle (errfd);

    // Create the subprocess.
    PROCESS_INFORMATION process;
    STARTUPINFO		start;
    memset (&start, 0, sizeof (start));
    start.cb = sizeof (start);

    if (! CreateProcess (NULL, (char *) command.c_str (), NULL, NULL,
			 TRUE, 0, NULL, NULL, &start, &process))
    {
	int error = GetLastError ();
	SetStdHandle (STD_INPUT_HANDLE, oldstdin);
	SetStdHandle (STD_OUTPUT_HANDLE, oldstdout);
	SetStdHandle (STD_ERROR_HANDLE, oldstderr);
	CloseHandle (incopy);
	CloseHandle (outcopy);
	CloseHandle (errcopy);
	CloseHandle (infd);
	CloseHandle (outfd);
	CloseHandle (errfd);
	throw IOError ("CreateProcess()", error);
    }

    m_sub = process.hProcess;
    m_pid = process.dwProcessId;

    // Restore redirected handles
    SetStdHandle (STD_INPUT_HANDLE, oldstdin);
    SetStdHandle (STD_OUTPUT_HANDLE, oldstdout);
    SetStdHandle (STD_ERROR_HANDLE, oldstderr);

    return m_pid;
}

pid_t
SubProcess::pid (void) const
{ return m_pid; }

bool
SubProcess::done (void)
{
    // If we've already waited and succeded, remember success.
    // (FIXME: Would rather not check this, now we can't detect an
    // inappropriate done()/wait() call!)
    if (m_sub == 0)
	return true;

    // Wait for the child.  We don't need to worry about collecting
    // the exit status as long as we have the process handle open.
    return WaitForSingleObject (m_sub, 0) == WAIT_OBJECT_0;
}

int
SubProcess::wait (void)
{
    // FIXME: Provide an interface to get wait4() resource info?
    // FIXME: wait with timeout?

    // If we've already been here, return the status we collected back then.
    if (m_sub == 0)
	return m_status;

    // Wait for the child.
    DWORD code = 0;
    if (WaitForSingleObject (m_sub, INFINITE) != WAIT_OBJECT_0
	|| ! GetExitCodeProcess (m_sub, &code))
	throw IOError ("WaitForSingleObject()", GetLastError ());

    m_status = code;

    // Mark the child reaped and return the status.
    CloseHandle (m_sub);
    m_sub = 0;
    return m_status;
}

void
SubProcess::terminate (void)
{ TerminateProcess (m_sub, ~0u); }

void
SubProcess::sysdetach (void)
{ if (m_sub) CloseHandle (m_sub); m_sub = 0; }

bool
SubProcess::exitNormal (int /* waitcode */)
{ return true; }

bool
SubProcess::exitBySignal (int /* waitcode */)
{ return false; }

int
SubProcess::exitStatus (int waitcode)
{ return waitcode; }

int
SubProcess::exitSignal (int /* waitcode */)
{ return 0; }

} // namespace lat
#endif // _WIN32
