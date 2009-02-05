#ifndef CLASSLIB_CUSTOM_STREAM_BUF_H
# define CLASSLIB_CUSTOM_STREAM_BUF_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"
# if HAVE_NEW_IOSTREAMS
#  include <streambuf>
# else
#  include <streambuf.h>
# endif

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class CustomStreamBuf : public std::streambuf
{
public:
#if HAVE_NEW_IOSTREAMS
# define CLASSLIB_STREAM_EOF		std::streambuf::traits_type::eof ()
# define CLASSLIB_IOS_BASE		std::ios_base
    typedef std::streamsize		size_type;
    typedef std::streamoff		offset_type;
    typedef std::streampos		pos_type;
    typedef std::ios_base::seekdir	seekdir;
    typedef std::ios_base::openmode	openmode;
#else
# define CLASSLIB_STREAM_EOF		EOF
# define CLASSLIB_IOS_BASE		ios
    typedef int				int_type;
    typedef char			char_type;
    typedef int				size_type;
    typedef std::streamoff		offset_type;
    typedef std::streampos		pos_type;
    typedef ios::seekdir		seekdir;
    typedef ios::openmode		openmode;
#endif
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_CUSTOM_STREAM_BUF_H
