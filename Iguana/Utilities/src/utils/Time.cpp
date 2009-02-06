//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/utils/Time.h"
#include "classlib/utils/SystemError.h"
#include "classlib/utils/LongLong.h"
#include "classlib/utils/Log.h"
#include "classlib/sysapi/Time.h"
#include <iostream>
#include <cstring>
#include <cstdio>

// FIXME: msvc++ rtl
//  _daylight
//  _timezone
//  _tzname
//  _ftime

namespace lat {
using namespace TimeConst;

//<<<<<< PRIVATE DEFINES                                                >>>>>>

#ifdef _WIN32
/** Seconds between 1601 and 1970, the Windows time base and UNIX time
    base.  1 January 1601, 00:00:00 to 1 January 1970, 00:00:00 is 369
    years.  To this we add the leap years from 1604 to 1968, excluding
    1700, 1800, 1900.  This makes (1968 - 1600) / 4 - 3 = 89 leap
    days, and a total of 134774 days.  Each day in the period had 24 *
    60 * 60 = 86400 seconds (no leap seconds).  Thus the difference is
    in total 134774 * 86400 = 11644473600 seconds.  For the actual
    bias this needs to be multiplied by 10000000 since Windows time
    uses 100ns resolution, not seconds.  */
# define SECS_1601_TO_1970	((369 * 365 + 89) * ULONG_LONG_CONST(86400))
#endif

//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>

logstream &operator<< (logstream &out, const Time &t)
{ return out << TimeSpan (t).seconds () << '.' << t.nanoformat (); }

// FIXME: Is this desirable?  The default is to just print out the large value.
std::ostream &operator<< (std::ostream &out, const Time &t)
{ return out << TimeSpan (t).seconds () << '.' << t.nanoformat (); }

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>

#if !HAVE_TIMEGM
static time_t timegm (struct tm *t)
{
    // This code is adapted from wine, samba
    time_t t1 = mktime (t);
    struct tm gmt = *gmtime (&t1);
    time_t t2 = mktime (&gmt);
    return t1 + (t1 - t2);
}
#endif

//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Initialise time to nanoseconds since 00:00:00 on January 1, 1970
    in UTC.  The time is local time if @a local is true (the default),
    otherwise in UTC.  Use #utcoffset() later on to change from one
    to the other.  Note that months are counted from zero and days
    from one as in the C standard functions; see the #month(), #day()
    etc methods for details on the ranges of accepted values.  */
Time::Time (int year, int month, int day,
	    int hour, int min, int sec,
	    ValueType nsecs,
	    bool local /* = true */)
{
    tm val;
    memset (&val, sizeof (val), 0);
    val.tm_sec = sec;
    val.tm_min = min;
    val.tm_hour = hour;
    val.tm_mday = day;
    val.tm_mon = month;
    val.tm_year = year > 1900 ? year - 1900 : year;
    val.tm_isdst = -1; // FIXME?

    m_nsecs = build (local, val, nsecs).m_nsecs;
}

#ifdef _WIN32
/** Windows-specific function to convert system time representation
    to a #Time class.  */
Time
Time::from (const FILETIME *systime)
{
    ValueType t = ((ValueType) systime->dwHighDateTime << 32)
		  + (ValueType) systime->dwLowDateTime;

    if (t)
	// Subtract bias (1970--1601 in 100ns), then convert to nanoseconds.
	t = (t - SECS_1601_TO_1970 * (SEC_NSECS/100)) * 100;

    return Time (t);
}
#endif

/** Return the current system time.  */
Time
Time::current (void)
{
#ifdef _WIN32
    FILETIME ftime;
    GetSystemTimeAsFileTime (&ftime);
    return from (&ftime);
#elif HAVE_CLOCK_GETTIME && HAVE_CLOCK_REALTIME
    struct timespec now;
    if (clock_gettime (CLOCK_REALTIME, &now) < 0)
	throw SystemError (errno);
    return Time (now.tv_sec, now.tv_nsec);
#else
    timeval tv;
    if (gettimeofday (&tv, 0) != 0)
	throw SystemError (errno);
    return Time (tv.tv_sec, tv.tv_usec * 1000);
#endif
}

/** Construct a time from local time @a base and a delta @a diff. */
Time
Time::build (bool local, const tm &base, TimeSpan diff /* = 0 */)
{ tm tmp (base); return Time (local ? mktime(&tmp) : timegm(&tmp), 0) + diff; }

/** Break up the time to the standard representation, either in UTC
    (if @a local is @c false) or local time (if @a local is @c true).
    If @a nsecpart is non-null, it is set to the nanosecond part that
    cannot be stored into @c tm.  */
tm
Time::split (bool local, long *nsecpart /* = 0 */) const
{
    if (nsecpart)
	*nsecpart = m_nsecs % SEC_NSECS;

    time_t val = m_nsecs / SEC_NSECS;
    return *(local ? localtime (&val) : gmtime (&val));
}

/** Break up the time to the standard library representation, keeping
    it in UTC.  If @a nsecpart is non-null, it is set to the
    nanosecond part that cannot be stored into @c tm.  */
tm
Time::utc (long *nsecpart /* = 0 */) const
{ return split (false, nsecpart); }

/** Break up the time to the standard library representation,
    converting it first to local time.  If @a nsecpart is non-null, it
    is set to the nanosecond part that cannot be stored into @c tm.  */
tm
Time::local (long *nsecpart /* = 0 */) const
{ return split (true, nsecpart); }

/** Get the year.  */
int
Time::year (bool local) const
{ return split (local).tm_year + 1900; }

/** Get the month, numbered [0,11].  */
int
Time::month (bool local) const
{ return split (local).tm_mon; }

/** Get the day of month, numbered [1,31].  */
int
Time::day (bool local) const
{ return split (local).tm_mday; }

/** Get the hour, numbered [0, 23].  */
int
Time::hour (bool local) const
{ return split (local).tm_hour; }

/** Get the minute, numbered [0, 59].  */
int
Time::minute (bool local) const
{ return split (local).tm_min; }

/** Get the seconds, numbered [0,61] (allowing one or two leap
    seconds, years with leap seconds can have the time Dec 31,
    23:59:60 (or :61).)  */
int
Time::second (bool local) const
{ return split (local).tm_sec; }

/** Get the nanoseconds.  There is no @c local argument since
    time zone and daylight savings never affects the value at
    the subsecond granularity. */
long
Time::nsecond (void) const
{ return m_nsecs % SEC_NSECS; }

/** Get the day of week, numbered [0,6] and starting from Sunday.  */
int
Time::weekday (bool local) const
{ return split (local).tm_wday; }

/** Check whether daylight savings is in effect.  This really only
    makes sense if @a local is true since daylight savings is never
    in effect for UTC time.  */
bool
Time::isdst (bool local) const
{ return split (local).tm_isdst > 0; }

/** Return the number of nanoseconds that needs to be added to UTC to
    translate this time to the local time (= nanoseconds east of
    UTC).  This accounts for the time zone and daylight savings
    settings of the local time as of the current value.  If
    @a daylight is non-null, it is set to indicate daylight savings
    status (that is, tm.tm_isdst for the effective local time).  */
Time::ValueType
Time::utcoffset (int *daylight /* = 0 */) const
{
    ValueType n = 0;

#if HAVE_TM_GMTOFF
    tm localtm = local ();
    n = localtm.tm_gmtoff;
    if (daylight) *daylight = localtm.tm_isdst;
#else
    // Adapted from WINE.
    time_t	utctime = m_nsecs / SEC_NSECS;
    tm		localtm = *localtime (&utctime);
    int		savedaylight = localtm.tm_isdst;
    tm		gmt = *gmtime (&utctime);

    gmt.tm_isdst = savedaylight;
    n = utctime - mktime (&gmt);

    if (daylight) *daylight = savedaylight;
#endif
    return n * SEC_NSECS;
}

/** Return the local timezone name that applies at this time value.
    On some platforms returns the most recent timezone name (dst or
    non-dst one depending on the time value), not the one that applies
    at the time value.  */
const char *
Time::timezone (int *daylight /* = 0 */) const
{
    tm localtm = local ();
    if (daylight) *daylight = localtm.tm_isdst;
#if HAVE_TM_ZONE
    return localtm.tm_zone;
#else
    // extern "C" { extern char *tzname [2]; }
    return tzname [localtm.tm_isdst > 0 ? 1 : 0];
#endif
}

/** Format the time using @c strftime.  */
std::string
Time::format (bool local, const std::string &spec) const
{
    // FIXME: This doesn't account for nsecs part!
    std::string	result;
    tm		time = split (local);
    int		length = 0;

    do
    {
	// Guess how much we'll expand.  If we go wrong, we'll expand again.
	result.resize (result.size() ? result.size()*2 : spec.size()*2, 0);
	length = ::strftime (&result[0], result.size(), spec.c_str(), &time);
    } while (! length);

    result.resize (length);
    return result;
}

/** Format the nanosecond fractional part of the time as a string.
    The arguments control the representation of the resulting value.
    The nanosecond part is printed as fixed nine-character-wide number
    and then excess zeroes are stripped off at the right end.  Use @a
    minwidth to force a specific number number of them to be left
    intact: the resulting number will have at least that many digits.
    Use @a maxwidth to truncate the value: the resulting number will
    have at most that many digits.  Both @a minwidth and @a maxwidth
    must be between one and nine inclusive and @a minwidth must be
    less or equal to @a maxwidth.  */
std::string
Time::nanoformat (int minwidth /* = 1 */, int maxwidth /* = 9 */) const
{
    ASSERT (minwidth >= 1);
    ASSERT (minwidth <= maxwidth);
    ASSERT (maxwidth <= 9);

    // Calculate the nanosecond fraction.  This will be < 1000000000.
    long value = m_nsecs % SEC_NSECS;
    ASSERT (value >= 0);
    ASSERT (value < SEC_NSECS);

    // Calculate modulus by which we truncate value.  If maxwidth is
    // say 3, we want to mask of the last 6 digits.
    long modulus = 1;
    for (int i = 0; i < 9 - maxwidth; ++i)
	modulus *= 10;

    // Round value by the desired modulus.
    long rem = value % modulus;
    value -= rem;
    if (rem > modulus / 2)
	value += modulus;

    // Format it, then strip off digits from the right as long as
    // we zeroes.  The above guarantees enough zeroes on right to
    // satisfy maxwidth so we need to concern ourselves only about
    // minwidth.
    char buf [10];
    char *p = buf + 8;
    sprintf (buf, "%09ld", value);
    while (p > buf + minwidth - 1 && *p == '0')
	*p-- = '\0';

    return buf;
}

//////////////////////////////////////////////////////////////////////
/** Convert the #Time @a t into a MS-DOS date format.  */
unsigned
Time::toDosDate (Time time)
{
    // Use local time since DOS does too.
    struct tm localtm = time.local ();

    unsigned mday = localtm.tm_mday;
    unsigned mon  = localtm.tm_mon + 1;
    unsigned year = (localtm.tm_year > 80 ? localtm.tm_year - 80 : 0);
    unsigned sec  = localtm.tm_sec / 2;
    unsigned min  = localtm.tm_min;
    unsigned hour = localtm.tm_hour;
    return (mday << 16 | mon << 21 | year << 25
	    | sec      | min << 5  | hour << 11);
}

/** Convert the MS-DOS date @a dosDate into a #Time.  */
Time
Time::fromDosDate (unsigned dosDate)
{
    // DOS times are generally local; treat it as UTC.  This avoids
    // any round-trip conversion and leaves only a presentation as an
    // issue.  Since not much can be known about the origin of the DOS
    // times, it's generally best to present them as such (= in UTC).
    struct tm localtm;
    memset (&localtm, 0, sizeof (localtm));
    localtm.tm_mday  = (dosDate >> 16) & 0x1f;
    localtm.tm_mon   = ((dosDate >> 21) & 0xf) - 1;
    localtm.tm_year  = ((dosDate >> 25) & 0x7f) + 80;
    localtm.tm_hour  = (dosDate >> 11) & 0x1f;
    localtm.tm_min   = (dosDate >> 5) & 0x3f;
    localtm.tm_sec   = (dosDate & 0x1f) * 2;
    localtm.tm_isdst = -1;

    return Time (mktime (&localtm), 0);
}

} // namespace lat
