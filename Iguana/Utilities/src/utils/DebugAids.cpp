//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/utils/DebugAids.h"
#include "classlib/utils/Signal.h"
#include "classlib/utils/Log.h"
#include "classlib/utils/SharedLibrary.h"
#include "classlib/utils/BitTraits.h"
#include "classlib/utils/StringFormat.h"
#include "classlib/sysapi/DebugAids.h"
#include <cstring>

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

/** The current assertion handler.  */
DebugAids::AssertHook	DebugAids::s_assertHook = &DebugAids::failStdxx;

/** The number of assertions to ignore.  */
int			DebugAids::s_assertIgnoreCount = 0;

/** The output file descriptor for #failStdxx().  */
IOFD			DebugAids::s_assertOutputFd = IOFD_INVALID;

/** The default output file descriptor for #stacktrace().  */
IOFD			DebugAids::s_stackTraceFd = IOFD_INVALID;

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>

#ifdef _WIN32
// /** WIN32 function to grab the current PC address from the SEH context.
//     We need this to grab the exception context so we can walk the stack
//     in #Debug::stacktrace().  We use SEH (as compiler-independently as
//     we can) as only XP 64-bit has RtlGetContext() function.  */
// static LONG CALLBACK
// GrabExceptionContext (PEXCEPTION_POINTERS info)
// {
//     *((CONTEXT *) info->ExceptionRecord->ExceptionInformation[0])
// 	= *info->ContextRecord;
//     return EXCEPTION_EXECUTE_HANDLER;
// }

/** Helper function to translate the virtual PC address @a addr into a
    logical address.  If the address translates to a known module (DLL
    or executable) memory mapping range, fills @a name (of maximum
    length @a length) with the name of the module, sets @a section to
    the index of the memory mapping section within the module, @a
    offset to a @a addr's relative offset within @a section, and
    returns @c true.  Otherwise returns @a false and @a name, @a
    section and @a offset will have undefined values.  Used to
    translate PC addresses to module addresses during the stack walk.  */
bool
GetLogicalAddress (PVOID addr, PTSTR name, DWORD length,
		   DWORD &section, DWORD &offset)
{
    MEMORY_BASIC_INFORMATION info;

    if (! VirtualQuery (addr, &info, sizeof (info)))
	return false;

    DWORD module = (DWORD) info.AllocationBase;
    if (! GetModuleFileName ((HMODULE) module, name, length))
	return false;

    PIMAGE_DOS_HEADER		dosheader = (PIMAGE_DOS_HEADER) module;
    PIMAGE_NT_HEADERS		ntheader
	= (PIMAGE_NT_HEADERS) (module + dosheader->e_lfanew);
    PIMAGE_SECTION_HEADER	sect = IMAGE_FIRST_SECTION (ntheader);
    DWORD			rva = (DWORD) addr - module;

    for (unsigned i = 0; i < ntheader->FileHeader.NumberOfSections; ++i,++sect)
    {
	DWORD sect_start = sect->VirtualAddress;
	DWORD sect_end = sect_start + std::max (sect->SizeOfRawData,
					        sect->Misc.VirtualSize);

	if ((rva >= sect_start) && (rva <= sect_end))
	{
	    section = i+1;
	    offset = rva - sect_start;
	    return true;
	}
    }

    ASSERT (false);
    return false;
}
#endif

#if __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) // FIXME: Check
extern "C" {
    typedef unsigned _Unwind_Ptr __attribute__((__mode__(__pointer__)));
    struct _Unwind_Context;
    typedef enum
    {
      _URC_NO_REASON = 0,
      _URC_FOREIGN_EXCEPTION_CAUGHT = 1,
      _URC_FATAL_PHASE2_ERROR = 2,
      _URC_FATAL_PHASE1_ERROR = 3,
      _URC_NORMAL_STOP = 4,
      _URC_END_OF_STACK = 5,
      _URC_HANDLER_FOUND = 6,
      _URC_INSTALL_CONTEXT = 7,
      _URC_CONTINUE_UNWIND = 8
    } _Unwind_Reason_Code;
    typedef _Unwind_Reason_Code (*_Unwind_Trace_Fn) (_Unwind_Context *, void *);
    extern _Unwind_Reason_Code _Unwind_Backtrace (_Unwind_Trace_Fn, void *);
    extern _Unwind_Ptr _Unwind_GetIP (_Unwind_Context *);
    extern _Unwind_Ptr _Unwind_GetRegionStart (_Unwind_Context *);
}

/** IA64-ABI specific stack walking routine.  This is mostly available
    on platforms with GCC 3.4+, but also with other compilers that have
    adpoted the same ABI standard.  This walker seems to be limited to
    walking only through frames with DWARF2 exception data, so it may
    not be able to go through all C libraries.  It does seem to be
    able to signal frames on a number of platforms however.  */
_Unwind_Reason_Code
unwindWalkStack (_Unwind_Context *ctx, void *data)
{
    IOFD		fd = *(IOFD *) data;
    iovec		bufs [5];
    int			nbufs = 0;
    char		addrbuf [5 + BitTraits<unsigned long>::HexDigits];
    char		diffbuf [10 + 2 * BitTraits<unsigned long>::HexDigits];
    static const char	trailer [] = "]\n";
    unsigned long	ip = _Unwind_GetIP (ctx);
    unsigned long	ir = _Unwind_GetRegionStart (ctx);
# if HAVE_DLADDR
    Dl_info		info;
    if (dladdr ((void *) ir, &info) && info.dli_fname && info.dli_fname[0])
    {
	const char	*libname = info.dli_fname;
	const char	*symname = (info.dli_sname && info.dli_sname[0]
				    ? info.dli_sname : "?");
	unsigned long	symaddr = (unsigned long) info.dli_saddr;
	bool		gte = (ip >= symaddr);
	unsigned long	diff = (gte ? ip - symaddr : symaddr - ip);

	bufs [nbufs].iov_base = addrbuf;
	bufs [nbufs].iov_len  = sprintf (addrbuf, " 0x%08lx ", ip);
	++nbufs;

	bufs [nbufs].iov_base = (char *) symname; // discard const
	bufs [nbufs].iov_len  = strlen (symname);
	++nbufs;

	bufs [nbufs].iov_base = diffbuf;
	bufs [nbufs].iov_len  = sprintf (diffbuf, " %s 0x%lx [",
					 gte ? "+" : "-", diff);
	++nbufs;

	bufs [nbufs].iov_base = (char *) libname; // discard const
	bufs [nbufs].iov_len  = strlen (libname);
	++nbufs;

	bufs [nbufs].iov_base = (char *) trailer; // discard const
	bufs [nbufs].iov_len  = 2;
	++nbufs;
    }
    else
# endif // HAVE_DLADDR
    {
	bufs [nbufs].iov_base = addrbuf;
	bufs [nbufs].iov_len  = sprintf (addrbuf, " 0x%08lx ", ip);
	++nbufs;

	bufs [nbufs].iov_base = diffbuf;
	bufs [nbufs].iov_len  = sprintf (diffbuf, " <?%08lx> + 0x%lx\n",
					 ir, ip - ir);
        ++nbufs;
    }

    writev (fd, bufs, nbufs);
    return _URC_NO_REASON;
}
#endif // GCC 3.4+

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>

#if HAVE_U_STACK_TRACE
// HP-UX stack walker (http://devresource.hp.com/STK/partner/unwind.pdf)
extern "C" void U_STACK_TRACE (void);
#endif

#if HAVE_XL_TRBK
// AIX stack walker (from xlf FORTRAN 90 runtime).
extern "C" void xl__trbk (void);
#endif

/** Fallback assertion validator for untyped pointer @a p.  @a p is
    the object, no object-specific validation is done.  This should
    not get called---someone forgot to define or declare the
    object-specific function.  */
void
AssertValid (const void *p)
{
    LOG (0, warning, LFassert,
	 "AssertValid(const void *p = " << p << ") called:\n"
	 " some class is missing `AssertValid' definition\n");
}

/** Validator for objects inheriting from #Debuggable.  @a p is the
    object to whose #Debuggable::validate() method will be called.  */
void
AssertValid (const Debuggable *p)
{ p->validate (); }

/** Debug dumper for objects inheriting from #Debuggable.  @a ref is
    the object whose #Debuggable::log() method will be called.  */
logstream &
operator<< (logstream &log, const Debuggable &ref)
{ ref.log (log); return log; }

//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** A place to set a break point for assertion failures or anything
    else that wants an explicit break point location (for example
    exception class constructors).  */
void
DebugAids::breakpoint (void)
{
    // FIXME: If the parent process is a debugger, (optionally?) drop
    // into it.  For example, if we have /proc/self/cmdline, look for
    // parent process' cmdline (/proc/{getppid()}/cmdline), and if it
    // looks like that of a debugger, execute break-point instruction
    // or some debugger trace message?
#ifdef _WIN32
    DebugBreak ();
#endif
}

/** Set the number of assertion failures to ignore before going into
    interactive mode; also returns the old setting.  If @a count is
    the default value -1, returns the current value without changing
    the setting.  Set to 0 to force a trap on every assertion
    failure. */
int
DebugAids::failIgnore (int count /* = -1 */)
{
    int old = s_assertIgnoreCount;
    if (count != -1)
	s_assertIgnoreCount = count;
    return old;
}

/** Set and return the file descriptor for assertion failure output.

    If @a fd is the default invalid descriptor value, returns the
    current value without changing the setting.  This value is only
    effective for #failStdxx() although you are welcome to use it for
    your own assertion hooks too. */
IOFD
DebugAids::failStdxxFd (IOFD fd /* = IOFD_INVALID */)
{
    if (s_assertOutputFd == IOFD_INVALID)
	s_assertOutputFd = STDERR_HANDLE;

    IOFD old = s_assertOutputFd;
    if (fd != IOFD_INVALID)
	s_assertOutputFd = fd;
    return old;
}

/** Set and return the assertion failure hook.

    If @a hook is the default null pointer, returns the current value
    without changing the setting.  Use this function to install an
    application-specific assertion failure handler, such as popping up
    a dialog.  The default hook is #failStdxx(); Microsoft Windows
    applications can also install #failMsgBox().

    Since #failStdxx() is by default interactive, an application
    wanting to run completely unattended should install an assertion
    handler that provides generated answers without user intervention
    (#failStdxx() has a limited such capability).  A good solution is
    a hook that answers the questions with the sequence `s', `c', `i'.
    
    If the application wants to run completely unattended, it should
    call this function to install an assertion hook that provides a
    answer without waiting for input (the default #failStdxx() is by
    default interactive but has a basic capability to provide quiet
    answers). */
DebugAids::AssertHook
DebugAids::failHook (AssertHook hook /* = 0 */)
{
    AssertHook old = s_assertHook;
    if (hook)
	s_assertHook = hook;
    return old;
}

/** An assertion failure handler that outputs to the standard error
    and reads input from standard input.

    This handler outputs @a message to the output file descriptor
    defined with #failStdxxFd(), by default the standard error.  It
    first prints out the failure information and then reads the action
    choice from the standard input.  However, if the @c $QUIET_ASSERT
    environment variable is set, answers will be read from it instead;
    once the characters in the variable are exhausted, an infinite
    sequence of @c i answers will be produced.  @c $QUIET_ASSERT's
    value should be sequence of characters acceptable to #AssertHook;
    in addition, a @c < causes the string to be re-interpreted from
    beginning: QUIET_ASSERT="si<" means an unending loop of
    alternating @c s and @c i.  */
char
DebugAids::failStdxx (const char *message)
{
    std::cout.flush ();
    STDC::fflush (stdout);

    std::cerr.flush ();
    STDC::fflush (stderr);

    if (s_assertOutputFd == IOFD_INVALID)
	s_assertOutputFd = STDERR_HANDLE;

    MYWRITE (s_assertOutputFd, message, STDC::strlen (message));

#if ! HAVE_STACK_BACKTRACE
    MYWRITELIT(s_assertOutputFd, ("(Attach debugger to this process"
				  " now to get a stack backtrace)\n"));
    MYWRITELIT(s_assertOutputFd, "[A]bort, [R]etry, [I]gnore, [C]ore? ");
#else
    MYWRITELIT(s_assertOutputFd, ("[A]bort, [R]etry, [I]gnore, [C]ore"
				  ", [S]tack trace? "));
#endif
    static const char	*quiet = getenv ("QUIET_ASSERT");
    static const char	*current = quiet;
    char		choice;

    if (current)
    {
	if (*current == '<')
	    current = quiet;

	choice = *current ? *current++ : 'i';
	MYWRITE (s_assertOutputFd, &choice, 1);
	MYWRITELIT (s_assertOutputFd, "\n");
    }
    else
	std::cin >> choice;

    return choice;
}

#ifdef _WIN32
/** An assertion failure handler that uses a Microsoft Windows
    system-modal dialog box.

    This handler outputs @a message in an abort-retry-ignore dialog
    box to the user.  The answer will be one of those three (that is,
    it is not possible to request a stack trace nor a core dump).  */
char
DebugAids::failMsgBox (const char *message)
{
    switch (::MessageBox (NULL, message, "Assertion Failed",
			  MB_SYSTEMMODAL | MB_ICONSTOP | MB_ABORTRETRYIGNORE))
    {
    case IDIGNORE:
	return 'i';

    case IDRETRY:
	return 'r';

    case IDABORT:
    default:
	return 'a';
    }
}
#endif // _WIN32

/** Implementation for the failure of #ASSERT.

    This function is the internal implementation for failed #ASSERT.
    @a file and @a line identify the source file and line of the
    failure expression.  @a function is the name of the function if
    the compiler can provide it, otherwise null.  @a expr is the
    stringified version of the assertion expression.

    This function formats the failure message, sends it off to the
    logging stream and then queries the current assertion hook (see
    #failHook() and #AssertHook) on what to do.  */
void
DebugAids::failed (const char *file, int line,
		   const char *function, const char *expr)
{
    if (s_assertIgnoreCount > 0) {
	--s_assertIgnoreCount;
	return;
    }

    std::string message ("assertion failed");
    if (expr && *expr)
    {
	message += ": `";
	message += expr;
	message += "'";
    }
    message += StringFormat (" (in %1:%2").arg (file).arg (line);
    if (function && *function)
    {
	message += ", ";
	message += function;
    }
    message += ")\n";

    LOG (0, error, LFassert, message);

    while (true)
    {
	char choice = s_assertHook ? (*s_assertHook) (message.c_str ()) : 'i';
	switch (tolower ((unsigned char) choice))
	{
	case 'a':
	    abort ();

	case 'r':
	    // break into the debugger (or Dr Watson log)
	    breakpoint ();
	    continue;

	case 'i':
	    return;

	case 'c':
	    // the signal raised will automatically kill the child
	    // since we are not inside a signal handler here.
	    coredump (SIGABRT);
	    continue;

	case 's':
	    stacktrace ();
	    continue;

	default:
	    continue;
	}
    }
}

/** Validation helper to verify strings.

    Checks that @a string points to valid memory for at least the
    distance @a length (if the default -1, until a null character).

    Returns @c true if the string is non-null and valid.  If the
    string is null or otherwise invalid memory, might return @c false
    or crash.  The latter will happen on systems where it is not
    possible to validate strings without reading them; at least you
    will get a direct indication where the invalid string has been
    used.

    The purpose of this function is to simplify assertions. */
bool
DebugAids::validate (const char *str, int length /*=-1*/)
{
    if (! str)
	return false;

#ifdef _WIN32
    return !IsBadStringPtr (str, length == -1 ? STDC::strlen (str) : length);
#else
    return validate (str, length == -1 ? STDC::strlen (str) : length, false);
#endif
}

/** Validation helper to verify unspecified memory blocks.

    Checks that @a data points to valid memory for at least the size
    of @a bytes.  If @a rw is set, checks that the memory can also be
    written to.

    Returns @c true if the memory is non-null and valid.  If @a data
    is null or otherwise does not fullfil the requirements, might
    return @c false or crash.  The latter will happen on systems where
    it is not possible to validate memory without accessing it; at
    least you will get a direct indication where the invalid data has
    been used.

    The purpose of this function is to simplify assertions.  */
bool
DebugAids::validate (const void *data, STDC::size_t bytes, bool rw)
{
    if (! data)
	return false;

#ifdef _WIN32
    return rw ? !IsBadWritePtr ((void *) data, bytes) : !IsBadReadPtr (data, bytes);
#else
    volatile unsigned char *rwdata = (volatile unsigned char *) data;
    for ( ; bytes; ++rwdata, --bytes)
    {
	unsigned char byte = *rwdata;

	if (rw)
	    *rwdata = byte;
    }

    // If we ever got this far...
    return true;
#endif
}

/** Set and return the file descriptor for stack trace output.

    If @a fd is the default invalid descriptor value, returns the
    current value without changing the setting.  This value is only
    effective for #stacktrace(), but can be overridden by the
    argument given to that function.  */
IOFD
DebugAids::stacktraceFd (IOFD fd /* = IOFD_INVALID */)
{
    if (s_stackTraceFd == IOFD_INVALID)
	s_stackTraceFd = STDERR_HANDLE;

    IOFD old = s_stackTraceFd;
    if (fd != IOFD_INVALID)
	s_stackTraceFd = fd;
    return old;
}

/** Produce a stack trace.

    Prints the current stack trace to file descriptor @a fd or if the
    default invalid descriptor, the currently registered stack trace
    descriptor as registered with #stacktraceFd().  Avoids unnecessary
    memory allocation so it should be safe to call this function even
    in dire situations.  On some systems the implementation always
    outputs to the standard error and has no means for redirection.
    On these systems an attempt is made to redirect standard error
    momentarily elsewhere and then redirect standard error to the
    desired file descriptor, invoke the output, and redirect standard
    error back to its original location.  If the redirection fails or
    the system has no stack tracing support, no stack trace is
    produced.  */
void
DebugAids::stacktrace (IOFD fd /* = IOFD_INVALID */)
{
    if (s_stackTraceFd == IOFD_INVALID)
	s_stackTraceFd = STDERR_HANDLE;

    if (fd == IOFD_INVALID)
	fd = s_stackTraceFd;

    std::cerr.flush ();
    fflush (stderr);

#ifdef _WIN32
    // FIXME: Autoload all these functions so users don't need to
    // link in imagehlp.dll.
    if (! SymInitialize (GetCurrentProcess (), NULL, TRUE))
    {
	MYWRITELIT (fd, ("failed to dump stack trace:"
			 " cannot get symbolic information\n"));
	return;
    }

    union SYMBUFFER {
	IMAGEHLP_SYMBOL	sym;
	BYTE		buffer [ sizeof (IMAGEHLP_SYMBOL) + 512 ];
    };

    unsigned		level = 0;
    CONTEXT		context;
    STACKFRAME		frame;
    SYMBUFFER		symbol;
    IMAGEHLP_MODULE	module;
    char		modulename [MAX_PATH];
    DWORD		section;
    DWORD		offset;
    char		buf [2*40+6];  // ample for two 128+ bit numbers
    // DWORD		exceptargs [] = { (DWORD) &context };

    // FIXME: XP 64-bit adds: RtlCaptureContext (&context);
    // This is documented to *not* work, but apparently it does.
    context.ContextFlags = CONTEXT_FULL;
    if (! GetThreadContext (GetCurrentThread (), &context))
	return;

    // LPTOP_LEVEL_EXCEPTION_FILTER oldseh
    //	= SetUnhandledExceptionFilter (&GrabExceptionContext);
    // RaiseException (0, 0, 1, exceptargs);
    // SetUnhandledExceptionFilter (oldseh);

    memset (&module, 0, sizeof (module));
    memset (&frame, 0, sizeof (frame));

    module.SizeOfStruct		= sizeof (module);

    frame.AddrPC.Offset		= context.Eip;
    frame.AddrPC.Mode		= AddrModeFlat;
    frame.AddrStack.Offset	= context.Esp;
    frame.AddrStack.Mode	= AddrModeFlat;
    frame.AddrFrame.Offset	= context.Ebp;
    frame.AddrFrame.Mode	= AddrModeFlat;

    while (true)
    {
	if (! StackWalk (IMAGE_FILE_MACHINE_I386,
			 GetCurrentProcess (),
			 GetCurrentThread (),
			 &frame,
			 &context,
			 NULL,
			 SymFunctionTableAccess,
			 SymGetModuleBase,
			 NULL)
	    || frame.AddrFrame.Offset == 0)
	    break;

	// FIXME: Throw away everything above stacktrace?  Keep looping
	// below until the name includes something we understand?

	// Print stack frame too?  If we know how many arguments there
	// are (from demangling function name -- see below, could count
	// commas), args are: *((ULONG *)frame.AddrFrame.Offset+2+ARG).
	MYWRITE (fd, buf, sprintf (buf, "(%2u) 0x%08lx 0x%08lx   ",
				   level, frame.AddrPC.Offset,
				   frame.AddrFrame.Offset));

	memset (&symbol, 0, sizeof (symbol));
	symbol.sym.SizeOfStruct = sizeof (symbol);
	symbol.sym.MaxNameLength = sizeof (symbol) - sizeof (symbol.sym);

	offset = 0;
	if (SymGetSymFromAddr (GetCurrentProcess (), frame.AddrPC.Offset,
			       &offset, &symbol.sym))
	{
	    // FIXME: Demangle name with:
	    //   UnDecorateSymbolName (name, undecname, sizeof (undecname),
	    //				UNDNAME_COMPLETE
	    //				| UNDNAME_NO_THISTYPE
	    // 				| UNDNAME_NO_SPECIAL_SYMS
	    // 				| UNDNAME_NO_MEMBER_TYPE
	    // 				| UNDNAME_NO_MS_KEYWORDS
	    // 				| UNDNAME_NO_ACCESS_SPECIFIERS);
	    MYWRITE (fd, symbol.sym.Name, STDC::strlen (symbol.sym.Name));
	    MYWRITE (fd, buf, sprintf (buf, " + %lx", offset));

	    if (SymGetModuleInfo (GetCurrentProcess(), frame.AddrPC.Offset,
				  &module))
	    {
		MYWRITELIT (fd, " [");
		MYWRITE (fd, module.ImageName,
			 STDC::strlen (module.ImageName));
		MYWRITELIT (fd, "]");
	    }
	}
	else
	{
	    GetLogicalAddress ((PVOID) frame.AddrPC.Offset,
			       modulename, sizeof (modulename),
			       section, offset);
	    MYWRITE (fd, buf, sprintf (buf, "%04lx:%08lx [", section, offset));
	    MYWRITE (fd, modulename, STDC::strlen (modulename));
	    MYWRITELIT (fd, "]");
	}
	MYWRITELIT (fd, "\n");
	++level;
    }
    SymCleanup (GetCurrentProcess ());

#elif (HAVE_U_STACK_TRACE || HAVE_XL_TRBK)		// hp-ux, aix
    // FIXME: deal with inability to duplicate the file handle
    int stderrfd = dup (STDERR_FILENO);
    if (stderrfd == -1)
	return;

    int newfd    = dup2 (fd, STDERR_FILENO);
    if (newfd == -1)
    {
	close (stderrfd);
	return;
    }

# if HAVE_U_STACK_TRACE		// hp-ux
    U_STACK_TRACE ();
# elif HAVE_XL_TRBK		// aix
    xl__trbk ();
# else
#  error "oops, you shouldn't have gotten here!"
# endif

    fflush (stderr);
    dup2 (stderrfd, STDERR_FILENO);
    close (newfd);
#elif HAVE_BACKTRACE_SYMBOLS_FD && HAVE_DLADDR		// linux
    // we could have used backtrace_symbols_fd, except its output
    // format is pretty bad, so recode that here :-(
    void		*trace [MAX_BACKTRACE_DEPTH];
    int			depth = backtrace (trace, MAX_BACKTRACE_DEPTH);

    iovec		bufs [5];
    int			nbufs = 0;
    char		addrbuf [5 + BitTraits<unsigned long>::HexDigits];
    char		diffbuf [7 + BitTraits<unsigned long>::HexDigits];
    static const char	trailer [] = "]\n";

    for (int n = 0; n < depth; ++n, nbufs = 0)
    {
	unsigned long	addr = (unsigned long) trace [n];
	Dl_info		info;

	if (dladdr (trace[n], &info) && info.dli_fname && info.dli_fname[0])
	{
	    const char		*libname = info.dli_fname;
	    const char		*symname = (info.dli_sname && info.dli_sname[0]
					    ? info.dli_sname : "?");
	    unsigned long	symaddr = (unsigned long) info.dli_saddr;
	    bool		gte = (addr >= symaddr);
	    unsigned long	diff = (gte ? addr - symaddr : symaddr - addr);

	    bufs [nbufs].iov_base = addrbuf;
	    bufs [nbufs].iov_len  = sprintf (addrbuf, " 0x%08lx ", addr);
	    ++nbufs;

	    bufs [nbufs].iov_base = (void *) symname; // discard const
	    bufs [nbufs].iov_len  = strlen (symname);
	    ++nbufs;

	    bufs [nbufs].iov_base = diffbuf;
	    bufs [nbufs].iov_len  = sprintf (diffbuf, " %s 0x%lx [",
					     gte ? "+" : "-", diff);
	    ++nbufs;

	    bufs [nbufs].iov_base = (void *) libname; // discard const
	    bufs [nbufs].iov_len  = strlen (libname);
	    ++nbufs;

	    bufs [nbufs].iov_base = (void *) trailer; // discard const
	    bufs [nbufs].iov_len  = 2;
	    ++nbufs;

	}
	else
	{
	    bufs [nbufs].iov_base = addrbuf;
	    bufs [nbufs].iov_len  = sprintf (addrbuf, " 0x%08lx ", addr);
	    ++nbufs;

	    bufs [nbufs].iov_base = (void *) "<unknown function>\n"; //no const
	    bufs [nbufs].iov_len  = 19;
	    ++nbufs;
	}

	writev (fd, bufs, nbufs);
    }

#elif HAVE_EXCPT_H && HAVE_PDSC_H && HAVE_RLD_INTERFACE_H // tru64
    // Tru64 stack walk.  Uses the exception handling library and the
    // run-time linker's core functions (loader(5)).  FIXME: Tru64
    // should have _RLD_DLADDR like IRIX below.  Verify and update.

    char	buffer [100 + BitTraits<unsigned long>::HexDigits * 2 + 11];
    sigcontext	context;
    int		rc = 0;

    exc_capture_context (&context);
    while (!rc && context.sc_pc)
    {
	// FIXME: Elf32?
	pdsc_crd *func, *base, *crd
	  = exc_remote_lookup_function_entry(0, 0, context.sc_pc, 0, &func, &base);
	Elf32_Addr addr = PDSC_CRD_BEGIN_ADDRESS(base, func);
	// const char *name = _rld_address_to_name(addr);
	const char *name = "<unknown function>";
	sprintf (buffer, " 0x%012lx %.100s + 0x%lx\n",
		 context.sc_pc, name, context.sc_pc - addr);
	write (fd, buffer, STDC::strlen(buffer));
	rc = exc_virtual_unwind(0, &context);
    }

#elif HAVE_EXCEPTION_H && defined __sgi			// irix
    // IRIX stack walk -- like Tru64 but with a little different names.
    // NB: The guard above is to protect against unrelated <exception.h>
    //   provided by some compilers (e.g. KCC 4.0f).
    // NB: libexc.h has trace_back_stack and trace_back_stack_and_print
    //   but their output isn't pretty and nowhere as complete as ours.
    char       buffer [340];
    sigcontext context;

    exc_setjmp (&context);
    while (context.sc_pc >= 4)
    {
	// Do two lookups, one using exception handling tables and
	// another using _RLD_DLADDR, and use the one with a smaller
	// offset.  For signal handlers we seem to get things wrong:
	// _sigtramp's exception range is huge while based on Dl_info
	// the offset is small -- but both supposedly describe the
	// same thing.  Go figure.
	char            *name = 0;
	const char      *libname = 0;
	const char      *symname = 0;
	Elf32_Addr      offset = ~0L;

	// Do the exception/dwarf lookup
	Elf32_Addr      pc = context.sc_pc;
	Dwarf_Fde       fde = find_fde_name (&pc, &name);
	Dwarf_Addr      low_pc = context.sc_pc;
	Dwarf_Unsigned  udummy;
	Dwarf_Signed    sdummy;
	Dwarf_Ptr       pdummy;
	Dwarf_Off       odummy;
	Dwarf_Error     err;

	symname = name;

	// Determine offset using exception descriptor range information.
	if (dwarf_get_fde_range (fde, &low_pc, &udummy, &pdummy, &udummy,
				 &odummy, &sdummy, &odummy, &err) == DW_DLV_OK)
	    offset = context.sc_pc - low_pc;

       // Now do a dladdr() lookup.  If the found symbol has the same
       // address, trust the more accurate offset from dladdr();
       // ignore the looked up mangled symbol name and prefer the
       // demangled name produced by find_fde_name().  If we find a
       // smaller offset, trust the dynamic symbol as well.  Always
       // trust the library name even if we can't match it with an
       // exact symbol.
	Elf32_Addr      addr = context.sc_pc;
	Dl_info         info;

	if (_rld_new_interface (_RLD_DLADDR, addr, &info))
	{
	    if (info.dli_fname && info.dli_fname [0])
		libname = info.dli_fname;

	    Elf32_Addr symaddr = (Elf32_Addr) info.dli_saddr;
	    if (symaddr == low_pc)
		offset = addr - symaddr;
	    else if (info.dli_sname
		     && info.dli_sname [0]
		     && addr - symaddr < offset)
	    {
		offset = addr - symaddr;
		symname = info.dli_sname;
	    }
	}

	// Print out the result
	if (libname && symname)
            write (fd, buffer, sprintf
		   (buffer, " 0x%012lx %.100s + 0x%lx [%.200s]\n",
		    addr, symname, offset, libname));
	else if (symname)
	    write (fd, buffer, sprintf
		   (buffer, " 0x%012lx %.100s + 0x%lx\n",
		    addr, symname, offset));
	else
	    write (fd, buffer, sprintf
		   (buffer, " 0x%012lx <unknown function>\n", addr));

	// Free name from find_fde_name().
	free (name);

	// Check for termination.  exc_unwind() sets context.sc_pc to
	// 0 or an error (< 4).  However it seems we can't unwind
	// through signal stack frames though this is not mentioned in
	// the docs; it seems that for those we need to check for
	// changed pc after find_fde_name().  That seems to indicate
	// end of the post-signal stack frame.  (FIXME: Figure out how
	// to unwind through signal stack frame, e.g. perhaps using
	// sigcontext_t's old pc?  Or perhaps we can keep on going
	// down without doing the symbol lookup?)
	if (pc != context.sc_pc)
	    break;

	exc_unwind (&context, fde);
    }

#elif defined PROG_PSTACK				// solaris
# ifdef PROG_CXXFILT
#  define CXXFILTER " | " PROG_CXXFILT
# else
#  define CXXFILTER
# endif
    // 64 should more than plenty for a space and a pid.
    char buffer [sizeof(PROG_PSTACK) + 1 + BitTraits<unsigned long>::Digits
		 + 3 + sizeof(PROG_CXXFILT) + BitTraits<int>::Digits + 1];
    sprintf (buffer, "%s %lu%s 1>&%d", PROG_PSTACK, (unsigned long) getpid (),
	     "" CXXFILTER, fd);
    system (buffer);
# undef CXXFILTER

#elif __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4)
    // FIXME: Check for _Unwind*, compilers other than GCC support this API
    _Unwind_Backtrace (unwindWalkStack, &fd);
#endif

    // FIXME: mpatrol has some generic unix unwind code.
    // FIXME: from unix faq: ask debugger to dump stack trace
    // with something like:
    //   - gdb: echo "thread apply all where\nwhere\ndetach" | gdb $prog $pid
    //   - dbx: echo "where\ndetach" | dbx -a $program_path $pid
    //   - dbx (aix): echo "where\ndetach" | dbx -p $program_path $pid
}

/** Drop a core dump and continue.

    Creates a core file for the current program state and continues
    execution.  @a sig should be the number of the signal from which
    the program should appear to have died; this should a fatal signal
    that does cause a core file to be created (or @c SIGUSR1).

    This works by forking the process and then killing the child with
    the given signal; the signal is automatically unblocked in the
    child to make sure the sure the signal is delivered.  Thus the
    function returns only once, in the parent process.

    This function can be safely installed directly as a signal
    handler.  #Signal::handleFatal() will do so for @c SIGUSR1 with
    suitable options.

    Note that this function does not change core dump resource limits,
    not even for the forked child process.  If core files are disabled
    through resource limits, no core file will be created despite your
    explicit request to create one.

    This concept was taken from DDD, the Data Display Debugger. */
void
DebugAids::coredump (int sig, ...)
{
#ifndef _WIN32
    // FIXME: Forking vs. threads -- need to sort out what is safe.
    // FIXME: Provide a resource limits interface so that core
    // resource limits can be raised?

    pid_t	corepid;
    int		status;

    ::unlink ("core");
    if ((corepid = ::fork ()) == 0)
    {
	// In child: re-raise the signal, thus killing the process and
	// producing a core dump.  Make sure 1) the signal is not
	// blocked so that we won't return to the caller, 2) we have a
	// signal that is fatal, 3) the signal falls to its default
	// handler to produce the dump.

#ifdef SIGUSR1
	// SIGUSR1 does not cause a core dump; use abort() instead
	if (sig == SIGUSR1)
	    sig = SIGABRT;	// Could be SIGIOT if SIGABRT is not defined
#endif
	Signal::handle (sig, (Signal::HandlerType) SIG_DFL);
	Signal::block (sig, false);
	Signal::raise (sig);

	// Yikes, this shouldn't happen.  ASSERT isn't right here.  If
	// raise() failed to deliver the signal, abort() is unlikely
	// to work any better, but try it anyway.  Then make sure we
	// die so that we won't return to the caller from the child.
	abort ();
	_exit (255);
    }
    else if (corepid > 0)
	::waitpid (corepid, &status, 0);
#endif // !_WIN32
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// Dummies for debuggable base classes
Debuggable::~Debuggable (void) { }
void Debuggable::validate (void) const { }
void Debuggable::log (logstream & /*to*/) const { }

} // namespace lat
