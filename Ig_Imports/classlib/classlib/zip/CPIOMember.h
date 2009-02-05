#ifndef CLASSLIB_CPIO_MEMBER_H
# define CLASSLIB_CPIO_MEMBER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/zip/CPIOConstants.h"
# include "classlib/iobase/Filename.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** A CPIO archive member.

    This class describes a single CPIO archive member.  The member
    cannot be read or written through this class, use @CPIOInputStream
    or @CPIOOutputStream instead.

    Note that when adding a member to @CPIOOutputStream, the file size
    must be known or the stream must be told to buffer the data to
    find out the size on its own.  If the size is given, it must match
    the actual amount of data output (this is asserted).  */
class CPIOMember : public CPIOConstants
{
public:
    CPIOMember (Filename name);
    CPIOMember (const CPIOMember &x);
    CPIOMember &operator= (const CPIOMember &x);
    ~CPIOMember (void);

    Filename		name (void) const;
    int			mode (void) const;
    unsigned		mtime (void) const;
    IOOffset		size (void) const;
    int			uid (void) const;
    int			gid (void) const;
    int			rdev (void) const;
    int			rdevmajor (void) const;
    int			rdevminor (void) const;
    int			dev (void) const;
    int			devmajor (void) const;
    int			devminor (void) const;
    int			ino (void) const;
    int			nlink (void) const;

    void		name (Filename name);
    void		mode (int value);
    void		mtime (unsigned value);
    void		size (IOOffset value);
    void		uid (int value);
    void		gid (int value);
    void		rdev (int value);
    void		rdev (int major, int minor);
    void		dev (int value);
    void		dev (int major, int minor);
    void		ino (int value);
    void		nlink (int value);

private:
    Filename		m_name;		//< Name of the archive member
    unsigned		m_mtime;	//< Modification time
    IOOffset		m_size;		//< File size
    int			m_mode;		//< UNIX file mode
    int			m_uid;		//< File owner id
    int			m_gid;		//< File group id
    int			m_rdevmajor;	//< Device type (major part)
    int			m_rdevminor;	//< Device type (minor part)
    int			m_devmajor;	//< Device id (major part)
    int			m_devminor;	//< Device id (minor part)
    int			m_ino;		//< File inode on the device
    int			m_nlink;	//< Number of links to the file
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_CPIO_MEMBER_H
