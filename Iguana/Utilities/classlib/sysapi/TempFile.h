#ifndef CLASSLIB_SYSAPI_TEMP_FILE_H
# define CLASSLIB_SYSAPI_TEMP_FILE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include <cerrno>
# ifdef _WIN32
#  include <windows.h>
#  include "classlib/sysapi/WindowsIO.h"
# else
#  include <unistd.h>
#  include <sys/stat.h>
#  include <fcntl.h>
# endif
# include <cstdio>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>

#ifndef TMP_MAX
# define TMP_MAX 16384
#endif

//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>

# if !HAVE_MKDTEMP || !HAVE_MKDTEMP_DECL
extern "C" char *mkdtemp (char *path);
# endif

# if !HAVE_MKSTEMP || !HAVE_MKSTEMP_DECL
extern "C" lat::IOFD mkstemp (char *path);
# endif

//<<<<<< CLASS DECLARATIONS                                             >>>>>>
//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // CLASSLIB_SYSAPI_TEMP_FILE_H
