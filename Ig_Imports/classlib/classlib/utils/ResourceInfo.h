#ifndef CLASSLIB_RESOURCE_INFO_H
# define CLASSLIB_RESOURCE_INFO_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"
# include <map>

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Access to resource consumption and limits.  */
class ResourceInfo
{
public:
    typedef unsigned long	Value;
    static const Value		LIM_INFINITE = ~0UL; /* FIXME: RLIM_INFINITY? */

    // Resource limit hardness
    enum Kind { SOFT, HARD };

    // Known resource limit types
    enum Limit
    {
	LIMIT_CPU_TIME,
	LIMIT_FILE_SIZE,
	LIMIT_CORE_SIZE,
	LIMIT_N_PROCESSES,
	LIMIT_N_OPEN_FILES,

	LIMIT_VMEMORY,
	LIMIT_VMEMORY_RSS,
	LIMIT_VMEMORY_LOCKED,
	LIMIT_VMEMORY_DATA,
	LIMIT_VMEMORY_STACK,

	LIMIT_MAX = LIMIT_VMEMORY_STACK
    };

    // Known resource types
    enum Resource
    {
	// Resources for usage consumption (rusage)
	RES_USER_TIME,		// seconds
	RES_SYSTEM_TIME,	// seconds
	RES_MAX_RSS,		// kB
	RES_TEXT_RSS,		// kB*s
	RES_DATA_RSS,		// kB*s
	RES_STACK_RSS,		// kB*s
	RES_MINOR_PAGE_FAULTS,
	RES_MAJOR_PAGE_FAULTS,
	RES_SWAPS,
	RES_FILESYS_OPS_IN,
	RES_FILESYS_OPS_OUT,
	RES_IPC_MSGS_SENT,
	RES_IPC_MSGS_RECEIVED,
	RES_SIGNALS,
	RES_VOLUNTARY_CONTEXT_SWITCHES,
	RES_INVOLUNTARY_CONTEXT_SWITCHES,

	// minfault (process / process + children)
	// majfault (process / process + children)
	// utime (process / process + children)
	// stime (process / process + children)
	// swaps (process / process + children)
	// times: user/system = cpu, idle (see TimeInfo)

	RES_VMEMORY,
	RES_VMEMORY_LOCKED,
	RES_VMEMORY_RSS,
	RES_VMEMORY_RSS_LIMIT,
	RES_VMEMORY_DATA,
	RES_VMEMORY_STACK,
	RES_VMEMORY_EXE,
	RES_VMEMORY_LIBRARIES,
	RES_VMEMORY_SHARED,
	RES_VMEMORY_DIRTY,
	RES_VMEMORY_PERCENTAGE,

	RES_VMEMORY_TEXT_BASE,
	RES_VMEMORY_TEXT_TOP,
	RES_VMEMORY_BSS_BASE,
	RES_VMEMORY_BSS_TOP,
	RES_VMEMORY_DATA_BASE,
	RES_VMEMORY_DATA_TOP,
	RES_VMEMORY_BREAK_BASE,
	RES_VMEMORY_BREAK_TOP,
	RES_VMEMORY_STACK_BASE,
	RES_VMEMORY_STACK_TOP,

	RES_VMEMORY_KERNEL_SP,
	RES_VMEMORY_KERNEL_IP,

	// file descriptors? (/proc/self/fd?)
	RES_N_FILE_DESCRIPTORS,
	RES_MAX_FILE_DESCRIPTORS,

	// process state
	RES_PROCESS_STATE,
	RES_PROCESS_FLAGS,
	RES_PROCESS_LWPS,
	RES_PROCESS_ADDRESS,
	RES_TIME_SLICE_SIZE,
	RES_PRIORITY_NICE,
	RES_PRIORITY,
	RES_TIMEOUT,
	RES_INTERVAL_TIME_LEFT,
	RES_START_TIME,
	RES_CPU,
	RES_CPU_PERCENTAGE,

	RES_INITIAL_ARGC,
	RES_INITIAL_ARGV,
	RES_INITIAL_ENVP,

	RES_DATA_MODEL,

	// pid, ppid, pgrp, sid, tty, tpgid?
	// memory area maps? mmaps?
	// root?
	// pending signals?
	// blocked signals?
	// ignored signals?
	// caught signals?
	// wait channel?

	// system resource status? (total vmem, swap, ...)?
	RES_MAX
    };

    enum Source {
	RES_FOR_SYSTEM,
	RES_FOR_SELF,
	RES_FOR_SELF_AND_CHILDREN
    };

    typedef std::map<Resource,Value> Status;

    static Value	getlimit  (Limit type, Kind which);
    static void		getlimits (Limit type, Value &soft, Value &hard);
    static Value	setlimit  (Limit type, Kind which, Value value);
    static void		setlimits (Limit type, Value soft, Value hard);

    static Status	info (Source from);
    static Status &	info (Source from, Status &which);

    static Status	info_system (void);
    static Status	info_self (void);
    static Status	info_self_cumulative (void);

    static const char *	name (Resource type);
    static const char *	unit (Resource type);
    static const char *	format (Resource type, Value value,
				char *buffer, size_t size);

    // FIXME: get vmstats
    //   - linux:   /proc/self/status
    //   - solaris: /proc/self/status (read pstatus.pr_brkbase,
    //               .pr_brksize, .pr_stkbase, .pr_stksize)
    //              or maybe psinfo (psinfo.pr_size, .pr_rssize)
    //   - unix: ps -o vsz= -o rss= -p <PID>

    // FIXME: file descriptor limits: getdtablehi() (maxfd + 1)

    // FIXME: resource usage
    //   - linux: read /proc/self/stat (or status)
    //   - solaris: read /proc/self/usage

    // FIXME: system information
    //   - linux: /proc/cpuinfo etc.
    //   - hpux:  pstat_getstatic(), pstat_getdynamic(), pstat_getvminfo()

    // FIXME: open files
    //   - linux, solaris: /proc/self/fd/N
    //   - hpux: pstat_getfile()

    // FIXME: for hp-ux, look pstat man page and Mike Stroyan's mails.

    // FIXME: for IRIX, look at http://reality.sgi.com/davea/software.html
    // and http://reality.sgi.com/performer/perf-95-09/0153.html
    // and IRIX Device Driver Programmer's Guide at
    //     http://dmawww.epfl.ch/ebt-bin/nph-dweb/dynaweb/SGI_Developer/
    //     DevDriver_PG/@Generic__BookTextView/2687
    // and http://dmawww.epfl.ch:80/ebt-bin/nph-dweb/dynaweb/SGI_Developer/
    //     T_IRIX_Prog/@Generic__BookView?DwebQuery=syssgi
    // Virtual and resident sizes of a process: syssgi(SGI_PROCSZ,
    // pid, &uiSZ, &uiRSS);
    // http://www.mcsr.olemiss.edu/cgi-bin/man-cgi?syssgi+2

    // FIXME: on linux, use /proc/self/statm to get process memory stats
    // FIXME: on solaris, use /proc/self/status, usage, map (or rmap?)

    // FIXME: get resource limits with getrlimit

    // FIXME: time usage: times(2)

private:
    struct Details
    {
	Resource	resource;
	const char	*name;
	const char	*unit;
	const char	*format;
    };

    static const Details	s_resource_details [];
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_RESOURCE_INFO_H
