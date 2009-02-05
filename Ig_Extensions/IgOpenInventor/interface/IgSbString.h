#ifndef IG_OPEN_INVENTOR_IG_SB_STRING_H
# define IG_OPEN_INVENTOR_IG_SB_STRING_H

# ifndef IGUANA_OBSOLETE
#  error This code is probably broken.  If you use it, please let us know.
#  error If it is needed, it should be cleaned up and use IgSoShapeKit.
#  error Do not release it in CMS IGUANA before it is rewritten.
# endif

//<<<<<< INCLUDES                                                       >>>>>>

#include <cstdlib>
#include <cstring>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>

#define STRDUP(str) ((str) ? (strcpy ((char*) malloc ((unsigned) strlen (str) + 1), str)) : 0)
#define STRDEL(str) { if ((str)) { free (str); str = NULL; }}

//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>
//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_OPEN_INVENTOR_IG_SB_STRING_H
