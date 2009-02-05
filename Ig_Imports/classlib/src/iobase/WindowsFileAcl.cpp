//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/FileAcl.h"
#include "classlib/sysapi/FileAcl.h"
#ifdef _WIN32

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>

#if 0
class FileAclWindows 
{
public:
    // FIXME: set (Filename name, UID uid, GID gid, logsrv, int attr);
    //          -> read it, set it, write it
    // FIXME: set (Filename name, int attr)
    //          -> set (name, UserInfo::uid (), UserInfo::gid (),
    //                  UserInfo::logonserver(), attr);

    static DWORD read (Filename name, PSECURITY_DESCRIPTOR sd, DWORD *size);
    // Just wrap GetFileSecurity ((OWNER|GROUP|DACL)_SECURITY_INFORMATION)

    static DWORD write (Filename name, PSECURITY_DESCRIPTOR sd, DWORD *size);
    // CreateFile (name, WRITE_(OWNER|DAC), FILE_SHARE_(READ|WRITE),
    //             &none, OPEN_EXISTING, FILE_(ATTRIBUTE_NORMAL|BACKUP_SEMANTICS),
    //             0)
    // write security info header (BackupWrite)
    // write the security descriptor; ignore errors from SAMBA and FAT
    // terminate the restore process
    // closehandle

    static bool get (Filename name, int *attr, UID *uid, GID *gid);
    // read(), GetSecurityDescriptorOwner, GetSecurityDescriptorGroup(),
    // GetSecurityDescriptorDacl().  Decode the ACLs (if any).

    // addAllow (PACL acl, int offset, DWORD attrs, PSID sid, size_t &len, DWORD inherit)
    // addDeny (PACL acl, int offset, DWORD attrs, PSID sid, size_t &len, DWORD inherit)

    static PSECURITY_DESCRIPTOR allocate (/* FIXME */);
};
# endif

//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

int
FileAcl::portable (void) const
{ return m_perms; }		// On windows we only keep the portable flags.

int
FileAcl::addPortable (int mask)
{ return m_perms |= mask; }	// On windows we only keep the portable flags.

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
# if 0
void
FileAcl::native (PSECURITY_DESCRIPTOR sa, void *buf, DWORD size)
{
    // FIXME: Owner, Group?
}

void FileAcl::addNative (DWORD attributes)
{
    Hidden;
    System;
    Archive;
    Encrypted;
    NotIndexed;
    Offline;
    Temporary;

    ::_dos_getfileattr (rStatus.m_sName, &rStatus.m_nAttribute);
    // Remove read-only flag (this was already returned by `stat'
    if (rStatus.m_nAttribute & _A_RDONLY)
	rStatus.m_nAttribute &= ~_A_RDONLY;

    // Check directory, it is considered executable in DOS. Networked systems
    // will not even show the directory if there are no permissions for it.
    if (rStatus.m_nAttribute & _A_SUBDIR) {
	rStatus.m_nMode |= S_IXUSR;
	rStatus.m_nAttribute &= ~_A_SUBDIR;
    }

#if 0
    // Check executability - would there be any faster methods for this?
    if (rStatus.m_sName.length () >= 4 && rStatus.m_eType != Directory) {
	string sTrailer (rStatus.m_sName.from ((int)rStatus.m_sName.length () - 4));

	sTrailer.downcase ();
	if (sTrailer == ".exe"
	    || sTrailer == ".com"
	    || sTrailer == ".bat"
	    || sTrailer == ".pif")
	    rStatus.m_nMode |= S_IXUSR;
    }
#endif

    return m_perms;
}
#endif

} // namespace lat
#endif // _WIN32
