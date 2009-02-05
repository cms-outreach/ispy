//<<<<<< INCLUDES                                                       >>>>>>

#include "src/iobase/IOSelectSig.h"
#include "classlib/utils/DebugAids.h"
# if HAVE_POSIX_RT_SIGNALS && HAVE_F_SETSIG && HAVE_F_SETAUXFL

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Attach the file descriptor from request @a req.  The caller should
    have already established handler (Signal::revert() is sufficient)
    for SIGIO and SIGURG.  */
void
IOSelectSigFd::attach (const IOSelectRequest &req)
{
    ASSERT (req.fd >= 0);
    int flags;
    if ((flags = fcntl (req.fd, F_GETAUXFL, 0)) == -1
	|| fcntl (req.fd, F_SETAUXFL, flags | O_ONESIGFD) == -1)
	throw IOError ("fcntl()", errno);

    IOSelectSig::attach (req);
}

/** Remove the client registration at @a index.  */
void
IOSelectSigFd::detach (const IOSelectRequest &req)
{
    ASSERT (req.fd >= 0);
    int flags;
    if ((flags = fcntl (req.fd, F_GETAUXFL, 0)) == -1
	|| fcntl (req.fd, F_SETAUXFL, flags & ~O_ONESIGFD) == -1)
	throw IOError ("fcntl()", errno);

    IOSelectSig::detach (req);
}

} // namespace lat
#endif // HAVE_POSIX_RT_SIGNALS && HAVE_F_SETSIG && HAVE_F_SETAUXFL
