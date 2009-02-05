#ifndef CLASSLIB_IO_STATUS_H
# define CLASSLIB_IO_STATUS_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/utils/Time.h"
# include "classlib/iobase/FileAcl.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IOStatus;
class logstream;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>

logstream &operator<< (logstream &log, const IOStatus &s);

//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Collection of file status information.

    Using this information has limited value because of two reasons:
    not all systems provide meaningful values for all the fields, and
    many of the fields are not accurate.

    Deductions from the file's owner and the access mode bits should
    be avoided: they do not work on ACL-based filesystems neither on
    UNIX nor on Windows.  Instead use the more targeted queries such
    as #Filename::isWritable() or #File::times().  These usually
    return correct results.

    Getting this information is not efficient on all systems.  This is
    another reason to avoid this class unless the application knows
    what it is doing (e.g. choosing to be unportable for performance
    reasons).  */
class IOStatus
{
    // FIXME: Allow client to set hints which parts to find out
public:
    /// File types.
    enum Type
    {
	TypeUnknown,		//< Unknown or unsupported.
	TypeFile,		//< Regular file.
	TypeDirectory,		//< Directory.
	TypeSocket,		//< Socket.
	TypeSymlink,		//< Symbolic link.
	TypeNetworkSpecial,	//< Network special file.
	TypeBlockDev,		//< Block device.
	TypeCharDev,		//< Character device.
	TypeFIFO,		//< FIFO (named or anonymous pipe).
	TypeVolumeId,		//< Volume label.
	TypeMessageQueue,	//< Message queue as a file type.
	TypeSemaphore,		//< Semaphore as a file type.
	TypeSharedMem,		//< Shared memory as a file type.
	TypeMemoryObject	//< Typed memory object as a file type.
    };

    enum {
	FlagUniqueValid = 1		//< #m_unique is valid.
    };

    Time		m_ctime;	//< The date the file was created.
    Time		m_mtime;	//< The most recent modification date.
    Time		m_atime;	//< The most recent access date.

    Type		m_type;		//< Type of the file.
    FileAcl		m_acl;		//< File access permissions.

    unsigned		m_flags;	//< Flags.
    unsigned		m_unique [3];	//< Unique file identification.

    unsigned		m_links;	//< Number of names the file has.
    IOOffset		m_size;		//< Size of the file.
    //std::string	m_name;		//< File name if known.

    ////////////////////////////////////////////////////////////
    bool		fill (IOFD fd);
    bool		fill (const char *name, bool traverseLinks = true);
    void		clear (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_IO_STATUS_H
