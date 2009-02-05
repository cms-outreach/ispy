//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/File.h"
#include "classlib/iobase/IOStatus.h"
#include "classlib/iobase/FileError.h"
#include "classlib/utils/Log.h"
#include "classlib/sysapi/File.h"
#ifndef _WIN32

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
    if ((copyfd = ::dup (fd)) == IOFD_INVALID)
	throw FileError ("dup()", errno);

    return copyfd;
}

void
File::sysopen (const char *name, int flags, FileAcl perms,
               IOFD &newfd, unsigned int& /*newflags*/)
{
    // Translate our flags to system flags.
    int openflags = 0;

    if ((flags & OpenRead) && (flags & OpenWrite))
	openflags |= O_RDWR;
    else if (flags & OpenRead)
	openflags |= O_RDONLY;
    else if (flags & OpenWrite)
	openflags |= O_WRONLY;

    if (flags & OpenNonBlock)
	openflags |= O_NONBLOCK;

    if (flags & OpenAppend)
	openflags |= O_APPEND;

#ifdef O_SYNC
    if (flags & OpenUnbuffered)
	openflags |= O_SYNC;
#else
    if (flags & OpenUnbuffered)
	newflags |= OpenUnbuffered;
#endif

    if (flags & OpenCreate)
	openflags |= O_CREAT;

    if (flags & OpenExclusive)
	openflags |= O_EXCL;

    if (flags & OpenTruncate)
	openflags |= O_TRUNC;

    if (flags & OpenNotCTTY)
	openflags |= O_NOCTTY;

    if ((newfd = ::open (name, openflags, perms.native ())) == -1)
	throw FileError ("open()", errno);
}

IOSize
File::read (void *into, IOSize n, IOOffset pos)
{
    ASSERT (pos >= 0);

#if HAVE_PREAD
    ssize_t s;
    do
	s = ::pread (fd (), into, n, pos);
    while (s == -1 && errno == EINTR);

    if (s == -1)
	throw FileError ("pread()", errno);

    return s;
#else
    return Storage::read (into, pos);
#endif
}

IOSize
File::write (const void *from, IOSize n, IOOffset pos)
{
    ASSERT (pos >= 0);

#if HAVE_PREAD
    ssize_t s;
    do
	s = ::pwrite (fd (), from, n, pos);
    while (s == -1 && errno == EINTR);

    if (s == -1)
	throw FileError ("pwrite()", errno);

    if (m_flags & OpenUnbuffered)
	// FIXME: Exception handling?
	flush ();

    return s;
#else
    IOSize s = Storage::write (from, pos);

    if (m_flags & OpenUnbuffered)
	// FIXME: Exception handling?
	flush ();

    return s;
#endif
}

IOOffset
File::size (void) const
{
    IOFD fd = this->fd ();
    ASSERT (fd != IOFD_INVALID);

    struct stat info;
    if (fstat (fd, &info) == -1)
	throw FileError ("fstat()", errno);

    return info.st_size;
}

//  IOOffset
//  File::position (void) const
//  {
//      IOFD fd = this->fd ();
//      ASSERT (fd != IOFD_INVALID);
// 
//      IOOffset result;
//      if ((result = ::lseek (fd, 0, SEEK_CUR)) == -1)
//  	throw FileError ("lseek()", errno);
// 
//      return result;
//  }

IOOffset
File::position (IOOffset offset, Relative whence /* = SET */)
{
    IOFD fd = this->fd ();
    ASSERT (fd != IOFD_INVALID);
    ASSERT (whence == CURRENT || whence == SET || whence == END);

    IOOffset result;
    int      mywhence = (whence == SET ? SEEK_SET
			 : whence == CURRENT ? SEEK_CUR
			 : SEEK_END);
    if ((result = ::lseek (fd, offset, mywhence)) == -1)
	throw FileError ("lseek()", errno);

    return result;
}

void
File::resize (IOOffset size)
{
    IOFD fd = this->fd ();
    ASSERT (fd != IOFD_INVALID);

    if (ftruncate (fd, size) == -1)
	throw FileError ("ftruncate()", errno);
}

void
File::flush (void)
{
    IOFD fd = this->fd ();
    ASSERT (fd != IOFD_INVALID);

#if HAVE_FDATASYNC
    if (fdatasync (fd) == -1)
	throw FileError ("fdatasync()", errno);
#elif HAVE_FSYNC
    if (fsync (fd) == -1)
	throw FileError ("fsync()", errno);
#endif
}

bool
File::sysclose (IOFD fd, int *error /* = 0 */)
{
    int ret = ::close (fd);
    if (error) *error = errno;
    return ret != -1;
}

void
File::times (Time *ctime, Time *mtime, Time *atime) const
{
    // FIXME: Some systems support ns-accuracy file times?
    IOFD fd = this->fd ();
    ASSERT (fd != IOFD_INVALID);
    struct stat info;
    if (fstat (fd, &info) == -1)
	throw FileError ("fstat()", errno);

    if (ctime)
	*ctime = Time (info.st_ctime, 0);
    if (mtime)
	*mtime = Time (info.st_mtime, 0);
    if (atime)
	*atime = Time (info.st_atime, 0);
}

bool
File::status (IOStatus &s) const
{ return s.fill (fd ()); }

} // namespace lat
#endif // !_WIN32
