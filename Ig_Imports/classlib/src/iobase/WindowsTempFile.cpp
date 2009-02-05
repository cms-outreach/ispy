//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/TempFile.h"
#include "classlib/utils/ProcessInfo.h"
#include "classlib/utils/Time.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/sysapi/TempFile.h"
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

Filename
TempFile::tempdir (void)
{
    // FIXME: Try some more modern directories?  windows dir?
    // FIXME: Should we try user's tmp directory first?

    Filename name;

    if (tryTempDir (name, getenv ("TEMP")))
	return name;
    
    if (tryTempDir (name, getenv ("TMPDIR")))
	return name;
    
    if (tryTempDir (name, getenv ("TMP")))
	return name;

    if (tryTempDir (name, "c:/temp"))
	return name;

    if (tryTempDir (name, "c:/tmp"))
	return name;

    // If all else fails, use the current directory.
    return Filename::thisdir ();
}

// Replacement functions for platforms that don't have one of the
// mkstemp or mkdtemp.  This was inspired by the implementations in
// INN (by Russ Alberry, in public domain), OpenBSD (BSD license) and
// liberty (GNU LGPL), trying to take the best of all three.  (The
// implementation at mktemp.org looks like a slightly modified version
// of BSD code.)
//
// NOTE: IF YOU CHANGE THIS, CHANGE THE ONE IN FilenameWindows.cxx TOO!

bool
TempFile::domktemp (char *path, IOFD *fd, bool dodir, int suffixlen)
{
    ASSERT (path);
    ASSERT (! (fd && dodir));

    static const char letters[] =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

    int		length;
    char	*replace;
    char	*suffix;
    char	*start;

    // Find the bounds of the template and make sure it is valid.
    for (replace = path; *replace; ++replace)
	;

    length = replace - path;
    if (length < suffixlen || length < 6)
    {
	errno = ERROR_INVALID_PARAMETER;
	return false;
    }

    replace -= suffixlen;
    suffix = replace;
    --replace;

    for (start = replace; start > path && start[-1] == 'X'; --start)
	;

    if (suffix - start < 6)
    {
	errno = ERROR_INVALID_PARAMETER;
	return false;
    }

    ASSERT (start >= path);
    ASSERT (*start == 'X');
    ASSERT (suffix > start);
    ASSERT (suffix [-1] == 'X');

    // Get some random data (FIXME: Use an interface if we have one?).
    // The nanosecond portion of current time is unlikely to be useful
    // (i.e. zeroes), so get it off and use the microseconds.
    IntBits<64>::SLeast	time = Time::current ().ns () / 1000;
    IntBits<64>::SLeast	usecs = time % 1000000;
    IntBits<64>::SLeast	secs = time / 1000000;
    IntBits<64>::SLeast	randnum = (usecs << 16) ^ secs ^ ProcessInfo::pid ();
    IntBits<64>::SLeast	working;

    // With six X's this will run for a long time if the target
    // directory doesn't exist, so make sure it does.
    if (fd || dodir)
	for (replace = start; replace > path; --replace)
	    // FIXME: WIN32: Drive and share handing?
	    if (*replace == '/' || *replace == '\\')
	    {
		char c = *replace;
		*replace = '\0';
		DWORD x = GetFileAttributes (path);
		*replace = c;

		if (x == INVALID_FILE_ATTRIBUTES)
		    return false;

		if (! (x & FILE_ATTRIBUTE_DIRECTORY))
		{
		    errno = ERROR_DIRECTORY;
		    return false;
		}

		break;
	    }

    // Try to find a working file name, but no more than TMP_MAX attempts.
    for (int tries = 0; tries < TMP_MAX; ++tries)
    {
	for (replace = start, working = randnum; replace < suffix; ++replace) 
	{
	    *replace = letters [working % 62];
	    working /= 62;
	}

	if (fd)
	{
	    // FIXME + FILE_ATTRIBUTE_HIDDEN, _TEMPORARY?
	    // FIXME security attributes to prevent others to read?
	    if ((*fd = CreateFile (path, GENERIC_READ | GENERIC_WRITE,
				   0 /* sharing */, 0 /* security attr */,
				   CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0))
		!= IOFD_INVALID)
		return true;

	    if (GetLastError () != ERROR_FILE_EXISTS
		&& GetLastError () != ERROR_NOACCESS
		&& GetLastError () != ERROR_SHARING_VIOLATION
		&& GetLastError () != ERROR_CANNOT_MAKE)
	    {
		errno = GetLastError ();
		return false;
	    }
	}
	else if (dodir)
	{
	    // FIXME WIN32 security attributes with access only to self
	    if (CreateDirectory (path, 0 /* security attr */))
		return true;

	    if (GetLastError () != ERROR_NOACCESS
		&& GetLastError () != ERROR_ALREADY_EXISTS)
	    {
		errno = GetLastError ();
		return false;
	    }
	}
	else if (GetFileAttributes (path) == INVALID_FILE_ATTRIBUTES)
	{
	    // FIXME: FILE_NOT_FOUND or PATH_NOT_FOUND or BAD_PATHNAME?
	    errno = GetLastError ();
	    return (errno == ERROR_FILE_NOT_FOUND
		    || errno == ERROR_PATH_NOT_FOUND);
	}

	// Relatively random increment.  Cut off the low portion of usecs
	// since it's often predictable.
	randnum += (usecs >> 10) & 0xfff;
    }

    errno = ERROR_FILE_EXISTS;
    return false;
}

} // namespace lat
#endif // _WIN32
