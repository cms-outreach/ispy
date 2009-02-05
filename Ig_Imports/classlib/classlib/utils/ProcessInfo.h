#ifndef CLASSLIB_PROCESS_INFO_H
# define CLASSLIB_PROCESS_INFO_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"
# include <sys/types.h>
# include <string>

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Describe the current process.  */
class ProcessInfo
{
public:
    static pid_t	pid (void);
    static pid_t	ppid (void);

    static int		argc (void);
    static char **	argv (void);

    static std::string	fullProgramName (void);
    static std::string	shortProgramName (void);

    // FIXME: get vmstats
    //   - linux:   /proc/self/status
    //   - solaris: /proc/self/status (read pstatus.pr_brkbase,
    //               .pr_brksize, .pr_stkbase, .pr_stksize)
    //              or maybe psinfo (psinfo.pr_size, .pr_rssize)
    //   - unix: ps -o vsz= -o rss= -p <PID>

    // FIXME: get vmmap
    //   - linux:   /proc/self/maps
    //   - solaris: /proc/self/map (prmap.pr_vaddr, .pr_size,
    //                .pr_mapname => /proc/self/object/$N)
    //   - hpux:    pstat_getprocvm()

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
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_PROCESS_INFO_H
