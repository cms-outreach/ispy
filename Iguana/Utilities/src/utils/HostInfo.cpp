//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/utils/HostInfo.h"
#include "classlib/sysapi/HostInfo.h"
#include "classlib/sysapi/Windows.h"
#include <cctype>

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

/** Get the computer host name.  */
std::string
HostInfo::name (void)
{
    // FIXME: == SystemInfo::nodename()?
#ifdef _WIN32
    char buf [MAX_COMPUTERNAME_LENGTH+1];
    DWORD size = sizeof (buf);
    if (GetComputerName (buf, &size))
	return buf;
#else
    char buf [MAXHOSTNAMELEN];
    if (::gethostname (buf, sizeof (buf)) != -1)
	return buf;
#endif

    // Return loop back address
    return "localhost";
}

/** Get the host name as known by dns, or the host name if dns doesn't
    know a name for this computer.  */
std::string
HostInfo::dnsname (void)
{
#ifdef _WIN32
    char  buf [512];
    DWORD size = sizeof (buf);

    if (GetComputerNameEx (ComputerNameDnsHostname, buf, &size))
	return buf;
    else
	return name ();
#else
    // Use host lookup to find the definition for our hostname, and
    // then pick the first fully qualified name.  This is necessary on
    // hosts whose hostname setting is distinct their network name
    // (e.g. roaming devices that move from one network to another and
    // have a pseudo-name in /etc/hosts to map that to localhost).  We
    // want this function to return the network name of the computer.

    std::string			fullname (fqdn ());
    std::string::size_type	dot = fullname.find ('.');
    return (dot != std::string::npos
	    ? std::string (fullname, 0, dot)
	    : fullname);
#endif
}

/** Get the domain name.  */
std::string
HostInfo::dnsdomain (void)
{
#ifdef _WIN32
    char  buf [512];
    DWORD size = sizeof (buf);

    if (GetComputerNameEx (ComputerNameDnsDomain, buf, &size))
	return buf;
    else
	return "";
#else
    // FIXME: what if the name is numbers?
    std::string			fullname (fqdn ());
    std::string::size_type	dot = fullname.find ('.');
    return (dot != std::string::npos
	    ? std::string (fullname, dot+1)
	    : "");
#endif
}

/** Get the fully qualified domain name.  */
std::string
HostInfo::fqdn (void)
{
#ifdef _WIN32
    char  buf [512];
    DWORD size = sizeof (buf);

    if (GetComputerNameEx (ComputerNameDnsFullyQualified, buf, &size))
	return buf;
    else
	return name ();
#else
    std::string hostname (name ());
    if (isfqdn (hostname.c_str ()))
	return hostname;

    // FIXME: gethostbyname is not thread safe
    if (hostent *host = gethostbyname (hostname.c_str ())) 
    {
	if (host->h_name && isfqdn (host->h_name))
	    return host->h_name;

	for (char **alias = host->h_aliases; alias && *alias; ++alias)
	    if (isfqdn (*alias))
		return *alias;
    }

    return hostname;
#endif
}

/** Check if @a name is a fully qualified domain name.  Returns @c
    true if the name contains at least one dot and other characters
    around that dot.  Does not return @c true for numeric addresses.  */
bool
HostInfo::isfqdn (const char *name)
{
    bool seenalpha = false;
    bool seendot = false;
    bool seenpre = false;
    bool seenpost = false;

    for ( ; name && *name && !seenpost; ++name)
    {
	if (*name != '.' && ! isdigit ((unsigned char) *name))
	    seenalpha = true;

	if (! seendot && *name != '.')
	    seenpre = true;
	else if (seenpre && *name == '.')
	    seendot = true;
	else if (seendot && *name != '.')
	    seenpost = true;
    }

    return seenpost && seenalpha;
}

} // namespace lat
