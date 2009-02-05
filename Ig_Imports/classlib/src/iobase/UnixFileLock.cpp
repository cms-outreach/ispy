//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/FileLock.h"
#include "classlib/iobase/File.h"
#include "classlib/iobase/FileError.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/sysapi/FileLock.h"
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
FileLock::locked (File *file,
		  IOOffset pos,
		  IOOffset length,
		  bool write /* = true */)
{
    IOFD fd = file->fd ();
    ASSERT (fd != IOFD_INVALID);
    ASSERT (pos + length >= 0);

    struct flock desc;
    desc.l_type		= write ? F_WRLCK : F_RDLCK;
    desc.l_whence	= SEEK_SET;
    desc.l_start	= pos;
    desc.l_len		= length;
    if (::fcntl (fd, F_GETLK, &desc) == -1)
	throw FileError ("fcntl()", errno);

    // FIXME: return l_pid, l_start, l_len of the whole lock?
    return desc.l_type == F_UNLCK;
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

    struct flock desc;
    desc.l_type		= write ? F_WRLCK : F_RDLCK;
    desc.l_whence	= SEEK_SET;
    desc.l_start	= pos;
    desc.l_len		= length;
    if (::fcntl (fd, wait ? F_SETLK : F_SETLKW, &desc) == -1)
	throw FileError ("fcntl()", errno);
}

void
FileLock::unlock (File *file, IOOffset pos, IOOffset length)
{
    ASSERT (file);
    IOFD fd = file->fd ();
    ASSERT (fd != IOFD_INVALID);
    ASSERT (pos + length >= 0);

    struct flock desc;
    desc.l_type		= F_UNLCK;
    desc.l_whence	= SEEK_SET;
    desc.l_start	= pos;
    desc.l_len		= length;
    if (::fcntl (fd, F_SETLK, &desc) == -1)
	throw FileError ("fcntl()", errno);
}

} // namespace lat
#endif // _WIN32
