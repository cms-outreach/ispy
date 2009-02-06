//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/utils/ProcessInfo.h"
#include "classlib/iobase/Filename.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/sysapi/ProcessInfo.h"
#include "classlib/sysapi/Windows.h"
#include <cstring>

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Get program's @c argv vector if available.  */
char **
ProcessInfo::argv (void)
{
#if HAVE_SYM_ARGV // WIN32
    return __argv;
#elif HAVE_SYM_DL_ARGV // Linux
    return _dl_argv;
#elif HAVE_CRT_EXTERNS_H // Mac OS X
    return *_NSGetArgv ();
#else
    static char *s_fakeargv [1] = { 0 };
    return s_fakeargv;
#endif
}

/** Get program's @c argc if available. */
int
ProcessInfo::argc (void)
{
#if HAVE_SYM_ARGC // WIN32
    return __argc;
#elif HAVE_CRT_EXTERNS_H // Mac OS X
    return *_NSGetArgc ();
#else
    int nargs = 0;
    for (char **args = argv (); args && *args; ++args)
	++nargs;

    return nargs;
#endif
}

/** Get program's full name if possible, otherwise best available name. */
std::string
ProcessInfo::fullProgramName (void)
{
#ifdef _WIN32
    std::string	result;
    size_t	size = 256;
    char	*buffer = 0;
    DWORD	len;

    do
    {
	delete [] buffer;
	buffer = new char [size *= 2];
	memset (buffer, 0, size);

	len = GetModuleFileName (0, buffer, size);
    } while (len >= size);

    if (len > 0)
	result = buffer;

    delete [] buffer;
    return result;
#elif __linux
    std::string	result;
    int		size = 256;
    char	*buffer = 0;
    int		len;

    do
    {
	delete [] buffer;
	buffer = new char [size *= 2];
	memset (buffer, 0, size);
	len = readlink ("/proc/self/exe", buffer, size);
    } while (len >= size);

    if (len > 0)
	result = buffer;

    delete [] buffer;
    return result;
#elif HAVE_MACH_O_DYLD_H
    std::string result;
    unsigned int size = 0;
    _NSGetExecutablePath (0, &size);
    result.resize (size);
    if (_NSGetExecutablePath (&result [0], &size) == 0)
    {
	result.resize (size-1);
	return result;
    }

    char **args = argv ();
    return args && *args ? *args : "";
#else
    char **args = argv ();
    return args && *args ? *args : "";
#endif
}

/** Get program's bare name if possible. */
std::string
ProcessInfo::shortProgramName (void)
{ return Filename (fullProgramName ()).nondirectory ().name (); }

/** Get the process id.  */
pid_t
ProcessInfo::pid (void)
{
#ifdef _WIN32
    return GetCurrentProcessId ();
#else
    return ::getpid ();
#endif
}

/** Get the parent process id. */
pid_t
ProcessInfo::ppid (void)
{
#ifdef _WIN32
    PROCESS_BASIC_INFORMATION pbi;
    if (NtQueryInformationProcess (GetCurrentProcess(),ProcessBasicInformation,
				   &pbi, sizeof (pbi), 0) == STATUS_SUCCESS)
	return pbi.InheritedFromUniqueProcessId;

    // FIXME: throw systemerror!
    ASSERT (false);
    return -1;
#else
    return ::getppid ();
#endif
}

} // namespace lat
