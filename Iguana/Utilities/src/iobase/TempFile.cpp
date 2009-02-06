//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/TempFile.h"
#include "classlib/iobase/File.h"
#include "classlib/iobase/FileSystemError.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/sysapi/TempFile.h"
#include <cstdlib>

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

bool
TempFile::tryTempDir (Filename &name, Filename candidate)
{
    if (! candidate.empty ()
	&& candidate.isDirectory ()
	&& ::access (candidate, R_OK | W_OK | X_OK) >= 0)
    {
	name = candidate;
	return true;
    }
    else
	return false;
}


/** @fn Filename TempFile::tempdir (void)

    Return the temporary file directory.  */

/** Create a named temporary file.

    This method attempts to create a uniquely named temporary file.
    If successful, it returns a pointer to an empty file open for
    reading and writing, and @a name has been modified to the name of
    the file.  The client should close and delete the file when it no
    longer needs it -- it will not be deleted automatically.  If the
    client needs the file but does not want it to exist in the file
    system (i.e. to create a file that no longer has a name in the
    file system and thus cannot be accessed except through the
    returned file object), simply call #remove() with @a name; the
    returned #File remains usable and will be deleted when closed or
    when the process exits, but will no longer have a name in the file
    system.

    On entry, @a name can be empty, in which case a unique name in
    #tempdir() is generated automatically.  If @a name is non-empty,
    it is used a prefix to which a unique suffix is appended to.  Note
    that no directory component is added to it, so if you want to
    create a file name with a particular prefix in #tempdir(), you
    will need to compose the desired name yourself.  Similarly, no
    slash is appended to the name, so if you are passing a name of
    a directory, be sure to provide the #asDirectory() form of it.

    The creation of the temporary file is guaranteed to be secure: it
    will not overwrite existing files or be otherwise subject to race
    conditions.  This is the reason for simultaneous return of both
    the open file and the name.  The file will be created with
    permissions 0600.

    In case of failing to properly create a temporary file, an
    appropriate #SystemError is thrown.  */
File *
TempFile::file (Filename &name)
{
    if (name.empty ())
	name = tempdir ().asDirectory ();

    name.append ("XXXXXX");

    // Null-terminate the name and try to create the file.  Our WIN32
    // replacement implementation returns a real Windows file handle,
    // not a CRTDLL file descriptor, so we can use it as it is.  It
    // also sets errno to Windows error codes, not CRTDLL ones.
    IOFD		fd;
    std::string		result (name);

    result.c_str ();
    if ((fd = domkstemp (&result [0])) == IOFD_INVALID)
	throw FileSystemError ("mkstemp()", errno);

    name = result;
    return new File (fd);
}

/** Create a named temporary directory.

    This is like #mktempfile except that instead of creating a file,
    it securely creates a named temporary directory with permissions
    0700.  @a name is modified to the name of the direcotory that has
    been created.  */
void
TempFile::dir (Filename &name)
{
    if (name.empty ())
	name = tempdir ().asDirectory ();

    name.append ("XXXXXX");

    // Null-terminate the name and try to create the directory.  Our
    // WIN32 replacement implementation sets errno to Windows error
    // codes, not CRTDLL ones.
    std::string result (name);
    result.c_str ();
    if (! domkdtemp (&result [0]))
	throw FileSystemError ("mkdtemp()", errno);

    name = result;
}

//////////////////////////////////////////////////////////////////////
/** @fn bool TempFile::domktemp (char *path, IOFD *fd, bool domkdir, int suffixlen)

    Find a unique file name based on template @a path in a secure way.
    A trailing suffix of length @a suffixlen is ignored.  The function
    can be used in one of three ways: to create a uniquely named file
    (mkstemp), to create a uniquely named directory (mkdtemp), or to
    simply look for a unique file name (mktemp).  The last mode should
    not be used at all, it is insecure.

    The @a path must contain at least six consequtive "X" letters at
    the end; if @a suffixlen is non-zero, then that many characters
    are ignored at the end, and the six "X" letters must come before
    those.  More "X"'s are used if given.  If there are not enough
    "X"'s, the function returns @c false and sets @c errno to EINVAL.
    If the target directory indicated in @a path does not exist, the
    function will return @c false and set @c errno accordingly.  If
    the function fails to create a unique name

    If the function succeeds, it returns @c true and @a path will have
    been modified to the unique name of the file or the directory.  If
    @a fd was non-null (indicating a file to be created), it contains
    the handle for the file opened for reading and writing exclusively
    by the user.  If dodir was @c true (indicating a directory to be
    created) the directory exists.  (If neither @a fd nor @a dodir are
    given, @a path contains the name that was unique when the function
    checked it, which is perfectly useless information.)

    If the function fails it returns @c false and @a path will have
    been mangled.  If the function fails to find a unique name in
    TMP_MAX attempts it sets @c errno to EEXIST.  If the creation of
    the file or the directory (or check for the name for the useless
    version) fails for a reason other than a name conflict, the
    function returns @c false and leaves @c errno to indicate the
    reason for the failure.  */

/** Create a unique temporary directory.  */
char *
TempFile::domkdtemp (char *path)
{
#if HAVE_MKDTEMP
    return mkdtemp (path);
#else
    return domktemp (path, 0, true, 0) ? path : 0;
# endif
}

/** Find a temporary file name.  Never use this.  */
char *
TempFile::domktemp (char *path)
{
#if HAVE_MKTEMP
    return mktemp (path);
#else
    return domktemp (path, 0, false, 0) ? path : 0;
#endif
}

/** Create a unique temporary file.  */
IOFD
TempFile::domkstemp (char *path)
{
#if HAVE_MKSTEMP
    return mkstemp (path);
#else
    IOFD fd; return domktemp (path, &fd, false, 0) ? fd : IOFD_INVALID;
# endif
}

/** Create a unique temporary file.  */
IOFD
TempFile::domkstemps (char *path, int suffixlen)
{
#if HAVE_MKSTEMPS
    return mkstemps (path, suffixlen);
#else
    IOFD fd; return domktemp (path, &fd, false, suffixlen) ? fd : IOFD_INVALID;
# endif
}

} // namespace lat
