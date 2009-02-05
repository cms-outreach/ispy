#ifndef CLASSLIB_SYSAPI_IO_CHANNEL_TRANSFER_H
# define CLASSLIB_SYSAPI_IO_CHANNEL_TRANSFER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"
# include <errno.h>
# if HAVE_LINUX_SENDFILE
#  include <sys/sendfile.h>
# elif HAVE_SOLARIS_SENDFILE || HAVE_FREEBSD_SENDFILE
#  include <sys/uio.h>
#  include <sys/socket.h>
# endif

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>
//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // CLASSLIB_SYSAPI_IO_CHANNEL_TRANSFER_H
