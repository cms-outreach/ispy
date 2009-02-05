#ifndef CLASSLIB_SYSAPI_ELF_ABI_H
# define CLASSLIB_SYSAPI_ELF_ABI_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"
# ifndef _WIN32
#  if HAVE_LOADER_H
#   include <loader.h>
#  endif
#  if HAVE_LINK_H
#   include <link.h>
#   include <limits.h>
#   include <sys/stat.h>
#   include <unistd.h>
#  endif
#  if HAVE_SGIDEFS_H	// irix n32, 64
#   include <sgidefs.h>
#   include <objlist.h>
#   include <obj_list.h>
#   include <obj.h>
#  endif
#  if HAVE_ELF_H
#   include <elf.h>
#  endif
# endif // ! _WIN32

//<<<<<< PUBLIC DEFINES                                                 >>>>>>

#if /* irix */       (defined ABI64 || defined _ABI64 ||		\
 		      (defined _MIPS_SIM && _MIPS_SIM == _MIPS_SIM_ABI64)) \
    /* solaris */ || (defined sparcv9 || defined _sparcv9		\
		      || defined __sparcv9 || defined __sparcv9__)	\
    /* tru64 */   || (defined arch64 || defined _arch64 ||		\
		      defined __arch64 || defined __arch64__)
# define ELF_ABI 64
#else
# define ELF_ABI 32
#endif
#ifndef ElfW
# define ElfW(type)   ElfW1(Elf,ELF_ABI,type)
# define ElfW1(e,w,t) ElfW2(Elf,w,_##t)
# define ElfW2(e,w,t) e ## w ## t
#endif

//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>
//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // CLASSLIB_SYSAPI_ELF_ABI_H
