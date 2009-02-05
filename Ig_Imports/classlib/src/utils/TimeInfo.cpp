//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/utils/TimeInfo.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/sysapi/TimeInfo.h"
#include "classlib/sysapi/Windows.h"

// see also http://high-res-timers.sourceforge.net/; linux kernel
// (arch/*/kernel/time.c; search get_cycles()); netbsd kernel;
// http://pdplab.trc.rwcp.or.jp/pdperf/timer-collection/; papi
// (http://icl.cs.utk.edu/projects/papi); gnu nana.

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

bool					TimeInfo::s_initialised = false;
unsigned				TimeInfo::s_features = 0;
double					TimeInfo::s_ghz = 1.;
TimeInfo::NanoSecs			TimeInfo::s_clockBase = 0;
double					TimeInfo::s_hiResFactor = 1.0;

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Take a high-precision nap.

    The length of the nap is given in @a nanosecs.  Ignores interrupts
    during the sleep if possible.  If possible, will sleep at least the
    requested time, but may also sleep longer.

    Not all systems allow you to sleep at nanosecond resolution, and
    may require additional settings to let you use the full available
    precision.  For example, the process may have to run at real-time
    privilege to get access to high-resolution scheduling.  In
    addition implementations vary in other aspects.  Some systems will
    make the process sleep till the end of the current time-slice,
    giving a sleep resolution more like milliseconds than nanoseconds.
    The length of the process time-slice may also vary for instance as
    a function of real-time privilege status. */
void
TimeInfo::nanosleep (NanoSecs nanosecs)
{
#ifdef _WIN32
    ::SleepEx (DWORD (nanosecs / 1e6), false);
#elif HAVE_NANOSLEEP
    struct timespec duration;
    struct timespec rem;
    duration.tv_sec = int (nanosecs / 1e9);
    duration.tv_nsec = int (nanosecs - duration.tv_sec * 1e9);
    while (::nanosleep (&duration, &rem) == -1 && errno == EINTR)
	duration = rem;
#elif HAVE_USLEEP
    ::usleep (nanosecs / 1e3);
#else
    // NB: Not all selects report how much time is left if the call is
    // interrupted.  So ignore errors and hope for the best...
    struct timeval tv;
    tv.tv_sec = int (nanosecs / 1e9);
    tv.tv_usec = int (nanosecs - tv.tv_sec * 1e9) / 1000;
    ::select (0, 0, 0, 0, &tv);
#endif
}

/** Take a high-precision nap, ignoring interruptions.  Like
    #nanosleep() except time is in microseconds.  */
void
TimeInfo::usleep (NanoSecs usecs)
{ nanosleep (usecs * 1e3); }

/** Take a high-precision nap, ignoring interruptions.  Like
    #nanosleep() except time is in milliseconds.  */
void
TimeInfo::msleep (NanoSecs msecs)
{ nanosleep (msecs * 1e6); }

/** Take a high-precision nap, ignoring interruptions.  Like
    #nanosleep() except time is in seconds.  */
void
TimeInfo::sleep (NanoSecs secs)
{ nanosleep (secs * 1e9); }

/** Calibration loop for measuring cycles per second on architectures
    where a cpu cycle counter is available.

    @a duration says how many nanoseconds to spin; it should be of a
    sizable order to make the measurement reasonaby accurate.  This
    calibration loop should be ran several times---at least twice---to
    get accurate enough resolution.

    As a result, sets @a low and @a high as the bounds for cpu cycles
    per second.  Note that @a low indicates the least number of cycles
    per second and therefore the slowest cpu speed.

    This method gives sensible results only on platforms with genuine
    cycle counters.  Do not call it on platforms that don't have
    one---they will end up using #s_ghz which we are still in the
    process of trying to determine!

    The code below and the calling code in #init() were adapted from
    GNU nana and from PAPI (http://icl.cs.utk.edu/projects/papi).  */
void
TimeInfo::spin (NanoSecs duration, NanoTicks &low, NanoTicks &high)
{
    // time measured around cycle boundaries
    NanoSecs startLow, startHigh;
    NanoSecs endLow, endHigh;

    // actual cycle limits measured
    NanoTicks start, end;

    // the timing spin loop
    startLow	= time ();
    start	= realCycles ();
    startHigh	= time ();

    do
	endLow = time ();
    while (endLow - startLow < duration);

    end		= realCycles ();
    endHigh	= time ();

    // compute the maximal boundaries
    low  = (NanoTicks) ((end - start) / (1e-9 * (endLow - startHigh)));
    high = (NanoTicks) ((end - start) / (1e-9 * (endHigh - startLow)));
}

/** Initialise high-resolution time measurement system.

    Determines the processor speed and/or clock resolution.  You
    should call this method before calling the cycle counter methods
    (direct cycle counters or the nanosecond counterparts), or the
    process time methods.  It is safe to call this more than once; all
    but the first call are ignored.

    This method (and subsequenty the high-resolution timers) will give
    incorrect results on multi-processor systems that have processors
    running at different frequencies.

    Records also the current #time() value such that #elapsedTime()
    can be used to measure monotonic time if the system has no process
    time accounting.  If a precise process start time is available,
    records that as the time base instead of the current #time().

    On systems where other means to determine cpu speed fail, uses a
    calibration spin loop that takes approximately half a second.  */
void
TimeInfo::init (void)
{
    if (s_initialised)
	return;

    // Get process real time base for #elapsedTime() and process
    // idle time calculations.  Note: we want this to be the first
    // thing we check, since the other things below may take time
    // (like the calibration loop).
    //
    // Assume first that no start time available from the system:
    // subtract current #processCpuTime() from current #time()
    // (assumed to use the same epoch!).  This will work reasonably
    // well if you call #init() early in the program: hopefully the
    // process will not have idled for too long during the start-up
    // (though it may have if there are many shared libraries and it
    // takes a long time to load them, for example due to loading them
    // from a networked file system).  Note that #processTimes() may
    // use s_clockBase but that's fine since s_clockBase is
    // currently zero anyway.
    s_clockBase = time () - processCpuTime ();

#ifdef __linux
    bool haveBase = false;
    FILE *status = 0;
    if (! haveBase && (status = fopen ("/proc/self/stat", "r")))
    {
	// Linux.  22nd field is process starttime %d; the first is
	// pid, then process name in parens; the rest of the fields
	// are simple values separated by spaces.  Skip the process
	// name by hand since it may contain spaces (it might also
	// contain parens, so we may still get things wrong).
	// Hopefully future versions do not change the format...
	static const int	START_TIME_FIELD = 22;
	int			field = 1;
	int			ch = 0;
	int			start;

	// skip first field
	while (ch != EOF && ch != ' ')
	    ch = fgetc (status);
	++field;

	// skip process name
	if ((ch = fgetc (status)) != '(')
	    ch = EOF;

	while (ch != EOF && ch != ')')
	    ch = fgetc (status);

	if ((ch = fgetc (status)) != ' ')
	    ch = EOF;
	++field;

	// skip the other fields in between
	for ( ; ch != EOF && field < START_TIME_FIELD; ++field)
	    while ((ch = fgetc (status)) != EOF && ch != ' ')
		;

	// get the start time
	if (ch != EOF && fscanf (status, "%d", &start) == 1)
	{
	    haveBase = true;
	    s_clockBase = start * (1e9 / CLK_TCK);
	}

	// done, ignore the rest
	fclose (status);
    }
#endif // __linux

#if 0 // HAVE_SYS_PSTAT_H			// (hp-ux)
    // Disabled: pst_start counts time from unix epoch which is
    // incompatible with other time accounting methods here.
    pst_status pstatus;
    if (!haveBase && pstat_getproc (&pstatus, sizeof (pstatus),
				    0, getpid()) != -1)
    {
	haveBase = true;
	s_clockBase = pstatus.pst_start * 1e9;
    }
#endif
#if 0 // HAVE_PROCFS_H && defined PCSET && defined PR_MSACCT // (solaris)
    // prusage.pr_create is based on gethrtime().  To enable this
    // code, #time() and #processTimes() need to be adjusted to be
    // gethrtime()-based.  However, using times() to get process times
    // has lower resolution, sometimes resulting in cpu time exceeding
    // real time; hence this code is disabled.
    if (! haveBase && (status = fopen ("/proc/self/usage", "r")))
    {
	struct prusage pstatus;
	if (fread (&pstatus, sizeof (pstatus), 1, status) == 1)
	{
	    haveBase = true;
	    s_clockBase = pstatus.pr_create.tv_sec * 1e9
			  + pstatus.pr_create.tv_nsec;
	}
	fclose (status);
    }
#endif
#if HAVE_MACH_MACH_TIME_H
    struct mach_timebase_info base;
    if (mach_timebase_info (&base) == 0)
	s_hiResFactor = double (base.numer) / double (base.denom);
#endif

    // Compute various feature bits
    s_features |= FEATURE_TIME_EPOCH;
#if HAVE_CPU_REAL_TIME_COUNTER \
    || HAVE_CLOCK_MONOTONIC \
    || HAVE_MACH_MACH_TIME_H \
    || !(HAVE_TIMES || HAVE_GETRUSAGE)
    s_features |= FEATURE_REAL_COUNT_EPOCH;
#endif
#if HAVE_TIMES || HAVE_GETRUSAGE || defined _WIN32
    s_features |= FEATURE_PROCESS_TIMES;
#endif

#ifdef __linux
    // Try to determine CPU speed (linux): look for a "cpu MHz :
    // <float>" line.
    if (FILE *cpu = fopen("/proc/cpuinfo", "r"))
    {
	char buf [512];
	while (fgets (buf, sizeof (buf), cpu))
	    if (!strncmp (buf, "cpu MHz", 7))
		if (char *colon = strchr (buf, ':'))
		{
		    s_features |= FEATURE_EXACT_MHZ;
		    s_ghz = atof (colon + 1) / 1000.;
	    	    s_hiResFactor = 1e9 / s_ghz;
		    break;
		}

	fclose (cpu);
    }
#elif HAVE_SYS_SYSCTL_H && defined CTL_HW && defined HW_CPU_FREQ // (darwin)
    if (! (s_features & FEATURE_EXACT_MHZ))
    {
	int cpuspeed;
	int mib [2] = { CTL_HW, HW_CPU_FREQ };
	size_t len = sizeof (cpuspeed);
	if (sysctl (mib, 2, &cpuspeed, &len, 0, 0) >= 0)
	{
	    s_features |= FEATURE_EXACT_MHZ;
	    s_ghz = cpuspeed * 1e-9;
	    // s_hiResFactor initialised above
	}
    }
#elif HAVE_SYS_SYSCTL_H && defined __FreeBSD__
    if (! (s_features & FEATURE_EXACT_MHZ))
    {
	int cpuspeed;
	size_t len = sizeof (cpuspeed);
	if (sysctlbyname ("machdep.tsc_freq", &cpuspeed, &len, 0, 0) >= 0)
	{
	    s_features |= FEATURE_EXACT_MHZ;
	    s_ghz = cpuspeed * 1e-9;
	    s_hiResFactor = 1e9 / s_ghz;
	}
    }
#elif HAVE_PMAPI_H					// (aix)
    if (! (s_features & FEATURE_EXACT_MHZ))
    {
	pm_info_t info;
	if (! pm_init (PM_VERIFIED|PM_UNVERIFIED|PM_CAVEAT, &info))
	{
	    s_features |= FEATURE_EXACT_MHZ;
	    s_ghz = pm_cycles () * 1e-9;
	    s_hiResFactor = 1e9 / s_ghz;
	}
    }
#elif HAVE_INVENT_H					// (irix)
    if (! (s_features & FEATURE_EXACT_MHZ))
    {
	// pretty eh?  we'll just trust a cpu comes our way :-) but
	// probably that's safe since this code is executing...
	inv_state_t *state = 0;
	setinvent_r (&state);
	do {
	    inventory_t *i = getinvent_r (state);
	    if (i->inv_class == INV_PROCESSOR && i->inv_type == INV_CPUBOARD)
	    {
		s_features |= FEATURE_EXACT_MHZ;
		s_ghz = ((int) i->inv_controller) / 1000.;
	    	s_hiResFactor = 1e9 / s_ghz;
		break;
	    }
	} while (true);
	endinvent_r (state);
    }
#elif defined _WIN32					// (windows)
    if (! (s_features & FEATURE_EXACT_MHZ))
    {
	size_t				sz;
	SYSTEM_INFO			si;
	PROCESSOR_POWER_INFORMATION	*info;
	LARGE_INTEGER			freq;

	// Determine cpu clock speed.  Assume all cpus run at roughly
	// the same clock speed and that we only care about the full
	// clock rate -- not whatever the os might have lowered it to.
	GetSystemInfo (&si);
	sz = si.dwNumberOfProcessors * sizeof (PROCESSOR_POWER_INFORMATION);
	info = new PROCESSOR_POWER_INFORMATION [si.dwNumberOfProcessors];
	if (CallNtPowerInformation (ProcessorInformation, 0, 0, info, sz) == STATUS_SUCCESS)
	{
	    s_features |= FEATURE_EXACT_MHZ;
	    s_ghz = info [0].MaxMhz / 1e3;
	}
	delete [] info;

	// Get cycles/tick multiplier
	QueryPerformanceFrequency (&freq);
	s_hiResFactor = freq.QuadPart / (s_ghz * 1e9);
    }
#elif HAVE_SYS_PROCESSOR_H && HAVE_PROCESSOR_INFO	// (solaris)
    if (! (s_features & FEATURE_EXACT_MHZ))
    {
	processor_info_t pinfo;
	if (! processor_info (0, &pinfo))
	{
	    s_features |= FEATURE_EXACT_MHZ;
	    s_ghz = pinfo.pi_clock / 1000.;
	    s_hiResFactor = 1e9 / s_ghz;
	}
    }
#elif HAVE_SYS_PSTAT_H					// (hp-ux)
    if (! (s_features & FEATURE_EXACT_MHZ))
    {
	pst_processor psp;
	if (pstat_getprocessor (&psp, sizeof(psp), 1, 0) != -1)
	{
	    s_features |= FEATURE_EXACT_MHZ;
	    s_ghz = psp.psp_iticksperclktick * sysconf(_SC_CLK_TCK) * 1e-9;
	    s_hiResFactor = 1e9 / s_ghz;
	}
    }
#endif

#if HAVE_CPU_REAL_CYCLE_COUNTER
    if (! (s_features & FEATURE_EXACT_MHZ))
    {
	// Try to measure the processor speed with a spin loop and the
	// cycle counter.  We need a reasonably long loop to get a
	// well calibrated result; try not to call this where time is
	// a critical resource.  We spin 200 ms twice to measure the
	// boundaries, plus an initial 1 us run to warm up the cache.
	static const int LOOPS = 2;
	NanoTicks	low, high;
	NanoTicks	max = 0, min = ~max;

	spin (1000, low, high);
	for (int i = 0; i < LOOPS; ++i)
	{
	    spin (1e9/5, low, high);
	    if (low < min)
		low = min;
	    if (high > max)
		max = high;
	}

	s_ghz = max * 1e-9;
	s_hiResFactor = 1e9 / s_ghz;
    }
#endif

#if HAVE_PROCFS_H && defined PCSET && defined PR_MSACCT // (solaris)
    // Enable microstate accounting for better gethrvtime precision.
    // FIXME: provide an option to disable this overhead?
    // FIXME: don't do this?  it seems to scare away truss
    int ctlfd = open ("/proc/self/ctl", O_WRONLY);
    if (ctlfd != -1)
    {
	long ctlop [2] = { PCSET, PR_MSACCT };
	write (ctlfd, ctlop, sizeof (ctlop));
	close (ctlfd);
    }
#endif

    // There is no way to get a cpu speed estimate: we can't get real
    // cpu cycle counts to run a calibration loop, nor can we get the
    // info from the operation system.  Just say we are running at one
    // gigahertz as this will translate the system timing information
    // best to cycles.  It won't be true, but hey, you can extend the
    // code above to deal better with your system and at least we are
    // not losing any timing data.  (The setting to a gigahertz
    // already took place in the s_ghz initialiser.)

    s_initialised = true;
}

/** Return the processor speed in megahertz as determined by #init().  */
double
TimeInfo::mhz (void)
{ return s_ghz * 1000.; }

/** Return the processor speed in gigahertz as determined by #init(). */
double
TimeInfo::ghz (void)
{ return s_ghz; }

/** Return the system capabilities as determined by #init(). */
unsigned
TimeInfo::features (void)
{ return s_features; }

/** Return high-resolution, monotonic system time in nanoseconds from
    some system- or process-dependent epoch.  Not dependent on
    #init().  */
TimeInfo::NanoSecs
TimeInfo::time (void)
{
#if HAVE_CLOCK_GETTIME && HAVE_CLOCK_MONOTONIC
    struct timespec now; clock_gettime (CLOCK_MONOTONIC, &now);
    return (now.tv_sec * 1e9) + now.tv_nsec;
#elif HAVE_TIMES
    struct tms now; return times (&now) * (1e9 / CLK_TCK);
#else
    // Do not use process-relative time: we probably can't measure real
    // time anyway (only user + system), and even if we can, we'll end
    // up using each other to calcuate the idle time.  FIXME: WIN32?
    return clock () * (1e9 / CLOCKS_PER_SEC);
#endif
}

/** Return the elapsed monotonic system real time in nanoseconds since
    the first call to #init().

    Not all systems can report process time usage.  On such systems
    use this function to get the most accurate monotonic real time
    distance available.  The time is measured from the time #init()
    was first called.  However, on some systems #init() can determine
    the actual process start time; on such systems elapsed time is
    measured from the real process start, not from the first call to
    #init().

    On systems where the exact process start time can be determined,
    it is common for #elapsedTime() to report a small time usage in
    the beginning of the process---more than the process start-up
    should have taken.  This is a result of the operating system
    beginning to count process time from the `fork()' and the parent
    process doing something before an `exec()' to start the child.  An
    interactive shell may for instance change environment settings and
    redirect file descriptors before actually executing the command,
    and the time spent to do these actions will be counted towards the
    child process.  For any program for which measuring the processing
    time is statistically meaningful the differences should be
    negligible if the program is invoked from a normal shell.  */
TimeInfo::NanoSecs
TimeInfo::elapsedTime (void)
{ return time () - s_clockBase; }

/** Return the resolution of the time returned by <<time>>.

    The returned value is nanoseconds and the best known estimate.  It
    may not match the system capabilities exactly.  Not dependent on
    #init().  */
TimeInfo::NanoSecs
TimeInfo::resolution (void)
{
#if HAVE_CLOCK_GETTIME && HAVE_CLOCK_MONOTONIC
    struct timespec res; clock_getres (CLOCK_MONOTONIC, &res);
    return (res.tv_sec * 1e9) + res.tv_nsec;
#elif HAVE_TIMES
    return 1e9 / CLK_TCK;
#else
    return 1e9 / CLOCKS_PER_SEC;
#endif
}

//////////////////////////////////////////////////////////////////////
/** Return high-resolution monotonic real time in nanoseconds.

    The returned time is based on the highest-resolution time
    available on the system, such as CPU cycle counter or a system
    timer.  If no high-resolution timer support has been detected,
    falls back on the standard @c clock() function which will still
    provide monotonic time but very likely at a rather poor
    resolution.

    On many systems #realNsecs() and #realCycles() are implemented
    in terms of each other; it is not meaningful to find out which one
    is the basis of the other.  Since on some systems they are in fact
    separate (the system may have support for both returning time in
    nanoseconds and reading the cpu cycles counts), you should not
    assume that the two correspond exactly; choose one and stick with
    the choice.  You can still of course convert cycles to nanosecond
    estimates using #ghz().

    Timing based on cpu cycles is unreliable on some multiprocessors
    and in presence of CPU power management.  On most multiprocessor
    systems each processor has its own cycle counter, and there is
    usually no guarantee that the program will execute only on a
    single CPU.  Also if the CPU is goes into a low-power or sleep
    mode, the time may no longer be counted linearly (although it
    should still be monotonic).  You can try using #virtNsecs()
    instead -- it suffers less from these problems; however, it is
    less common to find the same resolution counters for virtual time
    as for real time.

    The return value is monotonically growing time in nanoseconds from
    some system and processor dependent epoch (e.g. from CPU or system
    power-on).  (Monotonicity is subject to running on a single CPU.)  */
TimeInfo::NanoSecs
TimeInfo::realNsecs (void)
{
#if HAVE_GETHRTIME					// (solaris)
    // Note: cycles may return real cpu tick count
    return gethrtime ();
#elif HAVE_CLOCK_GETTIME && HAVE_CLOCK_MONOTONIC	// (posix)
    struct timespec now; clock_gettime (CLOCK_MONOTONIC, &now);
    return (now.tv_sec * 1e9) + now.tv_nsec;
#elif HAVE_CLOCK_GETTIME && HAVE_CLOCK_SGI_CYCLE	// (irix)
    // Note: cycles may return real cpu tick count
    struct timespec now; clock_gettime (CLOCK_SGI_CYCLE, &now);
    return (now.tv_sec * 1e9) + now.tv_nsec;
#elif HAVE_READ_REAL_TIME				// (aix power*)
    // Note: cycles may return real cpu tick count
    // Note: could be merged with the case below, aix always has
    //       accurate processor speed through pmapi.h; but use the
    //       documented method for now
    timebasestruct_t timebase;
    read_real_time (&timebase, TIMEBASE_SZ);
    time_base_to_time (&timebase, TIMEBASE_SZ);
    return (timebase.tb_high * 1e9) + timebase.tb_low;
#elif HAVE_CPU_REAL_CYCLE_COUNTER
    return realCycles () / s_ghz;
#elif HAVE_TIMES || HAVE_GETRUSAGE			// (posix)
    return processRealTime ();
#else							// (iso c standard)
    // #elif MACINTOSH:
    // UnsignedWide micros; Microseconds (&micros); return micros.lo;
    return clock () * (1e9 / CLOCKS_PER_SEC);
#endif
}

/** Return high-resolution monotonic real time in CPU cycle ticks.

    The returned time is based on the CPU cycle counters if such are
    available, otherwise it uses #realNsecs() and the best guess of
    CPU speed to come up with an educated CPU cycle estimate.
    However, it is not meaningful to try to find out which of the
    functions, #realNsecs() or #realCycles(), is the basis of the
    returned tick count.

    Timing based on cpu cycles is unreliable on some multiprocessors
    and in presence of CPU power management.  On most multiprocessor
    systems each processor has its own cycle counter, and there is
    usually no guarantee that the program will execute only on a
    single CPU.  Also if the CPU is goes into a low-power or sleep
    mode, the time may no longer be counted linearly (although it
    should still be monotonic).  You can try using #virtNsecs()
    instead -- it suffers less from these problems; however, it is
    less common to find the same resolution counters for virtual time
    as for real time.

    The return value is monotonically growing count of CPU clock
    cycles from some system and processor dependent epoch (usually CPU
    boot).  (Monotonicity is subject to running on a single CPU.)
    Note that on some CPUs the value may not have a very large range:
    it may be as small as a couple of seconds; this depends largely on
    the CPU speed and the number of significant bits in the cycle
    counter value.

    FIXME: provide means to get the number of significant bits?  */
TimeInfo::NanoTicks
TimeInfo::realCycles (void)
{
#if HAVE_READ_REAL_TIME					// (aix)
    timebasestruct_t timebase;
    read_real_time (&timebase, TIMEBASE_SZ);
    return ((NanoTicks) timebase.tb_high << 32) + timebase.tb_low;
#elif defined CPU_REAL_CYCLES_ASM			// (gcc)
    unsigned low, high; __asm__ volatile (CPU_REAL_CYCLES_ASM);
    return CPU_REAL_CYCLES_VALUE;
#elif HAVE_MACH_MACH_TIME_H				// (darwin)
    return NanoTicks (mach_absolute_time () * s_hiResFactor);
#elif defined _WIN32					// (windows)
    LARGE_INTEGER value;
    QueryPerformanceCounter (&value);
    return NanoTicks (value.QuadPart * s_hiResFactor);
#else
    // #elif HAVE_COMPAQ_ASM_SYNTAX
    //   This should work on a recent compaq tru64/linux c++ compiler
    //   #include <c_asm.h>
    //   return asm("rpcc %v0")
    //
    // #elif HAVE_PMAPI_H
    //   Use the pmapi (aix) calls to get events.  It should be able to
    //   pretty much any information we need.  For more details, go to
    //   http://as400bks.rochester.ibm.com/doc_link/en_US/a_doc_lib/
    //     aixprggd/genprogc/perfmon_api.htm#HDRPERFMON_API
    //   ("Performance Monitor API Programming Concepts" in AIX
    //   documentation library at http://as400bks.rochester.ibm.com/
    //   cgi-bin/ds_form?lang=en_US&viewset=AIX).  For example, it
    //   should provide user/kernel and program/thread specific data:
    //     For instance PM_CYC and PM_INST_CMPL are respectively the
    //     number of processor cycles and instruction completed and
    //     should exist on most processors.
    //
    // #elif IRIX
    //  See http://reality.sgi.com/performer/perf-99-03/0193.html for
    //  code to memory-map real-time-counter from the kernel to user
    //  space
    //   clock_getres(CLOCK_SGI_CYCLE, &res);
    //   clock_getres(CLOCK_SGI_FAST, &res);
    //   CLOCK_REALTIME
    //     The system's notion of the current time is obtained with
    //     this clock.  The time is expressed in seconds and
    //     nanoseconds since midnight (00:00) Coordinated Universal
    //     Time (UTC), January 1, 1970.  The resolution for of this
    //     clock is never worse than 100 HZ (which equals 10
    //     milliseconds).  The actual resolution of the clock depends
    //     on the timer capability of the underlying hardware(see
    //     timers(5)).
    //   CLOCK_SGI_CYCLE
    //     This clock allows easy access to the free running hardware
    //     counter.  This SGI specific clock maps the counter into
    //     the application address space and returns the current
    //     value. Handling of the counter wraps on machines with
    //     small counters is left to the application. To find the
    //     number of bit the counter supports the application may
    //     call syssgi() with the SGI_CYCLECNTR_SIZE request.  This
    //     clock only is usable as a clock_id for the clock_gettime()
    //     and clock_getres() functions. This clock is SGI specific
    //     and is not portable.
    //   CLOCK_SGI_FAST
    //     This clock has a higher resolution than CLOCK_REALTIME and
    //     is available to privileged users only.  This clock only is
    //     usable as a clock_id. for the clock_getres() and
    //     timer_create() functions. This clock is SGI specific and
    //     is not portable.
    //
    // The precision of the cycle counter is different in different
    // system types; for example, it is a 24-bit counter in the Indy
    // workstation, but a 64-bit counter in CHALLENGE and Onyx
    // systems. The rate at which the timer increments is its
    // resolution, and this also varies with the hardware type.
    //
    // The cycle counter is an addressable hardware device that you
    // can map into the address space of your process (see "Mapping
    // Physical Memory"). When this is done you can sample the cycle
    // counter as if it were a program variable. The code to do this
    // mapping is discussed in the syssgi(2) reference page under
    // SGI_QUERY_CYCLECNTR.
    //
    // However, the use of the hardware cycle counter has been
    // integrated into the POSIX timer support beginning in IRIX 6.2,
    // and this makes access to the cycle counter much simpler than
    // before:
    //   - In order to sample the cycle counter, call clock_gettime()
    //     passing CLOCK_SGI_CYCLE.
    //   - In order to find out the resolution (update frequency) of
    //     the cycle counter, call clock_getres() passing CLOCK_SGI_CYCLE.
    //   - In order to find out the precision of the cycle counter,
    //     call syssgi() passing SGI_CYCLECNTR_SIZE. The returned
    //     value is the number of bits in the counter.
    /* syssgi man page:

     SGI_CYCLECNTR_SIZE
                 Returns the size of the integer needed to read the cycle
                 counter.  This is useful along with SGI_QUERY_CYCLECNTR to
                 write machine independent code to read the free running
                 hardware counter.  On some machines the value needs to be
                 read into a 32 bit integer, and on others the cycle counter
                 needs to be read into a 64 bit integer.

     SGI_QUERY_CYCLECNTR
                 It is best to use the clock_gettime(2) interface with the
                 CLOCK_SGI_CYCLE clock instead of mapping the counter
                 yourself.  This is a much more portable solution.  That said,
                 this syssgi option returns information about the free running
                 hardware counter on systems that support a high resolution
                 timer.  Upon successful completion, an address for the
                 counter is returned.  The size of the integer needed to hold
                 the count can be found using the SGI_CYCLECNTR_SIZE syssgi
                 call.

                 On machines with 64-bit counters it can either be read with
                 two separate 32-bit accesses (one at the returned address,
                 and the other at the returned address plus four), or with a
                 single 64-bit "long long" access in MIPS3 or MIPS4
                 applications.

                 The address must be mapped read-only into the user process
                 space via the mmap(2) system call into /dev/mmem before any
                 access can be made.  Not all systems return page aligned
                 addresses, so the address must be masked to be page aligned,
                 and the offset added back to the returned mmap address, in
                 order for code to work on all systems, so typically you would
                 see code like this (ignoring error checking):

            #include <stddef.h>
            #include <sys/types.h>
            #include <fcntl.h>
            #include <sys/mman.h>
            #include <sys/syssgi.h>

            #if CYCLE_COUNTER_IS_64BIT
            typedef unsigned long long iotimer_t;
            #else
            typedef unsigned int iotimer_t;
            #endif
            __psunsigned_t phys_addr, raddr;
            unsigned int cycleval;
            volatile iotimer_t counter_value, *iotimer_addr;
            int fd, poffmask;
            poffmask = getpagesize() - 1;
            phys_addr = syssgi(SGI_QUERY_CYCLECNTR, &cycleval);
            raddr = phys_addr & ~poffmask;
            fd = open("/dev/mmem", O_RDONLY);
            iotimer_addr = (volatile iotimer_t *)mmap(0, poffmask, PROT_READ,
                 MAP_PRIVATE, fd, (off_t)raddr);
            iotimer_addr = (iotimer_t *)((__psunsigned_t)iotimer_addr +
                 (phys_addr & poffmask));
            counter_value = *iotimer_addr;

       For the Challenge/ONYX and OCTANE families 64-bit counter, define
       CYCLE_COUNTER_IS_64BIT.  Sixty-four bit (MIPS 3, MIPS 4) applications
       may simply use the example above with CYCLE_COUNTER_IS_64BIT defined.
       Thirty-two bit applications need to read the halves separately and
       check for wrap-around.  Pass iotimer_addr and two unsigned int pointers
       to the following routine:

            void read_time(volatile unsigned *iotimer_addr,
                           unsigned *high, unsigned *low)
             {
                while (1) {
                    *high = *iotimer_addr;
                    *low = *(iotimer_addr + 1);
                    if (*high == *iotimer_addr) {
                        return;
                    }
                }
             }

       This will put the top and bottom 32 bits of the time into the memory
       pointed to by high and low, respectively.  If you only need 32-bit
       precision, you may use the original example but replace the
       counter_value assignment with

            counter_value = *(iotimer_addr + 1);
    */
    //
    //   volatile unsigned long counter_value, *iotimer_addr;
    //   unsigned int cycleval;
    //   int fd, poffmask = getpagesize() - 1;
    //   __psunsigned_t phys_addr = syssgi( SGI_QUERY_CYCLECNTR, &cycleval );
    //   microseconds_per_click = (double)cycleval/1e6;
    //   nanoseconds_per_click = (double)cycleval/1e3;
    //   __psunsigned_t raddr = phys_addr & ~poffmask;
    //   clk = &dummy;
    //   if( (fd = open( "/dev/mmem", O_RDONLY )) < 0 )
    //   {
    //       perror( "/dev/mmem" );
    //       return;
    //   }
    //   iotimer_addr = (volatile unsigned long *)mmap(0,poffmask,PROT_READ,
    //						       MAP_PRIVATE,fd,
    //						       (off_t)raddr);
    //   iotimer_addr = (unsigned long *)((__psunsigned_t)iotimer_addr
    //					  + (phys_addr & poffmask));
    //   cycleCntrSize = syssgi( SGI_CYCLECNTR_SIZE );
    //   if( cycleCntrSize > 32 ) 
    //      ++iotimer_addr;
    //   clk = (unsigned long *)iotimer_addr;
    //
    // #elif HAVE_SYS_PFCNTR_H                        // (tru64)
    //   /dev/pfm on tru64 (sys/pfcntr.h); see also uprofile man page
    //   http://www.gmm.insa-tlse.fr/cgi-bin/man.cgi?section=7&
    //    topic=pfm&architecture=DEC
    //   note that these counters are much more specific than what we
    //   want, and only one process can have the /dev/pfm open at any
    //   one time.  so maybe not use them at all?
    // 
    //
    // #elif HAVE_MACHINE_INLINE_H && defined _MFCTL  // (hp-ux)
    // (FIXME: HP-UX: aCC doesn't support the _asm in _MFCTL)
    // NanoTicks time; _MFCTL(16,time); return time;
    //
    // #elif HAVE_SYS_PSTAT_H                         // (hp-ux)
    //   // pstat() only returns system clock ticks (1/100s), not cpu ones
    //   pst_status stat; pstat_getproc (&stat, sizeof (stat), 0, getpid());
    //   return stat.pst_cptickstotal;
    return (NanoTicks) (realNsecs () * s_ghz);
#endif
}

/** Return high-resolution monotonic virtual time in nanoseconds: like
    #realNsecs() except counts time only for this process.

    Like with the real-time counterparts, do not mix #virtualNsecs()
    and #virtualCycles() assuming you can directly map one to the
    other.  You can still of course convert cycles to nanosecond
    estimates using #ghz().

    The return value is monotonically growing time in nanoseconds from
    some system dependent epoch (possibly the process start-up).
    Unlike #realNsecs(), monotonicity is guaranteed since time is
    only counted for this process.  Not all systems can provide this
    information and always return zero. */
TimeInfo::NanoSecs
TimeInfo::virtualNsecs (void)
{
#if HAVE_GETHRTIME					// (solaris)
    return gethrvtime ();
#elif HAVE_CPU_VIRTUAL_CYCLE_COUNTER
    return virtualCycles () / s_ghz;
#elif HAVE_CLOCK_GETTIME && HAVE_CLOCK_PROCESS_CPUTIME_ID
    // FIXME: CLOCK_THREAD_CPUTIME_ID?
    struct timespec now; clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &now);
    return (now.tv_sec * 1e9) + now.tv_nsec;
#elif HAVE_CLOCK_GETTIME && HAVE_CLOCK_PROFILE		// (hp-ux)
    struct timespec now; clock_gettime (CLOCK_PROFILE, &now);
    return (now.tv_sec * 1e9) + now.tv_nsec;
#elif HAVE_CLOCK_GETTIME && HAVE_CLOCK_PROF		// (darwin)
    struct timespec now; clock_gettime (CLOCK_PROF, &now);
    return (now.tv_sec * 1e9) + now.tv_nsec;
#else
    return processCpuTime ();
#endif
}

/** Return high-resolution monotonic virtual time in CPU cycle ticks:
    like #realCycles() except counts ticks only for this process.

    Like with the real-time counterparts, do not mix #virtualNsecs()
    and #virtualCycles() assuming you can directly map one to the
    other.  You can still of course convert cycles to nanosecond
    estimates using #ghz().

    The return value is monotonically growing time in CPU cycles from
    some system dependent epoch (possibly the process startup).
    Unlike #realCycles(), monotonicity is guaranteed since time is
    only counted for this process.  Not all systems can provide this
    information and always return zero. */
TimeInfo::NanoTicks
TimeInfo::virtualCycles (void)
{
#if defined CPU_VIRTUAL_CYCLES_ASM			// (gcc on alpha)
    unsigned low, high; NanoTicks time; __asm__ volatile (CPU_VIRTUAL_CYCLES_ASM);
    return CPU_VIRTUAL_CYCLES_VALUE;
#else
    // #elif HAVE_PMAPI_H
    //   Use the pmapi (aix) calls to get events (see real_cycles).

    // #elif HAVE_SYS_PSTAT_H
    //   // pst_cpticks seems always empty so use the clock_gettime above
    //   pst_status stat; pstat_getproc (&stat, sizeof (stat), 0, getpid());
    //   return stat.pst_cpticks;
    return (NanoTicks) (virtualNsecs () * s_ghz);
#endif
}

//////////////////////////////////////////////////////////////////////
/** Get all process time statistics in one go.

    Sets @a user, @a system and @a real to the amount of CPU time
    consumed by the process in user mode, on its behalf in the kernel,
    and in real time, respectively.  The real time is monotonic real
    time, not system wall clock time.  All times are in nanoseconds.

    Not all systems can provide this information.  On such systems @a
    real will equal to #elapsedTime() and user and system times will
    be reported to be zero (resulting all time to be reported to be
    idle).  */
void
TimeInfo::processTimes (NanoSecs &user, NanoSecs &system, NanoSecs &real)
{
#if HAVE_TIMES						// (posix)
    // HP-UX has clock_gettime with CLOCK_VIRTUAL (user time) and
    // CLOCK_PROFILE (user + system time) with nsec accuracy, but to
    // use them we would have to call clock_gettime twice and then
    // times to get real time so don't bother and get them all through
    // times -- AFAIK HP-UX only counts clock ticks anyway.
    //
    // Note that on linux and hp-ux s_clockBase is precise.  Other
    // systems resort to estimated real time based on call to init.
    struct tms now; real = times (&now) * (1e9 / CLK_TCK);
    user = now.tms_utime * (1e9/CLK_TCK);
    system = now.tms_stime * (1e9/CLK_TCK);
    real -= s_clockBase;
#elif HAVE_GETRUSAGE					// (posix fallback)
    // Real time will be based on an estimate from call to init.
    struct rusage u; getrusage (RUSAGE_SELF, &u);
    NanoSecs now = time ();
    user = u.ru_utime.tv_secs * 1e9 + u.ru_utime.tv_usecs * 1e3;
    system = u.ru_stime.tv_secs * 1e9 + u.ru_stime.tv_usecs * 1e3;
    real = now - s_clockBase;
#elif defined _WIN32					// (windows)
    // Real time will be based on an estimate from call to init; the
    // start time reported by GetProcessTimes is based on system wall
    // clock and can't be trusted to be monotonic.
    FILETIME ftdummy, ftkernel, ftuser;
    GetProcessTimes (GetCurrentProcess (), &ftdummy, &ftdummy,
		     &ftkernel, &ftuser);
    NanoSecs now = time ();
    system = (((NanoTicks) ftkernel.dwHighDateTime << 32)
	      + ftkernel.dwLowDateTime) * 100;
    user   = (((NanoTicks) ftuser.dwHighDateTime << 32)
	      + ftuser.dwLowDateTime) * 100;
    real   = now - s_clockBase;
#else
    // Give real time but bail out on saying how much of it is user or
    // system time.  The real time is an estimate from call to init.
    user = system = 0;
    real = time () - s_clockBase;
#endif
}

/** Get user time consumed by the process in nanoseconds.

    Not all systems can provide this information.  On such systems
    #processRealTime() will equal to #elapsedTime() and user and
    system times will be reported to be zero (resulting all time to be
    reported to be idle).  */
TimeInfo::NanoSecs
TimeInfo::processUserTime (void)
{
    NanoSecs user, system, real;
    processTimes (user, system, real);
    return user;
}

/** Get system time consumed by the process in nanoseconds.

    Not all systems can provide this information.  On such systems
    #processRealTime() will equal to #elapsedTime() and user and
    system times will be reported to be zero (resulting all time to be
    reported to be idle).  */
TimeInfo::NanoSecs
TimeInfo::processSystemTime (void)
{
    NanoSecs user, system, real;
    processTimes (user, system, real);
    return system;
}

/** Get process cpu time (user + system) in nanoseconds.

    Not all systems can provide this information.  On such systems
    #processRealTime() will equal to #elapsedTime() and user and
    system times will be reported to be zero (resulting all time to be
    reported to be idle).  */
TimeInfo::NanoSecs
TimeInfo::processCpuTime (void)
{
    NanoSecs user, system, real;
    processTimes (user, system, real);
    return user + system;
}

/** Get process idle time in nanoseconds.

    Not all systems can provide this information.  On such systems
    #processRealTime() will equal to #elapsedTime() and user and
    system times will be reported to be zero (resulting all time to be
    reported to be idle).  */
TimeInfo::NanoSecs
TimeInfo::processIdleTime (void)
{
    NanoSecs user, system, real;
    processTimes (user, system, real);
    return real - (user + system);
}

/** Get process real time in nanoseconds.

    The process real time is the time from the process start up to now
    according to the monotonic system clock.  On some systems this
    information is not available and equals to #elapsedTime() (that
    is, an estimate based on the first call to #init()).  */
TimeInfo::NanoSecs
TimeInfo::processRealTime (void)
{
    NanoSecs user, system, real;
    processTimes (user, system, real);
    return real;
}

} // namespace lat
