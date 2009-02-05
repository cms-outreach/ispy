//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/File.h"
#include "classlib/iobase/IOStatus.h"
#include "classlib/iobase/FileError.h"
#include "classlib/utils/SystemError.h"
#include "classlib/utils/Log.h"
#include "classlib/sysapi/File.h"
#ifdef _WIN32

// FIXME: GetFileSizeEx
// FIXME: GetFileType --> FILE_TYPE_...
// FIXME: GetFileTimes
// FIXME: FlushFileBuffers

// FIXME: msvc++ rtl
//  snprintf exists (returns < 0 for lack of space)

namespace lat {
using namespace IOFlags;

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IOFD
File::sysduplicate (IOFD fd)
{
    IOFD copyfd;
    if (! DuplicateHandle (GetCurrentProcess (), fd,
			   GetCurrentProcess (), &copyfd,
			   0, true, DUPLICATE_SAME_ACCESS))
	throw FileError ("DuplicateHandle()", GetLastError ());

    return copyfd;
}

void
File::sysopen (const char *name, int flags, FileAcl perms,
	       IOFD &newfd, unsigned &newflags)
{
    // FIXME: non-blocking open of fifo (named pipe)?
    SECURITY_ATTRIBUTES	sa;
    DWORD		access = 0;
    DWORD		openmode = 0;
    DWORD		attrs = 0;
    DWORD		sharing = FILE_SHARE_READ
				  | FILE_SHARE_WRITE
				  | FILE_SHARE_DELETE;

    // Setup inherited security attributes (FIXME: merge somewhere else)
    sa.nLength = sizeof (sa);
    sa.bInheritHandle = true;
    sa.lpSecurityDescriptor = 0;

    // Determine access mode.
    if (flags & OpenRead)
	access |= GENERIC_READ;
    if (flags & OpenWrite)
	access |= GENERIC_WRITE;

    // Determine open mode.
    if ((flags & OpenTruncate) && (flags & OpenWrite))
	if (flags & OpenCreate)
	    openmode = CREATE_ALWAYS;
	else
	    openmode = TRUNCATE_EXISTING;
    else if (flags & OpenCreate)
	openmode = OPEN_ALWAYS;
    else
	openmode = OPEN_EXISTING;

    if ((flags & OpenExclusive) && (flags & OpenCreate))
	openmode = CREATE_NEW;

    // Remember append mode for later use.
    if (flags & OpenAppend)
	newflags |= OpenAppend;

    // Determine file attributes.
    attrs = FILE_ATTRIBUTE_NORMAL;
    // FIXME: if dir open -> attrs |= FILE_FLAG_BACKUP_SEMANTICS
    // FIXME: if serial dev -> attrs |= FILE_FLAG_OVERLAPPED
    // FIXME: if creating a real file and name part begins with `.'
    //   --> FILE_ATTRIBUTE_HIDDEN?

    // Set unbuffered I/O flag if requested.  FILE_FLAG_NO_BUFFERING
    // requires buffers to be aligned so it's not generally useful.
    if (flags & OpenUnbuffered)
	attrs |= FILE_FLAG_WRITE_THROUGH;

    // Set flags according to requested permissions.
    if (perms.readonly ())
	attrs |= FILE_ATTRIBUTE_READONLY;
    if (perms.portable () & FileAcl::Archive)
	attrs |= FILE_ATTRIBUTE_ARCHIVE;
    if (perms.portable () & FileAcl::Encrypted)
	attrs |= FILE_ATTRIBUTE_ENCRYPTED;
    if (perms.portable () & FileAcl::Hidden)
	attrs |= FILE_ATTRIBUTE_HIDDEN;
    if (perms.portable () & FileAcl::NotIndexed)
	attrs |= FILE_ATTRIBUTE_NOT_CONTENT_INDEXED;
    if (perms.portable () & FileAcl::Offline)
	attrs |= FILE_ATTRIBUTE_OFFLINE;
    if (perms.portable () & FileAcl::System)
	attrs |= FILE_ATTRIBUTE_SYSTEM;
    if (perms.portable () & FileAcl::Temporary)
	attrs |= FILE_ATTRIBUTE_TEMPORARY;

    // FIXME: Set security attributes when creating a file.
    // if (flags & OpenCreate)
    //     perms.fill (&sa, alloca(4096), 4096, xxx & S_IFDIR?)

    // FIXME: Overlapped?
    newfd = CreateFile (name, access, sharing, &sa, openmode, attrs, 0);
    if (newfd == IOFD_INVALID)
	// FIXME: Check for EISDIR too?
	if (GetLastError () == ERROR_INVALID_HANDLE)
	    throw FileError ("CreateFile()", new SystemError (0, SysErr::ErrNoSuchEntity));
	else
	    throw FileError ("CreateFile()", GetLastError ());
}

IOSize
File::read (void *into, IOSize n, IOOffset pos)
{
    ASSERT (pos >= 0);

    // Fill in OVERLAPPED structure to describe the read position
    // (nothing asynchronous here, so no event).
    DWORD	read;
    OVERLAPPED	ov;

    memset (&ov, 0, sizeof (ov));
    ov.Offset	  = pos & 0xffffffff;
    ov.OffsetHigh = pos >> 32;
    if (! ReadFile (fd (), into, n, &read, &ov))
	throw FileError ("ReadFile()", GetLastError ());

    return read;
}

IOSize
File::write (const void *from, IOSize n, IOOffset pos)
{
    ASSERT (pos >= 0);

    // Fill in OVERLAPPED structure to describe the write position
    // (nothing asynchronous here, so no event).
    DWORD	written;
    OVERLAPPED	ov;

    memset (&ov, 0, sizeof (ov));
    ov.Offset	  = pos & 0xffffffff;
    ov.OffsetHigh = pos >> 32;
    if (! WriteFile (fd (), from, n, &written, &ov))
	throw FileError ("WriteFile()", GetLastError ());

    return written;
}

IOOffset
File::size (void) const
{
    IOFD fd = this->fd ();
    ASSERT (fd != IOFD_INVALID);

    DWORD high = 0;
    DWORD low = GetFileSize (fd, &high);
    if (high == INVALID_FILE_SIZE && GetLastError () != NO_ERROR)
	throw FileError ("GetFileSize()", GetLastError ());

    return ((IOOffset) high << 32) + low;
}

//  IOOffset
//  File::position (void) const
//  {
//      IOFD fd = this->fd ();
//      ASSERT (fd != IOFD_INVALID);
// 
//      LONG  low = 0;
//      LONG  high = 0;
//      DWORD here = SetFilePointer (fd, low, &high, FILE_CURRENT);
//      if (here == INVALID_FILE_SIZE && GetLastError () != NO_ERROR)
//  	throw FileError ("SetFilePointer()", GetLastErrno ());
// 
//      return ((IOOffset) high) << 32 + (DWORD) low;
//  }

IOOffset
File::position (IOOffset offset, Relative whence /* = SET */)
{
    IOFD fd = this->fd ();
    ASSERT (fd != IOFD_INVALID);
    ASSERT (whence == CURRENT || whence == SET || whence == END);

    LONG  low = offset & 0xffffffff;
    LONG  high = offset >> 32;
    DWORD mywhence = (whence == SET ? FILE_BEGIN
		      : whence == CURRENT ? FILE_CURRENT
		      : FILE_END);
    DWORD here = SetFilePointer (fd, low, &high, mywhence);
    if (here == INVALID_FILE_SIZE && GetLastError () != NO_ERROR)
	throw FileError ("SetFilePointer()", GetLastError ());

    return ((IOOffset) high << 32) + (DWORD) low;
}

void
File::resize (IOOffset size)
{
    IOFD fd = this->fd ();
    ASSERT (fd != IOFD_INVALID);

    // FIXME: Thread safety?
    IOOffset old = position (0, CURRENT);

    position (size);
    if (! SetEndOfFile (fd))
    {
	int	err = GetLastError ();
	LONG	high = old >> 32;
	SetFilePointer (fd, old & 0xffffffff, &high, FILE_BEGIN);
	throw FileError ("SetEndOfFile()", err);
    }

    position (old);
}

void
File::flush (void)
{
    IOFD fd = this->fd ();
    ASSERT (fd != IOFD_INVALID);

    if (! FlushFileBuffers (fd))
	throw FileError ("FlushFileBuffers()", GetLastError ());
}

bool
File::sysclose (IOFD fd, int *error /* = 0 */)
{
    bool ret = CloseHandle (fd);
    if (error) *error = GetLastError ();
    return ret;
}

void
File::times (Time *ctime, Time *mtime, Time *atime) const
{
    IOFD fd = this->fd ();
    ASSERT (fd != IOFD_INVALID);
    BY_HANDLE_FILE_INFORMATION info;
    if (! GetFileInformationByHandle (fd, &info))
	throw FileError ("GetFileInformationByHandle()", GetLastError ());

    // FAT doesn't have atime/ctime: copy mtime.
    if (ctime)
	if (! info.ftCreationTime.dwLowDateTime
	    && ! info.ftCreationTime.dwHighDateTime)
	    *ctime = Time::from (&info.ftLastWriteTime);
	else
	    *ctime = Time::from (&info.ftCreationTime);

    if (mtime)
	*mtime = Time::from (&info.ftLastWriteTime);

    if (atime)
	if (! info.ftLastAccessTime.dwLowDateTime
	    && ! info.ftLastAccessTime.dwHighDateTime)
	    *atime = Time::from (&info.ftLastWriteTime);
	else
	    *atime = Time::from (&info.ftLastAccessTime);
}

bool
File::status (IOStatus &s) const
{ return s.fill (fd ()); }

} // namespace lat
#endif // _WIN32
