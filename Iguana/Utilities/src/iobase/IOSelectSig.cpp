//<<<<<< INCLUDES                                                       >>>>>>

#include "src/iobase/IOSelectSig.h"
# if HAVE_POSIX_RT_SIGNALS && HAVE_F_SETSIG
#  include "IOSelectPoll.h"
#  include "classlib/iobase/IOChannel.h"
#  include "classlib/iobase/IOError.h"
#  include "classlib/utils/Signal.h"
#  include "classlib/utils/DebugAids.h"

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

/** Initialise the selector strategy object.  */
IOSelectSig::IOSelectSig (void)
    : m_fallback (new IOSelectPoll),
      m_signal (-1)
{ sigemptyset (&m_sigset); clear (); }

/** Destruct the strategy object.  Assumes #IOSelector has already
    called #clear().  */
IOSelectSig::~IOSelectSig (void)
{
    // Remove remaining descriptors
    for (IOFD fd = m_fds.size () - 1; fd >= 0 && m_nfds; --fd)
	if (m_fds [fd].m_prev != UNUSED)
	{
	    IOSelectRequest req = { fd, 0, 0, IOSelectCB () };
	    detach (req);
	}

    ASSERT (m_fds.empty ());
    ASSERT (! m_nfds);

    // Purge pending signals
    wait (0);

    // Restore old signal mask (FIXME: correctness?)
    Signal::block (&m_sigset, false);

    // Now let fallback go
    delete m_fallback;
}

/** Set the real-time signal number that should be used with the
    selector.  This method must be called before the selector is
    further used in any way.  The caller must have already established
    handler (Signal::revert() is sufficient) for SIGIO and SIGURG.  */
void
IOSelectSig::setSignal (int sig)
{
    ASSERT (m_signal == -1);
    ASSERT (sig > 0);
    ASSERT (sig >= SIGRTMIN);
    ASSERT (sig <= SIGRTMAX);

    // Remember the signal
    m_signal = sig;

    // Block the given signal, SIGIO and SIGURG
    sigemptyset (&m_sigset);
    sigaddset (&m_sigset, sig);
    sigaddset (&m_sigset, SIGIO);
    sigaddset (&m_sigset, SIGURG);
    Signal::block (&m_sigset, true);
}

/** Add descriptor @a fd to the ready list.  */
void
IOSelectSig::addReady (IOFD fd)
{
    ASSERT (fd >= 0 && (size_t) fd < m_fds.size ());
    ASSERT (m_fds [fd].m_next < 0);
    ASSERT (m_fds [fd].m_prev < 0);

    if (! m_nready++)
    {
	// Ready list is empty, make this the entire list and the head of it
	ASSERT (m_readylist < 0);
	m_fds [fd].m_next = m_fds [fd].m_prev = fd;
	m_readylist = fd;
    }
    else
    {
	// Make this the last node in the circular list (== just before head)
	ASSERT (m_readylist >= 0);
	ASSERT (m_fds [m_readylist].m_next >= 0);
	ASSERT (m_fds [m_readylist].m_prev >= 0);

	IOFD prev = m_fds [m_readylist].m_prev;
	m_fds [fd].m_prev = prev;
	m_fds [fd].m_next = m_readylist;
	m_fds [prev].m_next = fd;
	m_fds [m_readylist].m_prev = fd;
    }
}

/** Delete descriptor @a fd from the ready list.  */
void
IOSelectSig::delReady (IOFD fd)
{
    ASSERT ((size_t) fd < m_fds.size ());
    ASSERT (m_nready);
    ASSERT (m_readylist >= 0);
    ASSERT (m_fds [m_readylist].m_next >= 0);
    ASSERT (m_fds [m_readylist].m_prev >= 0);
    ASSERT (m_fds [fd].m_next >= 0);
    ASSERT (m_fds [fd].m_prev >= 0);

    IOFD next = m_fds [fd].m_next;
    IOFD prev = m_fds [fd].m_prev;

    if (m_nready == 1)
    {
	// Ready list will become empty
	ASSERT (fd == m_readylist);
	ASSERT (next == prev);
	m_readylist = UNLINKED;
    }
    else
    {
	// Remove and close the gap
	if (m_readylist == fd)
	    m_readylist = next;

	m_fds [prev].m_next = next;
	m_fds [next].m_prev = prev;
    }

    m_fds [fd].m_next = m_fds [fd].m_prev = UNLINKED;
    --m_nready;
}

/** Flip ready list state of descriptor @a fd: if on the list, remove
    it, if not on the list, add it.  */
void
IOSelectSig::updateReady (IOFD fd)
{
    ASSERT ((size_t) fd < m_fds.size ());
    if (m_fds [fd].m_next >= 0)
    {
	// On the list, remove it if no longer ready
	if (! (m_fds [fd].m_ready & m_fds [fd].m_mask))
	    delReady (fd);
    }
    else
    {
	// Not on the list, add if ready now
	if (m_fds [fd].m_ready & m_fds [fd].m_mask)
	    addReady (fd);
    }
}

/** Attach the file descriptor specified by @a req.  The caller should
    have already established handler (Signal::default() is sufficient)
    for SIGIO and SIGURG.  */
void
IOSelectSig::attach (const IOSelectRequest &req)
{
    ASSERT (m_signal > 0);
    ASSERT (req.fd >= 0);

    // Remember selection into the by-descriptor table.  We assume
    // here that nobody is listening to the descriptor twice (a
    // general assumption made by several implementations).
    item unused = { UNUSED, UNUSED, 0, 0, ~0ul };
    if ((size_t) req.fd >= m_fds.size ())
	m_fds.resize (req.fd+1, unused);

    // Set descriptor flags.  The order here matters even with blocked
    // signals: we don't want notifications for wrong pid or signal.
    // Note that some signals may be queued between this and the poll
    // below.
    int flags, pid = getpid ();
    if (fcntl (req.fd, F_SETSIG, m_signal) == -1
	|| fcntl (req.fd, F_SETOWN, pid) == -1
	|| (flags = fcntl (req.fd, F_GETFL, 0)) == -1
	|| fcntl (req.fd, F_SETFL, flags | O_NONBLOCK | O_ASYNC) == -1)
	throw IOError ("fcntl()", errno);

    ++m_nfds;

    // Remember descriptor choices.  Poll the descriptor for its true
    // initial state -- signals are edge-triggered only, they tell us
    // about transitions, not the current state.
    pollfd pfd;
    pfd.fd = req.fd;
    pfd.events = POLLIN | POLLOUT | POLLPRI;
    pfd.revents = 0;
    if (poll (&pfd, 1, 0) == -1 || (pfd.revents & POLLNVAL))
	throw IOError ("poll()", errno);

    m_fds [req.fd].m_index = requests ();
    m_fds [req.fd].m_next = m_fds [req.fd].m_next = UNLINKED;
    m_fds [req.fd].m_ready = pfd.revents;
    m_fds [req.fd].m_mask =
	(((req.mask & (IOAccept | IORead)) ? POLLIN : 0)
	 | ((req.mask & (IOConnect | IOWrite)) ? POLLOUT : 0)
	 | ((req.mask & IOUrgent) ? POLLPRI : 0));

    addReady (req.fd);

    // Add it to backup, with full interest mask -- in case our client
    // later changes their mind about what they are interested in.
    // Code later on filters only bits we are really interested in
    // (FIXME: Does this handle timeouts correctly?)
    IOSelectRequest fbreq = {
	req.fd, req.channel, IORead | IOWrite | IOUrgent, req.cb };
    m_fallback->attach (fbreq);

    // Record request into base class tracker.
    addReq (req);
}

/** Remove the client registration @a req.  */
void
IOSelectSig::detach (const IOSelectRequest &req)
{
    ASSERT ((size_t) req.fd < m_fds.size ());
    ASSERT (m_signal > 0);
    ASSERT (m_nfds);

    // Set the descriptor not to emit signals any more.  Note that
    // signals might already have been queued for this descriptor;
    // code below protects against that.  Gotta love designs with
    // built-in race conditions...
    int flags;
    if ((flags = fcntl (req.fd, F_GETFL, 0)) == -1
	|| fcntl (req.fd, F_SETFL, flags & ~O_ASYNC) == -1)
	throw IOError ("fcntl()", errno);

    // Remove from ready list if on it
    if (m_fds [req.fd].m_next >= 0)
	delReady (req.fd);

    // Mark the entry unused.  Shrink the array if possible.
    size_t index = m_fds [req.fd].m_index;
    m_fds [req.fd].m_next = m_fds [req.fd].m_prev = UNUSED;
    m_fds [req.fd].m_mask = m_fds [req.fd].m_ready = 0;
    while (! m_fds.empty () && m_fds.back ().m_next == UNUSED)
	m_fds.pop_back ();

    // Update number of descriptor we have and update fallback
    --m_nfds;
    m_fallback->detach (req);

    // Remove request from the base class.
    removeReq (req, index);
}

/** Remove all clients from this selector.  */
void
IOSelectSig::clear (void)
{
    // Reset tracking indices
    m_readylist = UNLINKED;
    m_nready    = 0;
    m_nfds      = 0;

    // Clear vectors and reclaim memory
    std::vector<item> tmp;
    std::swap (m_fds, tmp);

    // Clear base class request tracker
    clearReqs ();
}

//////////////////////////////////////////////////////////////////////
/// Set the I/O event mask of registration at position @a index to @a value.
void
IOSelectSig::setMask (IOFD fd, unsigned value)
{
    ASSERT ((size_t) fd < m_fds.size ());
    ASSERT (request (m_fds [fd].m_index).fd == fd);
    ASSERT (m_signal > 0);
    ASSERT (m_nfds);

    // Remember the new mask and maybe update ready list.  (Fallback
    // needs no update since it has full interest mask -- FIXME?)
    request (m_fds [fd].m_index).mask = value;
    m_fds [fd].m_mask =
	(((value & (IOAccept | IORead)) ? POLLIN : 0)
	 | ((value & (IOConnect | IOWrite)) ? POLLOUT : 0)
	 | ((value & IOUrgent) ? POLLPRI : 0));
    updateReady (fd);
}

/// And the I/O event mask of registration at position @a index with @a value.
void
IOSelectSig::andMask (IOFD fd, unsigned value)
{
    ASSERT ((size_t) fd < m_fds.size ());
    setMask (fd, request (m_fds [fd].m_index).mask & value);
}

/// Or the I/O event mask of registration at position @a index with @a value.
void
IOSelectSig::orMask (IOFD fd, unsigned value)
{
    ASSERT ((size_t) fd < m_fds.size ());
    setMask (fd, request (m_fds [fd].m_index).mask | value);
}

//////////////////////////////////////////////////////////////////////
/** Wait on the registered descriptors for available I/O operations.

    If @a msecs is positive, waits at most that long for something to
    happen.  If it is negative, waits until at least one registered
    channel becomes available for I/O.  Setting @a msecs to zero
    specifies that the channels are probed without waiting.

    @return On return, the status' of all registered descriptors has
	    been updated to reflect the current I/O availability
	    status.  The return value is @c true if any descriptors
	    became ready, @c false otherwise.  Use #nextEvent()
	    repeatedly to process the ready ones.

    @throw  A #IOError in case of a problem.  */
bool
IOSelectSig::wait (long msecs)
{
    IOFD	fd;
    int		sig;
    siginfo_t	info;
    bool	overflowed = false;

    // If the ready list is not empty, don't wait.
    if (m_readylist != UNLINKED)
	msecs = 0;

    // Empty the queue until block would occur
    do {
	sig = Signal::wait (&m_sigset, &info, msecs);
	msecs = 0;

	if (sig == -1)
	    break;
	else if (sig == SIGIO || sig == SIGURG)
	{
	    // Real-time queue overflowed or we got SIGURG which can't
	    // be mapped to real-time signals and thus queued and
	    // counted.  Ditch remaining signals and invoke the
	    // fallback poll() selector to update to current status.
	    overflowed = true;
	    break;
	}

	ASSERT (sig == m_signal);

	// Remember the new event.  Protect against already-detached
	// descriptors that had events queued for them between last
	// call to wait() and the detach().  Gotta love designs with
	// inherent race-conditions in them...
	fd = info.si_fd;
	if ((size_t) fd >= m_fds.size () || m_fds [fd].m_next == UNUSED)
	    continue;

	m_fds [fd].m_ready |= info.si_band & (POLLIN|POLLPRI|POLLOUT
					      |POLLERR|POLLHUP|POLLNVAL);
	updateReady (fd);
    } while (true);

    if (overflowed)
    {
	// Ditch remaining signals.  POSIX says this clears the queue
	// -- but we can't leave the signal ignored, or it might be.
	// We don't need a handler for blocking, queuing and waiting
	// for signals, but ignored (as opposed to default) ones may
	// actually get completely ignored.  There's no race between
	// the calls here as all that matters is the state we'll get
	// from the fallback (though note that signals may get queued
	// while we use the fallback, in which case they will be read
	// the next time around, and the client must be prepared to
	// get false information).
	Signal::ignore (m_signal);
	Signal::revert (m_signal);

	IOSelectEvent	event;
	IOSelectCB	cb;

	m_fallback->wait (0);
	while ((fd = m_fallback->next (event, cb)) != IOFD_INVALID)
	{
	    if ((size_t) fd >= m_fds.size () || m_fds [fd].m_next == UNUSED)
		continue;

	    // Skip it if we are up to date
	    unsigned events
		= (((event.events & (IORead | IOAccept)) ? POLLIN : 0)
		   | ((event.events & (IOWrite | IOConnect)) ? POLLOUT : 0)
		   | ((event.events & IOUrgent) ? POLLPRI : 0));
	    if (m_fds [fd].m_ready == events)
		continue;

	    // Note that both the signal selector and poll() return
	    // all the readiness bits, not just the ones we are
	    // interested in at the moment.  This might result in
	    // false positives, but that's harmless (clients have to
	    // be prepared for that -- yuck, can we change that!?).
	    m_fds [fd].m_ready = events;
	    updateReady (fd);
	}
    }

    // Return an indication if anything was found.
    return m_nready > 0;
}

/** Get the next ready descriptor, as determined by last #wait().
    This might throw an #IOError if the next descriptor to be
    inspected was invalid.  */
IOFD
IOSelectSig::next (IOSelectEvent &event, IOSelectCB &cb)
{
    ASSERT (m_signal > 0);
    if (! m_nready)
	return IOFD_INVALID;

    ASSERT (m_readylist >= 0);
    ASSERT (m_fds [m_readylist].m_next >= 0);
    ASSERT (m_fds [m_readylist].m_prev >= 0);

    // Check that this hasn't gone bad.
    IOFD fd = m_readylist;
    int events = m_fds [m_readylist].m_ready;
    if (events & POLLNVAL)
	throw IOError ("poll()", EBADF);

    // Set up the event.
    IOSelectRequest &req = request (m_fds [fd].m_index);
    ASSERT (req.channel);
    ASSERT (req.channel->fd () == fd);
    ASSERT (req.fd == fd);
    cb = req.cb;
    event.source = req.channel;
    event.events = req.mask
		   & (((events & POLLIN) ? IORead | IOAccept : 0)
		      | ((events & POLLOUT) ? IOWrite | IOConnect : 0)
		      | ((events & POLLPRI) ? IOUrgent : 0)
		      | ((events & POLLHUP) ? 0 : 0)	// FIXME?
		      | ((events & POLLERR) ? 0 : 0));	// FIXME?

    delReady (fd);
    return fd;
}

} // namespace lat
#endif // HAVE_POSIX_RT_SIGNALS && HAVE_F_SETSIG
