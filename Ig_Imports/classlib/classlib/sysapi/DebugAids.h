#ifndef CLASSLIB_SYSAPI_DEBUG_AIDS_H
# define CLASSLIB_SYSAPI_DEBUG_AIDS_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"

# include <cctype>
# include <cstdio>
# include <cstdlib>
# include <iostream>
# include <iomanip>

# ifdef _WIN32
#  include <windows.h>
#  include <winnt.h>
#  include <imagehlp.h>
//#  include <io.h>
# else
#  include <unistd.h>
#  include <sys/wait.h>
#  if HAVE_BACKTRACE_SYMBOLS_FD		// GNU
#   include <execinfo.h>
#   include <sys/uio.h>
#  endif
#  if HAVE_DLADDR			// Linux, Solaris
#   include <dlfcn.h>
#  endif
# if HAVE_EXCEPTION_H
    // This is yucky.  KCC's <exception.h> that has nothing to do with the
    // header we are looking for (it redirect to <exception>).  This ugly
    // workaround allows us to find the (IRIX) header we are looking for.
#   if defined __KCC && defined __sgi
#     include </usr/include/exception.h>
#    elif defined __sgi
#     include <exception.h>
#    endif
#  endif
#  if HAVE_EXCPT_H			// IRIX
#   include <excpt.h>
#   undef try	  // Defined on SGI to structured exception handling goop
#   undef catch   // Defined on SGI to structured exception handling goop
#  endif
#  if HAVE_RLD_INTERFACE_H		// Tru64 (IRIX)
#   include <rld_interface.h>
#  endif
#  if HAVE_PDSC_H			// Tru64
#   include <pdsc.h>
#  endif
#  if __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) // GCC 3.4+ C++ ABI
#   include <sys/uio.h>
#  endif
# endif

//<<<<<< PUBLIC DEFINES                                                 >>>>>>

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

// Helper to write literals
# define MYWRITELIT(fd,str) MYWRITE(fd,str,sizeof(str)-1)

//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>

#if HAVE_BACKTRACE_SYMBOLS_FD
/** The maximum stack trace depth for systems where we request the
    stack depth separately (GNU libc-based systems).  */
static const int MAX_BACKTRACE_DEPTH = 128;
#endif

//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>
//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // CLASSLIB_SYSAPI_DEBUG_AIDS_H
