//<<<<<< INCLUDES                                                       >>>>>>

#include "src/iobase/IOSelectPoll.h"
#include "classlib/iobase/IOChannel.h"
#include "classlib/iobase/IOSelectEvent.h"
#include "classlib/iobase/IOError.h"
#include "classlib/utils/DebugAids.h"
#if HAVE_POLL

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
IOSelectPoll::IOSelectPoll (void)
{ clear (); }

/** Destruct the strategy object.  Assumes #IOSelector has already
    called #clear().  */
IOSelectPoll::~IOSelectPoll (void)
{ ASSERT (m_polls.empty ()); }

/** Attach the file descriptor @a fd with interest mask @a mask.  */
void
IOSelectPoll::attach (const IOSelectRequest &req)
{
    ASSERT (m_polls.size () == requests ());
    ASSERT (m_current <= m_polls.size ());

    // Insert the descriptor into the poll structure
    pollfd pfd;
    pfd.fd = req.fd;
    pfd.revents = 0;
    pfd.events = (((req.mask & (IOAccept | IORead)) ? POLLIN : 0)
		  | ((req.mask & (IOConnect | IOWrite)) ? POLLOUT : 0)
		  | ((req.mask & IOUrgent) ? POLLPRI : 0));
    m_polls.push_back (pfd);

    // Remember an index for this descriptor through a by-descriptor
    // table.  We need it to look up the mask later on.  This assumes
    // nobody listens to the descriptor twice, an assumption shared
    // with several implementations.
    fdmap (req.fd, requests ());

    // Record request into tracker.  We don't merge m_polls and
    // requests because we use m_polls directly in the system call.
    addReq (req);
}

/** Remove the client registration at @a index.  */
void
IOSelectPoll::detach (const IOSelectRequest &req)
{
    ASSERT (fdindex (req.fd) < requests ());
    ASSERT (m_polls.size () == requests ());
    ASSERT (m_polls.size () > 0);
    ASSERT (m_current <= m_polls.size ());

    // Find where the client is in the poll vector
    size_t index = fdindex (req.fd);
    ASSERT (index < m_polls.size ());
    ASSERT (m_polls [index].fd == req.fd);

    // Remove the registration from the poll vector and fd map (FIXME:
    // reclaim memory too?).  Remove request from base class.
    m_polls.erase (m_polls.begin () + index);
    removeReq (req, purge (req.fd));
    ASSERT (m_polls.empty () == ! fds ());

    // Adjust current file descriptor position.  Normally this won't
    // matter as we walk the list from top to bottom in next(), but
    // when a client is notified, it might deregister other clients
    // too.  So just to be sure adjust for it here if necessary.
    if (m_current >= index)
	--m_current;
}

/** Remove all clients from this selector.  */
void
IOSelectPoll::clear (void)
{
    // Reset tracking indices
    m_nready = -1;
    m_current = 0;

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
IOSelectPoll::setMask (IOFD fd, unsigned value)
{
    size_t index = fdindex (fd);
    ASSERT (index < requests ());
    ASSERT (m_polls.size () == requests ());
    ASSERT (m_polls.size () > 0);
    ASSERT (m_polls [index].fd == fd);

    unsigned mask = request (index).mask = value;
    m_polls [index].events =
	(((mask & (IOAccept | IORead)) ? POLLIN : 0)
	 | ((mask & (IOConnect | IOWrite)) ? POLLOUT : 0)
	 | ((mask & IOUrgent) ? POLLPRI : 0));
}

/// And the I/O event mask of registration at position @a index with @a value.
void
IOSelectPoll::andMask (IOFD fd, unsigned value)
{ setMask (fd, request (fdindex (fd)).mask & value); }

/// Or the I/O event mask of registration at position @a index with @a value.
void
IOSelectPoll::orMask (IOFD fd, unsigned value)
{ setMask (fd, request (fdindex (fd)).mask | value); }

//////////////////////////////////////////////////////////////////////
/** Wait on the registered descriptors for available I/O operations.

    If @a msecs is positive, waits at most that long for something to
    happen.  If it is negative, waits until at least one registered
    channel becomes available for I/O.  Setting @a msecs to zero
    specifies that the channels are probed without waiting.  If no
    channels have been registered, returns @c false immediately.

    @return On return, the status' of all registered descriptors has
	    been updated to reflect the current I/O availability
	    status.  The return value is @c true if any descriptors
	    became ready, @c false otherwise.  Use #nextEvent()
	    repeatedly to process the ready ones.

    @throw  A #IOError in case of a problem.  */
bool
IOSelectPoll::wait (long msecs)
{
    ASSERT (m_polls.size () == requests ());

    // Wait
    do
	// FIXME: Must recompute timeout!
	m_nready = ::poll (&m_polls [0], m_polls.size (), msecs);
    while (m_nready == -1 && errno == EINTR);

    if (m_nready == -1)
	throw IOError ("poll()", errno);

    // Warp current file descriptor position to the end.  This reduces
    // conflicts if clients attach() or detach() descriptors while
    // walking the ready list with next().
    m_current = m_polls.size ();

    // Indicate if any descriptor is ready for activity.
    return m_nready > 0;
}

/** Get the next ready descriptor, as determined by last #wait().
    This might throw an #IOError if the next descriptor to be
    inspected was invalid.  */
IOFD
IOSelectPoll::next (IOSelectEvent &event, IOSelectCB &cb)
{
    ASSERT (m_polls.size () == requests ());

    if (m_nready < 1)
	return IOFD_INVALID;

    // Iterate downwards as that reduces the number of conflicts if
    // clients invoke attach() or detach() while walking over the
    // results.  detach() adjusts m_current whenever descriptors are
    // removed below or at m_current.  Note that m_current is still
    // one plus that position we are interested in.
    ASSERT (m_current <= m_polls.size ());
    int events;
    while (m_current && ! (events = m_polls [m_current-1].revents))
	--m_current;

    if (! m_current)
	return IOFD_INVALID;

    // OK, m_current is now really the current position, and there is
    // something here (= in "events").  First sanity check.
    IOFD fd = m_polls [m_current].fd;
    ASSERT (fdindex (fd) == m_current);
    IOSelectRequest &req = request (m_current);
    ASSERT (req.channel);
    ASSERT (req.channel->fd () == fd);
    ASSERT (req.fd == fd);

    // Check that poll() didn't whine about bad descriptor.
    if (events & POLLNVAL)
	throw IOError ("poll()", EBADF);

    // Set up the event.
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

} // namespace lat
#endif // HAVE_POLL
