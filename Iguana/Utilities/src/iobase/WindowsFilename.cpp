//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/Filename.h"
#include "classlib/iobase/FileAcl.h"
#include "classlib/iobase/IOStatus.h"
#include "classlib/iobase/FileSystemError.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/utils/Time.h"
#include "classlib/sysapi/Filename.h"
#ifdef _WIN32

// FIXME: GetLogicalDrives?
// FIXME: GetTempFileName
// FIXME: CreateHardLink

// FIXME: GetCurrentDir
// FIXME: QueryDosDevice?

// FIXME: GetDiskFreeSpaceEx
//   -- GetVolumePathName (to set root dir of path)

// FIXME: volume mounts / reparse points?

// FIXME: msvc++ rtl
//  utime, futime
//  remove vs unlink (remove: can't be open)

// FIXME: msvc++ rtl
//  fullpath, makepath, splitpath
//  getdrive
//  getdcwd -> no slash if not root, 0 = default drive, no path -> mallocs
//  getcwd -> no path -> mallocs

// FIXME: msvc++ rtl
//  {open,get}osfhandle: int vs HANDLE
//  getdrives
//  chmod in io.h
//  chdir (note: cur drive, drive cwd can be changed)

// FIXME: `file' support? / GetBinaryType?

// FIXME: chdrive
// FIXME: getdrive
// FIXME: getdrives -> long (what on earth is this?)

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
    return ishome || ISSHARE (p) || (ISDRIVE (p) && ISPATHSEP (p+2));
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
bool
Filename::status (IOStatus &s, bool traverseLinks /* = true */) const
{ return s.fill (*this, traverseLinks); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
bool
Filename::exists (void) const
{ return GetFileAttributes (*this) != INVALID_FILE_ATTRIBUTES; }

bool
Filename::isDirectory (void) const
{
    DWORD attrs = GetFileAttributes (*this);
    return (attrs != INVALID_FILE_ATTRIBUTES
	    && (attrs & FILE_ATTRIBUTE_DIRECTORY));
}

bool
Filename::isRegular (void) const
{
    // Cut out directories.
    DWORD attrs = GetFileAttributes (*this);
    if (attrs == INVALID_FILE_ATTRIBUTES || attrs & FILE_ATTRIBUTE_DIRECTORY)
	return false;

    // See if it is a normal file.  To find out we must need a handle.
    HANDLE h = CreateFile (*this, 0 /* access */, 0 /* sharing */,
			   0 /* security */, OPEN_EXISTING,
			   FILE_ATTRIBUTE_NORMAL, 0 /* template */);
    if (h == INVALID_HANDLE_VALUE)
	return false;

    // FIXME: Check for symlink
    bool regular = GetFileType (h) == FILE_TYPE_DISK;
    CloseHandle (h);
    return regular;
}

bool
Filename::isSymlink (Filename *target /* = 0 */) const
{
    // FIXME: WIN32: recognise .lnk files (shortcuts) as symlinks.
    // FIXME: WIN32: hide or keep the .lnk extension visible?

    return false;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
Filename::normalise (void)
{
    // Translate possibly 8.3 to canonical long name.
    std::string longname (GetLongPathName (*this, 0, 0), '\0');
    if (GetLongPathName (*this, &longname[0], longname.size ()))
	m_name = longname;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
Filename
Filename::cwd (char drive)
{
    // getdcwd allocates the buffer automatically.
    char *dir = _getdcwd (tolower (drive) - 'a' + 1, 0, 128);

    if (! dir)
	throw FileSystemError ("getdcwd()", errno);

    // FIXME: does this need a leading slash?
    Filename result (dir);
    ::free (dir);
    return result;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
Filename
Filename::null (void)
{ return Filename ("nul"); }

Filename
Filename::root (void)
{ return Filename (""); } // There is no unique file system root on Windows

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
Filename::symlink (Filename target, Filename link)
{
    ASSERT (false); // FIXME: Simulate creating .lnk file

    // FIXME: This whole link business is somewhat strange.  We could
    // simluate creating .lnk files transparently (btw, they need to
    // contain absolute file name!), but then we should really switch
    // all uses of filenames within SealBase to FilenameConversion or
    // something to check links at all levels.  At least truename()
    // and bare bones isSymlink() and symlink() would work without,
    // but just about nothing else would, e.g. all calls to access()
    // and status() etc.
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
bool
Filename::domakedir (Filename name, FileAcl /* mode */, bool allowExisting)
{
    // Try to create the directory
    if (CreateDirectory (name, 0 /* security attr -- FIXME */))
	return true;

    DWORD attrs;
    int olderror = GetLastError ();
    if (allowExisting
	&& olderror == ERROR_ALREADY_EXISTS
	&& (attrs = GetFileAttributes (name)) != INVALID_FILE_ATTRIBUTES
	&& (attrs & FILE_ATTRIBUTE_DIRECTORY))
    {
	SetLastError (ERROR_SUCCESS);
	return true;
    }

    // Nope, restore error and return failure.
    SetLastError (olderror);
    return false;
}

int
Filename::doremove (const char *path)
{
    DWORD attrs = GetFileAttributes (path);
    if (attrs != INVALID_FILE_ATTRIBUTES)
	if (attrs & FILE_ATTRIBUTE_DIRECTORY)
	{
	    // Note: can't remove current working directory at all.
	    if (attrs & FILE_ATTRIBUTE_READONLY)
		// Can't remove own directories if it is read-only.
		SetFileAttributes (path, attrs & ~FILE_ATTRIBUTE_READONLY);

	    if (RemoveDirectory (path))
		return 0;

	    // If directory still exists, restore read-only attr.
	    if (attrs & FILE_ATTRIBUTE_READONLY)
		SetFileAttributes (path, attrs);
	}
	else
	{
	    // FIXME: If not symlink and ERROR_ACCESS_DENIED, try to
	    // make it writable and then try to remove it again.

	    // FIXME: Check for shortcuts (.lnk) and if so, take off
	    // FILE_ATTRIBUTE_READONLY.

	    if (DeleteFile (path))
		return 0;

	    // FIXME: If that failed, try CreateFile(FILE_FLAG_DELETE_ON_CLOSE)
	    // FIXME: Then rename to some other name, e.g. at root of volume?
	}

    errno = GetLastError ();
    return -1;
}

} // namespace lat
#endif // _WIN32
