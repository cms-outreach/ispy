#ifndef CLASSLIB_SYSAPI_SHARED_LIBRARY_H
# define CLASSLIB_SYSAPI_SHARED_LIBRARY_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/ElfAbi.h"
# ifdef _WIN32
#  include <windows.h>
#  include <winnt.h>
#  include <imagehlp.h>
# else
#  if HAVE_DLOPEN 
#   include <dlfcn.h>
#  elif HAVE_SHL_LOAD
#   include <dl.h>
#  elif HAVE_LOAD
#   include "utils/dlfcn.h"
#  endif
#  if HAVE_LOADER_H
#   include <loader.h>
#  endif
#  if HAVE_LINK_H
#   include <link.h>
#   include <limits.h>
#   include <sys/stat.h>
#   include <unistd.h>
#  endif
#  if HAVE_ELF_H
#   include <elf.h>
#  endif
#  if HAVE_SGIDEFS_H	// irix n32, 64
#   include <sgidefs.h>
#   include <objlist.h>
#   include <obj_list.h>
#   include <obj.h>
#  endif
#  if HAVE_MACH_O_DYLD_H // darwin
#   include <mach-o/dyld.h>
#   include <mach-o/getsect.h>
#  endif
# endif // _WIN32
# include <cstdio>
# include <cstdlib>
# include <errno.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>

#if HAVE_LOAD
# define PATH		"LIBPATH"
#elif HAVE_DLOPEN
# if defined __APPLE__ && defined __MACH__
#  define PATH		"DYLD_LIBRARY_PATH"
# else
#  define PATH		"LD_LIBRARY_PATH"
# endif
#elif HAVE_SHL_LOAD
# define PATH		"SHLIB_PATH"
#elif defined _WIN32
# define PATH		"PATH"
#else
# define PATH		0
#endif

//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>
//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // CLASSLIB_SYSAPI_SHARED_LIBRARY_H
