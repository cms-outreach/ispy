//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/Filename.h"
#include "classlib/iobase/IOStatus.h"
#include "classlib/iobase/FileSystemError.h"
#include "classlib/iobase/FilenameError.h"
#include "classlib/iobase/DirIterator.h"
#include "classlib/utils/SystemError.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/utils/Log.h"
#include "classlib/utils/UserInfo.h"
#include "classlib/utils/Environment.h"
#include "classlib/sysapi/Filename.h"
#include "src/utils/StringHack.h"
#include <iostream>
#include <cstdlib>
#include <map>

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>

logstream &operator<< (logstream &out, const Filename &name)
{ return out << static_cast<const char *> (name); }

std::ostream &operator<< (std::ostream &out, const Filename &name)
{ return out << static_cast<const char *> (name); }

//////////////////////////////////////////////////////////////////////
bool
operator== (const Filename &x, const Filename &y)
{ return x.m_name == y.m_name; } // FIXME: normalise?

bool
operator== (const char *x, const Filename &y)
{ return x == y.m_name; } // FIXME: normalise?

bool
operator== (const std::string &x, const Filename &y)
{ return x == y.m_name; } // FIXME: normalise?

bool
operator== (const Filename &x, const char *y)
{ return x.m_name == y; } // FIXME: normalise?

bool
operator== (const Filename &x, const std::string &y)
{ return x.m_name == y; } // FIXME: normalise?

//////////////////////////////////////////////////////////////////////
bool
operator!= (const Filename &x, const Filename &y)
{ return !(x == y); }

bool
operator!= (const char *x, const Filename &y)
{ return !(x == y); }

bool
operator!= (const std::string &x, const Filename &y)
{ return !(x == y); }

bool
operator!= (const Filename &x, const char *y)
{ return !(x == y); }

bool
operator!= (const Filename &x, const std::string &y)
{ return !(x == y); }

//////////////////////////////////////////////////////////////////////
bool
operator< (const Filename &x, const Filename &y)
{ return x.m_name < y.m_name; }

bool
operator< (const char *x, const Filename &y)
{ return x < y.m_name; }

bool
operator< (const std::string &x, const Filename &y)
{ return x < y.m_name; }

bool
operator< (const Filename &x, const char *y)
{ return x.m_name < y; }

bool
operator< (const Filename &x, const std::string &y)
{ return x.m_name < y; }

//////////////////////////////////////////////////////////////////////
bool
operator<= (const Filename &x, const Filename &y)
{ return x.m_name <= y.m_name; }

bool
operator<= (const char *x, const Filename &y)
{ return x <= y.m_name; }

bool
operator<= (const std::string &x, const Filename &y)
{ return x <= y.m_name; }

bool
operator<= (const Filename &x, const char *y)
{ return x.m_name <= y; }

bool
operator<= (const Filename &x, const std::string &y)
{ return x.m_name <= y; }

//////////////////////////////////////////////////////////////////////
bool
operator> (const Filename &x, const Filename &y)
{ return x.m_name > y.m_name; }

bool
operator> (const char *x, const Filename &y)
{ return x > y.m_name; }

bool
operator> (const std::string &x, const Filename &y)
{ return x > y.m_name; }

bool
operator> (const Filename &x, const char *y)
{ return x.m_name > y; }

bool
operator> (const Filename &x, const std::string &y)
{ return x.m_name > y; }

//////////////////////////////////////////////////////////////////////
bool
operator>= (const Filename &x, const Filename &y)
{ return x.m_name >= y.m_name; }

bool
operator>= (const char *x, const Filename &y)
{ return x >= y.m_name; }

bool
operator>= (const std::string &x, const Filename &y)
{ return x >= y.m_name; }

bool
operator>= (const Filename &x, const char *y)
{ return x.m_name >= y; }

bool
operator>= (const Filename &x, const std::string &y)
{ return x.m_name >= y; }

//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

Filename::Filename (void)
{}

Filename::Filename (const char *name)
    : m_name (name ? name : "")
{}

Filename::Filename (const std::string &name)
    : m_name (name)
{}

/** Don't use this method as an alternative to append().  It may
    not behave the same way for all combinations.   */
Filename::Filename (const Filename &dir,
		    const Filename &file,
		    bool appendonly /* = false */)
{
    if (appendonly)
    {
	m_name = dir.m_name;
	m_name += file;
    }
    else
    {
	// FIXME: If `file' is absolute, discard dir?  Clients should not
	// use this to concatenate names, that's what append() is for.
	if (dir.empty ())
	    m_name = file.m_name;
	else if (file.empty ())
	    m_name = dir.m_name;
	else
	{
	    m_name = dir.asDirectory ();
	    append (file);
	}
    }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
bool
Filename::empty (void) const
{ return m_name.empty (); }

std::string::size_type
Filename::size (void) const
{ return m_name.size (); }

void
Filename::resize (std::string::size_type n, char c /* = '\0' */)
{ return m_name.resize (n, c); }

Filename::operator const char * (void) const
{ return m_name.c_str (); }

const char *
Filename::name (void) const
{ return m_name.c_str (); }

// Filename::operator const std::string & (void) const
// { return m_name; }

char &
Filename::operator[] (ptrdiff_t n)
{
    ASSERT (n >= 0);
    ASSERT ((std::string::size_type) n < m_name.size ());
    return m_name [n];
}

char
Filename::operator[] (ptrdiff_t n) const
{
    ASSERT (n >= 0);
    ASSERT ((std::string::size_type) n < m_name.size ());
    return m_name [n];
}

//////////////////////////////////////////////////////////////////////
/** @fn bool Filename::isAbsolute (bool considerExpansions = false) const

    Check if this is an absolute file name.  If @a considerExpansions
    is @c true, considers file names beginning with a home directory
    indicator `~' absolute (as interpreted by #expand()). */

bool
Filename::isRelative (void) const
{ return ! m_name.empty () && ! isAbsolute (); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** @fn bool Filename::status (IOStatus &s, bool traverseLinks = true) const

    Check if a file system entity by this name exists and if so,
    return information about it in @a s.  If the name designates a
    symbolic link, it is traversed only if @a traverseLinks is the
    default @c true; otherwise information is returned about the link
    itself.

    If the entity exists, returns @c true and fills @a s with status
    information.  Otherwise returns @c false.  The return value may be
    @c false for an existing but inaccessible file.  Note that if the
    return value is @c true, the entity may not be a file, may not be
    readable or even accessible by the current user, and may have been
    deleted by the time this method returns.  */

/** @fn bool Filename::setStatus (IOStatus &s) const

    Set the attributes of the file system entity by this name to @a s.
    FIXME further documentation.  */

/** @fn bool Filename::exists (void) const

    Check if a file system entity by this name exists.  Returns @c
    true if so; the return value may be @c false for an existing but
    inaccessible file.  Even if the return value is @c true, it does
    not mean that the entity is a file, nor that it can be read, nor
    even that it can be accessed by the current user.  Note that @c
    true return value means little: the file system entity may have
    been deleted by the time this method returns.  */

/** @fn bool Filename::isDirectory (void) const

    Check if a directory by this name exists.  Symbolic links to
    directories count as directories; see isSymlink() to distinguish
    links.  Note that @c true return value means little: the directory
    may have been deleted by the time this method returns.  */

/** @fn bool Filename::isRegular (void) const

    Check if this names a regular file that can hold an ordinary
    stream of data bytes.  Note that @c true return value means
    little: the file may have been deleted by the time this method
    returns.  (FIXME: Is this reliable beyond unix?) */

/** @fn bool Filename::isSymlink (Filename *target = 0) const

    Check if this names a symbolic link.  If @a target is non-null and
    this is a link, @a target is set to the link target.  Note that @c
    true return value means little: by the time this method returns,
    the link may have been deleted.  */

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Check if this names a directory as a file and files in the
    directory can be opened by this user.  */
bool
Filename::isAccessibleDirectory (void) const
{ return isDirectory () && ::access (*this, X_OK) == 0; }

/** Check if deleting this file and rewriting it would preserve the
    owner attribute.  */
bool
Filename::isOwnershipPreserved (void) const
{
    // FIXME: WIN32?
    // FIXME: consider sticky parent directory?
    // FIXME: invisible owner features (e.g. afs)?  -- st_uid should
    // be the uid it would get if we did creat right now, except on
    // afs it would get current token, which might have nothing to do
    // with current uid.

    IOStatus s;
    return status (s) && s.m_acl.owner () == UserInfo::self ()->uid ();
}

/** Check if this names an existing file system entity that can be
    read by the current user.  */
bool
Filename::isReadable (void) const
{ return ::access (*this, R_OK) == 0; }

/** Check if this names a file system entity that can be written or
    created by the current user.  */
bool
Filename::isWritable (void) const
{ return ::access (*this, W_OK) == 0; }

/** Check if this file system entity can be executed by this user.
    For a directory this means that the user can access files in this
    directory.  (FIXME: is this a valid interface on windows?)  */
bool
Filename::isExecutable (void) const
{ return ::access (*this, X_OK) == 0; }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
Filename
Filename::asDirectory (void) const
{
    Filename result (*this);
    // FIXME: WIN32: Deal with drives: "C:" needs to be turned into
    // "C:CWD\" (or remain as itself), not "C:\".  Distinguish leading
    // `\\' and '\' (the latter being relative to the "current" drive)?
    if (! result.m_name.empty () && ! ISPATHSEP (result.m_name.end ()-1))
	result.m_name += PATHSEP;
    return result;
}

Filename
Filename::asFile (void) const
{
    // FIXME: WIN32: Deal with drive-only path on windows: don't turn
    // "C:\" into "C:", leave it as it is.  How about shares (\\host,
    // \\host\share)?  Don't turn a leading '\\' into a '\'.
    Filename			result (*this);
    std::string::iterator	pos = result.m_name.end ();
    std::string::iterator	start = result.m_name.begin ();

    // FIXME: Leave a leading sole '/'
    // FIXME: Don't skip initial drive
    // FIXME: Initial drive/share only?
    while (pos - start > 1 && ISPATHSEP (pos-1))
	--pos;

    result.m_name.erase (pos, result.m_name.end ());
    return result;
}

/** Return the directory component of this file name.  The return
    value is empty if this name does not include a directory part.
    The name is returned as a directory name (see #asDirectory())
    into which a file name component can be directly #append()'ed to.
    That is, given a name in unix syntax, returns a string ending in
    slash.  */
Filename
Filename::directory (void) const
{
    // FIXME: Check behaviour on WIN32
    const char *start = m_name.c_str ();
    const char *p = start + m_name.size ();

    // FIXME: keep initial drive (only in the beginning)
    while (p > start && ! ISPATHSEP (p-1))
	--p;

    return m_name.substr (0, p - start);
}

/** Return this file name without its directory component.  For
    example, in a unix syntax name, returns everything after the last
    slash or the entire name if it has no slash.  If the name is a
    directory name without a file name component (e.g. as returned by
    #asDirectory()), returns an empty name.  */
Filename
Filename::nondirectory (void) const
{
    // FIXME: Check behaviour on WIN32
    const char *start = m_name.c_str ();
    const char *p = start + m_name.size ();

    // FIXME: skip initial drive part
    while (p > start && ! ISPATHSEP (p-1))
	--p;

    return m_name.substr (p - start);
}

/** Return this file name relative to @a directory: a relative file
    name equivalent to this one when used with @a directory as the
    default directory.  If this is impossible (for example on Windows
    where the file name and the directory may be on different drives),
    returns the name unchanged.  */
Filename
Filename::relativeTo (const Filename& /*directory*/) const
{
    // FIXME: relative to /foo/bar/goo /foo/bar/goo/zoo -> ..!
    // FIXME: relative to /foo/bar/goo /doo/bar/zoo
    //   -> ../../../foo/bar/goo!
    ASSERT (false);
    return *this;
}

/** Return the name's final "extension".  The extension is the part
    that follows the last `.'.  If there is no extension or the the
    name ends in period, returns an empty name.  If @a has is given,
    it will be set to @c true if there was a period so the client can
    distinguish the trailing dot and no extension.  If @a period is
    true, the return value includes the period delimiting the
    extension.  */
Filename
Filename::extension (bool *has /* = 0 */, bool period /* = false */) const
{
    // FIXME: optimise with special loop that takes into account
    // directory separators, drives and all those oddities?
    Filename prefix = directory ();
    Filename name = nondirectory ();

    if (name == "." || name == "..")
	return "";

    std::string::size_type i = name.m_name.rfind ('.');
    if (i == std::string::npos)
	return "";

    if (has)
	*has = true;

    return name.m_name.substr(i + !period, name.m_name.length() - i - !period);
}

/** Return the this name without its final "extension".  The extension
    is the part that follows the last `.'.  */
Filename
Filename::sansExtension (void) const
{
    // FIXME: optimise with special loop that takes into account
    // directory separators, drives and all those oddities?
    Filename prefix = directory ();
    Filename name = nondirectory ();

    if (name == "." || name == "..")
	return *this;
    else
	return Filename(prefix, name.m_name.substr(0, name.m_name.rfind('.')));
}

Filename &
Filename::append (const char *component)
{ m_name.append (component); return *this; }

Filename &
Filename::append (const std::string &component)
{ m_name.append (component); return *this; }

static void
findTruename (Filename &name, int counter, std::map<Filename,Filename> &seen)
{
    bool done = false;
    while (! done)
    {
	if (--counter < 0)
	    throw FileSystemError (new SystemError (0, SysErr::ErrTooManySymbolicLinks));

	Filename dir (name.directory ());
	if (dir.empty ())
	    dir = Filename::cwd ().asDirectory ();

	// Get the truename of the directory.  If the file name is the
	// same as the directory name, we have a root directory.  If
	// we've already seen the directory, save time.  Otherwise get
	// the true name of the directory part and cache it in `seen'.
	Filename dirfile = dir.asFile ();
	if (dir == dirfile)
	    ;
	else if (seen.count (dir))
	    dir = seen [dir];
	else
	{
	    findTruename (dirfile, counter, seen);
	    seen [dir] = dirfile;
	    dir = dirfile;
	}

	// We do not use expand() but resolve `..' and `.'  ourselves.
	// Link targets might use `foo/../bar' with `foo' a link.
	// FIXME: Special treatment of `//.'?
	if (name.nondirectory () == "..")
	{
	    name = dir.asFile ().directory ().asFile ();
	    done = true;
	}
	else if (name.nondirectory () == ".")
	{
	    name = dir.asFile ();
	    done = true;
	}
	else
	{
	    // Put the name part back on the resolved directory
	    name = Filename (dir, name.nondirectory (), false);

	    // If it is a link, chase it, and start all over since the
	    // link may point to a directory name that uses links.  If
	    // it is not a link, we are done at this level.
	    Filename target;
	    if (name.isSymlink (&target))
	    {
		name = target.isAbsolute () ? target
		       : Filename (dir, target, false);
		done = false;
	    }
	    else
		done = true;
	}
    }
}

/** Return the true absolute name of this file name.  The true name is
    found by chasing symbolic links both at the level of the file and
    at the level of the directories containing it, until no links are
    left at any level.  */
Filename
Filename::truename (void) const
{
    // FIXME: Use realpath on systems that have it?
    Filename name (expand ());
    if (name.empty ())
	name = "/"; // FIXME: WIN32?

    // Normalise for platform conventions (Windows 8.3 -> long name)
    name.normalise ();

    // Translate the file name.
    std::map<Filename,Filename>	seen;
    findTruename (name, 100, seen);
    return name;
}

/** Chase links in this name until a name that is not a link.
    Does not examine containing directories for links unlike
    #truename().  */
Filename
Filename::chaseLinks (void) const
{
    Filename	name (*this);
    Filename	target;
    int		counter = 100;

    while (name.isSymlink (&target))
    {
	if (counter == 0)
	    // FIXME: provide file name
	    throw FileSystemError (new SystemError (0, SysErr::ErrTooManySymbolicLinks));

	// In a link `//' doesn't mean what we generally think it
	// does.  Replace slash sequences with a single one.
	std::string::size_type pos;
	while ((pos = target.m_name.find ("//")) != std::string::npos)
	{
	    std::string::size_type end = pos + 2;
	    while (end < target.m_name.length () && target.m_name [end] == '/')
		++end;
	    target.m_name.replace (pos, end - pos, "/");
	}

	// Handle `..' by hand since it needs to work in a directory
	// symlink target.  This is not quite complete; it doesn't
	// handle embedded `..' is cases like `./../foo' and
	// `foo/bar/../../../lose'.
	while (! STRCOMPARE (target.m_name, "../", 0, 3))
	{
	    target.m_name.erase (0, 3);
	    // Chase links in the default dir of the symlink and then
	    // get the parent of that directory.
	    name = name.expand ()
		   .directory ().asFile ().chaseLinks ()
		   .directory ();
	}

	name = target.expand (name.directory ());
	--counter;
    }

    return name;
}

//////////////////////////////////////////////////////////////////////
/** Return a file name to use for auto-saves.  */
Filename
Filename::autosaveName (void) const
{ return directory ().append ("#").append (nondirectory ()).append ("#"); }

/** Check if this name could have been yielded by autosave.  The name
    should not contain directory part at all.  */
bool
Filename::isAutosaveName (void) const
{
    const char *start = m_name.c_str ();
    const char *end = start + m_name.size ();
    return (*start == '#' && end > start && end[-1] == '#');
}

//////////////////////////////////////////////////////////////////////
// FIXME: provide option to copy or move for backup: copy causes
// other names to point to the changed file, move causes them
// to point to the backup.
//   - backup-by-copying
//   - backup-by-copying-when-linked
//   - backup-by-copying-when-mismatch (copy if that preserver owner or group)
//   - backup-inhibited
//
// - version-control: never / if-existing / always
// - kept-new-versions (2) - # of newest versions to keep when making a new one
// - kept-old-versions (2) - # of oldest versions to keep when making a new one
// - delete-old-versions => delete excess backup versions silently

/** Find a name for a backup file and suggestions for deletions.
    Returns a name for the backup file and sets @a deletions to a list
    of old versions to consider deleting now.  If the returned name is
    empty, don't make a backup.  */
Filename
Filename::findBackupName (std::vector<Filename> & /* deletions */) const
{ return backupName (); } // FIXME: Versioning

/** Create a non-numeric backup file name for this name.  */
Filename
Filename::backupName (void) const
{ Filename result (*this); result.append ("~"); return result; }

/** Check if this is a backup file name.  */
bool
Filename::isBackupName (void) const
{ return !m_name.empty () && *(m_name.end ()-1) == '~'; }

/** Return this name without backup versions or strings.  If @a
    keepVersionNr is @c true, do not remove backup version numbers,
    only true file version numbers.  */
Filename
Filename::sansVersions (bool /* keepVersionNr = false */) const
{
    // Backups have either a trailing `~' or `\.~[0-9]+(\.[0-9]+)*~'.
    // Skip either and return the rest.  (FIXME: this can return an
    // empty string if the whole file name is just ~ or the versioned
    // suffix -- is that ok?)
    //
    // NB: keepVersionNr really matters only on VMS, which we do not
    // support at the moment.
    const char *start = m_name.c_str ();
    const char *p = start + m_name.size ();
    if (p == start || p[-1] != '~')
	return *this;

    // Skip the trailing ~
    --p;

    // Skip trailing version number sequence if there is one.  Skip
    // only complete valid number blocks, and only those.
    if (p > start && isdigit ((unsigned char) p[-1]))
    {
	while (p > start && isdigit ((unsigned char) p[-1]))
	{
	    // Skip digit block
	    while (p > start && isdigit ((unsigned char) p[-1]))
		--p;

	    // If the digit block is preceded by a dot, skip it and go
	    // back to skipping digit blocks.  If it is not preceded
	    // by one, the while terminates and the `if' below catches
	    // the flaw and does the right thing.
	    if (p > start && p[-1] == '.')
		--p;
	}

	// Make sure we are seeing a valid suffix.  If not, assume it
	// was all part of the file name: go back to that trailing ~
	// we skipped in the beginning.  Otherwise, skip the preceding
	// ~ and dot.
	if (p <= start+2
	    || !isdigit ((unsigned char) *p)
	    || (p[-1] != '~' && p[-2] != '.'))
	    p = start + m_name.size () - 1;
	else
	    p -= 2;
    }

    // Return the part that is still left.
    return m_name.substr (0, p - start);
}

//////////////////////////////////////////////////////////////////////
void
Filename::expandUser (std::string		&namestr,
		      std::string::size_type	pos,
		      std::string		&dirstr)
{
    // Expand `~[user]'

    ASSERT (namestr [pos] == '~');
    const char *p = namestr.c_str () + pos;
    const char *start = ++p;

    if (! *p || ISPATHSEP (p+1))
    {
	// `~' by itself.
	dirstr = UserInfo::self ()->homedir ();
	namestr.erase (0, 1);
    }
    else
    {
	// `~user'.  If user not found, leave path unchanged.
	// FIXME: Windows DOMAIN\user support?
	while (*p && ! ISPATHSEP (p))
	    ++p;

	if (UserInfo *u = UserInfo::user (std::string (namestr, 1, p - start)))
	{
	    dirstr = u->homedir ();
	    namestr.erase (0, p - start + 1);
	}
    }
}

void
Filename::expandPrefix (Filename &name, const Filename &defdir)
{
    // Determine the prefix directory.  We can have three different
    // combinations of dirstr and name: 1) name is absolute; dirstr
    // will remain empty, 2) name is `~[user]' and becomes either
    // empty or the absolute subpath after the user prefix; dirstr
    // will be absolute, and 3) name is relative; dirstr will be
    // absolute (the default dir).
    std::string dirstr;

    // Expand `~[user]' prefix.
    if (name [0] == '~')
	expandUser (name.m_name, 0, dirstr);

    // Pick the default directory prefix if name is non-empty relative
    // path.  This is taken after the `~[user]' expansion only if the
    // user lookup failed, but not otherwise.
    if ((name.empty () || name.isRelative ()) && dirstr.empty ())
	dirstr = defdir.m_name;

    // If we have a directory part, add a trailing separator if name
    // is relative, remove it otherwise.  We can get here with both an
    // absolute name and absolute dirstr after `~[user]' expansion.
    // In that case we know that name starts with a separator -- it
    // cannot be a drive or a share on Windows.
    m_name = dirstr;
    if (! empty ())
    {
	if (name.isRelative ())
	    m_name = asDirectory ();
	else
	    m_name = asFile ();
    }

    // Finally, append the name part.  We have set things up to work
    // correctly in the three cases we can encounter.  The name should
    // be absolute, except that on Windows it may still begin with a
    // relative drive reference without the drive's current cwd.
    append (name);
}

void
Filename::expandClean (std::string &namestr, bool strip)
{
    // Canonicalize by removing every appearance of `/.' and `/foo/..'
    // except for leading `//./' (on Windows) or `/..'  (superroot on
    // some file systems).  If `/.' is the entire file name, keep the
    // `/'; otherwise delete both.  If strip, also shorten `...//' and
    // and `.../x:'

    // FIXME: /../../foo -> /foo?
    // FIXME: avoid foo/.. strippage? -- it is both good and bad...

    // Make sure the string is null-terminated for the right length
    namestr.c_str ();

    // Process it
    char	*begin = &namestr [0];
    char	*out = begin;
    const char	*p = begin;

    while (*p)
    {
	// Check for `...//' -> `/' except not a leading one on
	// WIN32.  On WIN32, use `...///' to begin a share.
	if (strip && ISPATHSEP (p) && ISPATHSEP (p+1) && (!ISWIN32 || p>begin))
	    ++p, out = begin;

	// Check for `.../x:' -> `x:'; expand drive cwd
	else if (strip && ((ISPATHSEP (p) && ISDRIVE (p+1))
			   || (out == begin && ISDRIVE (p))))
	{
	    if (out != begin)
		++p;

	    out = begin;
	    *out++ = *p++;
	    *out++ = *p++;
	    if (! ISPATHSEP (p)) 
	    {
		std::string::size_type	ppos = p - begin;
		Filename		dir (cwd (*begin).asDirectory ());

		namestr.insert (2, dir);
		namestr.c_str (); // FIXME: required?
		begin = &namestr [0];
		out = begin + 2 + dir.size ();
		p = begin + dir.size () + ppos;
	    }
	}

	// Check for initial `x:' to expand drive cwd
	else if (! strip && out == begin && ISDRIVE (p) && ! ISPATHSEP (p+2))
	{
	    *out++ = *p++;
	    *out++ = *p++;

	    std::string::size_type	ppos = p - begin;
	    Filename			dir (cwd (*begin));

	    if (p[2]) dir = dir.asDirectory ();
	    namestr.insert (2, dir);
	    namestr.c_str (); // FIXME: required?
	    begin = &namestr [0];
	    out = begin + 2 + dir.size ();
	    p = begin + dir.size () + ppos;
	}

	// Skip `..././' except on Windows retain a leading `//./'.
	// If the sole name is `/.', or on Windows `x:/.', keep the
	// slash; otherwise skip both.
	else if (ISPATHSEP (p) && p [1] == '.' && (! p[2] || ISPATHSEP (p+2)))
	{
	    if (ISWIN32 && out-begin == 1 && ISPATHSEP (begin))
		*out++ = *p++, *out++ = *p++;
	    else if (!p[2] && (out==begin || (out-begin==2 && ISDRIVE(begin))))
		*out++ = *p++, p++;
	    else
		p += 2;
	}

	// Skip `foo/..'; leave '/..' as it can be a "superroot" on
	// some file systems.  On Windows leave `x:/..' and `//..'
	// untouched; neither makes sense but avoids further havoc.
	else if (ISPATHSEP (p)
		 && p [1] == '.'
		 && p [2] == '.'
		 && (! p [3] || ISPATHSEP (p+3)))
	{
	    if (out-begin == 0
		|| (ISWIN32 && out-begin == 1 && ISPATHSEP (begin))
		|| (ISWIN32 && out-begin == 2 && ISDRIVE (begin)))
		*out++ = *p++, *out++ = *p++, *out++ = *p++;
	    else
	    {
		// Find the beginning of the previous name component
		while (--out > begin && ! ISPATHSEP (out))
		    ;

		ASSERT (ISPATHSEP (out));

		// Keep the slash at `out' only if the `..' isn't followed
		// by one and we are at the beginning of the name (`/',
		// `x:/' or `//').  Avoids confusing ourselves later on.
		if (! p [3]
		    && (out-begin == 0
			|| (ISWIN32 && out-begin == 1 && ISPATHSEP (begin))
			|| (ISWIN32 && out-begin == 2 && ISDRIVE (begin))))
		    ++out;

		p += 3;
	    }
	}

	// Skip this character
	else
	    *out++ = *p++;
    }

    // Shrink the string to the right size.
    namestr.resize (out - begin);
}

/** Convert this name to a canonical absolute name with many commonly
    convenient simplifications.

    The @a defaultDir argument specifies the directory to start with
    if the name is relative; if it is empty, current directory is used
    (see #cwd()).  The following substitutions are made on the name:
      - file name components that are `.' are removed (except leading
        `\\.\' is retained on windows)
      - file name components followed by `..' are removed, along with
        the `..' itself; this simplification is done without checking
        the resulting names in the file system
      - an initial `~/' expands to the current user's home directory
      - an initial `~USER/' expands to USER's home directory if it
        can be determined.

    @sa substitute(), abbreviate().  */
Filename
Filename::expand (const Filename &defaultDir /* = "" */) const
{
    // FIXME: see file name parsers in newstuf/src/*-filename-parser.cxx
    // FIXME: we accept /\foo as a share -- do we care?

    // FIXME: WIN32: convert to full name
    // FIXME: WIN32: Insert default drive, drive cwd if none are given
    // FIXME: WIN32: If length exceeds MAX_PATH, prefix with //?/ ?
    //   (also automatically strip it off in all ops, not just expand?
    //    can it be applied only to absolute paths, or relative as well?)

    // Get the default path
    Filename	defdir (defaultDir);
    Filename	name (*this);
    Filename	target;

    // Make default directory valid first; avoid infinite recursion
    if (defdir.empty ())
	defdir = cwd ();

    if (defdir.isRelative () && defdir != name)
	defdir = defdir.expand ();

    // Determine and expand prefix directory
    target.expandPrefix (name, defdir);

    // We now have our fully expanded name. Clean it up.
    expandClean (target.m_name, false);

    // Return it.
    return target;
}

/** Substitute environment variables in this file name.

    An occurence of `$FOO' where FOO is in @a env causes the value of
    FOO to be substituted in the name.  The variable name should be
    terminated with a character that is not a letter, digit or
    underscore; otherwise the entire variable name should be enclosed
    in braces.  A double `$' is replaced with a single one.

    The resulting name is canonicalised as with #expand() with the
    following additional operations:
      - if a `//' appears, all of the name through the first `/' is
        discarded (except initial share `//' is retained on windows)
      - if on windows a `/x:' appears, all of the name through the `/'
        is discarded; if the `:' is not followed by a directory
        separator, the drive's current directory is inserted
      - if a `/~' appears, all of the name through the `/' is discarded
        and user name expansion is applied as with #expand().
	
    @throws Bad variable reference syntax and references to undefined
	    variables cause a #FilenameError being thrown.

    @sa     expand(), abbreviate().  */
Filename
Filename::substitute (const Environment &env) const
{
    // FIXME: This should be a combination of word expansion with
    // variable expander (e.g. zsh, or basic sh), and a separate pass
    // here.  The method should probably take an optional word
    // expander and provide a default with sh expander.

    // see file name parsers in newstuf/src/*-filename-parser.cxx

    Filename	expanded;
    Filename	source (expand ());
    const char	*p = source;
    const char  *start = p;

    // Figure out the last point in the name where we are intersted in
    // by flushing ...//, .../~, .../x:.  Done first because we don't
    // want to do user expansion after variable expansion.
    while (*p)
    {
	// FIXME FIXME FIXME: Keep track of envar refs, do not do
	// these inside them!

	// Check for `...//' -> `/' except not a leading one on
	// WIN32.  On WIN32, use `...///' to restart with a share.
	if (ISPATHSEP (p) && ISPATHSEP (p+1) && (!ISWIN32 || p > start))
	    start = ++p;

	// WIN32: Check for `.../x:' -> `x:'
	else if (ISPATHSEP (p) && ISDRIVE (p+1))
	    start = ++p;

	// Check for `.../~' -> `~' (FIXME: check if user exists)
	else if (ISPATHSEP (p) && p[1] == '~')
	    start = ++p;

	// Skip this character
	else
	    ++p;
    }
    source.m_name.erase (0, start - (const char *) source);
    expanded.expandPrefix (source, cwd ());

    // Now expand variable references.
    std::string	&namestr = expanded.m_name;
    char	*begin;
    char	*out;

    namestr.c_str ();
    for (p = out = begin = &namestr [0]; *p; )
    {
	// Not a separator or a variable starter: skip.
	if (*p != '$')
	    *out++ = *p++;

	// Possibly a variable
	else
	{
	    const char	*anchor = p;
	    const char	*start = ++p;
	    const char	*end;
	    bool	gotone = false;

	    if (*p == '$')
		// Handle `$$' -> `$'
		++anchor, end = start;
	    else if (*p == '{')
	    {
		// Handle `${VAR}'
		for (++p, ++start; *p && *p != '}'; ++p)
		    ;

		// Complain about unterminated variable references.
		if (! *p)
		    throw FilenameError (FilenameError::UNTERMINATED_VAR,
					 std::string (start, p - start));

		gotone = true;
		end = p++;
	    }
	    else
	    {
		// Handle `$FOO_BAR5'.
		while (*p && (isalpha ((unsigned char) *p)
			      || isdigit ((unsigned char) *p)
			      || *p == '_'))
		    ++p;

		// Complain about a lone `$' not followed by a name.
		if (p == start)
		    throw FilenameError (FilenameError::BAD_SYNTAX);

		gotone = true;
		end = p;
	    }

	    // The variable name is now delimited by [start, end) and
	    // the whole reference (including ${}) by [anchor, p).
	    // Look it up in the environment.
	    std::string name (start, end - start);
	    std::string	value;

	    if (! gotone)
		// Copy the whole thing into output untouched.
		while (anchor != p)
		    *out++ = *anchor++;

	    else if (! env.get (name, value))
		// Didn't exist, complain.
		throw FilenameError (FilenameError::NO_SUCH_VAR, name);

	    else
	    {
		// Insert the string into the output.  This may end up
		// expanding the output too much, but that's ok, we'll
		// fix that at the end.
		std::string::size_type ppos = p - begin;
		std::string::size_type pos = out - begin;
		namestr.insert (pos, value);
		namestr.c_str ();
		begin = &namestr [0];
		out = begin + pos + value.size ();
		p = begin + ppos + value.size ();
	    }
	}
    }

    // Shrink the string to the right size and clean it up.
    namestr.resize (out - begin);
    expandClean (namestr, true);
    return expanded;
}

/** Return a version of this name shortened using @a abbrevs plus the
    basic abbreviate().  @a abbrevs should be a mapping of directory
    names to shorter names.  The substitutions are considered in the
    order given, against the beginning of the file name.  The whole
    list is applied in the order given, not just the first match.

    Use this method when the user has directories normally referred to
    via absolute symbolic links: make the replacement be the name of
    the link and the key the name it is linked to.  */
Filename
Filename::abbreviate (const AbbrevList &abbrevs) const
{
    // Apply each abbreviation in turn, in the order given, at the
    // beginning of the file name.
    std::string name (m_name);
    for (size_t i = 0; i < abbrevs.size (); ++i)
	if (! STRCOMPARE (name, abbrevs [i].first, 0, abbrevs[i].first.length()))
	    name.replace (0, abbrevs [i].first.length(), abbrevs [i].second);

    // Finally try the standard substitutions (user's home directory)
    return abbreviate (name);
}

/** Return a short version of this name using standard abbreviations:
    substitutes "~" for the user's home directory.  (FIXME: ~user?)  */
Filename
Filename::abbreviate (void) const
{ return abbreviate (m_name); }

/** Return a short version of this name using standard abbreviations:
    substitutes "~" for the user's home directory.  (FIXME: ~user?)  */
Filename
Filename::abbreviate (std::string name)
{
    // FIXME: NFS automounter?

    std::string home (UserInfo::self ()->homedir ());
    if (! STRCOMPARE (name, home, 0, home.length ()))
	name.replace (0, home.length (), "~");
    return name;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Generate the list of all completions of this name in directory @a
    dir into @a list.  These are all file names in directory @a dir
    prefixed by this name.  Returns the number of file names found.  */
size_t
Filename::complete (std::vector<Filename> &list, const Filename &dir) const
{
    DirIterator i (dir);
    DirIterator end;

    for ( ; i != end; ++i)
    {
	std::string item (*i);
	if (! STRCOMPARE (item, m_name, 0, m_name.length ()))
	    list.push_back (item);
    }

    return list.size ();
}

/** Complete this name in directory @a dir.  Extends this name to the
    longest common prefix to all file names in @a dir that start with
    this name.  Returns @c true if there is at least one matching
    file.  If the directory has only one file matching this name and
    the match is exact, @a exact will be set to @c true.  */
bool
Filename::complete (const Filename &dir, bool *exact)
{
    // Get all applicable completions
    std::vector<Filename> c;
    if (! complete (c, dir))
    {
	*exact = false;
	return false;
    }

    // Find the longest common prefix.  Start with the first match and
    // walk the rest of the list, shrinking to the longest prefix of
    // the next item in each step.  This will leave us with the
    // longest common prefix across all of them in the end.
    size_t prefix = m_name.size ();
    *exact = (c.size () == 1 && c [0].m_name == m_name);
    m_name = c [0].m_name;
    for (size_t i = 1; i < c.size (); ++i)
    {
	size_t length = std::min (c [i].m_name.size (), m_name.size ());
	size_t common = prefix;
	while (common < length && m_name [common] == c [i].m_name [common])
	    ++common;
	m_name.resize (common);
    }

    return true;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Glob this file name: expand wildcards in it.  Puts the list of
    file names that match this name, the pattern, into @list.  If the
    pattern is an absolute file name, the values are absolute also.
    If the pattern is a relative name, it is interpreted relative to
    @a dir and the returned names are also relative to that directory.
    However if @a absolute is @c true, they will be made absolute.
    Returns the number of names found.  */
size_t
Filename::glob (std::vector<Filename>& /*list*/,
                const Filename& /*dir*/,
                bool /*absolute = false */) const
{
    // FIXME: use ::wordexp()(?), ::glob(), ::fnmatch(), our own globbers, ...
    // FIXME: Move this out to globbers -- or keep just the basic code here?
    // FIXME: For unsafe globbing (e.g. the more powerful wordexp()), need
    // to provide security flags that by default are off.
    // FIXME: If system doesn't provide glob(), fallback on our own;
    // desperately try to avoid that, I don't want to code all the
    // POSIX locale stuff; maybe just provide a simpler one ourselves.
    // FIXME: allow quoting?
#if 0 && HAVE_GLOB
    // FIXME: absolute?
    // FIXME: Expose GLOB_NOCHECK (return pattern itself if no matches)?
    // FIXME: Expose GLOB_NOESCAPE (meta chars cannot be escaped)?

# ifndef GLOB_PERIOD
#  define GLOB_PERIOD 0
# endif
# ifndef GLOB_BRACE
#  define GLOB_BRACE 0
# endif
# ifndef GLOB_NOMAGIC
#  define GLOB_NOMAGIC 0
# endif
    glob_t	globbuf;
    int		flags = GLOB_NOSORT | GLOB_PERIOD | GLOB_BRACE | GLOB_NOMAGIC
			| GLOB_ERR; // FIXME: Do we want this last one?

    switch (::glob (name, flags, 0, &globbuf))
    {
    case 0:
	list.reserve (list.size () + globbuf.gl_pathc);
	for (size_t i = 0; i < globbuf.gl_pathc; ++i)
	    list.push_back (globbuf.gl_pathv [i]);
	::globfree (&globbuf);
	return globbuf.gl_pathc;

    case GLOB_NOMATCH:
	return 0;

    case GLOB_NOSPACE:
    case GLOB_ABORTED:
    default:
	// FIXME: throw SystemError (errno);
	return 0;
    }
#else
    ASSERT (false);
    return 0;
#endif
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Return the current directory as a file name.  */
Filename
Filename::cwd (void)
{
    // Try first with null size and buffer, getcwd may be able to
    // allocate a buffer large enough automatically.  If we can't, we
    // start with a modest size (INITIAL_LENGTH).  There's no point in
    // allocating huge strings of size PATH_MAX -- the current
    // directory is virtually never that large.  Most modern systems
    // do not have path length limits anyway, so PATH_MAX is not very
    // useful.
    static const size_t	INITIAL_LENGTH = 100;
    size_t		n = 0;
    char		*buf = 0;
    char		*dir = 0;

    do
	if (! (dir = getcwd (buf, n)))
        {
	    if (! buf)
		buf = new char [n = INITIAL_LENGTH];
	    else if (errno == ERANGE)
	    {
		if (buf)
		    delete [] buf;
		else
		    ::free (dir);
		buf = new char [n *= 2];
	    }
	    else
	    {
		if (buf)
		    delete [] buf;
		else
		    ::free (dir);
		 throw FileSystemError ("getcwd()", errno);
	    }
        }
    while (! dir);

    Filename result (dir);
    if (buf)
	delete [] buf;
    else
	::free (dir);
    return result;
 }

 /** @fn Filename Filename::cwd (char drive)

     Return the current directory for @a drive as a file name.  */

 //////////////////////////////////////////////////////////////////////
 /** @fn Filename Filename::null (void)

     Return the name of the null device on this system.  */

 /** @fn Filename Filename::root (void)

     Return the name of the file system root on this system.  */

 Filename
 Filename::updir (void)
 { return Filename (".."); }

 Filename
 Filename::thisdir (void)
 { return Filename ("."); }

 //////////////////////////////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////////
 /** Create a directory called @a name.  It is created with access mode
     @a mode.  If @a recursive is @c true, all intermediate directories
     are created, otherwise just one directory level can be made.  If
     @a allowExisting is @c true, the method will not fail if the
     directory already exists; @a recursive implies @a allowExisting.
     On return the directory will have been created; a #FileSystemError
     is thrown in case of failure.  */
 void
 Filename::makedir (Filename name,
		    FileAcl mode /* = 0755 */,
		    bool recursive /* = false */,
		    bool allowExisting /* = false */)
 {
     // FIXME: WIN32: Mixing system errors and errno!
     if (recursive)
	 allowExisting = true;

     // Make sure the name is in file name mode.
     name = name.asFile ();

     // Optimise common case.
     if (domakedir (name, mode, allowExisting))
	 return;

     if (! recursive)
	 throw FileSystemError ("domakedir()", errno);

     // Try to make each component in turn.
     Filename dirpart = name.directory ();
     Filename dirfile = dirpart.asFile ();
     if (dirfile != dirpart)
	 makedir (dirfile, mode, true);
     else
	 // Oops, something failed.  Either we failed to create the
	 // directory because of permissions and such, or the name is
	 // relative and current directory doesn't exist, or we just
	 // otherwise are very confused.
	 throw FileSystemError ("domakedir()", errno);

     // Try again.
     if (! domakedir (name, mode, allowExisting))
	 throw FileSystemError ("domakedir()", errno);
 }

 /** Remove a file or a directory called @a name.  If @a recursive
     is @c true, removes the entire directory tree.  If @a force is
     @c true, errors are ignored (FIXME: fiddles with permissions?).  */
 void
 Filename::remove (Filename name,
		   bool recursive /* = false */,
		   bool force /* = false */)
 {
     // FIXME: Should "force" fiddle with permissions?

     // Make sure the name is in file name mode.
     name = name.asFile ();

     // Optimise common case.  NB: remove() handles both files and dirs!
     if (doremove (name) >= 0 || (force && !recursive))
	 return;

     if (! recursive)
	 throw FileSystemError ("doremove()", errno);

     // Remove all subfiles.  Guard against non-existing directories in
     // case they disappear under our feet.
     try {
	 for (DirIterator files (name), end; files != end; ++files)
	     remove (Filename (name, *files), recursive, force);
     }
     catch (FileSystemError &)
     {
	 if (! force)
	     throw;
     }

     // Now try deleting it again.
     if (doremove (name) < 0)
	 throw FileSystemError ("doremove()", errno);
 }

 /** @fn int Filename::doremove (Filename name)

     Remove a file or a directory.  */

 /** @fn bool Filename::domakedir (Filename name, FileAcl mode, bool allowExisting)

     Create a directory called @a name with access rights @a mode.  If
     @a allowExisting is @c true, don't fail if the directory already
     exists.  */

 } // namespace lat
 //////////////////////////////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////////

 //////////////////////////////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////////
 #if 0
 //#doc		Returns the directory part in the file name. If there is no directory
 //		part in the name, the string will be empty. Otherwise, it will be the
 //		directory part including the trailing terminator.
 //<args>	rsFileName		Reference to the file name string.
 //<return>	The directory part string.
 string File::directoryPart (const char *pszFileName)
 {
     ASSERT (IsValidString (pszFileName));
     ASSERT (strlen (pszFileName) > 0);

     int		len = strlen (pszFileName);
     int		i = len;
     int		min = 0;

 #ifdef WINDOWS
     if (len >= 2 && isalpha (pszFileName [0]) && pszFileName [1] == ':')
	 min = 2;
 #endif

     while (i > min && pszFileName [i-1] != '/'
 #ifdef WINDOWS
	    && pszFileName [i-1] != '\\'
 #endif
	    )
	 i--;

     return string (pszFileName, i);
 }

 //#doc		Returns the non-directory part in the file name.
 //<args>	rsFileName		Reference to the file name string.
 //<return>	The non-directory part of the file name.
 string File::namePart (const char *pszFileName)
 {
     ASSERT (IsValidString (pszFileName));
     ASSERT (strlen (pszFileName) > 0);

     int		len = strlen (pszFileName);
     int		i = len;
     int		min = 0;

 #ifdef WINDOWS
     if (len >= 2 && isalpha (pszFileName [0]) && pszFileName [1] == ':')
	 min = 2;
 #endif

     while (i > min && pszFileName [i-1] != '/'
 #ifdef WINDOWS
	    && pszFileName [i-1] != '\\'
 #endif
	    )
	 i--;

     return string (pszFileName+i, len-i);
 }

 #ifdef WINDOWS
 static void NormalizeWindowsPath (string &str, bool bLowercase)
 {
     string::iterator	begin = str.begin ();
     string::iterator	end = str.end ();

     while (begin != end) {
	 if (bLowercase)
	     *begin = tolower (*begin);
	 if (*begin == '\\')
	     *begin = '/';
	 ++begin;
     }
 }
 #endif
 #endif // 0

 #if 0
 // Convert a file name to absolute and canonicalize it. The default
 // directory is used, if the file name is relative (i.e it does not
 // start with a slash). If the default is empty or missing, the current
 // directory is used. Path components `.' are removed and components
 // following `..' are removed with the `..' itself. `~/' expands to
 // home directory and `~user' expands to user's home directory. 
 // If `/~' appears, all of the file name through that `/' is discarded.
 // On systems having environment variables, `$VAR' where <VAR> is an
 // environment variable, are substituted for that variable's value.
 // If the variable is not terminated with a character not a letter,
 // digit or underscore, the entire variable should be enclosed in braces.
 // Simplifications for relative paths are done without checking the
 // existence in the file system.
 string File::expandName (const char *pszName,
			  const char *pszDefaultDirectory /* = NULL */)
 {
     ASSERT (IsValidString (pszName));
     ASSERT (! pszDefaultDirectory || IsValidString (pszDefaultDirectory));

     string	sName (pszName);
     int		i;

 #ifdef WINDOWS
     ::AnsiLower ((char *)sName.c_str());
     ::NormalizeWindowsPath (sName, false);
 #endif

     // Get the default path
     char	*pszCWD;

     string sDefaultDir (pszDefaultDirectory);
     if (sDefaultDir.empty ()) {
 #ifndef WINDOWS
	 pszCWD = ::getcwd (NULL, 1024);
 #else
	 pszCWD = ::getcwd (NULL, PATH_MAX);
 #endif
	 sDefaultDir = pszCWD ? pszCWD : "."; // !

	 ::free (pszCWD);
     }

 #ifdef WINDOWS
     // The file name is in OEM format
     ::NormalizeWindowsPath (sDefaultDir, true);
 #endif

     // Check for valid default directory
     if (sName != sDefaultDir
	 && ! sDefaultDir.empty ()
	 && (
 #ifndef WINDOWS
	     sDefaultDir [0] != '/'
 #else
		 ! (sDefaultDir.length () >= 3
		    && isalpha (sDefaultDir [0])
		    && sDefaultDir [1] == ':'
		    && sDefaultDir [2] == '/')
 #endif
	     ))
	 expandName (sDefaultDir, "");

     // If sName is absolute, flush ...//.
     if (! sName.empty ()
	 && (sName [0] == '/'
 #ifdef WINDOWS
	     || (sName.length () >= 3
		 && isalpha (sName [0])
		 && sName [1] == ':'
		 && sName [2] == '/')
 #endif
	     )) {
	 i = 0;
	 while (i < sName.length ()) {
	     // The path is known to be absolute, so we know the elements are separated by a '/'.
	     // If we encounter '//', we will just start again from that.

	     if (sName [i] == '/'
		 && i+1 < sName.length ()
		 && sName [i+1] == '/') {
		 sName.remove (0, i+1);
		 i = 0;
	     }

	     // Check `~'
	     if (sName [i] == '/'
		 && i+1 < sName.length ()
		 && sName [i+1] == '~') {
		 sName.remove (0, i+1);
		 i = 0;
	     }

	     i++;
	 }
     }

     // Determine the directory to start with and put it in pszDirectory.
     String	sDirectory;
     char	*p;

     // Check `~' prefix
     if (! sName.empty () && sName [0] == '~') {
	 if (sName [1] == '/'
	     || sName [1] == '\0') {
	     // ~ by itself
	     if (! (p = ::getenv ("HOME")))
		 sDirectory = "";
	     else
		 sDirectory = p;

 #ifdef WINDOWS
	     ::NormalizeWindowsPath (sDirectory, true);
 #endif
	     sName.remove (0, 1);
	 }
	 else {
	     // ~user
	     for (int i = 1; sName [i] && sName [i] != '/'; i++)
		 ;
 #ifndef WINDOWS
	     string sUser (sName, 1, i);

	     struct passwd *pEntry = ::getpwnam (sUser);
	     if (pEntry) {
		 sDirectory = pEntry->pw_dir;
		 sName.remove (0, i);
	     }
 #endif // WINDOWS
	     // If no user by that name was found, leave it there.
	 }
     }

     if (! sName.empty ()
	 && (
 #ifndef WINDOWS
	     sName [0] != '/'
 #else
	     ! (sName.length () >= 3
		&& isalpha (sName [0])
		&& sName [1] == ':'
		&& sName [2] == '/')
 #endif
	     )
	 && sDirectory.empty ())
	 sDirectory = sDefaultDir;

     string	sTarget;

     if (! sDirectory.empty ()) {
	 // Get rid of the trailing slash
	 int len = sDirectory.length ();

	 // If the home directory is the root directory, leave the '/'
	 if (len > 1 && sDirectory [len - 1] == '/')
	     sDirectory.remove (len-1, 1);
     }

     if (! sDirectory.empty ()) {
	 sTarget = sDirectory;
	 if (! (sName.empty ()
 #ifndef WINDOWS
		|| sName [0] == '/'
 #else
		    || (sName.length () >= 3
			&& isalpha (sName [0])
			&& sName [1] == ':'
			&& sName [2] == '/')
 #endif
		    ))
	     sTarget += '/';
     }

     sTarget += sName;

     // Canonicalize bt removing /. and /foo/.. if they appear.
     i = 0;

     while (i < sTarget.length ()) {
	 if (sTarget [i] != '/')
	     i++;

	 else if (i < sTarget.length ()-1
		  && sTarget [i] == '/'
		  && sTarget [i+1] == '/') {
 #ifndef WINDOWS
	     sTarget.remove (0, i+1);
	     i = 0;
 #else
	     sTarget.remove (2, i+1);
	     i = 2;
 #endif
	 }
	 else if ((i <= sTarget.length () - 3
		   && sTarget [i] == '/'
		   && sTarget [i+1] == '.'
		   && sTarget [i+2] == '/')
		  || (i == sTarget.length () - 2
		      && sTarget [i] == '/'
		      && sTarget [i+1] == '.')) {
	     // If `/.' is the entire filename, keep the `/'. Otherwise, delete both.
	     if (
 #ifndef WINDOWS
		 i == 0 && sTarget [i+2] == '\0'
 #else
		 i == 2 && sTarget [i+2] == '\0'
 #endif
		 ) {
 #ifndef WINDOWS
		 sTarget.remove (1, 1);
		 i = 1;
 #else
		 sTarget.remove (3, 1);
		 i = 3;
 #endif
	     }
	     else
		 sTarget.remove (i, 2);
	 }
	 else if (i <= sTarget.length () - 3
		  && sTarget [i] == '/'
		  && sTarget [i+1] == '.'
		  && sTarget [i+2] == '.'
		  // "/.." is the "superroot" on some file systems.
 #ifndef WINDOWS
		  && i != 0
 #else
		  && i != 2
 #endif
		  && (i <= sTarget.length () - 4 && sTarget [i+3] == '/'
		      || i == sTarget.lentgth () - 3)) {
	     for (int j = i; j != 0 && sTarget [j] != '/'; j--)
		 ;

	     if (j == 0 && sTarget [j] == '/')
		 j++;

	     sTarget.remove (j, i - j + 3);
	     i = j;
	 }
	 else
	     i++;
     }

 #ifdef WINDOWS
     // Check that there is drive id at the beginning
     NormalizeWindowsPath (sTarget, false);
     ::AnsiLower ((char *) sTarget.c_str ());
 #endif
     return sTarget;
 }
 #endif // 0

 //////////////////////////////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////////
 #if 0
 bool File::Status::readable (void) const
 {
     return ((UID () == m_uid && m_nMode & S_IRUSR)
	     || (GID () == m_gid && m_nMode & S_IRGRP)
	     || (m_nMode & S_IROTH));
 }

 bool File::Status::writable (void) const
 {
     return ((UID () == m_uid && m_nMode & S_IWUSR)
	     || (GID () == m_gid && m_nMode & S_IWGRP)
	     || (m_nMode & S_IWOTH));
 }

 bool File::Status::executable (void) const
 {
     return ((UID () == m_uid && m_nMode & S_IXUSR)
	     || (GID () == m_gid && m_nMode & S_IXGRP)
	     || (m_nMode & S_IXOTH));
 }

 bool File::Status::accessibleDirectory (void) const
 {
     return m_eType == Directory && executable ();
 }

 //////////////////////////////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////////

 void File::rename (const char *pszFileName, const char *pszNewName)
 {
 #ifdef WINDOWS
     char pszFile [PATH_MAX], pszNew [PATH_MAX];

     if (::strlen (pszFileName) >= PATH_MAX)
	 throw FileException (ENAMETOOLONG);

     if (::strlen (pszNewName) >= PATH_MAX)
	 throw FileException (ENAMETOOLONG);

     ::AnsiToOem (sFileName, pszFile);
     ::AnsiToOem (sNewName, pszNew);
 #else
     const char *pszFile = pszFileName;
     const char *pszNew = pszNewName;
 #endif

     int iError = ::rename (pszFile, pszNew);

     if (iError)
	 throw FileException (iError);
 }


 void File::copy (const char *pszTo, const char *pszFrom, bool bKeepDate /* = false */)
 {
     // We can only copy regular files and symbolic links.  Other files are not
     // copyable by us.

     Status	stat;
     class File	input (pszFrom, Read | Binary);

     bool bInputFileStatable = input.status (stat);

     if (bInputFileStatable
	 && stat.m_eType != Regular
	 && stat.m_eType != Symlink)
	 throw FileException (EISDIR);

     class File	output (pszTo, Write | Create | Truncate | Binary);
     char	szBuffer [2048];
     size_t	n;

     while ((n = input.read (szBuffer, sizeof (szBuffer))) > 0)
	 output.write (szBuffer, n);

     input.close ();
     output.close ();

     if (bInputFileStatable) {
	 Status	s;
	 status (pszTo, s);

	 if (bKeepDate) {
	     s.m_dtAccess = stat.m_dtAccess;
	     s.m_dtModify = stat.m_dtModify;
	 }

	 s.m_eType = stat.m_eType;
	 s.m_nAttribute = stat.m_nAttribute;
	 s.m_nMode = stat.m_nMode;
	 s.m_bSetUser = stat.m_bSetUser;
	 s.m_bSetGroup = stat.m_bSetGroup;
	 setStatus (pszTo, s);
     }
 }

 void File::remove (const char *pszFileName, bool bForce /* = false */)
 {
 #ifdef WINDOWS
     char pszFile [PATH_MAX];

     if (::strlen (pszFileName) >= PATH_MAX)
	 throw FileException (ENAMETOOLONG);

     ::AnsiToOem (pszFileName, pszFile);
 #else
     const char *pszFile = pszFileName;
 #endif

     int iError = ::remove (pszFile);
     if (iError && ! bForce)
	 throw FileException (iError);
 }

 bool File::exists (const char *pszName)
 {
     Status stat;

     return status (pszName, stat);
 }

 string File::temporary (void)
 {
     char	*pszTemp = ::tempnam (NULL, NULL);
     if (! pszTemp)
	 return "";
     else {
	 string s (pszTemp);
	 delete pszTemp;
	 return s;
     }
 }

 //////////////////////////////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////////
 bool
 File::status (const char *pszFileName, Status &rStatus,
	       bool bTraverseLinks /* = false */)
 {
     ASSERT (IsValidString (pszFileName));

     // Cannot get name from fd
     rStatus.m_sName = pszFileName;

     struct stat		buf;

     if (
 #ifndef WINDOWS
	 (bTraverseLinks && ::stat (rStatus.m_sName.c_str (), &buf) != -1)
 #else
	 ::stat (rStatus.m_sName.c_str (), &buf) != -1
 #endif
 #ifndef WINDOWS
	 || (::lstat (rStatus.m_sName.c_str (), &buf) != -1)
 #endif
	 ) {
	 rStatus.m_dtCreate = buf.st_ctime;
	 rStatus.m_dtModify = buf.st_mtime;
	 rStatus.m_dtAccess = buf.st_atime;

	 rStatus.m_nLinks = buf.st_nlink;
 #ifndef WINDOWS
	 rStatus.m_uid = buf.st_uid;
	 rStatus.m_gid = buf.st_gid;
 #else
	 rStatus.m_uid = UID ();
	 rStatus.m_gid = GID ();
 #endif
	 rStatus.m_nSize = buf.st_size;

	 rStatus.m_eType =
	     (S_ISREG (buf.st_mode) ? Regular :
	      S_ISDIR (buf.st_mode) ? Directory :
	      S_ISSOCK (buf.st_mode) ? SocketStream :
 #ifndef WINDOWS
	      S_ISLNK (buf.st_mode) ? Symlink :
	      S_ISNWK (buf.st_mode) ? NetworkSpecial :
 #endif
	      S_ISBLK (buf.st_mode) ? BlockSpecial :
	      S_ISCHR (buf.st_mode) ? CharacterSpecial :
	      S_ISFIFO (buf.st_mode) ? FIFOSpecial :
	      Regular);
	 rStatus.m_nMode = buf.st_mode & 0777;
 #ifdef S_ISCDF
	 rStatus.m_nAttribute = S_ISCDF (buf.st_mode) ? Hidden : Normal;
 #elif defined (WINDOWS)
	 ::_dos_getfileattr (rStatus.m_sName, &rStatus.m_nAttribute);
	 // Remove read-only flag (this was already returned by `stat'
	 if (rStatus.m_nAttribute & _A_RDONLY)
	     rStatus.m_nAttribute &= ~_A_RDONLY;

	 // Check directory, it is considered executable in DOS. Networked systems
	 // will not even show the directory if there are no permissions for it.
	 if (rStatus.m_nAttribute & _A_SUBDIR) {
	     rStatus.m_nMode |= S_IXUSR;
	     rStatus.m_nAttribute &= ~_A_SUBDIR;
	 }

 #if 0
	 // Check executability - would there be any faster methods for this?
	 if (rStatus.m_sName.length () >= 4 && rStatus.m_eType != Directory) {
	     string sTrailer (rStatus.m_sName.from ((int)rStatus.m_sName.length () - 4));

	     sTrailer.downcase ();
	     if (sTrailer == ".exe"
		 || sTrailer == ".com"
		 || sTrailer == ".bat"
		 || sTrailer == ".pif")
		 rStatus.m_nMode |= S_IXUSR;
	 }
 #endif // 0
 #else
	 rStatus.m_nAttribute = Normal;
 #endif
	 rStatus.m_bSetUser = buf.st_mode & S_ISUID;
	 rStatus.m_bSetGroup = buf.st_mode & S_ISGID;

	 return true;
     }
     else
	 return false;
 }

 void
 File::setStatus (const char *pszFileName, const Status &rStatus)
 {
     Status	stat;
     status (pszFileName, stat);


 #ifndef WINDOWS
     // Change uid
     if (rStatus.m_uid != stat.m_uid && rStatus.m_uid != -1)
	 if (::chown (pszFileName, rStatus.m_uid, (gid_t) -1) == -1)
	     throw SystemError (errno);
	 else
	     stat.m_uid = rStatus.m_gid;

     // Change gid
     if (rStatus.m_gid != stat.m_gid && rStatus.m_gid != -1)
	 if (::chown (pszFileName, (uid_t) -1, rStatus.m_gid) == -1)
	     throw SystemError (errno);
	 else
	     stat.m_gid = rStatus.m_gid;
 #endif

     // Change mode, if currently not writable
 #ifdef WINDOWS
     // Set the attribute: in Windows, this more like mode
     UINT	nAttrOld;
     UINT	nAttrNew;

     nAttrNew = rStatus.m_nAttribute;
     if (rStatus.m_eType == Directory) nAttrNew |= _A_SUBDIR;
     if (! rStatus.writable ()) nAttrNew |= _A_RDONLY;

     if (::_dos_getfileattr (sFileName, &nAttrOld) != 0)
	 throw SystemError (errno);
    
    // Check if the attribute is changed and the file is read only. If it is, set the
    // attribute so that we may set the other attributes too.
    if (nAttrOld != nAttrNew && (nAttrOld & _A_RDONLY)) {
	if (::_dos_setfileattr (sFileName, nAttrOld) != 0)
	    throw SystemError (errno);
    }
#endif
    // Note: in Windows, this is sharing, not mode
    if (CombineModes (rStatus.m_nMode, rStatus.m_eType, rStatus.m_nAttribute,
		      rStatus.m_bSetUser, rStatus.m_bSetGroup)
	!= CombineModes (stat.m_nMode, stat.m_eType, stat.m_nAttribute,
			 stat.m_bSetUser, stat.m_bSetGroup)
	&& ! stat.writable ())
	if (::chmod (pszFileName, rStatus.m_nMode) == -1)
	    throw SystemError (errno);
    
    // Change dates
    if (rStatus.m_dtAccess.getTime () != 0
	|| rStatus.m_dtModify.getTime () != 0) {
	struct utimbuf	timbuf;

	timbuf.actime = (rStatus.m_dtAccess.getTime () != 0 ?
			 rStatus.m_dtAccess.getTime () :
			 stat.m_dtAccess.getTime ());

	timbuf.modtime = (rStatus.m_dtModify.getTime () != 0 ?
			  rStatus.m_dtModify.getTime () :
			  stat.m_dtModify.getTime ());

	if (::utime (pszFileName, &timbuf) == -1)
	    throw SystemError (errno);
    }
    // Change mode, if currently writable (this is sharing in Windows)
    if (CombineModes (rStatus.m_nMode, rStatus.m_eType, rStatus.m_nAttribute,
		      rStatus.m_bSetUser, rStatus.m_bSetGroup)
	!= CombineModes (stat.m_nMode, stat.m_eType, stat.m_nAttribute,
			 stat.m_bSetUser, stat.m_bSetGroup)
	&& stat.writable ())
	if (::chmod (pszFileName, rStatus.m_nMode) == -1)
	    throw SystemError (errno);

#ifdef WINDOWS
    // Set the attribute now, if the file was't read-only (this is mode in Windows)
    if (nAttrOld != nAttrNew && ! (nAttrOld & _A_RDONLY))
	if (::_dos_setfileattr (pszFileName, nAttrNew) != 0)
	    throw SystemError (errno);
#endif
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
opennull ()
{
    // FIXME: REmove
    char szTempName [L_tmpnam];
    if (! pszFileName) {
	pszFileName = ::tmpnam (szTempName);
	nFlags = Read | Write | Create | Truncate;
	nPermissions = 0666;
    }
}

#endif // 0
