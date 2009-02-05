//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/FileLock.h"
#include "classlib/iobase/File.h"
#include "classlib/iobase/FileError.h"
#include "classlib/utils/SystemError.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/sysapi/FileLock.h"
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

bool
FileLock::locked (File *file,
		  IOOffset pos,
		  IOOffset length,
		  bool write /* = true */)
{
    IOFD fd = file->fd ();
    ASSERT (fd != IOFD_INVALID);
    ASSERT (pos + length >= 0);

    // FIXME: Not yet supported.  AFAIK, there's no way to find out
    // what locks are currently being held.  Maybe we could try
    // locking and if it succeeds, unlock it and report success.
    ASSERT (false);
    throw FileError (new SystemError (0, SysErr::ErrFunctionNotImplemented));
}

void
FileLock::lock (File *file,
		IOOffset pos,
		IOOffset length,
		bool write /* = true */,
		bool wait /* = false */)
{
    ASSERT (file);
    IOFD fd = file->fd ();
    ASSERT (fd != IOFD_INVALID);
    ASSERT (pos + length >= 0);

    ASSERT (! m_file);
    ASSERT (! m_offset);
    ASSERT (! m_length);

    m_file = file;
    m_offset = pos;
    m_length = length;

    OVERLAPPED	ov;
    DWORD	lower;
    DWORD	upper;
    DWORD	flags = (!wait ? LOCKFILE_FAIL_IMMEDIATELY : 0
			 | (write ? LOCKFILE_EXCLUSIVE_LOCK : 0));

    // Make sure length is positive.  Translate zero length to mean
    // the whole file including all future extensions (as per POSIX).
    if (length < 0)
    {
	length = -length;
	pos -= length;
    }

    if (length == 0)
	lower = upper = 0xffffffff;
    else
    {
	lower = length & 0xffffffff;
	upper = length >> 32;
    }

    // Fill in OVERLAPPED structure to describe the lock position
    // (nothing asynchronous here, so no event).
    memset (&ov, 0, sizeof (ov));
    ov.Offset	  = pos & 0xffffffff;
    ov.OffsetHigh = pos >> 32;

    if (! LockFileEx (fd, flags, 0, lower, upper, &ov))
	if ((flags & LOCKFILE_FAIL_IMMEDIATELY)
	    && GetLastError () == ERROR_LOCK_FAILED)
	    throw FileError ("LockFileEx()", new SystemError (0, SysErr::ErrTryAgain));
	else
	    throw FileError ("LockFileEx()", GetLastError ());
}

void
FileLock::unlock (File *file, IOOffset pos, IOOffset length)
{
    ASSERT (file);
    IOFD fd = file->fd ();
    ASSERT (fd != IOFD_INVALID);
    ASSERT (pos + length >= 0);

    OVERLAPPED	ov;
    DWORD	lower;
    DWORD	upper;

    // Make sure length is positive.  Translate zero length to mean
    // the whole file including all future extensions (as per POSIX).
    if (length < 0)
    {
	length = -length;
	pos -= length;
    }

    if (length == 0)
	lower = upper = 0xffffffff;
    else
    {
	lower = length & 0xffffffff;
	upper = length >> 32;
    }

    // Fill in OVERLAPPED structure to describe the lock position
    // (nothing asynchronous here, so no event).
    memset (&ov, 0, sizeof (ov));
    ov.Offset	  = pos & 0xffffffff;
    ov.OffsetHigh = pos >> 32;

    if (! UnlockFileEx (fd, 0, lower, upper, &ov))
	throw FileError ("UnlockFileEx()", GetLastError ());
}

} // namespace lat
#endif // _WIN32
