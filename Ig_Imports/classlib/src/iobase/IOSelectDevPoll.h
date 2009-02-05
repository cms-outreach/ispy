#ifndef CLASSLIB_IOSELECT_DEV_POLL_H
# define CLASSLIB_IOSELECT_DEV_POLL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "IOSelectFdMap.h"
# if HAVE_SYS_DEVPOLL_H
#  include <sys/time.h>
#  include <sys/types.h>
#  include <sys/poll.h>
#  ifdef __sun // horrible kludge for broken solaris header
     extern "C" {
#  endif
#  include <sys/devpoll.h>
#  include <unistd.h>
#  include <fcntl.h>
#  include <errno.h>
#  include <vector>

// #  include <sys/ioctl.h>
// #  include <sys/stat.h>
// #  include <stdlib.h>
// #  include <fcntl.h>
//
// #  ifndef POLLREMOVE
// #   define POLLREMOVE 0x1000
// #  endif

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Implementation of #IOSelectMethod using /dev/poll.  */
class IOSelectDevPoll : public IOSelectFdMap
{
public:
    IOSelectDevPoll (void);
    ~IOSelectDevPoll (void);

    // Attaching and detaching descriptors
    virtual void	attach    (const IOSelectRequest &req);
    virtual void	detach    (const IOSelectRequest &req);
    virtual void	clear     (void);

    // Descriptor listening states
    virtual void	setMask   (IOFD fd, unsigned value);
    virtual void	andMask   (IOFD fd, unsigned value);
    virtual void	orMask    (IOFD fd, unsigned value);

    // Selection
    virtual bool	wait      (long msecs);
    virtual IOFD	next      (IOSelectEvent &event, IOSelectCB &cb);

private:
    /** Array of pollfd's to pass to poll().  Compact, no gaps,
	no duplicate descriptors.  */
    std::vector<pollfd>	m_polls;

    /** File descriptor for /dev/poll.  */
    IOFD		m_dev;

    /** File descriptors left in #m_polls since last call to #next(). */
    int			m_nready;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
# endif // HAVE_SYS_DEVPOLL_H
#endif // CLASSLIB_IOSELECT_DEV_POLL_H
