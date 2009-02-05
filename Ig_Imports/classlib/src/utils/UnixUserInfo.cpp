//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/utils/UserInfo.h"
#include "classlib/utils/SystemError.h"
#include "classlib/sysapi/UserInfo.h"
#ifndef _WIN32

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

UserInfo::UserInfo (struct passwd *entry, UserInfo *next)
{
    // `entry' can be null only for myself.  If we can't look
    // ourselves up in the password file, fall back on some basic
    // environment variables.
    if (! entry)
    {
	m_uid = getuid ();
	m_euid = geteuid ();
	m_gid = getgid ();
	m_egid = getegid ();
	entry = ::getpwuid (m_uid);
    }

    if (! entry)
    {
	if (const char *id = ::getenv ("USER"))
	    m_id = id;

	if (const char *dir = ::getenv ("HOME"))
	    m_homepath = dir;
    }
    else
    {
	// m_domain, m_logsrc, m_homedrive will be left empty and do
	// not matter on unixen.
	m_id		= entry->pw_name;
	m_password	= entry->pw_passwd;
	m_uid		= entry->pw_uid;
	m_euid		= entry->pw_uid;
	m_gid		= entry->pw_gid;
	m_egid		= entry->pw_gid;
	m_shell		= entry->pw_shell;

	// Prefer $HOME over the directory specified in /etc/passwd
	if (const char *dir = ::getenv ("HOME"))
	    m_homepath = dir;
	else
	    m_homepath = entry->pw_dir;

	// Extract name from gecos -- it is the first subfield,
	// separated from the other subfields by commas.
	size_t end = 0;
	while (entry->pw_gecos [end] && entry->pw_gecos [end] != ',')
	    ++end;

	m_name		= std::string (entry->pw_gecos, end);
    }

    m_next = next;
}

UserInfo *
UserInfo::user (const std::string &id)
{
    // FIXME: make thread safe
    if (id.empty ())
	return self ();

    UserInfo *u = s_users;
    while (u && u->m_id != id)
	u = u->m_next;

    if (! u)
    {
	struct passwd *entry = getpwnam (id.c_str ());
	if (! entry)
	    return 0;

	u = s_users = new UserInfo (entry, s_users);
    }

    return u;
}

} // namespace lat
#endif // !_WIN32
