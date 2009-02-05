#ifndef CLASSLIB_FILENAME_H
# define CLASSLIB_FILENAME_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/iobase/FileAcl.h"
# include "classlib/utils/Time.h"
# include <string>
# include <vector>
# include <iosfwd>

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class Filename;
class FileAcl;
class File;
class Time;
class IOStatus;
class logstream;
class Environment;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>

logstream    &operator<<(logstream &out, const Filename &name);
std::ostream &operator<<(std::ostream &out, const Filename &name);

//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** File name object that hides the gory operating system details.

    Much of this logic comes from Emacs.  Properly used the interface
    is portable to a large variety of systems, including unix, windows
    and vms (vms is not supported by the implementation however).  The
    most important detail for clients to care about is to distinguish
    directories as <i>file names</i> and <i>directory names</i>.  When
    operations are perfomed on the directory itself or the name of the
    directory is presented to the user, the application should use the
    file name form.  When concatenating file name components together,
    the application should always use the directory name form, and
    directly concatenate (with append()) the file name part.  The
    methods of this class always perform the appropriate conversions
    and thus conveniently hide many of the details.

    All file names are expanded (see #expand()) when used internally.
    If the underlying system uses different character sets for the
    file system and the applications, the names are automatically
    converted: the names accepted by this interface are always in the
    character set most natural to the application.  */
class Filename
{
public:
    typedef std::vector< std::pair<std::string, std::string> > AbbrevList;

    Filename (void);
    Filename (const char *name);
    Filename (const std::string &name);
    Filename (const Filename &dir, const Filename &file,bool appendonly=false);
    // implicit copy constructor
    // implicit destructor
    // implicit assignment operator

    // Comparisons
    friend bool operator== (const Filename &x, const Filename &y);
    friend bool operator== (const char *x, const Filename &y);
    friend bool operator== (const std::string &x, const Filename &y);
    friend bool operator== (const Filename &x, const char *y);
    friend bool operator== (const Filename &x, const std::string &y);

    friend bool operator!= (const Filename &x, const Filename &y);
    friend bool operator!= (const char *x, const Filename &y);
    friend bool operator!= (const std::string &x, const Filename &y);
    friend bool operator!= (const Filename &x, const char *y);
    friend bool operator!= (const Filename &x, const std::string &y);

    friend bool operator< (const Filename &x, const Filename &y);
    friend bool operator< (const char *x, const Filename &y);
    friend bool operator< (const std::string &x, const Filename &y);
    friend bool operator< (const Filename &x, const char *y);
    friend bool operator< (const Filename &x, const std::string &y);

    friend bool operator<= (const Filename &x, const Filename &y);
    friend bool operator<= (const char *x, const Filename &y);
    friend bool operator<= (const std::string &x, const Filename &y);
    friend bool operator<= (const Filename &x, const char *y);
    friend bool operator<= (const Filename &x, const std::string &y);

    friend bool operator> (const Filename &x, const Filename &y);
    friend bool operator> (const char *x, const Filename &y);
    friend bool operator> (const std::string &x, const Filename &y);
    friend bool operator> (const Filename &x, const char *y);
    friend bool operator> (const Filename &x, const std::string &y);

    friend bool operator>= (const Filename &x, const Filename &y);
    friend bool operator>= (const char *x, const Filename &y);
    friend bool operator>= (const std::string &x, const Filename &y);
    friend bool operator>= (const Filename &x, const char *y);
    friend bool operator>= (const Filename &x, const std::string &y);

    // Basic access to the name
    bool		empty (void) const;
    std::string::size_type size (void) const;
    void		resize (std::string::size_type n, char c = '\0');
    /**/		operator const char * (void) const;
    // /**/		operator const std::string & (void) const;
    char &		operator[] (ptrdiff_t n);
    char		operator[] (ptrdiff_t n) const;
    const char *	name (void) const;

    // Various checks about the name
    bool		isAbsolute (bool considerExpansions = false) const;
    bool		isRelative (void) const;

    bool		exists (void) const;
    bool		isDirectory (void) const;
    bool		isRegular (void) const;
    bool		isSymlink (Filename *target = 0) const;

    bool		isAccessibleDirectory (void) const;
    bool		isOwnershipPreserved (void) const;
    bool		isReadable (void) const;
    bool		isWritable (void) const;
    bool		isExecutable (void) const;

    bool		status (IOStatus &s, bool traverseLinks = true) const;
    void		setStatus (const IOStatus &s) const;
    bool		times (Time *ctime, Time *mtime, Time *atime) const;

    // Name variants: representations
    Filename		asDirectory (void) const;
    Filename		asFile (void) const;

    // Name variants: basic parts
    Filename		drive (void) const;	// Win32 only
    Filename		share (void) const;	// Win32 only

    Filename		directory (void) const;
    Filename		nondirectory (void) const;
    Filename		relativeTo (const Filename &dir) const;
    Filename		extension (bool *has = 0, bool period = false) const;
    Filename		sansExtension (void) const;

    Filename &		append (const char *component);
    Filename &		append (const std::string &component);

    // Conversions: system translations
    Filename		truename (void) const;
    Filename		chaseLinks (void) const;

    // Name variants: autosave
    Filename		autosaveName (void) const;
    bool		isAutosaveName (void) const;

    // Name variants: backup
    Filename		findBackupName (std::vector<Filename> &deletions) const;
    Filename		backupName (void) const;
    bool		isBackupName (void) const;
    Filename		sansVersions (bool keepVersionNr = false) const;

    // Conversions: logical
    Filename		expand (const Filename &defaultDir = 0) const;
    Filename		substitute (const Environment &env) const;
    Filename		abbreviate (const AbbrevList &abbrevs) const;
    Filename		abbreviate (void) const;

    // Completion and globbing
    bool		complete (const Filename &dir, bool *exact);
    size_t		complete (std::vector<Filename> &list,
				  const Filename &dir) const;
    size_t		glob (std::vector<Filename> &list,
			      const Filename &dir,
			      bool absolute = false) const;

    // File (system) operations (FIXME: nonstatic?)
    static void		symlink (Filename target, Filename link);
    static void		makedir (Filename name,
				 FileAcl mode = 0755,
				 bool recursive = false,
				 bool allowExisting = false);
    static void		remove (Filename name,
				bool recursive = false,
				bool force = false);
    static void		rename (Filename name, Filename to);
    static void		move (Filename from, Filename to);
    static void		copy (Filename from, Filename to,
			      bool keepDate = false);

    // Global state
    static Filename	cwd (void);
    static Filename	cwd (char drive);

    // System details
    static Filename	null (void);
    static Filename	root (void);
    static Filename	updir (void);
    static Filename	thisdir (void);

private:
    void		normalise (void);

    static Filename	abbreviate (std::string name);
    static void		expandUser (std::string &namestr,
				    std::string::size_type pos,
				    std::string &dirstr);
    void		expandPrefix (Filename &name, const Filename &defdir);
    static void		expandClean (std::string &namestr, bool strip);

    // Implementation-specific functions
    static bool		domakedir (Filename name, FileAcl mode, bool allowExisting);
    static int		doremove (const char *path);

    std::string		m_name;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_FILENAME_H
