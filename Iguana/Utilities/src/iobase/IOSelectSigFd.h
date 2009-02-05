#ifndef CLASSLIB_IOSELECT_SIG_FD_H
# define CLASSLIB_IOSELECT_SIG_FD_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "IOSelectSig.h"
# if HAVE_POSIX_RT_SIGNALS && HAVE_F_SETSIG && HAVE_F_SETAUXFL

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Implementation of #IOSelectMethod using POSIX real-time queued
    SIGIO signal per file descriptor.  */
class IOSelectSigFd : public IOSelectBySig
{
public:
    // Attaching and detaching descriptors
    virtual void	attach   (const IOSelectRequest &req);
    virtual void	detach   (const IOSelectRequest &req);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
# endif // HAVE_POSIX_RT_SIGNALS && HAVE_F_SETSIG && HAVE_F_SETAUXFL
#endif // CLASSLIB_IOSELECT_SIG_FD_H
