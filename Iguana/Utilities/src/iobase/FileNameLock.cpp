//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/FileNameLock.h"
#include "classlib/iobase/FileSystemError.h"
#include "classlib/utils/SystemError.h"
#include "classlib/utils/StringFormat.h"
#include "classlib/utils/UserInfo.h"
#include "classlib/utils/HostInfo.h"
#include "classlib/utils/ProcessInfo.h"
#include "classlib/utils/Signal.h"
#include "classlib/utils/Unwind.h"
#include "classlib/sysapi/Filename.h"
#include "classlib/sysapi/FileNameLock.h"
#include <cstdlib>
#include <cctype>

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>

// FIXME: WIN32: SYSTEM_TIME_OF_DAY_INFORMATION stodi;
//   NtQuerySystemInformation (SystemTimeOfDayInformation,
//   &stodi, sizeof (stodi), 0) => stodi.BootTime
#ifdef BOOT_TIME
static Time
getBootTime (const char *file, bool newest)
{
    // Make sure the file exists (IRIX doesn't like nonexistent files)
    if (file)
    {
	int fd = open (file, O_RDONLY);
	if (fd < 0)
	    return 0;
	close (fd);

	utmpname (file);
    }

    // Find the next reboot record
    Time time = 0;
    setutent ();
    while (true)
    {
	utmp ut, *utp;
	ut.ut_type = BOOT_TIME;
	if (! (utp = getutid (&ut)))
	    break;

	// Compare reboot times and use the newest one
	if (utp->ut_time > time)
        {
	    time = Time (utp->ut_time, 0);
	    if (! newest)
		break;
        }

	// Advance to the next element to avoid repeats
	if (! (utp = getutent ()))
	    break;
    }
    endutent ();

    return time;
}
#endif

static Time
getBootTime (void)
{
    static bool	initialised = false;
    static Time	time = 0;

    if (initialised)
	return time;

    initialised = true;

    // FIXME: WIN32?  Time::now () - GetTickCount()?
    // FIXME: TimeInfo has some code to figure out system-wide epoch,
    //   and there's probably stuff that could be used to get the
    //   realtime (Time()) time of the last reboot

#ifndef _WIN32
# if defined CTL_KERN && defined KERN_BOOTTIME
    timeval	val;
    int		mib [2] = { CTL_KERN, KERN_BOOTTIME };
    size_t	size = sizeof (val);

    if (sysctl (mib, 2, &val, &size, 0, 0) >= 0)
	return time = Time (val.tv_sec, val.tv_usec * 1000);
# endif // defined CTL_KERN && defined KERN_BOOTTIME

    struct stat st;
    if (stat ("/var/run/random-seed", &st) == 0)
	// FIXME: nsec-accuracy st_xtime?
	return time = Time (st.st_mtime, 0);

# ifdef BOOT_TIME
    if ((time = getBootTime (0, false)).ns ())
	return time;

    if ((time = getBootTime ("/var/log/wtmp", true)).ns ())
	return time;
# endif
#endif

    return 0;
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

FileNameLock::FileNameLock (void)
    : m_pid (0),
      m_time (0)
{}

FileNameLock::FileNameLock (const Filename &name)
    : m_pid (0),
      m_time (0)
{ lock (name); }

FileNameLock::~FileNameLock ()
{ if (! m_filename.empty ()) unlock (); }

//////////////////////////////////////////////////////////////////////
FileNameLock &
FileNameLock::now (void)
{
    m_user = UserInfo::self ()->id ();
    m_host = HostInfo::fqdn ();
    m_pid  = ProcessInfo::pid ();
    m_time = getBootTime ();
    return *this;
}

Filename
FileNameLock::format (void) const
{
    // FIXME: WIN32 domain\user causes a slash!
    return Filename (StringFormat ("%1@%2.%3:%4").arg (m_user).arg (m_host)
		     .arg (m_pid).arg (TimeSpan (m_time).seconds ()));
}

bool
FileNameLock::parse (const Filename &from)
{
    const char *p = from;
    const char *namestart = 0;
    const char *nameend = 0;
    const char *hoststart = 0;
    const char *hostend = 0;
    const char *pidstart = 0;
    const char *pidend = 0;
    const char *timestart = 0;
    const char *timeend = 0;

    // Get the name part 
    for (namestart = p; *p != '@'; ++p)
	if (! *p)
	    return false;

    nameend = p;

    // Find the time separator, then walk back to find pid.
    for (hoststart = ++p; *p != ':'; ++p)
	if (! *p)
	    return false;

    pidend = p;
    for (pidstart = --p; isdigit (*p); --p)
	;
    if (*p != '.')
	return false;

    hostend = p;
    pidstart = p+1;

    // Scan the time
    for (p = timestart = pidend+1; *p; ++p)
	if (! isdigit (*p))
	    return false;
    timeend = p;

    // Make sure we got all the parts
    if (! (nameend - namestart)
	|| ! (hostend - hoststart)
	|| ! (pidend - pidstart)
	|| ! (timeend - timestart))
	return false;

    // OK, put them into the variables
    m_user.assign (namestart, nameend - namestart);
    m_host.assign (hoststart, hostend - hoststart);
    m_pid = strtoul (pidstart, 0, 10);
    m_time = Time (strtoul (timestart, 0, 10), 0);
    return true;
}

bool
FileNameLock::valid (void) const
{ return ! m_user.empty (); }

/** Return the user name of the lock holder.  */
const std::string &
FileNameLock::user (void) const
{ return m_user; }

/** Return the full qualified host name of the lock holder.  */
const std::string &
FileNameLock::host (void) const
{ return m_host; }

/** Return the process id of the lock holder.  */
pid_t
FileNameLock::pid (void) const
{ return m_pid; }

/** Return the time at which the lock was created.  */
Time
FileNameLock::time (void) const
{ return m_time; }

//////////////////////////////////////////////////////////////////////
Filename
FileNameLock::lockname (const Filename &name)
{
    // FIXME: if there is lock name clash (symlink by this name
    // already exists), add .%d (d = 0-9)?
    return name.directory ().append (".#")
	.append (name.nondirectory ()).expand ();
}

// Overwrites this if there is a lock for name
FileNameLock::Kind
FileNameLock::locker (const Filename &name)
{
    // FIXME: if no symlinks: write a lock file with the info in it?
    Filename l (lockname (name));

    // Try to read the link until we have a buffer long enough.
    Filename link;
    if (! l.isSymlink (&link))
	// No lock is ok, otherwise something strange happened.
	return l.exists () ? LockUnknown : LockNone;

    // Now check the validity of the link format
    if (! parse (link))
	return LockUnknown;

    // Now check the lock status.  If it is on another host, we can't
    // do much about the checking.
    if (host () != HostInfo::fqdn ())
	return LockOther;

    // Compare boot times.  If they are within an appropriate window,
    // assume the machine hasn't rebooted.  If the times don't match,
    // we can assume the machine was rebooted and the lock owner is
    // dead.  (FIXME: only if it is older?)
    TimeSpan diff = time () - getBootTime ();
    bool sameBoot = time ().ns () && (diff >= 0 ? +diff : -diff) <= 1000000000;

    // Current lock.  If it is me, indicate so.
    if (sameBoot && pid () == ProcessInfo::pid ())
	return LockSelf;

    // Ping the process to check if it is still alive
    if (sameBoot && pid() >= 0 && (Signal::kill(pid(),0) >= 0 || errno==EPERM))
	return LockOther;

    // The owner is dead or has a strage pid (<= 0): try to zap the lock
    // FIXME: Use Filename::remove (l, false, true), make it return true if it did
// #ifdef _WIN32
//     if (! DeleteFile (l))
// 	return LockUnknown;
// #else
    if (unlink (l) < 0)
	return LockUnknown;
// #endif

    // Nobody owns the lock now.
    return LockNone;
}

/** Try to create a lock for this file name.  Returns #LockCreated if
    the lock was successfully acquired, #LockExists if a lock exists,
    #LockFailure otherwise.  If @a force, tries to remove any existing
    locks if the lock creation fails.  Overwrites this with information
    on the attempted lock.  */
FileNameLock::Success
FileNameLock::tryLock (const Filename &filename, bool force)
{
    m_filename = filename;

    Filename	name (lockname (filename));
    Filename	target (now ().format ());

    // Try to create the lock.
    // FIXME: if no symlinks: write a lock file with the info in it.
    for (int i = 0; i < 2; ++i)
	try {
	    Filename::symlink (target, name);
	    break;
	} catch (FileSystemError &err) {
	    SystemError *syserr = dynamic_cast<SystemError *> (err.next ());
	    if (syserr && syserr->portable () == SysErr::ErrFileExists)
	    {
		// It failed because a conflicting file exists.  If this is
		// first time around and we are forcing it, remove the lock
		// and try again.  Otherwise indicate conflicting lock.
		if (i == 0 && force)
		{
		    // FIXME: check first it is a symlink in our format?
		    Filename::remove (name, false, true);
		    continue;
		}

		return LockExists;
	    }

	    return LockFailure;
	}

    return LockCreated;
}

/** Lock this file name if possible.  Does not use operating system's
    file locking primitives, but other magic (e.g. symlink) that tells
    other processes the file is locked and is portable across a large
    variety of operating systems, networks and file systems.

    @returns #LockSelf if the lock was successfully acquired (@c this
	     will reflect the lock information; #LockOther if another
	     process holds the lock (@c this will have lock owner
	     information), or #LockUnknown if lock creation fails.  */
FileNameLock::Kind
FileNameLock::tryLock (const Filename &name)
{
    Success code;
    while ((code = tryLock (name, false)) != LockCreated)
    {
	// If the symlink operation failed for some reason other than
	// there already existing a link, bail out.
	if (code == LockFailure)
	    return LockUnknown;

	ASSERT (code == LockExists);

	// Find out who's got the lock now.  Even though we just got
	// here because there is an existing lock, this may produce
	// LockNone since the lock may have been deleted just now.
	Kind kind = locker (name);
	switch (kind)
	{
	case LockSelf:    // We already have it
	case LockOther:   // Somebody else has it
	case LockUnknown: // Unidentified failure
	    return kind;

	default:
	    // Nobody has it, so next time around it should work
	    // or cause some sort of a permission problem.
	    ;
	}

	// We just deleted a stale lock: try to lock the file again
	// until we succceed or get an error different from existing
	// lock file.
    }

    // Success
    return LockSelf;
}


/** Create a lock for this file name.  Does not use operating system's
    file locking primitives, but other magic (e.g. symlink) that tells
    other processes the file is locked and is portable across a large
    variety of operating systems, networks and file systems.

    @throws If the lock file exists, throws a FilenameError with the
            lock information.  The caller can then check if the lock
	    is valid (i.e. the lock file is not taken by some other
	    random file), and if so, who holds the lock.  */
void
FileNameLock::lock (const Filename &name)
{
    // FIXME: app level (lock document): lock using truename if
    // have one and last save time < last mod time 

    // FIXME: app level: if some buffer visits this file, the file has
    // been changed on disk since last visit/save (and exists), ask
    // user what to do -- the file is obsolete; this belongs in app
    // layer

    // FIXME: all this is now app level!

    // Try to create the lock.  If successful, bail out immediately
    if (tryLock (name))
	return;

    // FIXME: Ask user with info whether to break the lock.  This
    // should throw an exception over our heads to refrain, return
    // true to indicate that we should grab the lock, or false to edit
    // the file even though it is locked.
#if 0
    if (false)
	// Steal it
	tryLock (name, true);
#endif
}

/** Remove the lock we previously created if the lock is still owned
    by the current process.  */
void
FileNameLock::unlock (void)
{ unlock (m_filename); }

/** Remove the lock for this file if it is owned by the current
    process.  */
void
FileNameLock::unlock (const Filename &name)
{
    // FIXME: App level (unlock document): unlock using truename
    // if have one and last save time < last mod time

    // FIXME: This mutates me to the current locker!
    if (locker (name) == LockSelf)
    {
	// FIXME: throw on failure?
	::unlink (lockname (name));
	reset ();
    }
}

// /** Remove all locks currently held by this process.  */
// void
// Filename::unlockAll (void) const
// {
//     // FIXME: app level: walk over all documents and unlock all
//     //  that 1) have truename, and 2) last save time < mod time
// }

/** Check if anybody holds a lock for this file.  Returns @c false if
    nobody holds the lock and @c true if it is current locked.  In the
    latter case, @a lock is set to null if the file is locked by the
    current process, otherwise filled with a new object pointing to
    locker details.  If another incompatible file is found where a
    lock was expected, the return value is @c true but @a lock details
    are not valid (see #valid()).  Note also that another process or
    thread might have already changed the locking status by the time
    this method returns.  */
bool
FileNameLock::isLocked (const Filename &name, FileNameLock **lock)
{
    *lock = 0;
    FileNameLock *l = new FileNameLock;
    switch (l->locker (name))
    {
    case LockNone:
	delete l;
	return false;

    case LockSelf:
	delete l;
	return true;

    case LockOther:
	*lock = l;
	return true;

    case LockUnknown:
	l->reset ();
	*lock = new FileNameLock;
	return true;

    default:
	ASSERT (false);
	return false;
    }
}

void
FileNameLock::reset (void)
{
    m_filename = "";
    m_user = "";
    m_host = "";
    m_pid = 0;
    m_time = 0;
}

} // namespace lat
