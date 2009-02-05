//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/IOStatus.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/sysapi/IOStatus.h"
#include "classlib/sysapi/Filename.h"
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

static void
aclFromAttrs (FileAcl &acl, DWORD attrs)
{
    acl.addPortable (FileAcl::ReadBits);
    if (! (attrs & FILE_ATTRIBUTE_READONLY))
	acl.addPortable (FileAcl::WriteBits);
    if (attrs & FILE_ATTRIBUTE_ARCHIVE)
	acl.addPortable (FileAcl::Archive);
    if (attrs & FILE_ATTRIBUTE_ENCRYPTED)
	acl.addPortable (FileAcl::Encrypted);
    if (attrs & FILE_ATTRIBUTE_HIDDEN)
	acl.addPortable (FileAcl::Hidden);
    if (attrs & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)
	acl.addPortable (FileAcl::NotIndexed);
    if (attrs & FILE_ATTRIBUTE_OFFLINE)
	acl.addPortable (FileAcl::Offline);
    if (attrs & FILE_ATTRIBUTE_SYSTEM)
	acl.addPortable (FileAcl::System);
    if (attrs & FILE_ATTRIBUTE_TEMPORARY)
	acl.addPortable (FileAcl::Temporary);
}

bool
IOStatus::fill (IOFD fd)
{
    if (fd == IOFD_INVALID)
    {
	clear ();
	return false;
    }

    clear ();

    // Try getting handle information.
    BY_HANDLE_FILE_INFORMATION info;
    if (! GetFileInformationByHandle (fd, &info))
    {
	// If unknown, char, pipe or remote: char device
	if (GetFileType (fd) != FILE_TYPE_DISK)
	    m_type = TypeCharDev;

	// If we can't get the size: char device; otherwise pick up size
	DWORD usize, lsize;
	lsize = GetFileSize (fd, &usize);
	if (lsize == 0xffffffff && GetLastError () != NO_ERROR)
	    m_type = TypeCharDev;
	else
	    m_size = ((IOOffset) usize << 32) + lsize;

	// Pretend its writable, it probably isn't (FIXME)
	m_acl.addPortable (FileAcl::ReadBits | FileAcl::WriteBits);
	return true;
    }

    // Record file times.  FAT doesn't have atime/ctime: copy mtime.
    m_ctime = Time::from (&info.ftCreationTime);
    m_mtime = Time::from (&info.ftLastWriteTime);
    m_atime = Time::from (&info.ftLastAccessTime);
    if (! info.ftLastAccessTime.dwLowDateTime
	&& ! info.ftLastAccessTime.dwHighDateTime)
	m_atime = m_mtime;
    if (! info.ftCreationTime.dwLowDateTime
	&& ! info.ftCreationTime.dwHighDateTime)
	m_ctime = m_mtime;

    // FIXME: Number of links to a directory?
    m_links = info.nNumberOfLinks;
    m_size  = ((IOOffset) info.nFileSizeHigh << 32) + info.nFileSizeLow;

    // FIXME: GetDriveType of the rootdir of this file (need a name!);
    //   if it is one of DRIVE_FIXED, _REMOVABLE, _CDROM or _RAMDISK,
    //   nFileIndex{Low,High} is valid at least across reboots.
    // m_flags = FlagUniqueValid;
    m_unique [0] = info.dwVolumeSerialNumber;
    m_unique [1] = info.nFileIndexHigh;
    m_unique [2] = info.nFileIndexLow;

    if (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	m_type = TypeDirectory;
    // else if (symlink?) m_type = TypeSymlink;
    // else if (socket?)  m_type = TypeSocket;

    // FIXME: Get file security descriptor and fill in mode, uid, gid.
    //  If the security descriptor exists and FILE_ATTRIBUTE_READONLY,
    //  turn off write access for ugo.  If no file type given yet, set
    //  TypeFile.

    // m_acl = 0;
    // m_acl.applyNative (hfi.dwFileAttributes);
    aclFromAttrs (m_acl, info.dwFileAttributes);

    if (m_type == TypeDirectory)
	m_acl.addPortable (FileAcl::ExecBits);
    else if (m_type != TypeUnknown)
	;
    // else if (socket?) m_type = TypeSocket;
    else
	switch (GetFileType (fd))
	{
	case FILE_TYPE_CHAR:
	case FILE_TYPE_UNKNOWN:
	    m_type = TypeCharDev;
	    break;

	case FILE_TYPE_DISK:
	    m_type = TypeFile;
	    // FIXME: Check if has executable magic chars? |= FileAcl::ExecBits
	    // FIXME: Check if name is .exe, .com, .bat?   |= FileAcl::ExecBits
	    break;

	case FILE_TYPE_PIPE:
	    m_type = TypeFIFO;
	    // FIXME? m_acl = RBITS | WBITS |WGRP | WOTH
	    break;
	}

    // FIXME: m_acl.owner
    // FIXME: m_acl.group
    return true;
}

bool
IOStatus::fill (const char *name, bool traverseLinks /* = true */)
{
    ASSERT (name);

    HANDLE h;
    DWORD attrs = GetFileAttributes (name);
    if (attrs == INVALID_FILE_ATTRIBUTES)
    {
	clear ();
	return false;
    }

    // Get information about it: need a handle, and then we can get
    // the details through the handle.  (FIXME: If this fails, should
    // we still provide some information -- we may be able to do it if
    // we call GetFileAttributes()?  It would help things like
    // "exist()", which needs only the above call.)
    if ((h = CreateFile (name, 0 /* access */, 0 /* sharing */,
			 0 /* security */, OPEN_EXISTING,
			 FILE_ATTRIBUTE_NORMAL, 0 /* template */))
	!= IOFD_INVALID)
    {
	bool ret = fill (h);
	CloseHandle (h);
	return ret;
    }

    // FIXME: Do we ever need to fall back on this code?
    clear ();

    char		drive [5];
    WIN32_FIND_DATA	fnd;

    // Get information about the file.  If it is a drive spec only,
    // we need to use "x:\*", not just the drive.
    if (name [3] == '\0' && ISDRIVE (name))
    {
	drive [0] = *name;
	drive [1] = ':';
	drive [2] = '\\';
	drive [3] = '*';
	drive [4] = 0;
	name = drive;
    }

    if ((h = FindFirstFile (name, &fnd)) == IOFD_INVALID)
	return false;
    FindClose (h);

    // Fill in the information we have access to.
    m_ctime = Time::from (&fnd.ftCreationTime);
    m_mtime = Time::from (&fnd.ftLastWriteTime);
    m_atime = Time::from (&fnd.ftLastAccessTime);

    m_links = 1;
    m_size = ((IOOffset) fnd.nFileSizeHigh << 32) | fnd.nFileSizeLow;

    // FIXME: acl

    // FIXME: Check for symlink
    if (fnd.dwFileAttributes != INVALID_FILE_ATTRIBUTES)
    {
	if (fnd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	    m_type = IOStatus::TypeDirectory;
	else
	    m_type = IOStatus::TypeFile;

        aclFromAttrs (m_acl, fnd.dwFileAttributes);
    }

    return true;
}

} // namespace lat
#endif // _WIN32
