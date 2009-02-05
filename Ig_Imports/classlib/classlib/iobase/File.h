#ifndef CLASSLIB_FILE_H
# define CLASSLIB_FILE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/iobase/IOChannel.h"
# include "classlib/iobase/IOFlags.h"
# include "classlib/iobase/Storage.h"
# include "classlib/iobase/FileAcl.h"
# include <string>

namespace lat {

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class File;
class IOStatus;
class Time;
class logstream;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>

logstream &operator<< (logstream &log, const File &f);

//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Basic file-related functions.  Most operations related to file
    names or file system can be found in #Filename.  */
class File : public IOChannel, public Storage
{
public:
    File (void);
    File (IOFD fd, bool autoclose = true);
    File (const char *name, int flags = IOFlags::OpenRead, FileAcl perms = 0666);
    File (const std::string &name, int flags = IOFlags::OpenRead, FileAcl perms = 0666);
    ~File (void);
    // implicit copy constructor
    // implicit assignment operator

    // virtual File *	copy (void) const;
    // FIXME: dup, dup2 support?
    
    // FIXME: File: freopen (mode)
    // FIXME: File: fdopen()
    // FIXME: File: fileno()
    virtual void	create (const char *name,
				bool exclusive = false,
				FileAcl perms = 0666);
    virtual void	create (const std::string &name,
				bool exclusive = false,
				FileAcl perms = 0666);
    virtual void	open (const char *name,
			      int flags = IOFlags::OpenRead,
			      FileAcl perms = 0666);
    virtual void	open (const std::string &name,
			      int flags = IOFlags::OpenRead,
			      FileAcl perms = 0666);
    virtual void	attach (IOFD fd);

    using Storage::read;
    using Storage::write;
    using Storage::position;

    virtual IOSize	read (void *into, IOSize n);
    virtual IOSize	read (void *into, IOSize n, IOOffset pos);
    virtual IOSize	readv (IOBuffer *into, IOSize length);

    virtual IOSize	write (const void *from, IOSize n);
    virtual IOSize	write (const void *from, IOSize n, IOOffset pos);
    virtual IOSize	writev (const IOBuffer *from, IOSize length);

    // virtual bool	eof (void) const;
    virtual IOOffset	size (void) const;
    virtual IOOffset	position (IOOffset offset, Relative whence = SET);

    virtual void	resize (IOOffset size);

    virtual void	flush (void);
    virtual void	close (void);
    virtual void	abort (void);

    virtual void	times (Time *ctime, Time *mtime, Time *atime) const;
    virtual bool	status (IOStatus &s) const;

    virtual void	setAutoClose (bool closeit);

    // FIXME: fdatasync, fsync
    // FIXME: sendfile?

    // FIXME: fcntl (getflags, ...) support?
    // FIXME: ioctl support?

    // FIXME: mount points interface?
    //   - unix: getmntent, ... -- /proc/mounts
    //   - windows: get root volume mount points

private:
    enum { InternalAutoClose = 4096 }; //< Close on delete

    File (IOFD fd, unsigned flags);

    File *		duplicate (bool copy) const;
    File *		duplicate (File *child) const;
    static IOFD		sysduplicate (IOFD fd);
    static void		sysopen (const char *name, int flags, FileAcl perms,
				 IOFD &newfd, unsigned &newflags);
    static bool		sysclose (IOFD fd, int *error = 0);

    unsigned		m_flags;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_FILE_H
