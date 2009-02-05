#ifndef CLASSLIB_TIME_H
# define CLASSLIB_TIME_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/utils/TimeConst.h"
# include "classlib/utils/DebugAids.h"
# include "classlib/utils/IntBits.h"
# include <string>
# include <iosfwd>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

struct tm;
# ifdef _WIN32
typedef struct _FILETIME FILETIME;
# endif

namespace lat {
class Time;
class TimeSpan;
class logstream;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>

logstream& operator<< (logstream &out, const Time &time);
std::ostream& operator<< (std::ostream &out, const Time &time);

//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** A difference between two #Time values.  In addition to supporting
    normal integer artihmetic and comparisons, the span can also be
    converted to a number useful units.

    @sa #Time.  */
class TimeSpan
{
    friend class Time;
public:
    typedef TimeConst::TimeValue ValueType;

    TimeSpan (void);
    TimeSpan (Time t);
    TimeSpan (ValueType nsecs);
    TimeSpan (ValueType secs, ValueType nsecs);
    TimeSpan (long days, int hours, int mins, int secs, ValueType nsecs);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    long		days (void) const;
    long		hours (void) const;
    long		minutes (void) const;
    long		seconds (void) const;

    long		lastHours (void) const;
    long		lastMinutes (void) const;
    long		lastSeconds (void) const;
    long		lastNSeconds (void) const;

    TimeSpan &		operator+= (TimeSpan x);
    TimeSpan &		operator-= (TimeSpan x);
    TimeSpan &		operator*= (TimeSpan n);
    TimeSpan &		operator/= (TimeSpan n);
    TimeSpan &		operator%= (TimeSpan n);

    ValueType		ns (void) const;

private:
    ValueType		m_nsecs;	//< The span length.
};

/** Calendar time in nanoseconds since 00:00:00 on January 1, 1970,
    Coordinated Universal Time (UTC).

    #Time is represented internally as UTC time, but it can also be
    converted to the local time as necessary.  Most methods take an
    argument flag @c local to indicate which time interpretation is
    desired by the client, and automatically perform the necessary
    adjustments.  The client can also find out about the difference
    between UTC time and local time using the #utcoffset() method,
    and the time zone name with #timezone() method.  Both allow the
    client to discover whether daylight savings is in effect.

    The native representation of #Time is not well suited for human
    handling of time.  #Time provides access in more convenient terms
    such as #year(), #month() and #day(); more are available through
    conversion into a #TimeSpan.  #Time can also be converted to and
    from ISO C standard @c tm structure.  Note however that unlike C's
    @c mktime() which always assumes @c tm in local time, #Time fully
    supports all conversions between local and universal time.  Thus
    it is possible for example to #build() a UTC time directly from a
    @c tm.

    #Time behaves as an integral type.  Differences in time values are
    represented as a #TimeSpan.  Usual integral arithmetic works with
    both types.  Output works in general as any other integral type,
    however since the #ValueType can be a wide type, it may be poorly
    supported by the @c iostream; if so, including the @c LongLong.h
    header will help.  Note that the output value will usually be very
    large as #Time is represented in nanoseconds, not seconds!  When
    constructing #Time values in seconds, such as when reading in, do
    remember to use the two-argument constructor taking seconds and
    nanoseconds instead of the default single-argument one.

    #Time can be formatted into a string using the #format() method,
    which uses the versatile @c strftime() function.  Since the latter
    works on seconds at best (through a struct @c tm), the subsecond
    part cannot be formatted; the #nanoformat() method is provided to
    overcome this limitation.  To combine #format() and #nanoformat()
    output use a suitable #StringFormat pattern.

    #Time is linked to the system's concept of calendar time and is
    therefore may not be linear nor monotonic.  System time can jump
    arbitrarily in either direction as real time clock is corrected or
    the system is suspended.  The local time may also jump due to
    daylight savings.  The process' ability to sample system time can
    be limited for reasons such as getting swapped out.  #TimeInfo
    provides an alternative time measurement facility not linked to
    calendar and guaranteed to grow monotonically -- though not always
    linearly.  Note that few systems actually provide wall-clock time
    in nanosecond resolution.  Not all system provide an interface to
    get time at that resolution, let alone track it so precisely.

    Because of the time warp issues, scheduling events using #Time is
    not straightforward.  Application code should understand whether
    it is dealing with concrete or abstract calendar calculations, and
    how the events it schedules are linked to wall clock time.

    For calculations on concrete calendar as perceived by people use
    #local() after plain #Time and #TimeSpan integer arithmetic.  The
    method accounts for timezone and daylight savings definitions.  To
    schedule events use #build() to derive times from #local() time to
    get values comparable to the system time returned by #current().
    The applications should know whether events are scheduled in UTC
    or local time---"meeting at 9:00 on Wednesday morning" when the
    device switches timezones may be known to be at 9:00 in the new
    timezone (= locked to local time), or in the timezone where the
    event was created (= locked to UTC).  The #build() and #split()
    methods allow either format to be used, the application just needs
    to know which one to use.  It is also easy to convert between the
    two using #utcoffset().

    For calculations using an abstract calendar, without timezone or
    daylight savings, use #Time in its native UTC representation and
    integer arithmetic with #Time and #TimeSpan.  Do note however that
    "T + 24 hours" may not be the same hour the next day in the local
    calendar time -- timezone changes and daylight savings make a
    difference.  This may require the application to accept as user
    input exception rules to its usual calendar calculations.

    To schedule events, one should choose between three choices: UTC
    time, local time, or delta time.  For the first two cases system
    time should be polled regularly to see if any of the recorded
    events have expired.  It is not a good idea to sleep until the
    next scheduled event, as the system time may jump during the nap;
    instead sleep small increments, recheck the current time after
    each nap and trigger the events that have expired.  A policy must
    be applied when the system time warps; this can happen both
    forwards and backwards with both local and UTC time (daylight
    savings or timezone changes for mobile devices are common local
    time change reasons, but the system time can be updated for any
    reason, e.g. when the real time clock is wrong, or if the system
    is suspended for a long time).  Some events should be executed
    only once in case of time warps backwards.  If the time jumps
    forwards, several events may need to be dealt with in one go.  In
    either case the application should guard against major time
    changes: long system suspends, moving mobile devices and major
    time updates may result in a large number of "missed" events.  One
    possibility is to provide a user-configurable "excessive time
    drift limit" (e.g. N hours): if time changes by more than that,
    missed events are not triggered.

    For the final case of using delta times, sort upcoming events by
    their deltas from the previous event---not by the time they are
    anticipated to occur.  Capture current time before and after the
    sleep and pull events off the queue based on the difference (the
    sleep time may exceed the requested time).  Either guard against
    long time warps like suspends or schedule timer events cautiously.
    Using #TimeInfo as schedule base solves such issues simply.  To
    cope with backward system time jumps when using #Time as schedule
    base, assume that sleeps always last at least the requested time;
    if the time delta over the nap is less than the requested, assume
    time warp (this is not foolproof against interrupted system calls
    but works for many event scheduling situations).

    @sa #TimeInfo for monotonic time not related to the calendar.  */
class Time
{
    friend class TimeSpan;
public:
    typedef TimeConst::TimeValue ValueType;

    Time (void);
    Time (TimeSpan ts);
    Time (ValueType nsecs);
    Time (ValueType secs, ValueType nsecs);
    Time (int year, int month, int day, int hour, int min, int sec,
	  ValueType nsecs, bool local = true);
    // implicit copy constructor
    // implicit assingment operator
    // implicit destructor

    static Time		epoch   (void);
    static Time		current (void);
# ifdef _WIN32
    static Time		from    (const FILETIME *systime);
# endif
    static Time		build   (bool local, const tm &base, TimeSpan diff = 0);

    tm			split   (bool local, long *nsecpart = 0) const;
    tm			utc     (long *nsecpart = 0) const;
    tm			local   (long *nsecpart = 0) const;

    int			year    (bool local) const;
    int			month   (bool local) const;
    int			day     (bool local) const;
    int			hour    (bool local) const;
    int			minute  (bool local) const;
    int			second  (bool local) const;
    long		nsecond (void) const;
    int			weekday (bool local) const;
    bool		isdst   (bool local) const;

    ValueType		utcoffset (int *daylight = 0) const;
    const char *	timezone (int *daylight = 0) const;

    Time &		operator+= (TimeSpan x);
    Time &		operator-= (TimeSpan x);

    ValueType		ns (void) const;

    std::string		format (bool local, const std::string &spec) const;
    std::string		nanoformat (int minwidth = 1, int maxwidth = 9) const;

    static bool		isLeap (int year);

    // Conversion helpers
    static unsigned	toDosDate (Time time);
    static Time		fromDosDate (unsigned dosDate);

private:
    ValueType		m_nsecs;	//< Time value as nsecs from #epoch().
};

//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

/** Initialize an empty (zero) time value.  */
inline
Time::Time (void)
    : m_nsecs (0)
{}

/** Initialize time to @a nsecs nanoseconds since 00:00:00 on January
    1, 1970 in UTC.  */
inline
Time::Time (ValueType nsecs)
    : m_nsecs (nsecs)
{ ASSERT (m_nsecs >= 0); }

/** Initialize time to @a ts nanoseconds since 00:00:00 on January 1,
    1970 in UTC.  */
inline
Time::Time (TimeSpan ts)
    : m_nsecs (ts.m_nsecs)
{ ASSERT (m_nsecs >= 0); }

/** Initialize time to @a secs (seconds) and @a nsecs (nanoseconds)
    summed since 00:00:00 on January 1, 1970 in UTC.  */
inline
Time::Time (ValueType secs, ValueType nsecs)
    : m_nsecs (secs * TimeConst::SEC_NSECS + nsecs)
{ ASSERT (m_nsecs >= 0); }

/** Return the time as nanoseconds since 00:00:00 on January 1, 1970
    in UTC.  */
inline Time::ValueType
Time::ns (void) const
{ return m_nsecs; }

/** Add the specified amount to the time.  Note that #Time is always
    expressed in UTC.  */
inline Time &
Time::operator+= (TimeSpan x)
{ ASSERT (x.m_nsecs >= 0 || m_nsecs >= -x.m_nsecs);
  m_nsecs += x.m_nsecs; return *this; }

/** Subtract the specified amount from the time.  Note that #Time is
    always expressed in UTC.  */
inline Time &
Time::operator-= (TimeSpan x)
{ ASSERT (x.m_nsecs < 0 || m_nsecs >= -x.m_nsecs);
  m_nsecs -= x.m_nsecs; return *this; }

/** Return the time for the epoch (= zero time).  */
inline Time
Time::epoch (void)
{ return Time (0); }

/** Check if the @a year is a leap-year.  */
inline bool
Time::isLeap (int year)
{
    return ((year % 4) == 0
	    && ((year % 100) != 0
		|| (year % 400) == 0));
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Initialize an empty (zero) time difference.  */
inline
TimeSpan::TimeSpan (void)
    : m_nsecs (0)
{}

/** Initialize a time span from #Time @a t. */
inline
TimeSpan::TimeSpan (Time t)
    : m_nsecs (t.m_nsecs)
{}

/** Initialize a time span to a specific length. */
inline
TimeSpan::TimeSpan (ValueType nsecs)
    : m_nsecs (nsecs)
{}

/** Initialise a time span to a specific length.  The value is
    initialised to the sum of the parts---the parts do not need to
    fall into their "natural" ranges.  The values are normalised to
    the natural meanings (e.g. 1000 seconds - 500 nanoseconds), so be
    careful with signs if you are producing values from other sources.

    @param secs		Seconds.
    @param nsecs	Nanoseconds.  */
inline
TimeSpan::TimeSpan (ValueType secs, ValueType nsecs)
{ m_nsecs = secs * TimeConst::SEC_NSECS + nsecs; }

/** Initialise a time span to a specific length.  The value is
    initialised to the sum of the parts---the parts do not need to
    fall into their "natural" ranges.  The values are normalised to
    the natural meanings (e.g. 1000 seconds - 500 nanoseconds), so be
    careful with signs if you are producing values from other sources.

    @param days		Whole days.
    @param hours	Whole hours.
    @param mins		Whole minutes.
    @param secs		Whole seconds.
    @param nsecs	Nanoseconds.  */
inline
TimeSpan::TimeSpan (long days, int hours, int mins, int secs, ValueType nsecs)
{ m_nsecs = (secs + 60 * (mins + 60 * (hours + 24*days)))*TimeConst::SEC_NSECS + nsecs; }

/** Get the number of complete days in the span.  */
inline long
TimeSpan::days (void) const
{ return long(m_nsecs / TimeConst::SEC_NSECS / TimeConst::SECS_PER_DAY); }

/** Get the number of complete hours in the span.  */
inline long
TimeSpan::hours (void) const
{ return long(m_nsecs / TimeConst::SEC_NSECS / TimeConst::SECS_PER_HOUR); }

/** Get the number of complete minutes in the span.  */
inline long
TimeSpan::minutes (void) const
{ return long(m_nsecs / TimeConst::SEC_NSECS / 60); }

/** Get the number of complete seconds in the span.  */
inline long
TimeSpan::seconds (void) const
{ return long(m_nsecs / TimeConst::SEC_NSECS); }

/** Return the time span as nanoseconds.  */
inline TimeSpan::ValueType
TimeSpan::ns (void) const
{ return m_nsecs; }

/** Get the number of complete hours in the last incomplete day of the
    span.  */
inline long
TimeSpan::lastHours (void) const
{ return hours () - days () * 24; }

/** Get the number of complete minutes in the last incomplete hour of
    the span.  */
inline long
TimeSpan::lastMinutes (void) const
{ return minutes () - hours () * 60; }

/** Get the number of complete seconds in the last incomplete minute
    of the span. */
inline long
TimeSpan::lastSeconds (void) const
{ return seconds () - minutes () * 60; }

/** Get the number of nanoseconds in the last incomplete second
    of the span. */
inline long
TimeSpan::lastNSeconds (void) const
{ return long(m_nsecs % TimeConst::SEC_NSECS); }

/** Add to a time span.  */
inline TimeSpan &
TimeSpan::operator+= (TimeSpan x)
{ m_nsecs += x.m_nsecs; return *this; }

/** Subtract from a time span.  */
inline TimeSpan &
TimeSpan::operator-= (TimeSpan x)
{ m_nsecs -= x.m_nsecs; return *this; }

/** Multiply a time span.  */
inline TimeSpan &
TimeSpan::operator*= (TimeSpan x)
{ m_nsecs *= x.m_nsecs; return *this; }

/** Divide a time span.  */
inline TimeSpan &
TimeSpan::operator/= (TimeSpan x)
{ m_nsecs /= x.m_nsecs; return *this; }

/** Compute a modulo of a time span.  */
inline TimeSpan &
TimeSpan::operator%= (TimeSpan x)
{ m_nsecs %= x.m_nsecs; return *this; }

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>

inline Time
operator+ (Time t, TimeSpan ts)
{ return Time (t.ns () + ts.ns ()); }

inline Time
operator+ (TimeSpan ts, Time t)
{ return Time (t.ns () + ts.ns ()); }

inline TimeSpan
operator- (Time t1, Time t2)
{ return TimeSpan (t1.ns () - t2.ns ()); }

inline Time
operator- (Time t, TimeSpan ts)
{ return Time (t.ns () - ts.ns ()); }

inline TimeSpan
operator* (Time t, TimeSpan ts)
{ return TimeSpan (t.ns () * ts.ns ()); }

inline TimeSpan
operator/ (Time t, TimeSpan ts)
{ return TimeSpan (t.ns () / ts.ns ()); }

inline TimeSpan
operator% (Time t, TimeSpan ts)
{ return TimeSpan (t.ns () % ts.ns ()); }

inline bool
operator== (Time t1, Time t2)
{ return t1.ns () == t2.ns (); }

inline bool
operator!= (Time t1, Time t2)
{ return t1.ns () != t2.ns (); }

inline bool
operator< (Time t1, Time t2)
{ return t1.ns () < t2.ns (); }

inline bool
operator<= (Time t1, Time t2)
{ return t1.ns () <= t2.ns (); }

inline bool
operator> (Time t1, Time t2)
{ return t1.ns () > t2.ns (); }

inline bool
operator>= (Time t1, Time t2)
{ return t1.ns () >= t2.ns (); }

inline bool
operator! (Time t)
{ return ! t.ns (); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

inline TimeSpan
operator+ (TimeSpan ts)
{ return ts; }

inline TimeSpan
operator- (TimeSpan ts)
{ return TimeSpan (-ts.ns ()); }

inline TimeSpan
operator+ (TimeSpan ts1, TimeSpan ts2)
{ return TimeSpan (ts1.ns () + ts2.ns ()); }

inline TimeSpan
operator- (TimeSpan ts1, TimeSpan ts2)
{ return TimeSpan (ts1.ns () - ts2.ns ()); }

inline TimeSpan
operator* (TimeSpan ts1, TimeSpan ts2)
{ return TimeSpan (ts1.ns () * ts2.ns ()); }

inline TimeSpan
operator/ (TimeSpan ts1, TimeSpan ts2)
{ return TimeSpan (ts1.ns () / ts2.ns ()); }

inline TimeSpan
operator% (TimeSpan ts1, TimeSpan ts2)
{ return TimeSpan (ts1.ns () % ts2.ns ()); }

inline bool
operator== (TimeSpan t1, TimeSpan t2)
{ return t1.ns () == t2.ns (); }

inline bool
operator!= (TimeSpan t1, TimeSpan t2)
{ return t1.ns () != t2.ns (); }

inline bool
operator< (TimeSpan t1, TimeSpan t2)
{ return t1.ns () < t2.ns (); }

inline bool
operator<= (TimeSpan t1, TimeSpan t2)
{ return t1.ns () <= t2.ns (); }

inline bool
operator> (TimeSpan t1, TimeSpan t2)
{ return t1.ns () > t2.ns (); }

inline bool
operator>= (TimeSpan t1, TimeSpan t2)
{ return t1.ns () >= t2.ns (); }

inline bool
operator! (TimeSpan ts)
{ return ! ts.ns (); }

} // namespace lat
#endif // CLASSLIB_TIME_H
