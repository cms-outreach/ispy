#ifndef CLASSLIB_TAR_MEMBER_H
# define CLASSLIB_TAR_MEMBER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/zip/TarConstants.h"
# include "classlib/iobase/Filename.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** A TAR archive member.

    This class describes a single TAR archive member.  The member
    cannot be read or written through this class, use @TarInputStream
    or @TarOutputStream instead.

    Note that when adding a member to @TarOutputStream, the file size
    must be known or the stream must be told to buffer the data to
    find out the size on its own.  If the size is given, it must match
    the actual amount of data output (this is asserted).  */
class TarMember : public TarConstants
{
public:
    TarMember (Filename name);
    TarMember (const TarMember &x);
    TarMember &operator= (const TarMember &x);
    ~TarMember (void);

    Filename		name (void) const;
    int			mode (void) const;
    unsigned		mtime (void) const;
    IOOffset		size (void) const;
    int			uid (void) const;
    std::string		uname (void) const;
    int			gid (void) const;
    std::string		gname (void) const;
    Type		type (void) const;
    Filename		linkname (void) const;
    int			devmajor (void) const;
    int			devminor (void) const;

    void		name (Filename name);
    void		mode (int value);
    void		mtime (unsigned value);
    void		size (IOOffset value);
    void		uid (int value);
    void		uname (const std::string &name);
    void		gid (int value);
    void		gname (const std::string &name);
    void		type (Type value);
    void		type (char value);
    void		linkname (Filename value);
    void		device (int major, int minor);

private:
    Filename		m_name;		//< Name of the archive member
    unsigned		m_mtime;	//< Modification time
    // unsigned		m_atime;	//< Last access time (GNU extension)
    // unsigned		m_ctime;	//< Creation time (GNU extension)
    IOOffset		m_size;		//< File size
    int			m_mode;		//< UNIX file mode
    int			m_uid;		//< File owner id
    std::string		m_uname;	//< File owner name
    int			m_gid;		//< File group id
    std::string		m_gname;	//< File group name
    Type		m_type;		//< File type
    Filename		m_link;		//< Symlink target for links
    int			m_devmajor;	//< Device major number for devices
    int			m_devminor;	//< Device minor number for devices
    bool		m_ok;		//< Flag to indicate the file is ok
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_TAR_MEMBER_H
