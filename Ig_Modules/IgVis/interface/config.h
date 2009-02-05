#ifndef IG_VIS_CONFIG_H
# define IG_VIS_CONFIG_H

//<<<<<< INCLUDES                                                       >>>>>>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>

/** @def IG_VIS_API
    @brief A macro that controls how entities of this shared library are
    exported or imported on Windows platforms (the macro expands
    to nothing on all other platforms).  The definitions are
    exported if #IG_VIS_BUILD_DLL is defined, imported
    if #IG_VIS_BUILD_ARCHIVE is not defined, and left
    alone if latter is defined (for an archive library build).  */

/** @def IG_VIS_BUILD_DLL
    @brief Indicates that the header is included during the build of
    a shared library of this package, and all entities marked
    with #IG_VIS_API should be exported.  */

/** @def IG_VIS_BUILD_ARCHIVE
    @brief Indicates that this library is or was built as an archive
    library, not as a shared library.  Lack of this indicates
    that the header is included during the use of a shared
    library of this package, and all entities marked with
    #IG_VIS_API should be imported.  */

# ifndef IG_VIS_API
#  ifdef _WIN32
#   if defined IG_VIS_BUILD_DLL
#    define IG_VIS_API __declspec(dllexport)
#   elif ! defined IG_VIS_BUILD_ARCHIVE
#    define IG_VIS_API __declspec(dllimport)
#   endif
#  endif
# endif

# ifndef IG_VIS_API
#  define IG_VIS_API
# endif

#ifdef __KCC // this is required to fix up the SoIndexedFaceSet include file
#define __hpux
#endif

// #ifdef WIN32
// #pragma warning (once: 4786)
// #pragma warning (once: 4251)
// #endif

#endif // IG_VIS_CONFIG_H
