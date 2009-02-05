//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Framework/IgApplication/interface/IgOnCrashService.h"
#include "classlib/utils/DebugAids.h"
#include <iostream>
#include <stdio.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IgOnCrashService::FatalSignalHook
IgOnCrashService::s_signalHook = defaultFatalSignal;

IgOnCrashService::FatalExceptionHook
IgOnCrashService::s_exceptionHook = defaultFatalException;

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

bool
IgOnCrashService::init (const char *appname)
{
    // FIXME: For GUI app drivers, install fatal X error handler too
    // (or redirect whatever Qt has already), and hook it into
    // fatal_error_hook structure.

    // FIXME: Maybe install a graphical notification dialog in the app
    // driver?  If yes, create the dialog beforehand, and when showing
    // it, make sure the dialog is mapped and X is synchronised before
    // returning from the equivalent of the fatal_error_hook().  This
    // will allow us to exit if we get another signal (or an X error?)
    // during that time.

    // FIXME: If we have subprocesses that we are communicating with
    // (or a thread, or another interruptable subunit), the SIGINT
    // handler should send an interrupt to it (a C-c character for
    // example).

    // FIXME: App driver should disable core dump on fatal signals
    // while in the main event loop (that is, allow them from init
    // till exit sequence begins) -- except if the user has requested
    // otherwise.

    // FIXME: Install a quit cleanup handler?

    ASSERT (s_signalHook == defaultFatalSignal);
    ASSERT (s_exceptionHook == defaultFatalException);
    lat::Signal::handleFatal (appname, -1, fatalSignal);
    return true;
}

/** Set fatal signal handler.

    Sets current fatal signal handler to @a hook and returns the old
    one.  If @a hook is null, doesn't change the current value, just
    returns the old one.  The caller should restore the old handler
    when the new one becomes invalid.  It can also call the old
    handlers in a chain if desirable.  */
IgOnCrashService::FatalSignalHook
IgOnCrashService::fatalSignalHook (FatalSignalHook hook)
{
    FatalSignalHook old = s_signalHook;
    if (hook)
	s_signalHook = hook;
    return old;
}

/** Set fatal exception handler.

    Sets current fatal exception handler to @a hook and returns the
    old one.  If @a hook is null, doesn't change the current value,
    just returns the old one.  The caller should restore the old
    handler when the new one becomes invalid.  It can also call the
    old handlers in a chain if desirable.  */
IgOnCrashService::FatalExceptionHook
IgOnCrashService::fatalExceptionHook (FatalExceptionHook hook)
{
    FatalExceptionHook old = s_exceptionHook;
    if (hook)
	s_exceptionHook = hook;
    return old;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Title template for crashes; has two %s args: topic, title.  */
const char *
IgOnCrashService::fatalTitle (void)
{
    return "\n%s (%s).\n\n";
}

/** Message template for crashes; has one %s arg: reason.  */
const char *
IgOnCrashService::fatalMessage (void)
{
    // FIXME: put in apps etc. from service constructor args?
    return "Oh dear!  You have found a bug in " IGUANA_APP ".\n\n"

	"If you can reproduce this bug, please send a bug report\n"
	"to <iguana-developers@cern.ch>, giving a subject like\n\n"

	"    %s in " IGUANA_APP " " IGUANA_VERSION " (" IGUANA_HOST ")\n\n"

	"To enable us to fix the bug, please include the following information:\n"
	"* What you were doing to get this message.  Please report all the facts.\n"
	"* The following build configuration information:\n"
	"   " IGUANA_UNAME "\n"
	"   " IGUANA_CXX " (" IGUANA_COMPILER ")\n"
	"   built by " IGUANA_WHO " on " __DATE__ " " __TIME__ "\n"
	"* The stack trace and loaded libraries list following this message.\n"
	"Please read also the section \"Reporting Bugs\" in the " IGUANA_APP " manual.\n\n"

	"We thank you for your support and our apologies for the inconvenience.\n\n";
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
IgOnCrashService::printFatalMessage (const char *topic, const char *title,
				     const char *reason)
{
    std::cerr.flush ();
    fprintf (stderr, fatalTitle (), topic, title);
    fprintf (stderr, fatalMessage (), reason);
}

bool
IgOnCrashService::fatalSignal (int sig, siginfo_t *info, void *x)
{
    // FIXME: should interrupt here any subprocesses/threads
    // FIXME: should this be a service others can hook into?
    // FIXME: deal with recursive crashes
    // FIXME: be smarter about SIGINT
    ASSERT (s_signalHook);
    return s_signalHook (sig, info, x);
}

bool
IgOnCrashService::defaultFatalSignal (int sig, siginfo_t *info, void *x)
{
    char	buf [128];
    int		signum = sig < 0 ? -sig : sig;
    bool	have_core = sig < 0;
    const char	*name = lat::Signal::name (signum);

    if (name)
	sprintf (buf, "`%.100s' signal", name);
    else
	sprintf (buf, "Signal %d", signum);

    if (have_core)
	strcat (buf, " (core dumped)");

    printFatalMessage ("Internal error", buf, buf);
    return lat::Signal::fatalDump (sig, info, x);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
IgOnCrashService::explainException (const char *type,
				    const char *what,
				    std::string &topic,
				    std::string &title,
				    std::string &reason)
{
    if (! what || ! *what)
	what = "C++ exception";

    topic  = "Internal error";
    title  = what;
    reason = what;

    // GCC prepends the name length to type names, strip them off
    while (type && isdigit (*type))
	type++;

    if (type && *type)
	topic = type;

    // Convert underscores to spaces
    std::string::size_type pos = topic.find ('_');
    while (pos != std::string::npos)
    {
	topic.replace (pos, 1, 1, ' ');
	pos = topic.find ('_');
    }

    // Uppercase first character
    topic[0] = toupper (topic[0]);
    if (type && *type)
	reason.insert (0, topic + ": ");
}

void
IgOnCrashService::printFatalException (const char *type, const char *what)
{
    std::string	title;
    std::string reason;
    std::string topic;

    explainException (type, what, title, reason, topic);
    printFatalMessage (title.c_str (), reason.c_str (), topic.c_str ());
}

void
IgOnCrashService::fatalException (const char *type, const char *what)
{
    // FIXME: should be dialog in a graphical app!
    // FIXME: should interrupt here any subprocesses/threads
    // FIXME: should this be a service others can hook into?
    
    ASSERT (s_exceptionHook);
    s_exceptionHook (type, what);
}

void
IgOnCrashService::defaultFatalException (const char *type, const char *what)
{
    printFatalException (type, what);
}
