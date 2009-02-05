//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/IOStatus.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/sysapi/IOStatus.h"
#ifndef _WIN32

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>

static bool
fillFromStat (IOStatus &s, const struct stat &buf)
{
    // FIXME: some systems have ns-accuracy times?
    s.m_ctime = Time (buf.st_ctime, 0);
    s.m_mtime = Time (buf.st_mtime, 0);
    s.m_atime = Time (buf.st_atime, 0);

    s.m_flags = IOStatus::FlagUniqueValid;
    s.m_unique [0] = buf.st_dev;
    s.m_unique [1] = buf.st_ino;
    s.m_unique [2] = 0;

    s.m_links = buf.st_nlink;
    s.m_size  = buf.st_size;
    s.m_acl.owner (buf.st_uid);
    s.m_acl.group (buf.st_gid);

    // FIXME: This *could* be useful information
    // s.m_rdev  = buf.st_rdev;

    // Map type.  The only one missing here is TypeVolumeId.
    s.m_type  =
	(S_ISREG       (buf.st_mode) ? IOStatus::TypeFile
	 : S_ISCHR     (buf.st_mode) ? IOStatus::TypeCharDev
	 : S_ISBLK     (buf.st_mode) ? IOStatus::TypeBlockDev
	 : S_ISDIR     (buf.st_mode) ? IOStatus::TypeDirectory
	 : S_ISFIFO    (buf.st_mode) ? IOStatus::TypeFIFO
	 : S_ISLNK     (buf.st_mode) ? IOStatus::TypeSymlink
	 : S_ISSOCK    (buf.st_mode) ? IOStatus::TypeSocket
	 : S_ISNWK     (buf.st_mode) ? IOStatus::TypeNetworkSpecial
	 : S_TYPEISMQ  (&buf)	     ? IOStatus::TypeMessageQueue
	 : S_TYPEISSEM (&buf)	     ? IOStatus::TypeSemaphore
	 : S_TYPEISSHM (&buf)	     ? IOStatus::TypeSharedMem
	 : S_TYPEISMO  (&buf)	     ? IOStatus::TypeMemoryObject
	 : IOStatus::TypeUnknown);

    // On unix FileAcl is native.
    s.m_acl = FileAcl (buf.st_mode, buf.st_uid, buf.st_gid);
    return true;
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

bool
IOStatus::fill (IOFD fd)
{
    m_ctime = m_mtime = m_atime = 0;
    m_type = TypeUnknown;
    m_acl = 0;
    m_flags = 0;
    m_unique [0] = m_unique [1] = m_unique [2] = 0;
    m_links = 0;
    m_size = 0;

    struct stat st;
    if (fd != IOFD_INVALID && fstat (fd, &st) >= 0)
	return fillFromStat (*this, st);
    else
	return false;
}

bool
IOStatus::fill (const char *name, bool traverseLinks /* = true */)
{
    ASSERT (name);

    m_ctime = m_mtime = m_atime = 0;
    m_type = TypeUnknown;
    m_acl = 0;
    m_flags = 0;
    m_unique [0] = m_unique [1] = m_unique [2] = 0;
    m_links = 0;
    m_size = 0;

    struct stat st;
    if ((traverseLinks ? ::stat (name, &st) : ::lstat (name, &st)) >= 0)
	return fillFromStat (*this, st);
    else
	return false;
}

} // namespace lat
#endif // !_WIN32
