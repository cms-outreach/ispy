#ifndef CLASSLIB_SYSAPI_IO_STATUS_H
# define CLASSLIB_SYSAPI_IO_STATUS_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"
# ifdef _WIN32
#  include <windows.h>
# else
#  include <unistd.h>
#  include <sys/stat.h>
#  include <fcntl.h>
# endif

//<<<<<< PUBLIC DEFINES                                                 >>>>>>

# ifndef S_ISDIR
#  define S_ISDIR(_M)		 ((_M & S_IFMT)==S_IFDIR)
#  define S_ISCHR(_M)		 ((_M & S_IFMT)==S_IFCHR)
#  define S_ISBLK(_M)		 ((_M & S_IFMT)==S_IFBLK)
#  define S_ISREG(_M)		 ((_M & S_IFMT)==S_IFREG)
#  define S_ISFIFO(_M)		 ((_M & S_IFMT)==S_IFIFO)
#  define S_ISSOCK(_M)		 ((_M & S_IFMT)==S_IFSOCK)
#  define S_ISLNK(_M) 		 ((_M & S_IFMT)==S_IFLNK)
# endif

# ifndef S_IFNWK
#  define S_IFNWK		0
#  define S_ISNWK(_M)		(0)
# endif

# ifndef S_IFCDF
#  define S_IFCDF		0
#  define S_ISCDF(_M)		(0)
# endif

# ifndef S_TYPEISMQ
#  define S_TYPEISMQ(b)		(0)
# endif

# ifndef S_TYPEISSEM
#  define S_TYPEISSEM(b)	(0)
# endif

# ifndef S_TYPEISSHM
#  define S_TYPEISSHM(b)	(0)
# endif

# ifndef S_TYPEISMO
#  define S_TYPEISMO(b)		(0)
# endif

//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>
//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // CLASSLIB_SYSAPI_IO_STATUS_H
