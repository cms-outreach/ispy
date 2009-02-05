#ifndef CLASSLIB_IOSELECT_SELECT_H
# define CLASSLIB_IOSELECT_SELECT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "IOSelectFdMap.h"
# ifndef _WIN32
#  include <sys/time.h>
#  include <sys/types.h>
#  include <sys/select.h>
#  include <unistd.h>
#  include <string.h>
#  include <errno.h>

//  #  include <fcntl.h>
//  #  if TIME_WITH_SYS_TIME
//  #   include <sys/time.h>
//  #   include <time.h>
//  #  else
//  #   if HAVE_SYS_TIME_H
//  #    include <sys/time.h>
//  #   else
//  #    include <time.h>
//  #   endif
//  #  endif

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Implementation of #IOSelectMethod using select().  */
class IOSelectSelect : public IOSelectFdMap
{
public:
    IOSelectSelect (void);
    ~IOSelectSelect (void);

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
    /** The highest file descriptor seen so far.  */
    IOFD		m_maxfd;

    /** The last file descriptor inspected by #next().  */
    IOFD		m_current;

    /** Number of file descriptors left ready in #m_rlast, #m_wlast
	and #m_elast in total.  Initialised by #wait() and decremented
	in #next().  */
    int			m_nready;

    /** File descriptors registered for read or accept events.  */
    fd_set		m_r;

    /** File descriptors registered for write or connect events.  */
    fd_set		m_w;

    /** File descriptors registered for urgent events.  */
    fd_set		m_e;

    /** File descriptors ready for read or accepted at last #wait(). */
    fd_set		m_rstatus;

    /** File descriptors ready for write or connected at last #wait(). */
    fd_set		m_wstatus;

    /** File descriptors ready with urgent data at last #wait(). */
    fd_set		m_estatus;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
# endif // ! _WIN32
#endif // CLASSLIB_IOSELECT_SELECT_H
