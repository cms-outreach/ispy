#ifndef CLASSLIB_USER_INFO_H
# define CLASSLIB_USER_INFO_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"
# include <sys/types.h>
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

struct passwd;

namespace lat {
// FIXME: Windows
typedef uid_t	UID;
typedef gid_t	GID;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Describe a user, and by default the current user.  */
class UserInfo
{
public:
    UID			uid (bool effective = true);
    GID			gid (bool effective = true);
    std::string		id (void);
    std::string		domain (void);
    std::string		logonserver (void);
    std::string		name (void);
    std::string		homedir (void);
    std::string		homedrive (void);
    std::string		password (void);
    std::string		shell (void);

    static UserInfo *	self (void);
    static UserInfo *	user (const std::string &id);

private:
    UserInfo (const std::string &id, UserInfo *next);
    UserInfo (struct passwd *entry, UserInfo *next);

    static UserInfo	*s_self;
    static UserInfo	*s_users;

    UserInfo		*m_next;
    UID			m_uid;
    UID			m_euid;
    GID			m_gid;
    GID			m_egid;
    std::string		m_id;
    std::string		m_password;
    std::string		m_name;
    std::string		m_domain;
    std::string		m_logsrv;
    std::string		m_homepath;
    std::string		m_homedrive;
    std::string		m_shell;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_USER_INFO_H
