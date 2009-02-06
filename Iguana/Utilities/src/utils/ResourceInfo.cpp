//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/utils/ResourceInfo.h"
#include "classlib/utils/SystemError.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/sysapi/ResourceInfo.h"
#include <cstring>

// FIXME: On Windows, call GetSystemInfo, GetSystemMetrics (only a few),
// 

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

const ResourceInfo::Details	ResourceInfo::s_resource_details [] = {
    { RES_USER_TIME,		"User time",				"msecs", 0 },
    { RES_SYSTEM_TIME,		"System time",				"msecs", 0 },
    { RES_MAX_RSS,		"Maximum resident set size",		"kB",    0 },
    { RES_TEXT_RSS,		"Integrated text resident set size",	"kB*s",  0 },
    { RES_DATA_RSS,		"Integrated data resident set size",	"kB*s",  0 },
    { RES_STACK_RSS,		"Integrated stack resident set size",	"kB*s",  0 },
    { RES_MINOR_PAGE_FAULTS,	"Minor page faults",			"",      0 },
    { RES_MAJOR_PAGE_FAULTS,	"Major page faults",			"",      0 },
    { RES_SWAPS,		"Swaps",				"",      0 },
    { RES_FILESYS_OPS_IN,	"File-system block operations in",	"",      0 },
    { RES_FILESYS_OPS_OUT,	"File-system block operations out",	"",      0 },
    { RES_IPC_MSGS_SENT,	"IPC messages sent",			"",      0 },
    { RES_IPC_MSGS_RECEIVED,	"IPC messages received",		"",      0 },
    { RES_SIGNALS,		"Signals",				"",      0 },
    { RES_VOLUNTARY_CONTEXT_SWITCHES, "Voluntary context switches",	"",      0 },
    { RES_INVOLUNTARY_CONTEXT_SWITCHES, "Involuntary context switches",	"",      0 },

    { RES_VMEMORY,		"Virtual memory size",			"B",     0 },
    { RES_VMEMORY_LOCKED,	"Virtual memory locked",		"B",     0 },
    { RES_VMEMORY_RSS,		"Resident set size",			"B",     0 },
    { RES_VMEMORY_RSS_LIMIT,	"Resident set size limit",		"B",     0 },
    { RES_VMEMORY_DATA,		"Virtual memory data",			"B",     0 },
    { RES_VMEMORY_STACK,	"Virtual memory stack",			"B",     0 },
    { RES_VMEMORY_EXE,		"Virtual memory program",		"B",     0 },
    { RES_VMEMORY_LIBRARIES,	"Virtual memory libraries",		"B",     0 },
    { RES_VMEMORY_SHARED,	"Virtual memory shared",		"B",     0 },
    { RES_VMEMORY_DIRTY,	"Virtual memory dirty",			"B",     0 },
    { RES_VMEMORY_PERCENTAGE,	"Percentage of system memory used",	"%/100", 0 },

    { RES_VMEMORY_TEXT_BASE,	"Virtual memory text start",		"",  "%lx" },
    { RES_VMEMORY_TEXT_TOP,	"Virtual memory text end",		"",  "%lx" },
    { RES_VMEMORY_BSS_BASE,	"Virtual memory bss start",		"",  "%lx" },
    { RES_VMEMORY_BSS_TOP,	"Virtual memory bss top",		"",  "%lx" },
    { RES_VMEMORY_DATA_BASE,	"Virtual memory data start",		"",  "%lx" },
    { RES_VMEMORY_DATA_TOP,	"Virtual memory data top",		"",  "%lx" },
    { RES_VMEMORY_BREAK_BASE,	"Virtual memory heap start",		"",  "%lx" },
    { RES_VMEMORY_BREAK_TOP,	"Virtual memory heap top",		"",  "%lx" },
    { RES_VMEMORY_STACK_BASE,	"Virtual memory stack start",		"",  "%lx" },
    { RES_VMEMORY_STACK_TOP,	"Virtual memory stack end",		"",  "%lx" },
    { RES_VMEMORY_KERNEL_SP,	"Virtual memory kernel sp",		"",  "%lx" },
    { RES_VMEMORY_KERNEL_IP,	"Virtual memory kernel ip",		"",  "%lx" },
    
    { RES_N_FILE_DESCRIPTORS,	"File descriptors in use",		"",      0 },
    { RES_MAX_FILE_DESCRIPTORS,	"File descriptors limit",		"",      0 },

    { RES_PROCESS_STATE,	"Process state",			"",   "%c" },
    { RES_PROCESS_FLAGS,	"Process flags",			"",  "%lx" },
    { RES_PROCESS_LWPS,		"Kernel threads",			"",      0 },
    { RES_PROCESS_ADDRESS,	"Process address",			"",  "%lx" },

    { RES_TIME_SLICE_SIZE,	"Time left in next timeslice",		"msecs", 0 },
    { RES_PRIORITY_NICE,	"Priority (\"nice\" value)",		"",  "%ld" },
    { RES_PRIORITY,		"Priority (scheduling)",		"",  "%ld" },

    { RES_TIMEOUT,		"Timeout",				"msecs", 0 },
    { RES_INTERVAL_TIME_LEFT,	"Time until next interval alarm",	"msecs", 0 },
    { RES_START_TIME,		"Start time",				"msecs", 0 },
    { RES_CPU,			"CPU",					"",      0 },
    { RES_CPU_PERCENTAGE,	"Percentage of cpu used recently",	"%/100", 0 },
    
    { RES_DATA_MODEL,		"Data model",				"",      0 },

    { RES_INITIAL_ARGC,		"Initial argc",				"",      0 },
    { RES_INITIAL_ARGV,		"Initial argv",				"",  "%lx" },
    { RES_INITIAL_ENVP,		"Initial envp",				"",  "%lx" },

    { RES_MAX,			0,					0,       0 },
};

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

const char *
ResourceInfo::name (Resource type)
{
    for (int i = 0; s_resource_details [i].resource != RES_MAX; ++i)
	if (s_resource_details [i].resource == type)
	    return s_resource_details [i].name;

    return "Unknown resource";
}

const char *
ResourceInfo::unit (Resource type)
{
    for (int i = 0; s_resource_details [i].resource != RES_MAX; ++i)
	if (s_resource_details [i].resource == type)
	    return s_resource_details [i].unit;

    return "";
}

const char *
ResourceInfo::format (Resource type, Value value, char *buffer, size_t size)
{
    int i;
    for (i = 0; s_resource_details [i].resource != RES_MAX; ++i)
	if (s_resource_details [i].resource == type)
	    break;

    if (! s_resource_details [i].format)
	sprintf (buffer, "%lu", value);
    else if (s_resource_details [i].format[1] == 'c')
	sprintf (buffer, "%c", (char) value);
    else
	sprintf (buffer, s_resource_details [i].format, value);

    // FIXME: guard against size overflow (e.g. with snprintf), but at
    // least null terminate it always so that we don't get run-away
    // stack smashing
    buffer [size-1] = 0;
    return buffer;
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

ResourceInfo::Value
ResourceInfo::getlimit (Limit type, Kind kind)
{
    Value soft = LIM_INFINITE;
    Value hard = LIM_INFINITE;
    getlimits (type, soft, hard);
    return kind == SOFT ? soft : hard;
}

void
ResourceInfo::getlimits (Limit type, Value &soft, Value &hard)
{
    // FIXME: WIN32: NtQueryInformationProcess (ProcessQuotaLimits);
    // FIXME: WIN32: NtQueryInformationProcess (ProcessPooledUsageAndLimits);

    // Linux man pages say the resource argument to getrlimit is an
    // `int', but it really is `enum __rlimit_resource'.  This causes
    // compilers to warn (or choke) if we try to remap `type' to an an
    // `int' and then invoke getrlimit.  Do this the hard way.

    soft = hard = LIM_INFINITE;
    switch (type)
    {
    case LIMIT_CPU_TIME:
# if HAVE_GETRLIMIT && defined RLIMIT_CPU
	GETRLIMIT (RLIMIT_CPU);
# endif
	break;

    case LIMIT_FILE_SIZE:
# if HAVE_GETRLIMIT && defined RLIMIT_FSIZE
	GETRLIMIT (RLIMIT_FSIZE);
# elif HAVE_ULIMIT && defined UL_GETFSIZE
	GETULIMIT (UL_GETFSIZE, 1);
# elif HAVE_ULIMIT && defined UL_GFILLIM
	GETULIMIT (UL_GFILLIM, 512);
# endif
	break;	

    case LIMIT_CORE_SIZE:
# if HAVE_GETRLIMIT && defined RLIMIT_CORE
	GETRLIMIT (RLIMIT_CORE);
# endif
	break;	

    case LIMIT_N_PROCESSES:
# if HAVE_GETRLIMIT && defined RLIMIT_NPROC
	GETRLIMIT (RLIMIT_NPROC);
# endif
	break;	

    case LIMIT_N_OPEN_FILES:
# if HAVE_GETRLIMIT && defined RLIMIT_NOFILE
	GETRLIMIT (RLIMIT_NOFILE);
# elif HAVE_SC_OPEN_MAX
	soft = hard = sysconf (_SC_OPEN_MAX);
# elif HAVE_GETDTABLESIZE
	soft = hard = getdtablesize ();
# elif HAVE_ULIMIT && defined UL_GDESLIM
	GETULIMIT (UL_GDESLIM, 1);
# elif defined OPEN_MAX
	soft = hard = OPEN_MAX;
# elif defined NOFILE
	soft = hard = NOFILE;
# endif
	break;

    case LIMIT_VMEMORY:
# if HAVE_GETRLIMIT && defined RLIMIT_AS
	GETRLIMIT (RLIMIT_AS);
# elif HAVE_ULIMIT && defined UL_GMEMLIM
	GETULIMIT (UL_GMEMLIM, 1);
# endif
	break;

    case LIMIT_VMEMORY_RSS:
# if HAVE_GETRLIMIT && defined RLIMIT_RSS
	GETRLIMIT (RLIMIT_RSS);
# endif
	break;

    case LIMIT_VMEMORY_DATA:
# if HAVE_GETRLIMIT && defined RLIMIT_DATA
	GETRLIMIT (RLIMIT_DATA);
# endif
	break;

    case LIMIT_VMEMORY_STACK:
# if HAVE_GETRLIMIT && defined RLIMIT_STACK
	GETRLIMIT (RLIMIT_STACK);
# endif
	break;

    case LIMIT_VMEMORY_LOCKED:
# if HAVE_GETRLIMIT && defined RLIMIT_MEMLOCK
	GETRLIMIT (RLIMIT_MEMLOCK);
# endif
	break;
    }
}

//////////////////////////////////////////////////////////////////////
ResourceInfo::Value
ResourceInfo::setlimit (Limit type, Kind which, Value value)
{
    Value old_soft = LIM_INFINITE;
    Value old_hard = LIM_INFINITE;

    getlimits (type, old_soft, old_hard);
    if (which == SOFT)
    {
	setlimits (type, value, old_hard);
	return old_soft;
    }
    else
    {
	setlimits (type, old_soft, value);
	return old_hard;
    }
}

void
ResourceInfo::setlimits (Limit type, Value soft, Value hard)
{
    switch (type)
    {
    case LIMIT_CPU_TIME:
# if HAVE_GETRLIMIT && defined RLIMIT_CPU
	SETRLIMIT (RLIMIT_CPU);
# endif
	break;

    case LIMIT_FILE_SIZE:
# if HAVE_GETRLIMIT && defined RLIMIT_FSIZE
	SETRLIMIT (RLIMIT_FSIZE);
# endif
	break;

    case LIMIT_CORE_SIZE:
# if HAVE_GETRLIMIT && defined RLIMIT_CORE
	SETRLIMIT (RLIMIT_CORE);
# endif
	break;

    case LIMIT_N_PROCESSES:
# if HAVE_GETRLIMIT && defined RLIMIT_NPROC
	SETRLIMIT (RLIMIT_NPROC);
# endif
	break;

    case LIMIT_N_OPEN_FILES:
# if HAVE_GETRLIMIT && defined RLIMIT_NOFILE
	SETRLIMIT (RLIMIT_NOFILE);
# endif
	break;

    case LIMIT_VMEMORY:
# if HAVE_GETRLIMIT && defined RLIMIT_AS
	SETRLIMIT (RLIMIT_AS);
# endif
	break;

    case LIMIT_VMEMORY_RSS:
# if HAVE_GETRLIMIT && defined RLIMIT_RSS
	SETRLIMIT (RLIMIT_RSS);
# endif
	break;

    case LIMIT_VMEMORY_DATA:
# if HAVE_GETRLIMIT && defined RLIMIT_DATA
	SETRLIMIT (RLIMIT_DATA);
# endif
	break;

    case LIMIT_VMEMORY_STACK:
# if HAVE_GETRLIMIT && defined RLIMIT_STACK
	SETRLIMIT (RLIMIT_STACK);
# endif
	break;

    case LIMIT_VMEMORY_LOCKED:
# if HAVE_GETRLIMIT && defined RLIMIT_MEMLOCK
	SETRLIMIT (RLIMIT_MEMLOCK);
# endif
	break;
    }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
ResourceInfo::Status
ResourceInfo::info (Source from)
{
    switch (from)
    {
    case RES_FOR_SYSTEM:
	return info_system ();
    case RES_FOR_SELF:
	return info_self ();
    case RES_FOR_SELF_AND_CHILDREN:
	return info_self_cumulative ();
    default:
	ASSERT (false);
	return Status ();
    }
}

ResourceInfo::Status
ResourceInfo::info_system (void)
{
    Status s;

    // FIXME: get system info

    return s;
}


ResourceInfo::Status
ResourceInfo::info_self (void)
{
    Status s;

    // FIXME: get information for this process
    // FIXME: WIN32: Look at cygwin's /proc stuff
    // FIXME: WIN32: GetProcessIoCounters()
    // FIXME: WIN32: GetProcessHandleCount()
    // FIXME: WIN32: GetProcessWorkingSetSize()
    // FIXME: WIN32:  -- NtQueryInformationProcess (ProcessVmCounters);
    // FIXME: WIN32: GetStartupInfo()
    // FIXME: WIN32: GetCommandLine()
    // FIXME: WIN32: GetProcessTimes ()
    // FIXME: WIN32:  -- NtQueryInformationProcess (ProcessTimes);
    // FIXME: WIN32: NtQueryInformationProcess (ProcessPooledUsageAndLimits);
    // FIXME: WIN32: NtQueryInformationProcess (ProcessHandleCount);
    // FIXME: WIN32: GetProcessPriorityBoost ()
    // FIXME: WIN32:  -- NtQueryInformationProcess (ProcessPriorityBoost);

#if HAVE_GETRUSAGE
    // Get information using `getrusage'.  On many systems the values
    // are unreliable, so set only non-zero fields.  It could of
    // course happen that zero is the right value, only this produces
    // much more sensible results on most platforms.  At any rate,
    // code below adds to these in a platform specific manner more
    // accurately.
    rusage values;
    ::getrusage (RUSAGE_SELF, &values);
    s [RES_USER_TIME]			= values.ru_utime.tv_sec * 1000
					  + values.ru_utime.tv_usec / 1000;
    s [RES_SYSTEM_TIME]			= values.ru_stime.tv_sec * 1000
					  + values.ru_stime.tv_usec / 1000;
    if (values.ru_maxrss)
	s [RES_MAX_RSS]			= values.ru_maxrss;
    if (values.ru_ixrss)
	s [RES_TEXT_RSS]		= values.ru_ixrss;
    if (values.ru_idrss)
	s [RES_DATA_RSS]		= values.ru_idrss;
    if (values.ru_isrss)
	s [RES_STACK_RSS]		= values.ru_isrss;
    if (values.ru_minflt)
	s [RES_MINOR_PAGE_FAULTS]	= values.ru_minflt;
    if (values.ru_majflt)
	s [RES_MAJOR_PAGE_FAULTS]	= values.ru_majflt;
    if (values.ru_nswap)
	s [RES_SWAPS]			= values.ru_nswap;
    if (values.ru_inblock)
	s [RES_FILESYS_OPS_IN]		= values.ru_inblock;
    if (values.ru_oublock)
	s [RES_FILESYS_OPS_OUT]		= values.ru_oublock;
    if (values.ru_msgsnd)
	s [RES_IPC_MSGS_SENT]		= values.ru_msgsnd;
    if (values.ru_msgrcv)
	s [RES_IPC_MSGS_RECEIVED]	= values.ru_msgrcv;
    if (values.ru_nsignals)
	s [RES_SIGNALS]			= values.ru_nsignals;
    if (values.ru_nvcsw)
	s [RES_VOLUNTARY_CONTEXT_SWITCHES]= values.ru_nvcsw;
    if (values.ru_nivcsw)
	s [RES_INVOLUNTARY_CONTEXT_SWITCHES]= values.ru_nivcsw;

#endif // HAVE_GETRUSAGE

    void *stack_top = &stack_top;
    s [RES_VMEMORY_STACK_TOP] = (Value) stack_top;

#if 0 && HAVE_SYM_BSS_START
    // FIXME: Disabled for now, symbol too unreliable?
    s [RES_VMEMORY_BSS_BASE] = (Value) __bss_start;
#endif
#if 0 && HAVE_SYM_DATA_START
    // FIXME: Disabled for now, symbol too unreliable?
    s [RES_VMEMORY_DATA_BASE] = (Value) __data_start;
#endif
#if HAVE_SYM_EDATA
    s [RES_VMEMORY_DATA_TOP]  = (Value) _edata;
#endif
#if HAVE_SYM_END
    s [RES_VMEMORY_BREAK_BASE] = (Value) _end;
#endif
#ifndef _WIN32
    s [RES_VMEMORY_BREAK_TOP] = (Value) sbrk (0);
#endif

#ifdef __linux
    if (FILE *info = fopen ("/proc/self/stat", "r"))
    {
	// Linux.  The first field is pid, then process name in
	// parens; the rest of the fields are simple values
	// separated by spaces.  Skip the process name by hand
	// since it may contain spaces (it might also contain
	// parens, so we may still get things wrong).  Hopefully
	// future versions do not change the format...
	struct PROC_STAT_FIELD {
	    Resource	resource;
	    const char	*format;
	};

	static PROC_STAT_FIELD fields [] = {
	    { RES_MAX,		0 /* "%d" */ },	// pid
	    { RES_MAX,		0 /* "%s" */ },	// (process-name)
	    { RES_PROCESS_STATE,	"%c" },	// state (RSDZT)
	    { RES_MAX,		0 /* "%d" */ },	// ppid
	    { RES_MAX,		0 /* "%d" */ },	// pgrp
	    { RES_MAX,		0 /* "%d" */ },	// sid
	    { RES_MAX,		0 /* "%d" */ },	// tty
	    { RES_MAX,		0 /* "%d" */ },	// tpgid
	    { RES_MAX,		0 /* "%u" */ },	// flags
	    { RES_MINOR_PAGE_FAULTS,	"%u" },	//
	    { RES_MAX,		0 /* "%u" */ },	// self+kids minflt
	    { RES_MAJOR_PAGE_FAULTS,	"%u" },	//
	    { RES_MAX,		0 /* "%u" */ },	// self+kids majflt
	    { RES_MAX,		0 /* "%d" */ },	// utime in jiffies
	    { RES_MAX,		0 /* "%d" */ },	// stime in jiffies
	    { RES_MAX,		0 /* "%d" */ },	// self+kids utime
	    { RES_MAX,		0 /* "%d" */ },	// self+kids stime
	    //{ RES_TIME_SLICE_SIZE,	"%d" },	// max jiffies
	    { RES_PRIORITY,		"%d" },	// priority
	    { RES_PRIORITY_NICE,	"%d" },	// nice value
	    { RES_TIMEOUT,		"%d" },	// jiffies
	    { RES_INTERVAL_TIME_LEFT,	"%d" },	// jiffies
	    { RES_START_TIME,		"%d" },	// jiffies from boot
	    { RES_VMEMORY,		"%u" },	// bytes
	    { RES_VMEMORY_RSS,		"%u" },	// pages - 3
	    { RES_VMEMORY_RSS_LIMIT,	"%u" },	// bytes
	    { RES_VMEMORY_TEXT_BASE,	"%u" },	// (boundary)
	    { RES_VMEMORY_TEXT_TOP,	"%u" },	// (boundary)
	    { RES_VMEMORY_STACK_BASE,	"%u" },	//
	    { RES_VMEMORY_KERNEL_SP,	"%u" },	//
	    { RES_VMEMORY_KERNEL_IP,	"%u" },	//
	    { RES_MAX,		0 /* "%d" */ },	// pending signals
	    { RES_MAX,		0 /* "%d" */ },	// blocked signals
	    { RES_MAX,		0 /* "%d" */ },	// ignored signals
	    { RES_MAX,		0 /* "%d" */ },	// caught signals
	    { RES_MAX,		0 /* "%u" */ },	// wait "channel"

	    { RES_SWAPS,		"%u" },	// # swaps
	    { RES_MAX,		0 /* "%u" */ },	// self+kids # swaps
	    { RES_MAX,		0 /* "%d" */ },	// exit_signal
	    { RES_CPU,			"%d" },	// processor
	};
	static const int	N_FIELDS = sizeof (fields)
					   / sizeof (PROC_STAT_FIELD);
	int			field = 0;
	int			ch = 0;
	unsigned		uval;
	int			dval;

	// skip first field
	while (ch != EOF && ch != ' ')
	    ch = fgetc (info);
	++field;

	// skip process name
	if ((ch = fgetc (info)) != '(')
	    ch = EOF;

	while (ch != EOF && ch != ')')
	    ch = fgetc (info);

	if ((ch = fgetc (info)) != ' ')
	    ch = EOF;
	++field;

	// start reading the fields
	for ( ; ch != EOF && ! feof (info) && field < N_FIELDS; ++field)
	{
	    if (fields [field].resource == RES_MAX)
		while ((ch = fgetc (info)) != EOF && ch != ' ')
		    ;
	    else if (fields [field].format[1] == 'u')
	    {
		if (fscanf (info, fields [field].format, &uval) == 1)
		    s [fields [field].resource] = uval;
		ch = fgetc (info);
	    }
	    else if (fields [field].format[1] == 'd')
	    {
		if (fscanf (info, fields [field].format, &dval) == 1)
		    s [fields [field].resource] = (unsigned) dval;
		ch = fgetc (info);
	    }
	    else if (fields [field].format[1] == 'c')
	    {
		s [fields [field].resource] = fgetc (info);
		ch = fgetc (info);
	    }
	}

	fclose (info);

	// postprocess the values to uniform units
	// s [RES_TIME_SLICE_SIZE]	*= 10; // 100 jiffies/sec => msecs
	s [RES_TIMEOUT]			*= 10; // 100 jiffies/sec => msecs
	s [RES_INTERVAL_TIME_LEFT]	*= 10; // 100 jiffies/sec => msecs 
	s [RES_START_TIME]		*= 10; // 100 jiffies/sec => msecs
	s [RES_VMEMORY_RSS]     	 = (s [RES_VMEMORY_RSS] + 3)
					   * getpagesize ();
    }
#endif // __linux

    if (FILE *info = fopen ("/proc/self/status", "r"))	// linux
    {
	char buf [512];
	int  val;
	while (fgets (buf, sizeof (buf), info))
	{
	    if (!strncmp (buf, "VmSize:", 7) && sscanf (buf+7, "%d", &val))
		s [RES_VMEMORY] = val * 1024;
	    else if (!strncmp (buf, "VmLck:", 6) && sscanf (buf+6, "%d", &val))
		s [RES_VMEMORY_LOCKED] = val * 1024;
	    else if (!strncmp (buf, "VmRSS:", 6) && sscanf (buf+6, "%d", &val))
		s [RES_VMEMORY_RSS] = val * 1024;
	    else if (!strncmp (buf, "VmStk:", 6) && sscanf (buf+6, "%d", &val))
		s [RES_VMEMORY_STACK] = val * 1024;
	    else if (!strncmp (buf, "VmExe:", 6) && sscanf (buf+6, "%d", &val))
		s [RES_VMEMORY_EXE] = val * 1024;
	    else if (!strncmp (buf, "VmLib:", 6) && sscanf (buf+6, "%d", &val))
		s [RES_VMEMORY_LIBRARIES] = val * 1024;
	}
	fclose (info);
    }

#ifndef _WIN32 // guard required for getpagesize
    if (FILE *info = fopen ("/proc/self/statm", "r"))	// linux
    {
	// size, resident, share, trs, lrs, drs, dirty
	int junk, share = 0, dirty = 0;
	if (fscanf (info, "%d %d %d %d %d %d %d", &junk, &junk,
		    &share, &junk, &junk, &junk, &dirty) == 7)
	{
	    int pagesize = getpagesize ();
	    s [RES_VMEMORY_SHARED]	= share * pagesize;
	    s [RES_VMEMORY_DIRTY]	= dirty * pagesize;
	}
	fclose (info);
    }
#endif // !_WIN32

#if HAVE_PROCFS_H && defined PCSET && defined PR_MSACCT // (solaris)
    if (FILE *info = fopen ("/proc/self/status", "r"))
    {
	struct pstatus values;
	if (fread (&values, sizeof (values), 1, info) == 1)
	{
	    // pr_sigpend, pr_utime, pr_stime, pr_cutime, pr_cstime
	    // pr_sigtrace, pr_flttrace, pr_sysentry, pr_sysexit,
	    // pr_dmodel?
	      
	    s [RES_PROCESS_FLAGS]	= values.pr_flags;
	    s [RES_PROCESS_LWPS]	= values.pr_nlwp;

	    s [RES_VMEMORY_BREAK_BASE]	= values.pr_brkbase;
	    s [RES_VMEMORY_BREAK_TOP]	= values.pr_brkbase
					  + values.pr_brksize;
	    s [RES_VMEMORY_STACK_BASE]	= values.pr_stkbase;
	    s [RES_VMEMORY_STACK_TOP]	= values.pr_stkbase
					  + values.pr_stksize;
	}
	fclose (info);
    }
    if (FILE *info = fopen ("/proc/self/psinfo", "r"))
    {
	struct psinfo values;
	if (fread (&values, sizeof (values), 1, info) == 1)
	{
	    // pr_ttydev, pr_time (cpu time), pr_ctime (time for reaped children)

	    s [RES_PROCESS_LWPS]	= values.pr_nlwp;
	    s [RES_PROCESS_ADDRESS]	= values.pr_addr;
	    s [RES_PROCESS_STATE]	= values.pr_lwp.pr_sname;
	    s [RES_PRIORITY_NICE]	= values.pr_lwp.pr_nice - NZERO;
	    s [RES_PRIORITY]		= values.pr_lwp.pr_pri;
	    s [RES_CPU]			= values.pr_lwp.pr_onpro;

	    s [RES_VMEMORY]		= values.pr_size * 1024;
	    s [RES_VMEMORY_RSS]		= values.pr_rssize * 1024;
	    s [RES_VMEMORY_PERCENTAGE]	= int(values.pr_pctmem*(10000./0x8000)+.5);
	    s [RES_CPU_PERCENTAGE]	= int(values.pr_pctcpu*(10000./0x8000)+.5);

	    s [RES_START_TIME]		= values.pr_start.tv_sec * 1000
					  + values.pr_start.tv_nsec / 1000000;

	    s [RES_INITIAL_ARGC]	= values.pr_argc;
	    s [RES_INITIAL_ARGV]	= values.pr_argv;
	    s [RES_INITIAL_ENVP]	= values.pr_envp;

	    s [RES_DATA_MODEL]		= values.pr_dmodel;

	    // pr_lwp.pr_wchan
	}
	fclose (info);
    }

    if (FILE *info = fopen ("/proc/self/usage", "r"))
    {
	struct prusage values;
	if (fread (&values, sizeof (values), 1, info) == 1)
	{
	    // pr_create, pr_rtime (total lwp real (elapsed) time)
	    // pr_utime (user level CPU time), pr_stime (system call CPU time)
	    // pr_ttime (other system trap CPU time), pr_tftime (text page fault sleep time)
	    // pr_dftime (data page fault sleep time), pr_kftime (kernel page fault sleep time)
	    // pr_ltime (user lock wait sleep time), pr_slptime (all other sleep time)
	    // pr_wtime (wait-cpu (latency) time), pr_stoptime (stopped time)
	    // pr_minf (minor page faults), pr_majf (major page faults)
	    // pr_nswap (swaps), pr_inblk (input blocks), pr_oublk(output blocks)
	    // pr_msnd (messages sent), pr_mrcv (messages received)
	    // pr_sigs (signals received), pr_vctx (voluntary context switches)
	    // pr_ictx (involuntary context switches), pr_sysc (system calls)
	    // pr_ioch (chars read and written)
	}
	fclose (info);
    }
#endif // Solaris HAVE_PROCFS_H

#if HAVE_SYSSGI
    // syssgi(SGI_PROCSZ, getpid (), &uisz, &uirss)
#endif

    return s;
}

ResourceInfo::Status
ResourceInfo::info_self_cumulative (void)
{
    Status s;

    // FIXME: add childrens information (or set only those
    // that can be measured cumulatively?)

    return s;
}

ResourceInfo::Status &
ResourceInfo::info (Source from, Status &which)
{
    // FIXME: optimise to get only the ones we need to get?
    Status all (info (from));
    for (int i = 0; i < RES_MAX; ++i)
    {
	Resource res = Resource (i);
	if (which.count (res) && all.count (res))
	    which [res] = all [res];
	else
	    which.erase (res);
    }

    return which;
}

} // namespace lat
