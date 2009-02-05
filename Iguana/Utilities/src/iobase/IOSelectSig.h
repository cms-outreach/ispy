#ifndef CLASSLIB_IOSELECT_SIG_H
# define CLASSLIB_IOSELECT_SIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "IOSelectPoll.h"
# if HAVE_POSIX_RT_SIGNALS && HAVE_F_SETSIG
#  include <sys/time.h>
#  include <sys/types.h>
#  include <unistd.h>
#  include <signal.h>
#  include <errno.h>
#  include <fcntl.h>

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Implementation of #IOSelectMethod using POSIX real-time queued
    SIGIO signals.  (FIXME: Which descriptor types are accepted?)
    (FIXME: Note: This method won't work with multiple selector
    on the same descriptor because of signal usage.)

    The client must invoke #setSignal() before adding, detaching or
    waiting on file descriptors.  The call to #setSignal() must be
    preceded by establishing handlers for SIGIO and SIGURG and the
    real-time signal given to #setSignal (Signal::revert() is
    sufficient).  In other words, #setSignal() should be called
    immediately after the selector is constructed, and should be
    preceded by setting up the appropriate handlers.

    Note that this selection method is plagued by race conditions
    unlike most other selectors, mainly because of the way signals
    work.  It is not possible to guarantee that a #wait() will return
    100% accurate information about the readiness.  It will of course
    return all ready descriptors, but it may also indicate descriptors
    ready when in reality they are not.  Clients enabling this method
    mustbe ready to handle situations where a descriptor is returned
    as ready for I/O, but actually isn't.  This condition does not
    happen with other methods (unless multi-threaded races exist).

    In other words, the usability of this selector method depends
    highly on the actual I/O rate.  */
class IOSelectSig : public IOSelectMethod
{
public:
    IOSelectSig (void);
    ~IOSelectSig (void);

    virtual void	setSignal (int sig);

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
    static const int UNUSED   = -2;
    static const int UNLINKED = -1;

    void		addReady    (IOFD fd);
    void		delReady    (IOFD fd);
    void		updateReady (IOFD fd);

    /** Per file descriptor data.  */
    struct item {
	/** Next file descriptor in the circular ready list.  This is
	    either another file descriptor (or this fd if the list
	    only has one item), #UNUSED to mark this fd unused, or
	    #UNLINKED to indicate this fd is not on the ready
	    list.  */
	IOFD		m_next;

	/** Previous file descriptor in the circular ready list.  This
	    is either another file descriptor (or this fd if the list
	    only has one item), #UNUSED to mark this fd unused, or
	    #UNLINKED to indicate this fd is not on the ready
	    list.  */
	IOFD		m_prev;

	/** poll() interest mask we are currently interested in.  */
	unsigned	m_mask;

	/** poll() interest mask this descriptor is ready for.  */
	unsigned	m_ready;

	/** Index of this file descriptor in request list.  */
	size_t		m_index;
    };

    IOSelectPoll	*m_fallback;		//< Fallback poller.
    int			m_signal;		//< Signal number.
    sigset_t		m_sigset;		//< Signal set.
    IOFD		m_readylist;		//< Index of first fd ready.
    size_t		m_nready;		//< Number of ready fds.
    size_t		m_nfds;			//< Nr of fds in use in m_fds.
    std::vector<item>	m_fds;			//< Per file descriptor data.
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
# endif // HAVE_POSIX_RT_SIGNALS && HAVE_F_SETSIG
#endif // CLASSLIB_IOSELECT_SIG_H
