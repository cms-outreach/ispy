#ifndef IG3DMATERIAL_EDITOR_CONFIG_H
# define IG3DMATERIAL_EDITOR_CONFIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>

/** @def IG3DMATERIAL_EDITOR_API
  @brief A macro that controls how entities of this shared library are
         exported or imported on Windows platforms (the macro expands
         to nothing on all other platforms).  The definitions are
         exported if #IG3DMATERIAL_EDITOR_BUILD_DLL is defined, imported
         if #IG3DMATERIAL_EDITOR_BUILD_ARCHIVE is not defined, and left
         alone if latter is defined (for an archive library build).  */

/** @def IG3DMATERIAL_EDITOR_BUILD_DLL
  @brief Indicates that the header is included during the build of
         a shared library of this package, and all entities marked
	 with #IG3DMATERIAL_EDITOR_API should be exported.  */

/** @def IG3DMATERIAL_EDITOR_BUILD_ARCHIVE
  @brief Indicates that this library is or was built as an archive
         library, not as a shared library.  Lack of this indicates
         that the header is included during the use of a shared
         library of this package, and all entities marked with
         #IG3DMATERIAL_EDITOR_API should be imported.  */

# ifndef IG3DMATERIAL_EDITOR_API
#  ifdef _WIN32
#    if defined IG3DMATERIAL_EDITOR_BUILD_DLL
#      define IG3DMATERIAL_EDITOR_API __declspec(dllexport)
#    elif ! defined IG3DMATERIAL_EDITOR_BUILD_ARCHIVE
#      define IG3DMATERIAL_EDITOR_API __declspec(dllimport)
#    endif
#  endif
# endif

# ifndef IG3DMATERIAL_EDITOR_API
#  define IG3DMATERIAL_EDITOR_API
# endif

#endif // IG3DMATERIAL_EDITOR_CONFIG_H
