//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/utils/Signal.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/utils/Log.h"
#include "classlib/utils/SharedLibrary.h"
#include "classlib/utils/ProcessInfo.h"
#include "classlib/utils/BitTraits.h"
#include "classlib/sysapi/Signal.h"
#include <cstring>

/* http://dmawww.epfl.ch/ebt-bin/nph-dweb/dynaweb/SGI_Developer/
     T_IRIX_Prog/@Generic__BookTextView/7525

  POSIX		        SVR4              BSD 4.2
  =========================================================
  sigaction(2)          sigset(2)         sigvec(3) 
  sigsetops(3)          signal(2)         signal(3) 
  sigaltstack(2) 	
			
  sigqueue(2)           sigsend(2)        kill(3)   
  kill(2)               kill(2)           killpg(3)
  pthread_kill(3P) 	
			
  sigprocmask(2)        sighold(2)        sigblock(3)  
  pthread_sigmask(3P)   sigrelse(2)       sigsetmask(3)
			
  sigpending(2)         n.a.              n.a.
			
  sigsuspend(2)         sigpause(2)       sigpause(3)
			
  sigwait(2)            n.a.              n.a.
  sigwaitinfo(2) 
  sigtimedwait(2)
*/

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>

/** Dummy handler type for standard @c signal() function.  */
extern "C" { typedef void (*DummyHandlerType) (int); }

//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>

/** Shared library dump callback for #Signal::fataldump().  Allocated
    at initialisation time in #Signal::handleFatal() so that we won't
    need to allocate memory in the middle of a fatal handler.  This
    variable is deliberately private to this file (to keep #Signal
    headers from referring to #SharedLibrary for the local class). */
static SharedLibrary::InfoHandler *SignalDumpCallback = 0;

//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

/** Indicator that the application has been crashed: that a fatal
    signal has been delivered.  */
bool			Signal::s_crashed = false;

/** Indicator that we are currently executing inside #fatal().  Used
    to protect against signals delivered during recovery attempts.  */
int			Signal::s_inFatal = 0;

/** The current application name.  */
const char		*Signal::s_applicationName = 0;

/** The output file descriptor for #fataldump().  */
IOFD			Signal::s_fatalFd = IOFD_INVALID;

/** The application handler hook for fatal signals.  */
Signal::FatalHook	Signal::s_fatalHook = 0;

/** The application main return hook for fatal signals.  */
Signal::FatalReturn	Signal::s_fatalReturn = 0;

/** The current fatal signal handling options.  */
unsigned		Signal::s_fatalOptions = 0;

/** The application handler hook for quitting-related signals.  */
Signal::QuitHook	Signal::s_quitHook = 0;

#if !HAVE_POSIX_SIGNALS || !SA_SIGINFO
/** Actual signal handlers when POSIX signals are not available.
    These are required so that we can send pass correct (= null)
    arguments to the registered handler when the system is not passing
    anything, or garbage.  If this happens, #handle() will register
    #trampoline() as the signal handler and register the signal in
    this table, and #trampoline just looks the actual handler here. */
Signal::HandlerType	Signal::s_trampolines [NSIG];
#endif

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>

/** Internal #Signal::fataldump() dumper to produce the list of
    currently loaded shared libraries.  Called for every library
    detected by #SharedLibrary::loaded().

    @param info Information about this particular library.
    @param fd   The file descriptor to output to.  */
static void
SignalDumpLibs (const SharedLibrary::LibraryInfo &info, IOFD fd)
{
    char buf [BitTraits<unsigned long>::HexDigits + 5];
    MYWRITE (fd, buf, sprintf (buf, " 0x%08lx ", info.m_text_start));
    MYWRITE (fd, info.m_filename, strlen (info.m_filename));
    MYWRITE (fd, "\n", 1);
}

#ifdef _WIN32
/** Windows hook to translate uncaught structured exception handling
    (SEH) events into calls to #Signal::fatal().  */
static LONG CALLBACK
SEHFatal (PEXCEPTION_POINTERS info)
{
    Signal::fatal (SIGABRT, info->ExceptionRecord, info->ContextRecord);
    return EXCEPTION_EXECUTE_HANDLER;
}
#endif // _WIN32

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

#if !HAVE_POSIX_SIGNALS
/** Internal signal handler trampoline to convert handler arguments
    to look more like POSIX signals.  The actual handler must have
    been installed into #s_trampolines by #handle().  */
void
Signal::trampoline (int sig)
{
    ASSERT (sig > 0 && sig < NSIG);
    ASSERT (s_trampolines [sig]);
    siginfo_t info;
    memset (&info, 0, sizeof (info));
    s_trampolines [sig] (sig, &info, 0);
}
#endif

/** Return the name of the signal number @a sig.  The returned memory
    is statically allocated and must not be freed. */
const char *
Signal::name (int sig)
{
#if HAVE_STRSIGNAL
    return strsignal (sig);
#elif HAVE_SYS_SIGLIST
    return sys_siglist [sig];
#else
    // This is not thread safe.  But if you have threads, you probably
    // have strsignal() as well (FIXME: check WIN32).
    static char	buf [NSIG] [8 + BitTraits<int>::Digits];
    if (! buf [sig][0])
	sprintf (buf [sig], "Signal %d", sig);
    return buf [sig];
#endif
}

/** Return the current handler for signal number @a sig and its
    blocked signals in @a mask (if non-null).  */
Signal::HandlerType
Signal::handler (int sig, sigset_t *mask /* = 0 */)
{
    ASSERT (sig > 0 && sig < NSIG);

    // Get the handler
#if HAVE_POSIX_SIGNALS
    struct sigaction old;
    STDC::memset (&old, 0, sizeof (old));
    if (sigaction (sig, &old, 0) == 0)
    {
	if (mask)
	    *mask = old.sa_mask;
	return (HandlerType) old.sa_handler;
    }
    else
	return (HandlerType) SIG_ERR;
#else // ! HAVE_POSIX_SIGNALS
    HandlerType old = (HandlerType) signal (sig, SIG_DFL);
    signal (sig, (DummyHandlerType) old);
    return old;
#endif // HAVE_POSIX_SIGNALS
}

/** Install a new signal handler @a handler for signal number
    @a sig and returns the old handler.

    This method uses the POSIX signal handling primitives if they are
    available, failing which falling back to the C standard @c
    signal() function.

    When POSIX signals are used, signals other than @a sig are blocked
    according to @a blockMask (if null, no change is made) during the
    execution of @a handler.  Note that the signal itself is always
    blocked during the handler execution and need not be mentioned in
    the mask explicitly.  System calls are made restartable although
    this has little impact as this library always restarts interrupted
    system calls automatically despite the signal handling settings.

    (FIXME: Expose option SA_NOCLDSTOP, SA_ONSTACK?)
    (FIXME: Threads vs. signals)  */
Signal::HandlerType
Signal::handle (int sig, HandlerType handler, const sigset_t *blockMask /*=0*/)
{
    ASSERT (sig > 0 && sig < NSIG);
    LOG (0, trace, LFsignal, "[" << sig << "] (" << name (sig) << ") = "
	 << (void *) handler << '\n');

    HandlerType oldhandler;
#if !HAVE_POSIX_SIGNALS || !SA_SIGINFO
    // Switch to using trampoline if we don't have the necessary
    // arguments.  FIXME: multiple threads; WIN32?
    oldhandler = s_trampolines [sig];
    if (handler == (HandlerType) SIG_IGN || handler == (HandlerType) SIG_DFL)
	s_trampolines [sig] = 0;
    else
    {
        s_trampolines [sig] = handler;
        handler = (HandlerType) &trampoline;
    }
#endif

    // Set the handler
#if HAVE_POSIX_SIGNALS
    struct sigaction old, act;
    STDC::memset (&act, 0, sizeof (act));
    STDC::memset (&old, 0, sizeof (old));
    act.sa_flags = SA_RESTART | SA_SIGINFO;
    act.sa_sigaction = handler;
    sigemptyset (&act.sa_mask);
    if (blockMask)
	act.sa_mask = *blockMask;
    else if (sigaction (sig, &old, 0) == 0)
	act.sa_mask = old.sa_mask;
    else
	sigemptyset (&act.sa_mask);

    // There isn't much we can do to check the return status.  We get
    // called in all sorts fragile places like signal handlers, and
    // those are not the place for throwing exceptions or asserting.
    if (sigaction (sig, &act, &old) == -1)
	return (HandlerType) SIG_ERR;
    oldhandler = (HandlerType) old.sa_sigaction;
#else // ! HAVE_POSIX_SIGNALS
    (HandlerType) ::signal (sig, (DummyHandlerType) handler);
#endif // HAVE_POSIX_SIGNALS
    return oldhandler;
}

/** Revert the signal number @a sig back to its default behaviour.  */
void
Signal::revert (int sig)
{ handle (sig, (HandlerType) SIG_DFL); }

/** Ignore the signal number @a sig.  */
void
Signal::ignore (int sig)
{ handle (sig, (HandlerType) SIG_IGN); }

/** Block or unblock the signal number @a sig.  The signal is blocked
    if @a sense is @c true, unblocked otherwise.  This function is
    implemented only on systems with POSIX signals.  */
void
Signal::block (int sig, bool sense)
{
#if HAVE_POSIX_SIGNALS
    // FIXME: threads -- need to use pthread_sigmask
    sigset_t mask;
    sigemptyset (&mask);
    sigaddset (&mask, sig);
    block (&mask, sense);
#endif
}

/** Block or unblock the signals specified by @a mask.  The signals
    are blocked if @a sense is @c true, unblocked otherwise.  This
    function is implemented only on systems with POSIX signals. */
void
Signal::block (const sigset_t *mask, bool sense)
{
#if HAVE_POSIX_SIGNALS
    // FIXME: threads -- need to use pthread_sigmask
    sigprocmask (sense ? SIG_BLOCK : SIG_UNBLOCK, mask, 0);
#endif
}

/** Set the list of currently blocked signals to @a mask and return
    the old setting in @a old (if non-null).  This function is
    implemented only on systems with POSIX signals.  */
void
Signal::mask (const sigset_t *mask, sigset_t *old /* = 0 */)
{
#if HAVE_POSIX_SIGNALS
    // FIXME: threads -- need to use pthread_sigmask
    sigprocmask (SIG_SETMASK, mask, old);
#endif
}

//////////////////////////////////////////////////////////////////////
/** Raise the signal number @a sig.  Returns the exit code from the @c
    raise() system call (or @c kill() if @c raise() does not exist).  */
int
Signal::raise (int sig)
{
#if HAVE_RAISE
    return ::raise (sig);
#else
    return ::kill (getpid (), sig);
#endif
}

/** Send the signal @a sig to process identified by @a process.
    Implemented only on unixen.  */
int
Signal::kill (pid_t process, int sig)
{
    // FIXME: sending signals to threads?
#ifndef _WIN32
    return ::kill (process, sig);
#else
    return 0;
#endif
}

/** Queue signal @a sig with additional data @a value for @a process.
    Implemented only on systems with POSIX real-time signals.  */
int
Signal::queue (pid_t process, int sig, int value /* = 0 */)
{
#if HAVE_POSIX_RT_SIGNALS
    union sigval v;
    v.sival_int = value;
    return sigqueue (process, sig, v);
#else
    return 0;
#endif
}

/** Queue signal @a sig with additional data @a value for @a process.
    Implemented only on systems with POSIX real-time signals.  */
int
Signal::queue (pid_t process, int sig, void *value)
{
#if HAVE_POSIX_RT_SIGNALS
    union sigval v;
    v.sival_ptr = value;
    return sigqueue (process, sig, v);
#else
    return 0;
#endif
}

/** Queue signal @a sig for this process with additional data @a
    value.  Implemented only on systems with POSIX real-time
    signals.  */
int
Signal::queue (int sig, int value /* = 0 */)
{
#if HAVE_POSIX_RT_SIGNALS
    return queue (getpid (), sig, value);
#else
    return 0;
#endif
}

/** Queue signal @a sig for this process with additional data @a
    value.  Implemented only on systems with POSIX real-time
    signals.  */
int
Signal::queue (int sig, void *value)
{
#if HAVE_POSIX_RT_SIGNALS
    return queue (getpid (), sig, value);
#else
    return 0;
#endif
}

/** Check if @a sig is pending for this process.  */
bool
Signal::pending (int sig)
{ sigset_t s; pending (&s); return sigismember (&s, sig); }

/** Return in @a mask the list of signals pending for this process. */
void
Signal::pending (sigset_t *mask)
{
#if HAVE_POSIX_SIGNALS
    ASSERT (mask);
    sigpending (mask);
#endif
}

/** Temporarily replace the signal mask of the process with @a mask
    and then suspend until a signal is received. */
void
Signal::suspend (const sigset_t *mask)
{
#if HAVE_POSIX_SIGNALS
    ASSERT (mask);
    sigsuspend (mask);
#endif
}

/** Suspend the thread waiting for signal @a sig at most @a msecs
    milliseconds.  If @a msecs is negative (the default), waits until
    a signal is delivered.  Otherwise waits upto the specified time
    limit.  Returns @c true if the signal was received.  Note that the
    signal must be blocked (in a multi-threaded application in all the
    threads, not just the calling one) and not be ignored before
    callign this function; if a handler is registered, it won't be
    called.  Implemented only on systems with POSIX real-time
    signals.  */
bool
Signal::wait (int sig, siginfo_t *info /* = 0 */, long msecs /* = -1 */)
{
    sigset_t s;
    sigemptyset (&s);
    sigaddset (&s, sig);
    return wait (&s, info, msecs) == sig;
}

/** Suspend the thread waiting for signals specified by @a mask for at
    most @a msecs milliseconds.  If @a msecs is negative (the
    default), waits until a signal is delivered.  Otherwise waits upto
    the specified time limit.  Returns the number of the signal that
    was received, or -1 if the time limit expired.  If @a info is
    given, fills it with the information that the handler would have
    otherwise been given.  Note that the signals must be blocked (in a
    multi-threaded application in all the threads, not just the
    calling one) and not be ignored before callign this function; if a
    handler is registered, it won't be called.  Implemented only on
    systems with POSIX real-time signals.  */
int
Signal::wait (const sigset_t *mask,
	      siginfo_t *info /* = 0 */,
	      long msecs /* = -1 */)
{
#if HAVE_POSIX_RT_SIGNALS
    siginfo_t	myinfo;
    timespec	ts;

    if (msecs < 0)
	sigwaitinfo (mask, &myinfo);
    else
    {
	ts.tv_sec = msecs / 1000;
	ts.tv_nsec = (msecs % 1000) * 1000000;
	if (sigtimedwait (mask, &myinfo, &ts) == -1 && errno == EINTR)
	    // FIXME: deal with other error codes (NB: EAGAIN == timed out)
	    return -1;
    }

    if (info)
	*info = myinfo;

    return myinfo.si_signo;
#else
    return 0;
#endif
}

//////////////////////////////////////////////////////////////////////
/* Install #quit() as the handler for quitting-related signals.

   This method installs #quit() as the handler for quitting-related
   signals such as SIGHUP, SIGTERM and SIGQUIT.  Upon signal delivery
   @a hook will be invoked; if it returns @c true, #quit() proceeds to
   exit by re-raising the signal (in order to make the program's exit
   status reflect the signal exit).  If the @a hook returns @c false,
   the signal is effectively ignored.  Note however that certain
   options to #fatal() also cause the quit hook to be invoked.  */
void
Signal::handleQuit (QuitHook hook /* = 0 */)
{
    static int hups [] = {
#ifdef SIGHUP
	// hang up (lost terminal or process group leader)
	SIGHUP,
#endif
#ifdef SIGTERM
	// terminate (e.g. system going down)
	SIGTERM,
#endif
#ifdef SIGQUIT
	// user request to quit and leave debuggable state (from quit
	// key on controlling terminal)
	SIGQUIT,
#endif
	-1
    };

    if (hook)
	s_quitHook = hook;

    for (unsigned sig = 0; hups [sig] != -1; ++sig)
	handle (hups [sig], quit);
}

/** Install default handler for fatal signals.

    This method installs a handler for fatal signals such as floating
    point exceptions, illegal instructions, and memory violations.
    The behaviour is more precisely determined by @a options, a
    bitwise or of the option constants defined in the class
    declaration.

    @a applicationName sets the application name to be used to report
    the signal in #fatalDump().  @a fd sets the file descriptor to
    which the fatal signal message is written; by default this will be
    the standard error output.  @a hook sets the pre-exit application
    hook to invoke, @a mainreturn sets the hook to return to back to
    the application "main loop" (i.e. ignore the signal by jumping out
    of the signal back to the somewhere higher up in the application).

    Options left to default values will not change the current state.
    This allows one to re-install signal handlers without disturbing
    already registered information.  Use this to restore handlers
    after someother library has meddled with the handlers.

    This installs #fatal() as the handler for fatal signals and on
    Windows for otherwise unhandled fatal structured exceptions.  If
    #FATAL_ON_QUIT is included in @a options, quitting related signals
    (see #quit()) are also considered fatal.  If #FATAL_ON_INT is set,
    SIGINT is considered fatal---but see also #fatal() documentation.
    If #USR1_DUMP_CORE is set, #DebugAids::coredump is registered as a
    handler for SIGUSR1 (please note the security risks of this option
    in its docuementation).

    A multi-threaded application should call this method in each
    thread.  (FIXME: Calling this in one thread and blocking signals
    in others won't work on Linux, and in any case will probably
    produce non-sense stack traces (unless stacktrace can be fixed to
    dump the stacks of all the threads).  Since the handler is always
    the same, I am not sure it will make the slightest difference
    which thread catches the signals, and on the other hand, it is
    best to dump the problems in the faulting thread if possible.)  */
void
Signal::handleFatal (const char  *applicationName /* = 0 */,
		     IOFD	 fd /* = IOFD_INVALID */,
		     FatalHook	 hook /* = 0 */,
		     FatalReturn mainreturn /* = 0 */,
		     unsigned    options /* = FATAL_DEFAULT */)
{
    // FIXME: Provide means to install handlers for fatal signals that
    // an application has requested and app was supposed to register a
    // handler before making the request?  (So that if the app handler
    // is not installed for some reason, an internal error hook can
    // run?)  Such fatal signals include:
    //   - SIGPIPE: read or write to broken pipe; child died
    //     (read or write to socket with ASYNC io?)
    //   - SIGLOST: lost a resource (e.g., lock on nfs server reboot)
    //   - SIGALRM: interval timer elapsed
    //   - SIGUSR1, SIGUSR2
    //   - SIGPOLL: pollable streams device events
    //   - SIGIO: i/o possible (from async i/o)
    //   - SIGVTALRM: virtual timer expired
    //   - SIGPROF: profiling timer expired
    //   - SIGRTMIN - SIGRTMAX: POSIX real-time signals
    //
    // Some of these the application should probably just #block()
    // (e.g. SIGPIPE).  Some of them the app should block and then
    // wait or poll for events (SIGPOLL, SIGIO, possibly SIGALRM, the
    // real-time signals if they are used).

    static int hups [] = {
#ifdef SIGHUP
	SIGHUP,		// hang up (lost terminal or process group leader)
#endif
#ifdef SIGTERM
	SIGTERM,	// terminate (e.g. system going down)
#endif
#ifdef SIGQUIT
	SIGQUIT,	/* user request to quit and leave debuggable
			   state (from quit key on controlling
			   terminal) */

#endif
	-1
    };

    static int fatals [] = {
#ifdef SIGFPE
	SIGFPE,		// arithmetic exception
#endif
#ifdef SIGILL
	SIGILL,		// illegal instruction
#endif
#ifdef SIGSEGV
	SIGSEGV,	// illegal address
#endif
#ifdef SIGBUS
	SIGBUS,		// hardware exception
#endif
#ifdef SIGIOT
	SIGIOT,		/* IOT trap.  Before SIGABRT so that if SIGIOT
			   == SIGABRT then SIGABRT overrides SIGIOT;
			   SIGABRT is in ISO C and POSIX.1, SIGIOT is
			   not. */
#endif
#ifdef SIGABRT
	SIGABRT,	// abort

#endif
#ifdef SIGTRAP
	SIGTRAP,	// trace/breakpoint reached
#endif
#ifdef SIGEMT
	SIGEMT,		// emulation trap (may be used by profiler?)
#endif
#ifdef SIGSYS
	SIGSYS,		// invalid system call
#endif
#ifdef SIGXCPU
	SIGXCPU,	// cpu time limit exceeded
#endif
#ifdef SIGXFSZ
	SIGXFSZ,	// file size limit exceeded
#endif
	-1
    };

    // Make sure `strsignal' is properly initialised.
    name (1);

    // Remember app name if specified
    if (applicationName && *applicationName)
	s_applicationName = applicationName;

    // Automatically initialise s_fatalFd on first access
    if (s_fatalFd == IOFD_INVALID)
	s_fatalFd = STDERR_HANDLE;

    // Remember the fatal output fd if defined
    if (fd != IOFD_INVALID)
	s_fatalFd = fd;

    // Now that we know the fd, setup a callback for dumping shared
    // libraries via #SignalDumpLibs.  This avoids having to allocate
    // memory for the callback implementation in the middle of a fatal
    // signal, and on the other hand avoids a global object which
    // might not be initialised yet.
    delete SignalDumpCallback;
    SignalDumpCallback = new SharedLibrary::InfoHandler
			 (CreateCallback (&SignalDumpLibs, s_fatalFd));

    // Remember the hooks if specified
    if (hook)
	s_fatalHook = hook;

    if (mainreturn)
	s_fatalReturn = mainreturn;

    // Remember the new options
    s_fatalOptions = options;

    // Signal::fatal() requires this, otherwise weird things can happen.
    // Programs not wanting to return to main should set FATAL_AUTO_EXIT.
    ASSERT (s_fatalReturn || (s_fatalOptions & FATAL_AUTO_EXIT));

    // Install signal handlers.
    if (options & FATAL_ON_QUIT)
	for (unsigned sig = 0; hups [sig] != -1; ++sig)
	    handle (hups [sig], fatal);

    for (unsigned sig = 0; fatals [sig] != -1; ++sig)
	handle (fatals [sig], fatal);

#ifdef SIGINT
    // interrupt key from controlling terminal
    if (options & FATAL_ON_INT)
	handle (SIGINT, fatal);
#endif

#ifdef SIGUSR1
    // program-defined signals SIGUSR1 and SIGUSR2
    if (options & USR1_DUMP_CORE)
	handle (SIGUSR1, (HandlerType) DebugAids::coredump);
#endif

#ifdef _WIN32
    SetUnhandledExceptionFilter (&SEHFatal);
#endif
}

/** The quit signal handler.

    This is the handler installed by #handleQuit().  Please use
    #handleQuit() and this method instead of installing your own
    handlers with #handle().

    This handler first invokes the application hook if one was given
    to #handleQuit().  If the hook returns @c true, the signal handler
    for this signal (number @a sig) is reset to its default handler,
    and the signal is re-raised.  This causes the program to exit via
    the signal and have a the correct exit status.

    The application should do whatever is necessary for a graceful
    shutdown.  Note however that this signal may arrive asynchronously
    at any time, hence it probably isn't safe to allocate memory, use
    the standard output streams, and so forth.  What you can do is to
    set a flag, return @c false to return back to your application,
    detect the flag setting and drain your current event loop, and
    then quit.  But do note that if #FATAL_AUTO_EXIT was set in call
    to #handleFatal(), #fatal() will call #quit() which in turn calls
    the application hook.  Thus the hook should make sure it returns
    @c true if the application has crashed as noted in the
    documentation for <<QuitHook>>.  */
void
Signal::quit (int sig, siginfo_t *info, void *x)
{
    // Quit if no hook has been registered: we are coming in via
    // FATAL_AUTO_EXIT in fatal and the application did not call
    // handleQuit.
    if (! s_quitHook || (*s_quitHook) (sig, info, x))
    {
	// Reactivate the default signal handling behaviour for this
	// signal, which is to terminate the application one way or
	// the other.  Then exit through the signal.  This makes the
	// process exit status correct.
	revert (sig);
	raise (sig);
    }
}

/** The fatal signal handler.

    This is the handler installed by #handleFatal().  Please use
    #handleFatal() and this method instead of installing your handlers
    with #handle().  You should be able use the handler options to
    specify all the control you need.

    The first thing this handler does is to reinstall itself for the
    benefit of platforms with single-delivery signals.  Immediately
    after that it unblocks the delivery of that signal again, in case
    the signal handler itself gets in trouble.  The next step is to
    check if the current crash level (the recursion of calls to
    #fatal(), see #fatalLevel()) exceeds the predefined limit of 4; if
    so, we give up and let the application die with this this signal.
    The handler then determines whether the signal is fatal:
    everything except SIGINT is, and SIGINT is fatal if #FATAL_ON_INT
    was set.  If the signal is fatal, crash indicator is set (see
    #crashed()).

    If this is not a nested fatal signal, the signal is fatal, and
    #FATAL_DUMP_CORE is set, the handler tries dump a core file.  Then
    the handler will either attempt to quit or to return to the main
    program depending on #FATAL_AUTO_EXIT option setting.  If it is
    set or this is a nested fatal signal, the handler will attempt to
    exit as follows: the application hook (or #fatalDump() in its
    absence) is invoked.  If the hook returns @c true, #quit() is
    called; otherwise the signal handler will return (and crash or get
    an infinite sequence of fatal signals).  Note that if an
    application hook is registered, #fataldump() is not called by
    default; the application hook must invoke it itself to get the
    dump.

    If #FATAL_AUTO_EXIT is not set, the application must have
    registered a main return hook, which will be invoked.  The hook
    must not return, but do a @c siglongjmp back to the main program
    (it should not throw unless all code is built with options that
    allow exceptions to be thrown from signal handlers).  Note that
    the fatal signal may be asynchronous and may have arisen in code
    left in unsafe state, so returning back to the main program may
    not buy you much.  It may make sense for a few things like rogue
    null pointer dereferences or floating point exceptions.

    An interactive application using a main return hook should do
    something like this when the @c sigsetjmp in the main loop
    returns:
      - disable "main loop entered" status
      - inform the user about the fatal error (e.g. with a popup);
        the popup window should be precreated for best stability
      - reset any locks the application holds, especially for user
	interface, including status bars, wait icons etc.
      - suggest to run a debugger against the program right there
      - in a windowing system ungrab pointer, keyboard and the server
      - unblock the signal via #block(sig, false) as the operating
        system may think the signal is still being processed
      - add an idle processor to re-return the "main loop entered"
	once all pending event queue events have been drained
      - go onto processing gui events

    Using a main return will most likely leak memory like a sieve, but
    in balance, the application just got a fatal signal and the leak
    is unlikely to be the greatest concern.  */
void
Signal::fatal (int sig, siginfo_t *info, void *x)
{
    ASSERT (s_fatalReturn || (s_fatalOptions & FATAL_AUTO_EXIT));

#if !HAVE_POSIX_SIGNALS
    // Reinstall the handler for poor SVR4 systems that reset signal
    // handlers upon delivery.  I doubt this code ever gets run on a
    // system without sigaction, but let's be ultracorrect.
    handle (sig, &fatal);
#endif

    // Unblock the signal itself so that if we get this again, we'll
    // enter the handler again.  Otherwise if the user's hook or
    // fatalDump has trouble, we'll hang until someone sends us a
    // different signal.
    block (sig, false);

    // Check that we aren't going too deep in fatal handlers.  We
    // allow a little nesting as sometimes the handlers gets tangled
    // up producing a dump, but after an extra signal can finish.  If
    // the nesting exceeds our limit, we give up and exit with default
    // signal behaviour: no hooks any more, they've had their chance.
    if (++s_inFatal > 4)
    {
	revert (sig);
	raise (sig);
    }

    // Check if this signal is fatal.  If so, indicate we've crashed.
    bool fatal = (sig != SIGINT) || (s_fatalOptions & FATAL_ON_INT);
    if (fatal)
	s_crashed = true;

    // Create core file if requested (without interrupting the program)
    bool haveCore = false;
    if (s_inFatal == 1 && fatal && (s_fatalOptions & FATAL_DUMP_CORE))
    {
        DebugAids::coredump (sig);
#ifndef _WIN32
	struct stat st;
	haveCore = (::stat ("core", &st) == 0
		    && S_ISREG (st.st_mode)
		    && st.st_size > 0);
#endif
    }

    // Check if we are done -- if so, commit a suicide, it should be
    // painless.  s_fatalHook and s_quitHook ought to be protected
    // from infinitely looping signals, either using #fatalLevel() or
    // deregistering themselves on the first call.
    if (s_inFatal > 1 || (s_fatalOptions & FATAL_AUTO_EXIT))
    {
	if (s_fatalHook
	    ? (*s_fatalHook) (haveCore ? -sig : sig, info, x)
	    : fatalDump (haveCore ? -sig : sig, info, x))
	    // Suicide: re-raise the signal; we'll die as we return.
	    quit (sig, info, x);
        return;
    }

    // Return to main program.
    --s_inFatal;
    (*s_fatalReturn) (haveCore ? -sig : sig, info, x);
}

/** Return the description for signal info code @a code for signal
    number @a sig.  The code should come from @c siginfo_t::si_code.  */
const char *
Signal::describe (int sig, int code)
{
    static struct { int sig; int code; const char *desc; } s_infos [] = {
#if HAVE_POSIX_SIGNALS
	{ -1,	   SI_USER,	"user sent: kill, sigsend or raise" },
# ifdef SI_KERNEL
	{ -1,	   SI_KERNEL,	"kernel" },
# endif
	{ -1,	   SI_QUEUE,	"sigqueue" },
	{ -1,	   SI_TIMER,	"timer expired" },
	{ -1,	   SI_MESGQ,	"mesq state changed" },
	{ -1,	   SI_ASYNCIO,	"AIO completed" },
# ifdef SI_SIGIO // not solaris
	{ -1,	   SI_SIGIO,	"queued SIGIO" },
# endif

# ifdef ILL_NOOP // darwin
	{ SIGILL,  ILL_NOOP,	"noop" },
# endif
	{ SIGILL,  ILL_ILLOPC,	"illegal opcode" },
# ifdef ILL_ILLOPN // not darwin
	{ SIGILL,  ILL_ILLOPN,	"illegal operand" },
# endif
# ifdef ILL_ILLADR // not darwin
	{ SIGILL,  ILL_ILLADR,	"illegal addressing mode" },
# endif
	{ SIGILL,  ILL_ILLTRP,	"illegal trap" },
	{ SIGILL,  ILL_PRVOPC,	"priveleged opcode" },
# ifdef ILL_PRVREG // not darwin
	{ SIGILL,  ILL_PRVREG,	"privileged register" },
# endif
# ifdef ILL_COPROC // not darwin
	{ SIGILL,  ILL_COPROC,	"coprocessor error" },
# endif
# ifdef ILL_BADSTK // not darwin
	{ SIGILL,  ILL_BADSTK,	"internal stack error" },
# endif

# ifdef FPE_NOOP // darwin
	{ SIGFPE,  FPE_NOOP,	"noop" },
# endif
# ifdef FPE_INTDIV // not darwin
	{ SIGFPE,  FPE_INTDIV,	"integer divide by zero" },
# endif
# ifdef FPE_INTOVF // not darwin
	{ SIGFPE,  FPE_INTOVF,	"integer overflow" },
# endif
	{ SIGFPE,  FPE_FLTDIV,	"flaoting point divide by zero" },
	{ SIGFPE,  FPE_FLTOVF,	"floating point overflow" },
	{ SIGFPE,  FPE_FLTUND,	"floating point underflow" },
	{ SIGFPE,  FPE_FLTRES,	"floating point inexact result" },
	{ SIGFPE,  FPE_FLTINV,	"floating point in valid operation" },
# ifdef FPE_FLTSUB // not darwin
	{ SIGFPE,  FPE_FLTSUB,	"subscript out of range" },
# endif

# ifdef SEGV_NOOP // darwin
	{ SIGSEGV, SEGV_NOOP,	"noop" },
# endif
	{ SIGSEGV, SEGV_MAPERR,	"address not mapped to object" },
	{ SIGSEGV, SEGV_ACCERR,	"invalid permissions for mapped object" },

# ifdef BUS_NOOP // darwin
	{ SIGBUS,  BUS_NOOP,	"noop" },
# endif
	{ SIGBUS,  BUS_ADRALN,	"invalid address alignment" },
# ifdef BUS_ADRERR // not darwin
	{ SIGBUS,  BUS_ADRERR,	"non-existent physical address" },
# endif
# ifdef BUS_OBJERR // not darwin
	{ SIGBUS,  BUS_OBJERR,	"object specific hardware error" },
# endif

# ifdef TRAP_BRKPT // not darwin
	{ SIGTRAP, TRAP_BRKPT,	"process break point" },
# endif
# ifdef TRAP_TRACE // not darwin
	{ SIGTRAP, TRAP_TRACE,	"process trace trap" },
# endif
	    
# ifdef CLD_NOOP // darwin
	{ SIGCHLD, CLD_NOOP,	"noop" },
# endif
	{ SIGCHLD, CLD_EXITED,	"child has exited" },
	{ SIGCHLD, CLD_KILLED,	"child was killed" },
	{ SIGCHLD, CLD_DUMPED,	"child terminated abnormally" },
	{ SIGCHLD, CLD_TRAPPED,	"traced child has trapped" },
	{ SIGCHLD, CLD_STOPPED,	"child has stopped" },
	{ SIGCHLD, CLD_CONTINUED,"stopped child has continued" },

# ifdef SIGPOLL // not darwin
	{ SIGPOLL, POLL_IN,	"data input available" },
	{ SIGPOLL, POLL_OUT,	"output buffers available" },
	{ SIGPOLL, POLL_MSG,	"input message available" },
	{ SIGPOLL, POLL_ERR,	"i/o error" },
	{ SIGPOLL, POLL_PRI,	"high priority input available" },
	{ SIGPOLL, POLL_HUP,	"device disconnected" },
# endif
#endif  // HAVE_POSIX_SIGNALS

	{ -1,      -1,		0 }
    };

    for (unsigned i = 0; s_infos [i].desc; ++i)
	if ((s_infos [i].sig == -1 || s_infos [i].sig == sig)
	    && s_infos [i].code == code)
	    return s_infos [i].desc;

    return "*unknown reason*";
}

/** Utility function to dump the signal info descriptor for signal @a
    sig, as obtained for instance through signal handler paramters or
    #wait().  The output is written directly to the file descriptor @a
    fd, using @a buf as the formatting buffer.  */
void
Signal::dumpInfo (IOFD fd, char *buf, int sig, const siginfo_t *info)
{
    if (! info)
	return;

#ifdef _WIN32
# define DOCODE(x) case x: name = #x
    // NB: siginfo_t == EXCEPTION_RECORD.
    const char *name = 0;

    switch (info->ExceptionCode)
    {
	DOCODE(STATUS_ABANDONED_WAIT_0);
	DOCODE(STATUS_ACCESS_VIOLATION);
	DOCODE(STATUS_ARRAY_BOUNDS_EXCEEDED);
	DOCODE(STATUS_BREAKPOINT);
	DOCODE(STATUS_CONTROL_C_EXIT);
	DOCODE(STATUS_DATATYPE_MISALIGNMENT);
	DOCODE(STATUS_FLOAT_DENORMAL_OPERAND);
	DOCODE(STATUS_FLOAT_DIVIDE_BY_ZERO);
	DOCODE(STATUS_FLOAT_INEXACT_RESULT);
	DOCODE(STATUS_FLOAT_INVALID_OPERATION);
	DOCODE(STATUS_FLOAT_OVERFLOW);
	DOCODE(STATUS_FLOAT_STACK_CHECK);
	DOCODE(STATUS_FLOAT_UNDERFLOW);
	DOCODE(STATUS_GUARD_PAGE_VIOLATION);
	DOCODE(STATUS_ILLEGAL_INSTRUCTION);
	DOCODE(STATUS_INTEGER_DIVIDE_BY_ZERO);
	DOCODE(STATUS_INTEGER_OVERFLOW);
	DOCODE(STATUS_INVALID_DISPOSITION);
	DOCODE(STATUS_IN_PAGE_ERROR);
	DOCODE(STATUS_NONCONTINUABLE_EXCEPTION);
	DOCODE(STATUS_NO_MEMORY);
	DOCODE(STATUS_PENDING);
	DOCODE(STATUS_PRIVILEGED_INSTRUCTION);
	DOCODE(STATUS_SINGLE_STEP);
	DOCODE(STATUS_STACK_OVERFLOW);
	DOCODE(STATUS_TIMEOUT);
	DOCODE(STATUS_USER_APC);
	DOCODE(STATUS_WAIT_0);
    }
    // -> DWORD ExceptionCode
    // -> DWORD ExceptionFlags
    // -> EXCEPTION_RECORD *ExceptionRecord
    // -> PVOID ExceptionAddress
    // -> DWORD NumberParameters
    // -> DWORD ExceptionInfo [MAX_PARAMETERS (15)]
    if (name)
	MYWRITE (fd, buf, sprintf (buf, "Exception: %s\n", name));
    else
	MYWRITE (fd, buf, sprintf (buf, "Exception %lu\n",
				   info->ExceptionCode));
    MYWRITE (fd, buf, sprintf (buf, "  addr = %08lx", info->ExceptionAddress));

#elif HAVE_POSIX_SIGNALS
    // These should always be set.
    write (fd, buf, sprintf (buf,
			     "  signo  = %d, errno = %d, code = %d (%s)\n",
			     info->si_signo, info->si_errno, info->si_code,
			     describe (sig, info->si_code)));

    // These are set if the signal was sent by kill, POSIX signal
    // send or SIGCHLD.
    write (fd, buf, sprintf (buf, "  pid    = %ld, uid = %ld\n",
			     (long) info->si_pid, (long) info->si_uid));

    // Child status for SIGCHLD.
    if (sig == SIGCHLD)
	write (fd, buf, sprintf (buf,
				 "  status = %ld, utime = %ld, stime = %ld\n",
				 (long) info->si_status,
				 (long) info->si_utime,
				 (long) info->si_stime));

    // These are set if the POSIX signal sender passed them.
    write (fd, buf, sprintf (buf, "  value  = (%d, %p)\n",
			     info->si_int, info->si_ptr));

    // This is the interesting address for memory faults.
    if (sig == SIGILL || sig == SIGFPE || sig == SIGSEGV || sig == SIGBUS)
	write (fd, buf, sprintf (buf, "  addr   = %p\n", info->si_addr));

# ifdef SIGPOLL // not darwin
    // SIGPOLL status data.
    if (sig == SIGPOLL)
	write (fd, buf, sprintf (buf, "  band   = %ld, fd = %d\n",
				 (long) info->si_band, info->si_fd));
# endif
#endif // HAVE_POSIX_SIGNALS
}

/** Utility function to dump memory section from @a data for @a n
    bytes.  Used to dump machine context on platforms where we don't
    know any better.  The output is written directly to the file
    descriptor @a fd, using @a buf as the formatting buffer.  */
void
Signal::dumpMemory (IOFD fd, char *buf, const void *data, size_t n)
{
    for (size_t i = 0; i < n; )
    {
	size_t m = sprintf (buf, "\n ");
	for (size_t j = 0; i < n && j < 32; ++j, ++i)
	    m += sprintf (buf + m, "%s%02x",
			  j % 4 == 0 ? " " : "",
			  (unsigned int) (((const unsigned char *) data) [i]));

	MYWRITE (fd, buf, m);
    }
}

/** Utility function to dump the process context, as obtained for
    instance through signal handler paramters, unix @c getcontext() or
    Windows @c GetThreadContext().  The output is written directly to
    the file descriptor @a fd, using @a buf as the formatting buffer.  */
void
Signal::dumpContext (IOFD fd, char *buf, const void *context)
{
#if defined _WIN32 && defined _M_IX86
    const CONTEXT *uc = static_cast<const CONTEXT *> (context);
    MYWRITE (fd, buf, sprintf (buf, "\n"
			       "\n  eip: %04lx:%08lx           eflags: %08lx"
			       "\n  eax: %08lx   ebx: %08lx"
			       "   ecx: %08lx   edx: %08lx"
			       "\n  esi: %08lx   edi: %08lx"
			       "   ebp: %08lx   esp: %08lx"
			       "\n   ds: %04lx        es: %04lx"
			       "    fs: %04lx        ss: %04lx",
			       uc->SegCs, uc->Eip, uc->EFlags,
			       uc->Eax, uc->Ebx, uc->Ecx, uc->Edx,
			       uc->Esi, uc->Edi, uc->Ebp, uc->Esp,
			       uc->SegDs, uc->SegEs, uc->SegFs, uc->SegSs));

    MYWRITE (fd, buf, sprintf (buf,
			       "\n  FPU:  control = %08lx"
			       "\n        status  = %08lx"
			       "\n        tag     = %08lx"
			       "\n        ip      = %04lx:%08lx"
			       "\n        data    = %04lx:%08lx"
			       "\n        state   = %08lx",
			       uc->FloatSave.ControlWord,
			       uc->FloatSave.StatusWord,
			       uc->FloatSave.TagWord,
			       uc->FloatSave.ErrorSelector,
			       uc->FloatSave.ErrorOffset,
			       uc->FloatSave.DataSelector,
			       uc->FloatSave.DataOffset,
			       uc->FloatSave.Cr0NpxState));

    for (int i = 0; i < 8; ++i)
	MYWRITE (fd, buf, sprintf (buf,
				   "\n    %%fp%d = [%02x%02x:%02x%02x%02x%02x"
				   "%02x%02x%02x%02x]",
				   i,
				   uc->FloatSave.RegisterArea [i * 10 + 0],
				   uc->FloatSave.RegisterArea [i * 10 + 1],
				   uc->FloatSave.RegisterArea [i * 10 + 2],
				   uc->FloatSave.RegisterArea [i * 10 + 3],
				   uc->FloatSave.RegisterArea [i * 10 + 4],
				   uc->FloatSave.RegisterArea [i * 10 + 5],
				   uc->FloatSave.RegisterArea [i * 10 + 6],
				   uc->FloatSave.RegisterArea [i * 10 + 7],
				   uc->FloatSave.RegisterArea [i * 10 + 8],
				   uc->FloatSave.RegisterArea [i * 10 + 9]));
    MYWRITE (fd, "\n", 1);

#elif HAVE_POSIX_SIGNALS
    // FIXME: how much of this is defined in POSIX or ABIs?
    const ucontext_t *uc = static_cast<const ucontext_t *> (context);
    const mcontext_t *mc = &uc->uc_mcontext;
    write (fd, buf, sprintf (buf, "  stack  = (%x, %x, %p)",
			     uc->uc_stack.ss_flags,
			     unsigned(uc->uc_stack.ss_size),
			     uc->uc_stack.ss_sp));

    write (fd, buf, sprintf (buf, "\n"));
#if defined __i386 && defined __linux
# if !defined REG_CS && defined CS
#  define REG_CS	CS
#  define REG_DS	DS
#  define REG_ES	ES
#  define REG_FS	FS
#  define REG_SS	SS
#  define REG_EIP	EIP
#  define REG_EFL	EFL
#  define REG_EAX	EAX
#  define REG_EBX	EBX
#  define REG_ECX	ECX
#  define REG_EDX	EDX
#  define REG_ESI	ESI
#  define REG_EDI	EDI
#  define REG_EBP	EBP
#  define REG_ESP	ESP
#  define REG_UESP	UESP
#  define REG_TRAPNO	TRAPNO
#  define REG_ERR	ERR
# endif
    write (fd, buf, sprintf (buf,
			     "\n  eip: %04x:%08x           eflags: %08x"
			     "\n  eax: %08x   ebx: %08x"
			     "   ecx: %08x   edx: %08x"
			     "\n  esi: %08x   edi: %08x"
			     "   ebp: %08x   esp: %08x"
			     "\n   ds: %04x        es: %04x"
			     "        fs: %04x        ss: %04x",
			     mc->gregs [REG_CS] & 0xffff, mc->gregs [REG_EIP],
			     mc->gregs [REG_EFL],
			     mc->gregs [REG_EAX], mc->gregs [REG_EBX],
			     mc->gregs [REG_ECX], mc->gregs [REG_EDX],
			     mc->gregs [REG_ESI], mc->gregs [REG_EDI],
			     mc->gregs [REG_EBP], mc->gregs [REG_ESP],
			     mc->gregs [REG_DS] & 0xffff,
			     mc->gregs [REG_ES] & 0xffff,
			     mc->gregs [REG_FS] & 0xffff,
			     mc->gregs [REG_SS] & 0xffff));

    write (fd, buf, sprintf (buf,
			     "\n\n  signal esp: %08x"
			     "  trap: %d/%d"
			     "  oldmask: %08lx   cr2: %08lx",
			     mc->gregs [REG_UESP],
			     mc->gregs [REG_TRAPNO], mc->gregs [REG_ERR],
			     mc->oldmask, mc->cr2));

    if (mc->fpregs)
    {
	write (fd, buf, sprintf (buf,
				 "\n"
				 "\n  FPU:  control = %08lx"
				 "\n        status  = %08lx"
				 "\n        tag     = %08lx"
				 "\n        ip      = %04lx:%08lx"
				 "\n        data    = %04lx:%08lx"
				 "\n        state   = %08lx",
				 mc->fpregs->cw, mc->fpregs->sw, mc->fpregs->tag,
				 mc->fpregs->cssel & 0xffff, mc->fpregs->ipoff,
				 mc->fpregs->datasel & 0xffff, mc->fpregs->dataoff,
				 mc->fpregs->status));

	for (int i = 0; i < 8; ++i)
	    write (fd, buf, sprintf (buf,
				     "\n    %%fp%d = [%04hx:%04hx%04hx%04hx%04hx]",
				     i,
				     mc->fpregs->_st [i].exponent,
				     mc->fpregs->_st [i].significand [0],
				     mc->fpregs->_st [i].significand [1],
				     mc->fpregs->_st [i].significand [2],
				     mc->fpregs->_st [i].significand [3]));
    }
    
#elif __APPLE__ && defined __ppc__
    write (fd, buf, sprintf (buf, "\n  dar: %08lx   dsisr: %08lx  exception: %08lx",
			     (*mc)->es.dar, (*mc)->es.dsisr, (*mc)->es.exception));

    write (fd, buf, sprintf (buf,
			     "\n  srr0: %08x  srr1: %08x   cr: %08x         xer: %08x"
			     "\n  lr: %08x    ctr: %08x    vrsave: %08x     fpscr: %08x",
			     (*mc)->ss.srr0, (*mc)->ss.srr1, (*mc)->ss.cr, (*mc)->ss.xer,
			     (*mc)->ss.lr, (*mc)->ss.ctr, (*mc)->ss.vrsave, (*mc)->fs.fpscr));

    write (fd, buf, sprintf (buf, "\n  vrvalid: %08x  vscr: %08lx:%08lx:%08lx:%08lx\n",
			     (*mc)->vs.save_vrvalid,
			     (*mc)->vs.save_vscr [0], (*mc)->vs.save_vscr [1],
			     (*mc)->vs.save_vscr [2], (*mc)->vs.save_vscr [3]));

    for (unsigned int *regs = &(*mc)->ss.r0, i = 0; i < 32; i += 4)
        write (fd, buf, sprintf (buf, "\n  r%-2d  %08x   r%-2d  %08x   r%-2d  %08x   r%-2d  %08x",
				 i, regs [i], i+1, regs [i+1], i+2, regs [i+2], i+3, regs [i+3]));
    for (int i = 0; i < 32; ++i)
        write (fd, buf, sprintf (buf, "\n  fp%-2d %016qx (%f)", i,
				 *(unsigned long long *) &(*mc)->fs.fpregs [i],
				 (*mc)->fs.fpregs [i]));
    for (int i = 0; i < 32; ++i)
	write (fd, buf, sprintf (buf, "\n  vr%-2d %08lx:%08lx:%08lx:%08lx", i,
				 (*mc)->vs.save_vr[i][0], (*mc)->vs.save_vr[i][1],
				 (*mc)->vs.save_vr[i][2], (*mc)->vs.save_vr[i][3]));
#elif __sun
    for (int i = 0; i < NGREG; i++)
	write (fd, buf, sprintf (buf, "%s  %%r%02d = %08x",
				 i % 4 == 0 ? "\n" : "", i, mc->gregs [i]));
#else
    dumpMemory (fd, buf, mc, sizeof (*mc));
#endif // __i386 && __linux, __sun, other

    write (fd, "\n", 1);
#endif // HAVE_POSIX_SIGNALS
}

/** Dump application state information on a fatal signal.

    Use this method to dump program state on a delivery of a fatal
    signal.  #fatal() uses this function automatically if no fatal
    handler hook has not been registered by the application.

    This function attempts to be as maximally robust given that it
    runs in a signal handler in conditions where the program by
    definition is unstable.  In other words, it allocates no memory
    and writes its output directly to a file descriptor with direct
    system calls.  For this reason some initialisation is required;
    use #handleFatal() to register the current application name and an
    output file descriptor, preferably as early in the program as
    possible.

    The dump will consist of the following items:
     - if #FATAL_DUMP_SIG option is set:
       - the application name if registered with #handleFatal()
       - a title describing telling which fatal signal has been
         received (defined by @a sig, the signal number, or its
         negative if core has been dumped)
       - information available from the @a info argument
     - if #FATAL_DUMP_CONTEXT option is set, all the available
       CPU context information like registers
     - if #FATAL_DUMP_STACK option is set, the stack trace
     - if #FATAL_DUMP_LIBS option is set, the list of currently
       loaded shared libraries

    This always returns @c true so it is convenient for the
    application fatal hook to return with a call to this function.

    Note that this function will not flush @c std::cerr or @c stderr
    before producing output, for stability reasons.  If the streams
    have unflushed output in their buffers, that output may get mixed
    with unbuffered direct output from this function.  If you wish to
    avoid this mixup and are willing to take the risk that those calls
    might crash, install an application hook that flushes the streams
    and then calls this function. */
bool
Signal::fatalDump (int sig, siginfo_t *info, void *extra)
{
    char buf [SIGNAL_MESSAGE_BUFSIZE];  // = [256], %.200s below is connected
    bool haveCore = false;
    if (sig < 0)
    {
	sig = -sig;
	haveCore = true;
    }

    if (s_fatalOptions & FATAL_DUMP_SIG)
    {
	MYWRITE (s_fatalFd, "\n", 1);
	if (s_applicationName)
	{
	    MYWRITE (s_fatalFd, s_applicationName,
		     STDC::strlen (s_applicationName));
	    MYWRITE (s_fatalFd, " ", 1);
	}

	MYWRITE (s_fatalFd, buf,
		 sprintf (buf, "(pid=%ld ppid=%ld) received fatal signal %d"
			  " (%.100s)%s\n",
			  (long) ProcessInfo::pid (), (long) ProcessInfo::ppid (),
			  sig, name (sig), haveCore ? " (core dumped)" : ""));

	MYWRITE (s_fatalFd, buf, sprintf(buf,"signal context:\n"));
	dumpInfo (s_fatalFd, buf, sig, info);
    }

    if (s_fatalOptions & FATAL_DUMP_CONTEXT)
	dumpContext (s_fatalFd, buf, extra);

    if (s_fatalOptions & FATAL_DUMP_STACK)
    {
	MYWRITE (s_fatalFd, buf, sprintf(buf,"\nstack trace:\n"));
	DebugAids::stacktrace (s_fatalFd);
    }

    if (s_fatalOptions & FATAL_DUMP_LIBS)
    {
	MYWRITE (s_fatalFd, buf, sprintf(buf,"\nshared libraries present:\n"));
	try { SharedLibrary::loaded (*SignalDumpCallback); }
	catch (...) { ; }
    }

    return true;
}

/** Return the file descriptor #fataldump() uses for output.
    Registered through #handleFatal().  */
IOFD
Signal::handleFatalFd (void)
{
    // Automatically initialise on first access.
    if (s_fatalFd == IOFD_INVALID)
	s_fatalFd = STDERR_HANDLE;

    return s_fatalFd;
}

/** Return the application fatal signal hook.  Registered through
    #handleFatal().  @sa #fatal() */
Signal::FatalHook
Signal::handleFatalHook (void)
{ return s_fatalHook; }

/** Return the application fatal signal return hook.  Registered
    through #handleFatal().  @sa #fatal().  */
Signal::FatalReturn
Signal::handleFatalReturn (void)
{ return s_fatalReturn; }

/** Return the current fatal signal handling options.  Set on
    invocation to #handleFatal().  */
unsigned
Signal::handleFatalOptions (void)
{ return s_fatalOptions; }

/** Return the current application quit signal hook.  Registered
    through #handleQuit().  */
Signal::QuitHook
Signal::handleQuitHook (void)
{ return s_quitHook; }

/** Return the depth to which #fatal() is currently recursively
    entered, or zero if #fatal() is not currently active.  Use this
    method in application fatal hook to decide which operations are
    safe to perform.  For example, if the attempts to notify the user
    result in further signals, it is best to avoid such attempts at
    deeper recursion levels.  Currently #fatal() ceases to call the
    application's hooks and forces termination if the nesting level
    reaches 4. */
int
Signal::fatalLevel (void)
{ return s_inFatal; }

/** Return the crash status indicator: @c true if a fatal signal has
    been received since the program started.  Set if #fatal() is
    entered with a fatal signal.  */
bool
Signal::crashed (void)
{ return s_crashed; }

} // namespace lat
