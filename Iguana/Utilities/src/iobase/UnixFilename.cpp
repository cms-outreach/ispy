//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/Filename.h"
#include "classlib/iobase/FileAcl.h"
#include "classlib/iobase/FileSystemError.h"
#include "classlib/iobase/IOStatus.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/utils/Time.h"
#include "classlib/sysapi/Filename.h"
#include <cstdlib>
#include <cstring>
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

bool
Filename::isAbsolute (bool considerExpansions /* = false */) const
{
    const char	*p = m_name.c_str ();
    bool	ishome = considerExpansions && *p == '~';
    return ishome || ISPATHSEP (p);
}

bool
Filename::status (IOStatus &s, bool traverseLinks /* = true */) const
{ return s.fill (*this, traverseLinks); }

bool
Filename::exists (void) const
{ struct stat st; return ::stat (*this, &st) == 0; }

bool
Filename::isDirectory (void) const
{ struct stat st; return ::stat (*this, &st) == 0 && S_ISDIR (st.st_mode); }

bool
Filename::isRegular (void) const
{ struct stat st; return ::stat (*this, &st) == 0 && S_ISREG (st.st_mode); }

bool
Filename::isSymlink (Filename *target /* = 0 */) const
{
#ifdef S_IFLNK
    // We don't even try lstat().  Some systems have magic files that
    // are symlinks and for which readlink() works, but do not appear
    // to be so from lstat() information (e.g. under /proc).  So we
    // just readlink() it, and if that works, well, it's a link.
    //
    // There is no direct way to find out how much buffer space to
    // allocate.  We just need to keep trying until our buffer is
    // large enough.  Note that readlink() doesn't null-terminate the
    // string -- we clear the buffer before reading to make sure it is
    // properly terminated.  This also means that if n == size, we
    // still didn't have a buffer large enough (it could also mean
    // that readlink copied just that many bytes and there was more to
    // come).
    int		size = 50;
    char	*buffer = 0;
    int		n;
	    
    do
    {
	delete [] buffer;
	buffer = new char [size *= 2];
	memset (buffer, 0, size);

	errno = 0;
	n = readlink (*this, buffer, size);
#ifdef ERANGE
	if (n == -1 && errno == ERANGE)
	    n = size;
#endif
    } while (n >= size);

    // Check for failure: not a symlink
    if (n == -1)
    {
	delete [] buffer;
	if (target)
	    *target = "";
	return false;
    }

    // Symlink was detected and target successfully read.
    ASSERT (n < size);
    ASSERT (! buffer [n]);
    if (target)
	*target = buffer;

    delete [] buffer;
    return true;
#else // ! S_IFLNK
    return false;
#endif // S_IFLNK
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
Filename::normalise (void)
{}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
Filename
Filename::cwd (char /* drive */)
{ abort (); return Filename (); } 

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
Filename
Filename::null (void)
{ return Filename ("/dev/null"); }

Filename
Filename::root (void)
{ return Filename ("/"); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
Filename::symlink (Filename target, Filename link)
{
#ifdef S_IFLNK
    if (::symlink (target, link) < 0)
	throw FileSystemError ("symlink()", errno);
#else
    ASSERT (false); // FIXME: throw?
#endif
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
bool
Filename::domakedir (Filename name, FileAcl mode, bool allowExisting)
{
    // Try to create the directory
    if (mkdir (name, mode.native ()) >= 0)
	return true;

    // See if it failed because the directory already existed.
    int olderrno = errno;
    struct stat st;
    if (allowExisting && stat (name, &st) >= 0 && S_ISDIR (st.st_mode))
    {
	errno = 0;
	return true;
    }

    // Nope, restore errno and return failure.
    errno = olderrno;
    return false;
}

int
Filename::doremove (const char *path)
{ return ::remove (path); }

} // namespace lat
#endif // !_WIN32
