#ifndef CLASSLIB_SYSAPI_FILENAME_H
# define CLASSLIB_SYSAPI_FILENAME_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/IOTypes.h"
# include <cerrno>
# ifdef _WIN32
#  include <windows.h>
#  include <direct.h>
#  include "classlib/sysapi/WindowsIO.h"
# else
#  include <unistd.h>
#  include <sys/stat.h>
#  include <fcntl.h>
#  if HAVE_GLOB
#   include <glob.h>
#  endif
# endif

//<<<<<< PUBLIC DEFINES                                                 >>>>>>

/// File name component separator (for systems with a simple convention)
# ifdef _WIN32

/** Valid driver letters.  */
static const char s_drives [] = ("abcdefghijklmnopqrstuvwxyz"
				 "ABCDEFGHIJKLMNOPQRSTUVWXYZ");

/** Check if @a x is a drive letter.  */
#  define ISDRIVECHR(x)	(strchr (s_drives, (x)))

/** Check if the pointer @a p points to a directory separator.  */
#  define ISPATHSEP(p)	(*(p) == '\\' || *(p) == '/')

/** Check if the pointer @a p points to a Windows share ('\\').  */
#  define ISSHARE(p)	(ISPATHSEP(p) && ISPATHSEP(p+1))

/** Check if the pointer @a p points a Windows drive spec (`x:').  */
#  define ISDRIVE(p)	(ISDRIVECHR(*(p)) && *((p)+1) == ':')

/** Path separator character to append to strings.  */
#  define PATHSEP	'\\'

/** Indicate whether Windows special rules should be used (yes).  */
#  define ISWIN32	true

// Map to hidden names
#  define access	_access
#  define unlink	_unlink
# else // ! _WIN32

/** Check if the pointer @a p points to a directory separator.  */
#  define ISPATHSEP(p)	(*(p) == '/')

/** Check if the pointer @a p points to a Windows share (no).  */
#  define ISSHARE(p)	false

/** Check if the pointer @a p points to a Windows drive spec (no).  */
#  define ISDRIVE(p)	false

/** Path separator character to append to strings.  */
#  define PATHSEP	'/'

/** Indicate whether Windows special rules should be used (no).  */
#  define ISWIN32	false

# endif // _WIN32

//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>
//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // CLASSLIB_SYSAPI_FILENAME_H
