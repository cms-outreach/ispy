#ifndef CLASSLIB_SYSAPI_SIGNAL_H
# define CLASSLIB_SYSAPI_SIGNAL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"
# include <cstdio>
# include <cstdlib>
# include <cerrno>

# ifdef _WIN32
#  include <windows.h>
# else
#  include <unistd.h>
#  include <sys/stat.h>
#  include <ucontext.h>
# endif

//<<<<<< PUBLIC DEFINES                                                 >>>>>>

// Not all systems with POSIX signals necessarily have SA_SIGINFO.
# ifndef SA_SIGINFO
#  define SA_SIGINFO 0
# endif

// Hacks for fields we might not have in siginfo_t.  Just print zero.
# if !HAVE_SIGINFO_SI_UTIME		// darwin
#  define si_utime si_signo ? 0 : 0
#  define si_stime si_signo ? 0 : 0
# endif
# if !HAVE_SIGINFO_SI_INT
#  if HAVE_SIGINFO_SI_VALUE		// darwin
#   define si_int   si_value.sigval_int
#   define si_ptr   si_value.sigval_ptr
#  else					// (none known)
#   define si_int   si_signo ? 0 : 0
#   define si_ptr   si_signo ? (void *) 0 : (void *) 0
#  endif
# endif

// Windows doesn't have this, so fake a suitable substitute
# ifdef _WIN32
#  define STDERR_HANDLE GetStdHandle (STD_ERROR_HANDLE)
# else
#  define STDERR_HANDLE STDERR_FILENO
# endif

// Define a suitable wrapper to write to system file descriptors.
// This is needed because on Windows we are using HANDLEs, not the
// compiler's crippled posixy interface.
# ifdef _WIN32
#  define MYWRITE(fd,data,n)	do { DWORD written; WriteFile(fd,data,n,\
					&written,0); } while (0)
# else
#  define MYWRITE(fd,data,n)	write(fd,data,n)
# endif

//<<<<<< PUBLIC CONSTANTS                                               >>>>>>

/** Maximum length of a signal message.  Used for local format buffers
    for the signal number and name message in #Signal::fataldump() and
    the currently loaded shared library message in #SignalLibDump().
    Make this long enough to fit long shared library names.  */
static const int SIGNAL_MESSAGE_BUFSIZE = 256;

//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>

#if HAVE_RAISE && !HAVE_RAISE_DECL
// raise() declaration for systems that have the function but do
// not declare it in their headers.  (FIXME: Are there any?)
extern "C" int raise (int signalnr);
#endif

#if HAVE_STRSIGNAL && !HAVE_STRSIGNAL_DECL
// strsignal() declaration for systems that have the function but do
// not declare it in their headers.  (FIXME: Are there any?)
extern "C" char *strsignal (int signalnr);
#endif

//<<<<<< CLASS DECLARATIONS                                             >>>>>>
//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // CLASSLIB_SYSAPI_SIGNAL_H
