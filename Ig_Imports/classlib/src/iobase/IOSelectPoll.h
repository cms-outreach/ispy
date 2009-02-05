#ifndef CLASSLIB_IOSELECT_POLL_H
# define CLASSLIB_IOSELECT_POLL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "IOSelectFdMap.h"
# if HAVE_POLL
#  include <sys/time.h>
#  include <sys/types.h>
#  include <sys/poll.h>
#  include <unistd.h>
#  include <errno.h>
#  include <vector>

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Implementation of #IOSelectMethod using poll().  */
class IOSelectPoll : public IOSelectFdMap
{
public:
    IOSelectPoll (void);
    ~IOSelectPoll (void);

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

    /** Number of events left in #m_polls since last call to #next().  */
    int			m_nready;

    /** The last file descriptor inspected by #next().  Counts down
	from the top after each #wait().  */
    size_t		m_current;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
# endif // ! _WIN32
#endif // CLASSLIB_IOSELECT_POLL_H
