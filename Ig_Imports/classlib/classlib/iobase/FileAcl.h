#ifndef CLASSLIB_FILE_ACL_H
# define CLASSLIB_FILE_ACL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/utils/UserInfo.h"
# undef unix // FIXME: visible on too many systems

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

// FIXME: Encapsulate UNIX file permssions (ugo/rwx, sticky, suid/sgid)
// and Windows SECURITY_ATTRIBUTES
class FileAcl
{
public:
    // FIXME: Owner, group recorded here?

    // Protection bits: (perm & FileAcl::XXXBits) >> FileAcl::XXXOffset
    enum {
	ReadBit		= 4,
	WriteBit	= 2,
	ExecBit		= 1,

	OwnerOffset	= 6,
	OwnerBits	= 7 << OwnerOffset,
	OwnerRead	= ReadBit  << OwnerOffset,
	OwnerWrite	= WriteBit << OwnerOffset,
	OwnerExec	= ExecBit  << OwnerOffset,

	GroupOffset	= 3,
	GroupBits	= 7 << GroupOffset,
	GroupRead	= ReadBit  << GroupOffset,
	GroupWrite	= WriteBit << GroupOffset,
	GroupExec	= ExecBit  << GroupOffset,

	OtherOffset	= 0,
	OtherBits	= 7 << OtherOffset,
	OtherRead	= ReadBit  << OtherOffset,
	OtherWrite	= WriteBit << OtherOffset,
	OtherExec	= ExecBit  << OtherOffset,

	ReadBits	= OwnerRead | GroupRead | OtherRead,
	WriteBits	= OwnerWrite | GroupWrite | OtherWrite,
	ExecBits	= OwnerExec | GroupExec | OtherExec,

	Sticky		= 1 << 10,
	SetGID		= 1 << 11,
	SetUID		= 1 << 12,

	// Non-standard specialities (Windows, HP-UX, ...)
	Hidden		= 1 << 13,	 // S_CDF on HP-UX
	System		= 1 << 14,
	Archive		= 1 << 15,

	Encrypted	= 1 << 16,
	NotIndexed	= 1 << 17,
	Offline		= 1 << 18,
	Temporary	= 1 << 19
    };

    FileAcl (void);
    FileAcl (int unixPerms, UID uid = UID (-1), GID gid = GID (-1));
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    UID			owner (void) const;
    void		owner (UID uid);
    GID			group (void) const;
    void		group (GID gid);

    int			portable (void) const;
    int			addPortable (int mask);

    int			unix (void) const;
    int			addUnix (int mask);

    bool		readonly (void) const;

#ifdef _WIN32
    // void		native (SECURITY_ATTRIBUTES *sa, void *buf, DWORD size) const;
    // int		addNative (DWORD attributes);
#else
    int			native (void) const;
    int			addNative (int mode);
#endif

private:
    UID			m_owner;	//< File's owner.
    GID			m_group;	//< File's group (FIXME: Windows?).
    int			m_perms;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_FILE_ACL_H
