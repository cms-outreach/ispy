//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/TempFile.h"
#include "classlib/utils/ProcessInfo.h"
#include "classlib/utils/Time.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/sysapi/TempFile.h"
#include <cstdlib>
#include <cstdio>
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

Filename
TempFile::tempdir (void)
{
    Filename name;

    if (tryTempDir (name, getenv ("TMPDIR")))
	return name;
    
    if (tryTempDir (name, getenv ("TMP")))
	return name;

    if (tryTempDir (name, getenv ("TEMP")))
	return name;
    
# ifdef P_tmpdir
    if (tryTempDir (name, P_tmpdir))
	return name;
# endif

    if (tryTempDir (name, "/tmp"))
	return name;

    if (tryTempDir (name, "/usr/tmp"))
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
    struct stat	st;

    // Find the bounds of the template and make sure it is valid.
    for (replace = path; *replace; ++replace)
	;

    length = replace - path;
    if (length < suffixlen || length < 6)
    {
	errno = EINVAL;
	return false;
    }

    replace -= suffixlen;
    suffix = replace;
    --replace;

    for (start = replace; start > path && start[-1] == 'X'; --start)
	;

    if (suffix - start < 6)
    {
	errno = EINVAL;
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
	    if (*replace == '/')
	    {
		int		x;
		char		c = *replace;

		*replace = '\0';
		x = stat (path, &st);
		*replace = c;

		if (x < 0)
		    return false;

		if (! S_ISDIR (st.st_mode))
		{
		    errno = ENOTDIR;
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
	    if ((*fd = open (path, O_RDWR | O_CREAT | O_EXCL, 0600)) >= 0)
		return true;

	    if (errno != EEXIST)
		return false;
	}
	else if (dodir)
	{
	    if (mkdir (path, 0700) >= 0)
		return true;

	    if (errno != EEXIST)
		return false;
	}
	else if (lstat (path, &st) < 0)
	    return errno == ENOENT;

	// Relatively random increment.  Cut off the low portion of usecs
	// since it's often predictable.
	randnum += (usecs >> 10) & 0xfff;
    }

    errno = EEXIST;
    return false;
}

} // namespace lat
#endif // !_WIN32
