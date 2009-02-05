#ifndef IG_NTUPLE_MODEL_CONFIG_H
# define IG_NTUPLE_MODEL_CONFIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>

/** @def IG_NTUPLE_MODEL_API
  @brief A macro that controls how entities of this shared library are
         exported or imported on Windows platforms (the macro expands
         to nothing on all other platforms).  The definitions are
         exported if #IG_NTUPLE_MODEL_BUILD_DLL is defined, imported
         if #IG_NTUPLE_MODEL_BUILD_ARCHIVE is not defined, and left
         alone if latter is defined (for an archive library build).  */

/** @def IG_NTUPLE_MODEL_BUILD_DLL
  @brief Indicates that the header is included during the build of
         a shared library of this package, and all entities marked
	 with #IG_NTUPLE_MODEL_API should be exported.  */

/** @def IG_NTUPLE_MODEL_BUILD_ARCHIVE
  @brief Indicates that this library is or was built as an archive
         library, not as a shared library.  Lack of this indicates
         that the header is included during the use of a shared
         library of this package, and all entities marked with
         #IG_NTUPLE_MODEL_API should be imported.  */

# ifndef IG_NTUPLE_MODEL_API
#  ifdef _WIN32
#    if defined IG_NTUPLE_MODEL_BUILD_DLL
#      define IG_NTUPLE_MODEL_API __declspec(dllexport)
#    elif ! defined IG_NTUPLE_MODEL_BUILD_ARCHIVE
#      define IG_NTUPLE_MODEL_API __declspec(dllimport)
#    endif
#  endif
# endif

# ifndef IG_NTUPLE_MODEL_API
#  define IG_NTUPLE_MODEL_API
# endif

#endif // IG_NTUPLE_MODEL_CONFIG_H
