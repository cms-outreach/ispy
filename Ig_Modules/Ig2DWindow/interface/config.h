#ifndef IG_2D_WINDOW_CONFIG_H
# define IG_2D_WINDOW_CONFIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>

/** @def IG_2D_WINDOW_API
  @brief A macro that controls how entities of this shared library are
         exported or imported on Windows platforms (the macro expands
         to nothing on all other platforms).  The definitions are
         exported if #IG_2D_WINDOW_BUILD_DLL is defined, imported
         if #IG_2D_WINDOW_BUILD_ARCHIVE is not defined, and left
	 alone if latter is defined (for an archive library build).  */

/** @def IG_2D_WINDOW_BUILD_DLL
  @brief Indicates that the header is included during the build of
         a shared library of this package, and all entities marked
	 with #IG_2D_WINDOW_API should be exported.  */

/** @def IG_2D_WINDOW_BUILD_ARCHIVE
  @brief Indicates that this library is or was built as an archive
	 library, not as a shared library.  Lack of this indicates
	 that the header is included during the use of a shared
	 library of this package, and all entities marked with
	 #IG_2D_WINDOW_API should be imported.  */

# ifndef IG_2D_WINDOW_API
#  ifdef _WIN32
#   if defined IG_2D_WINDOW_BUILD_DLL
#    define IG_2D_WINDOW_API __declspec(dllexport)
#   elif ! defined IG_2D_WINDOW_BUILD_ARCHIVE
#    define IG_2D_WINDOW_API __declspec(dllimport)
#   endif
#  endif
# endif

# ifndef IG_2D_WINDOW_API
#  define IG_2D_WINDOW_API
# endif

#endif // IG_2D_WINDOW_CONFIG_H
