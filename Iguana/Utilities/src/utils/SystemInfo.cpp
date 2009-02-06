//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/utils/SystemInfo.h"
#include "classlib/utils/StringFormat.h"
#include "classlib/sysapi/SystemInfo.h"
#include <cstring>

// FIXME: msvc++ rtl
//  _cpuid

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

/** Return the system page size in bytes.  This is not necessarily the
    same as the hardware page size.  Returns zero if the page size
    cannot be determined.  */
size_t
SystemInfo::pagesize (void)
{
#ifdef _WIN32
    SYSTEM_INFO si;
    GetSystemInfo (&si);
    return si.dwPageSize;
#elif HAVE_GETPAGESIZE
    return getpagesize ();
#elif defined _SC_PAGESIZE
    return sysconf (_SC_PAGESIZE);
#elif defined _SC_PAGE_SIZE
    return sysconf (_SC_PAGE_SIZE);
#elif HAVE_SYS_SYSCTL_H && defined CTL_HW && defined HW_PAGESIZE
    int size;
    int mib [2] = { CTL_HW, HW_PAGESIZE };
    size_t len = sizeof (size);
    if (sysctl (mib, 2, &size, &len, 0, 0) >= 0)
	return size;
    return 0;
#else
    return 0;
#endif
}

/** Return the system memory allocation granularity.  On Windows this
    defines among other things the minimum unit at which files can be
    mapped in via #FileView.  On UNIX systems this is defined to 1,
    i.e. every byte offset in a file can be mapped from.  */
size_t
SystemInfo::allocGranularity (void)
{
#ifdef _WIN32
    SYSTEM_INFO si;
    GetSystemInfo (&si);
    return si.dwAllocationGranularity;
#else
    return 1;
#endif
}

/** Return the number of physical memory pages on the system.  */
size_t
SystemInfo::physicalPages (void)
{
#ifdef _WIN32
    MEMORYSTATUS memstat;
    memset (&memstat, 0, sizeof (memstat));
    memstat.dwLength = sizeof (memstat);
    GlobalMemoryStatus (&memstat);
    return memstat.dwTotalPhys / pagesize ();  // FIXME: need the divide?
#elif defined _SC_PHYS_PAGES
    return sysconf (_SC_PHYS_PAGES);
#elif HAVE_SYS_SYSCTL_H
    size_t len;
    int mib [2];
# if defined CTL_VM && defined VM_METER
    vmtotal info;
    mib [0] = CTL_VM;
    mib [1] = VM_METER;
    len = sizeof (info);
    if (sysctl (mib, 2, &info, &len, 0, 0) >= 0)
	return info.t_rm;

    // The above fails on darwin, so fall through to the other one.
    // For darwin we should use host_statistics(HOST_VM_INFO) for
    // memory information and host_info(HOST_BASIC_INFO) followed
    // by NXGetArchInfoFromCpuType for CPU info.
# endif // CTL_VM && VM_METER
# if defined CTL_HW && defined HW_PHYSMEM
    int size;
    mib [0] = CTL_HW;
    mib [1] = HW_PHYSMEM;
    len = sizeof (size);
    if (sysctl (mib, 2, &size, &len, 0, 0) >= 0)
	return size / getpagesize ();
# endif // CTL_HW && HW_PHYSMEM
    return 0;
#else
    return 0;
#endif
}

/** Return the number of available physical memory pages on the system.  */
size_t
SystemInfo::physicalPagesAvailable (void)
{
#ifdef _WIN32
    MEMORYSTATUS memstat;
    memset (&memstat, 0, sizeof (memstat));
    memstat.dwLength = sizeof (memstat);
    GlobalMemoryStatus (&memstat);
    return memstat.dwAvailPhys / pagesize ();  // FIXME: need the divide?
#elif defined _SC_AVPHYS_PAGES
    return sysconf (_SC_AVPHYS_PAGES);
#elif HAVE_SYS_SYSCTL_H
    size_t len;
    int mib [2];
# if defined CTL_VM && defined VM_METER
    vmtotal info;
    mib [0] = CTL_VM;
    mib [1] = VM_METER;
    len = sizeof (info);
    if (sysctl (mib, 2, &info, &len, 0, 0) >= 0)
	return info.t_arm;

    // The above fails on darwin, so fall through to the other one.
    // For darwin we should use host_statistics(HOST_VM_INFO) for
    // memory information and host_info(HOST_BASIC_INFO) followed
    // by NXGetArchInfoFromCpuType for CPU info.
# endif // CTL_VM && VM_METER
# if defined CTL_HW && defined HW_USERMEM
    int size;
    mib [0] = CTL_HW;
    mib [1] = HW_USERMEM;
    len = sizeof (size);
    if (sysctl (mib, 2, &size, &len, 0, 0) >= 0)
	return size / getpagesize ();
# endif // CTL_HW && HW_PHYSMEM
    return physicalPages ();
#else
    return physicalPages ();
#endif
}

/** Return the total virtual memory pages on the system (= physical
    memory + swap space).  This information is not available on all
    systems; if not, the physical memory size is returned.  Note that
    the result of multiplying the return value by the page size may
    not fit in a @c size_t. */
size_t
SystemInfo::virtualPages (void)
{
#ifdef _WIN32
    MEMORYSTATUS memstat;
    memset (&memstat, 0, sizeof (memstat));
    memstat.dwLength = sizeof (memstat);
    GlobalMemoryStatus (&memstat);
    return memstat.dwTotalVirtual / pagesize ();  // FIXME: need the divide?
#elif defined HAVE_SYS_SYSCTL_H && defined CTL_VM && defined VM_METER
    // FIXME: This fails on darwin.  Use host_statistics(HOST_VM_INFO)
    // plus scan stat.st_size of all files /private/var/vm/swapfile*.
    // Maybe swapctl(SWAP_NSWAP) + swapctl(SWAP_STATS) works?
    vmtotal info;
    int mib [2] = { CTL_VM, VM_METER };
    size_t len = sizeof (info);
    if (sysctl (mib, 2, &info, &len, 0, 0) >= 0)
	return info.t_avm;

    return physicalPages ();
#elif defined __linux
    // Read linux /proc/meminfo.  Be careful to work correctly on
    // systems with more memory than fits in size_t.  The meminfo file
    // has two sets of entries, one summary with byte sizes, which we
    // want to avoid.  The second is separate lines for each category
    // expressed in kilobytes ("Category: XXXX kB").  We pick those
    // since it gives us chance to fit the values in size_t.  The
    // categories that interest are "MemTotal" and "SwapTotal" -- the
    // sum of those is what we want (divided by pagesize () / 1024).
    //
    // This cannot be optimised, the values can change any time.
    if (FILE *f = fopen ("/proc/meminfo", "r"))
    {
	char	buf [128];
	size_t	mem = 0;
	size_t	swap = 0;
	int	gotit = 0;

	while (fgets (buf, sizeof (buf), f))
	    if (! strncmp (buf, "MemTotal:", 9))
		gotit |= 1, mem = atol (buf + 9);
	    else if (! strncmp (buf, "SwapTotal:", 10))
		gotit |= 2, swap = atol (buf + 10);

	fclose (f);

	if (gotit == 3)
	    return (mem + swap) / (pagesize () / 1024);
    }

    return physicalPages ();
#else
    return physicalPages ();
#endif
}

/** Return the available virtual memory pages on the system (=
    physical memory + swap space).  This information is not available
    on all systems; if not, the available physical memory size is
    returned.  Note that the result of multiplying the return value by
    the page size may not fit in a @c size_t.  */
size_t
SystemInfo::virtualPagesAvailable (void)
{
#ifdef _WIN32
    MEMORYSTATUS memstat;
    memset (&memstat, 0, sizeof (memstat));
    memstat.dwLength = sizeof (memstat);
    GlobalMemoryStatus (&memstat);
    return memstat.dwAvailVirtual / pagesize ();  // FIXME: need the divide?
#elif defined HAVE_SYS_SYSCTL_H && defined CTL_VM && defined VM_METER
    // FIXME: This fails on darwin.  Use host_statistics(HOST_VM_INFO)
    // plus scan stat.st_size of all files /private/var/vm/swapfile*.
    // Maybe swapctl(SWAP_NSWAP) + swapctl(SWAP_STATS) works?
    vmtotal info;
    int mib [2] = { CTL_VM, VM_METER };
    size_t len = sizeof (info);
    if (sysctl (mib, 2, &info, &len, 0, 0) >= 0)
	return info.t_free;

    return physicalPagesAvailable ();
#elif defined __linux
    // Read linux /proc/meminfo.  Be careful to work correctly on
    // systems with more memory than fits in size_t.  The meminfo file
    // has two sets of entries, one summary with byte sizes, which we
    // want to avoid.  The second is separate lines for each category
    // expressed in kilobytes ("Category: XXXX kB").  We pick those
    // since it gives us chance to fit the values in size_t.  The
    // categories that interest are "MemTotal" and "SwapTotal" -- the
    // sum of those is what we want (divided by pagesize () / 1024).
    //
    // This cannot be optimised, the values can change any time.
    if (FILE *f = fopen ("/proc/meminfo", "r"))
    {
	char	buf [128];
	size_t	mem = 0;
	size_t	swap = 0;
	int	gotit = 0;

	while (fgets (buf, sizeof (buf), f))
	    if (! strncmp (buf, "MemFree:", 8))
		gotit |= 1, mem = atol (buf + 8);
	    else if (! strncmp (buf, "SwapFree:", 9))
		gotit |= 2, swap = atol (buf + 9);

	fclose (f);

	if (gotit == 3)
	    return (mem + swap) / (pagesize () / 1024);
    }

    return physicalPagesAvailable ();
#else
    return physicalPagesAvailable ();
#endif
}

//////////////////////////////////////////////////////////////////////
/** Return the number of processors on the system.  To find out how
    many of them are actually online, use #processorsOnline(). */
size_t
SystemInfo::processors (void)
{
#ifdef _WIN32
    SYSTEM_INFO si;
    GetSystemInfo (&si);
    return si.dwNumberOfProcessors;
#elif defined _SC_NPROCESSORS_CONF
    return sysconf (_SC_NPROCESSORS_CONF);
#elif HAVE_SYS_SYSCTL_H && defined CTL_HW && defined HW_NCPU
    int n;
    int mib [2] = { CTL_HW, HW_NCPU };
    size_t len = sizeof (n);
    if (sysctl (mib, 2, &n, &len, 0, 0) >= 0)
	return n;
    return 1;
#else
    return 1;
#endif
}

/** Return the number of processors currently online.  To find out
    how many processors there are in total, use #processors(). */
size_t
SystemInfo::processorsOnline (void)
{
#ifdef _WIN32
    SYSTEM_INFO si;
    size_t	nproc = 0;

    GetSystemInfo (&si);
    nproc = si.dwNumberOfProcessors;
    for (unsigned n = 0; n < si.dwNumberOfProcessors; ++n)
	if (si.dwActiveProcessorMask & (1 << n))
	    ++nproc;

    return nproc;
#elif defined _SC_NPROCESSORS_ONLN
    return sysconf (_SC_NPROCESSORS_ONLN);
#else
    return processors ();
#endif
}

//////////////////////////////////////////////////////////////////////
/** Return the system's computer name.  This should be the same as the
    name returned by #HostInfo::name().  It is usually but not always
    the same as the network name returned by #HostInfo::dnsname().
    Some systems, Windows in particular but also many roaming devices,
    identify the computer separately from the network name.  The value
    returned by this function should not be used in a network context,
    use #HostInfo::dnsname() for such needs.  */
std::string
SystemInfo::nodename (void)
{
    // FIXME: Merge with #HostInfo::name.
    // FIXME: sysctl CTL_KERN:KERN_HOSTID/HOSTNAME?

#ifdef _WIN32
    char	buf [MAX_COMPUTERNAME_LENGTH + 1];
    DWORD	len = sizeof (buf);
    GetComputerName (&buf [0], &len);
    return buf;
#else
    struct utsname n;
    if (uname (&n) >= 0)
	return n.nodename;
#endif

    return "";
}

/** Return the name of this computer's domain.  This is not the
    internet domain name as #HostInfo::dnsdomain() returns, but the
    local cluster domain name if one has been configured: usually NIS
    or yellow pages domain name.  Note that on Windows this is not the
    user's logon domain, use #UserInfo::domain() to retrieve that. */
std::string
SystemInfo::domainname (void)
{
    // FIXME: sysctl CTL_KERN:KERN_DOMAINNAME?
#if ! defined _WIN32 && HAVE_UTSNAME_DOMAINNAME
    struct utsname n;
    if (uname (&n) >= 0)
	return n.domainname;
#endif

    return "";
}

/** Return the operating system name.  */
std::string
SystemInfo::osname (void)
{
#ifdef _WIN32
    return "Windows";
#else
    struct utsname n;
    if (uname (&n) >= 0)
	return n.sysname;

    return "unknown";
#endif
}

/** Return the operating release.  */
std::string
SystemInfo::osrelease (void)
{
#ifdef _WIN32
    OSVERSIONINFO vi;
    memset (&vi, 0, sizeof (vi));
    vi.dwOSVersionInfoSize = sizeof (vi);
    GetVersionEx (&vi);
    switch (vi.dwPlatformId)
    {
    case VER_PLATFORM_WIN32_NT:
      return StringFormat ("NT%1.%2.%3")
	    .arg (vi.dwMajorVersion)
	    .arg (vi.dwMinorVersion)
	    .arg (vi.dwBuildNumber)
	    .value ();

    case VER_PLATFORM_WIN32_WINDOWS:
	// We'll never run on anything less than NT5 (Windows 2000),
	// but keep the code here anyway for my oddball curiosity.
	switch (vi.dwMinorVersion)
	{
	case 0:  return strchr (vi.szCSDVersion, 'C') ? "95OSR2" : "95";
	case 10: return strchr (vi.szCSDVersion, 'A') ? "98SE" : "98";
	case 90: return "ME";
	}
    }
#else
    struct utsname n;
    if (uname (&n) >= 0)
	return n.release;
#endif

    return "unknown";
}

/** Return a description of the operating version.  */
std::string
SystemInfo::osversion (void)
{
#ifdef _WIN32
    OSVERSIONINFO vi;
    memset (&vi, 0, sizeof (vi));
    vi.dwOSVersionInfoSize = sizeof (vi);
    GetVersionEx (&vi);
    return vi.szCSDVersion;
#else
    struct utsname n;
    if (uname (&n) >= 0)
	return n.version;
    return "unknown";
#endif
}

/** Return the operating system's machine (hardware) type.  */
std::string
SystemInfo::machine (void) // hardware?
{
#ifdef _WIN32
    SYSTEM_INFO si;
    GetSystemInfo (&si);
    switch (si.wProcessorArchitecture)
    {
    case PROCESSOR_ARCHITECTURE_INTEL:		return "intel";
    case PROCESSOR_ARCHITECTURE_IA64:		return "ia64";
    case PROCESSOR_ARCHITECTURE_ALPHA:		return "alpha";
    case PROCESSOR_ARCHITECTURE_MIPS:		return "mips";
#ifdef PROCESSOR_ARCHITECTURE_IA32_ON_WIN64
    case PROCESSOR_ARCHITECTURE_IA32_ON_WIN64:	return "ia32-win64";
#endif
#ifdef PROCESSOR_ARCHITECTURE_ALPHA64
    case PROCESSOR_ARCHITECTURE_ALPHA64:	return "alpha64";
#endif
#ifdef PROCESSOR_ARCHITECTURE_AMD64
    case PROCESSOR_ARCHITECTURE_AMD64:		return "amd64";
#endif
#ifdef PROCESSOR_ARCHITECTURE_MSIL
    case PROCESSOR_ARCHITECTURE_MSIL:		return "msil";
#endif
#ifdef PROCESSOR_ARCHITECTURE_SHX
    case PROCESSOR_ARCHITECTURE_SHX:		return "shx";
#endif
#ifdef PROCESSOR_ARCHITECTURE_ARM
    case PROCESSOR_ARCHITECTURE_ARM:		return "arm";
#endif
    default:  return StringFormat ("#%1").arg (si.wProcessorArchitecture).value ();
    }
// #elif defined HAVE_SYS_SYSCTL_H && defined CTL_HW && defined HW_MACHINE
//     std::string result;
//     int mib [2] = { CTL_HW, HW_MACHINE };
//     size_t len;
//     if (sysctl (mib, 2, 0, &len, 0, 0) >= 0)
//     {
//  	result.resize (len, '\0');
// 	if (sysctl (mib, 2, &result[0], len, 0, 0) >= 0)
// 	    return result;
//     }
#else
    struct utsname n;
    if (uname (&n) >= 0)
	return n.machine;
#endif
    return "unknown";
}

/** Return the host processor type.  */
std::string
SystemInfo::processor (void)
{
    // FIXME: should we take an argument to indicate which cpu?
    // FIXME: IsProcessorFeaturePresent?
    // FIXME: try various compilation time options?
    // FIXME: try various sorts of cpuid() functions?
    // FIXME: how precise should this try to be?
    // FIXME: what is this for anyway?
    std::string result = "unknown";
#ifdef _WIN32
    SYSTEM_INFO si;
    GetSystemInfo (&si);
    result = StringFormat ("L%1.R%2")
	     .arg (si.wProcessorLevel)
	     .arg (si.wProcessorRevision)
	     .value ();
#elif defined HAVE_SYS_SYSCTL_H && defined CTL_HW && defined HW_MODEL
    // FIXME: Try HW_MACHINE_ARCH too?
    int mib [2] = { CTL_HW, HW_MODEL };
    size_t len = 0;
    if (sysctl (mib, 2, 0, &len, 0, 0) >= 0)
    {
	std::string val (len, '\0');
	if (sysctl (mib, 2, &val[0], &len, 0, 0) >= 0)
	    result = val.substr (0, len-1);
    }
#elif HAVE_SYS_PROCESSOR_H && HAVE_PROCESSOR_INFO	// (solaris)
    processor_info_t pinfo;
    if (! processor_info (0, &pinfo))
	result = std::string (pinfo.pi_processor_type)
		 + "/" + pinfo.pi_fputypes;
#elif defined __linux
    // Get CPU description from /proc/cpuinfo on linux.  This is a
    // very pretty string -- it may well contain spaces.
    if (FILE *f = fopen ("/proc/cpuinfo", "r"))
    {
	char buf [128];
	char *name = 0;
	char *colon;
	while (! name && fgets (buf, sizeof (buf), f))
	    if (! strncmp (buf, "model name", 10)
		&& (colon = strchr (buf, ':'))
		&& colon [1] == ' ' && colon [2])
		name = colon + 2;

	fclose (f);

	if (name)
	    result = name;
    }
#endif

    return result;
}

//////////////////////////////////////////////////////////////////////
// Windows-specific methods

/** Return the Windows directory.  Returns an empty string on
    non-Windows platforms.  */
std::string
SystemInfo::windowsDirectory (void)
{
#ifdef _WIN32
    char buf [MAX_PATH];
    if (! GetWindowsDirectory (buf, sizeof (buf)))
	return buf;
#endif
    return "";
}

/** Return the Windows system directory.  Returns an empty string
    on non-Windows platforms.  */
std::string
SystemInfo::windowsSystemDirectory (void)
{
#ifdef _WIN32
    char buf [MAX_PATH];
    if (! GetSystemDirectory (buf, sizeof (buf)))
	return buf;
#endif
    return "";
}

} // namespace lat
