#ifndef CLASSLIB_IOSFWD_H
# define CLASSLIB_IOSFWD_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"
# if HAVE_NEW_IOSTREAMS
#  include <iosfwd>
# endif

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

# if HAVE_NAMESPACE_STD && HAVE_NAMESPACES
namespace std
{
# endif
# if HAVE_NEW_IOSTREAMS
    class ios_base;
#  if !HAVE_STREAMBUF_IN_IOSFWD
    typedef basic_streambuf< char,ios_traits<char> >			streambuf;
#  endif
# endif
# if HAVE_NAMESPACE_STD && HAVE_NAMESPACES
}
# endif

# if !HAVE_NEW_IOSTREAMS
class ios;
class istream;
class ostream;
class streambuf;
class stringbuf;
class filebuf;
# endif

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // CLASSLIB_IOSFWD_H
