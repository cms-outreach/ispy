//<<<<<< INCLUDES                                                       >>>>>>

#include "src/iobase/IOSelectDevPoll.h"
#include "classlib/iobase/IOChannel.h"
#include "classlib/iobase/IOError.h"
#include "classlib/utils/DebugAids.h"
#if HAVE_SYS_DEVPOLL_H

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
IOSelectDevPoll::IOSelectDevPoll (void)
{
    if ((m_dev = open ("/dev/poll", O_RDWR)) < 0)
	throw IOError ("open(/dev/poll)", errno);

    clear ();
}

/** Destruct the strategy object.  Assumes #IOSelector has already
    called #clear().  */
IOSelectDevPoll::~IOSelectDevPoll (void)
{ ASSERT (m_polls.empty ()); close (m_dev); }

/** Attach the file descriptor @a fd with interest mask @a mask.  */
void
IOSelectDevPoll::attach (const IOSelectRequest &req)
{
    // Remember an index for this descriptor through a by-descriptor
    // table.  We need it to look up the mask later on.  This assumes
    // nobody listens to the descriptor twice, an assumption shared
    // with several implementations.
    fdmap (req.fd, requests ());

    // Record request into base class tracker.
    addReq (req);

    // Insert the descriptor into /dev/poll and add space to the
    // result array (note that nothing goes into the latter yet).
    pollfd pfd;
    pfd.fd = req.fd;
    pfd.events = (((req.mask & (IOAccept | IORead)) ? POLLIN : 0)
		  | ((req.mask & (IOConnect | IOWrite)) ? POLLOUT : 0)
		  | ((req.mask & IOUrgent) ? POLLPRI : 0));

    if (write (m_dev, &pfd, sizeof (pfd)) != sizeof (pfd))
	throw IOError ("write(/dev/poll)", errno);

    m_polls.resize (m_polls.size () + 1);
    ASSERT (m_polls.size () == requests ());
}

/** Remove the client registration at @a index.  */
void
IOSelectDevPoll::detach (const IOSelectRequest &req)
{
    ASSERT (fdindex (req.fd) < requests ());
    ASSERT (m_polls.size () == requests ());
    ASSERT (m_polls.size () > 0);
    ASSERT (m_polls [fdindex (req.fd)].fd == req.fd);

    // Remove the descriptor from those monitored by /dev/poll
    pollfd pfd;
    pfd.fd = req.fd;
    pfd.events = POLLREMOVE;
    if (write (m_dev, &pfd, sizeof (pfd)) != sizeof (pfd))
	throw IOError ("write(/dev/poll)", errno);

    // Shrink the result array and fd map (FIXME: reclaim memory too?).
    // Remove request from base class.
    m_polls.resize (m_polls.size () - 1);
    removeReq (req, purge (req.fd));
    ASSERT (m_polls.empty () == ! fds ());
}

/** Remove all clients from this selector.  */
void
IOSelectDevPoll::clear (void)
{
    // Reset tracking indices
    m_nready = 0;

    // Clear vectors and reclaim memory
    std::vector<pollfd> tmp;
    std::swap (m_polls, tmp);
    compact ();

    // Clear base class request tracker
    clearReqs ();
}

//////////////////////////////////////////////////////////////////////
/// Set the I/O event mask of registration at position @a index to @a value.
void
IOSelectDevPoll::setMask (IOFD fd, unsigned value)
{
    ASSERT (fdindex (fd) < requests ());
    ASSERT (m_polls.size () == requests ());
    ASSERT (m_polls.size () > 0);
    ASSERT (m_polls [fdindex (fd)].fd == fd);

    // Remember the new mask.
    unsigned mask = request (fdindex (fd)).mask = value;

    // Write the new information to /dev/poll.
    pollfd	pfd [2];
    int		n = 0;

#ifdef __solaris
    pfd [n].fd = fd;
    pfd [n].events = POLLREMOVE;
    ++n;
#endif

    pfd [n].fd = fd;
    pfd [n].events = (((mask & (IOAccept | IORead)) ? POLLIN : 0)
		      | ((mask & (IOConnect | IOWrite)) ? POLLOUT : 0)
		      | ((mask & IOUrgent) ? POLLPRI : 0));

    if (write (m_dev, &pfd[0], sizeof (pollfd) * n) != sizeof (pollfd) * n)
	throw IOError ("write(/dev/poll)", errno);
}

/// And the I/O event mask of registration at position @a index with @a value.
void
IOSelectDevPoll::andMask (IOFD fd, unsigned value)
{ setMask (fd, request (fdindex (fd)).mask & value); }

/// Or the I/O event mask of registration at position @a index with @a value.
void
IOSelectDevPoll::orMask (IOFD fd, unsigned value)
{ setMask (fd, request (fdindex (fd)).mask | value); }

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
IOSelectDevPoll::wait (long msecs)
{
    ASSERT (m_polls.size () == requests ());

    // Wait
    dvpoll info;

    info.dp_timeout = msecs;
    info.dp_nfds = m_polls.size ();
    info.dp_fds = &m_polls[0];

    do
	// FIXME: Must recompute timeout!
	m_nready = ::ioctl (m_dev, DP_POLL, &info);
    while (m_nready == -1 && errno == EINTR);

    if (m_nready == -1)
	throw IOError ("ioctl(/dev/poll)", errno);

    // Indicate if any descriptor is ready for activity.
    return m_nready > 0;
}

/** Get the next ready descriptor, as determined by last #wait().
    This might throw an #IOError if the next descriptor to be
    inspected was invalid.  */
IOFD
IOSelectDevPoll::next (IOSelectEvent &event, IOSelectCB &cb)
{
    ASSERT (m_polls.size () == requests ());

    while (m_nready >= 1)
    {
	// Get next descriptor off the result queue.
	m_nready--;
	int  events = m_polls [m_nready].revents;
	IOFD fd = m_polls [m_nready].fd;
	ASSERT (events > 0);
	ASSERT (fd >= 0);

	// Check that detach() hasn't been called for this descriptor.
	if ((size_t) fd >= fds () || fdindex (fd) == ~0ul)
	    continue;

	// Check that /dev/poll didn't whine about bad descriptor.
	if (events & POLLNVAL)
	    throw IOError ("ioctl(/dev/poll)", EBADF);

	// Set up the event.
	IOSelectRequest &req = request (fdindex (fd));
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

	return fd;
    }

    return IOFD_INVALID;
}

} // namespace lat
#endif // HAVE_SYS_DEVPOLL_H
