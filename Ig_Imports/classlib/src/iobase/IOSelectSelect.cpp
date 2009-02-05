//<<<<<< INCLUDES                                                       >>>>>>

#include "src/iobase/IOSelectSelect.h"
#include "classlib/iobase/IOSelectEvent.h"
#include "classlib/iobase/IOError.h"
#include "classlib/utils/DebugAids.h"
#ifndef _WIN32

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
IOSelectSelect::IOSelectSelect (void)
{ clear (); }

/** Destruct the strategy object.  Assumes #IOSelector has already
    called #clear().  */
IOSelectSelect::~IOSelectSelect (void)
{ ASSERT (m_maxfd == -1); }

/** Attach the file descriptor @a fd with interest mask @a mask.  */
void
IOSelectSelect::attach (const IOSelectRequest &req)
{
    ASSERT (req.fd >= 0 && req.fd < FD_SETSIZE);

    // Record information into the file descriptor sets.
    if (req.fd > m_maxfd)
	m_maxfd = req.fd;

    if (req.mask & (IORead | IOAccept))
	FD_SET (req.fd, &m_r);

    if (req.mask & (IOWrite | IOConnect))
	FD_SET (req.fd, &m_w);

    if (req.mask & IOUrgent)
	FD_SET (req.fd, &m_e);

    // Remember an index for this descriptor through a by-descriptor
    // table.  We need it to look up the mask later on.  This assumes
    // nobody listens to the descriptor twice, an assumption shared
    // with several implementations.
    fdmap (req.fd, requests ());

    // Record request into base class tracker.
    addReq (req);
}

/** Remove the client registration at @a index.  */
void
IOSelectSelect::detach (const IOSelectRequest &req)
{
    ASSERT (req.fd >= 0 && req.fd <= m_maxfd);
    ASSERT (fdindex (req.fd) < requests ());

    // Remove registrations
    setMask (req.fd, 0);
    removeReq (req, purge (req.fd));

    // Update limit
    while (m_maxfd >= 0
	   && ! (FD_ISSET (m_maxfd, &m_r)
		 || FD_ISSET (m_maxfd, &m_w)
		 || FD_ISSET (m_maxfd, &m_e)))
	--m_maxfd;
}

/** Remove all clients from this selector.  */
void
IOSelectSelect::clear (void)
{
    // Reset member variables to empty subscription.
    m_maxfd = -1;
    m_current = -1;
    m_nready = 0;
    FD_ZERO (&m_r);
    FD_ZERO (&m_w);
    FD_ZERO (&m_e);

    // Clear base class request tracker and fd map
    compact ();
    clearReqs ();
}

//////////////////////////////////////////////////////////////////////
/// Set the I/O event mask of registration at position @a index to @a value.
void
IOSelectSelect::setMask (IOFD fd, unsigned value)
{
    ASSERT (fd >= 0 && fd <= m_maxfd);
    size_t index = fdindex (fd);
    ASSERT (index < requests ());
    ASSERT (request (index).fd == fd);

    unsigned mask = request (index).mask = value;

    FD_CLR (fd, &m_r);
    if (mask & (IOAccept | IORead))
	FD_SET (fd, &m_r);

    FD_CLR (fd, &m_w);
    if (mask & (IOConnect | IOWrite))
	FD_SET (fd, &m_w);

    FD_CLR (fd, &m_e);
    if (mask & IOUrgent)
	FD_SET (fd, &m_e);
}

/// And the I/O event mask of registration at position @a index with @a value.
void
IOSelectSelect::andMask (IOFD fd, unsigned value)
{ setMask (fd, request (fdindex (fd)).mask & value); }

/// Or the I/O event mask of registration at position @a index with @a value.
void
IOSelectSelect::orMask (IOFD fd, unsigned value)
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
IOSelectSelect::wait (long msecs)
{
    // Copy current state and initialise time span
    memcpy (&m_rstatus, &m_r, sizeof (m_r));
    memcpy (&m_wstatus, &m_w, sizeof (m_w));
    memcpy (&m_estatus, &m_e, sizeof (m_e));

    struct timeval tv;
    tv.tv_sec = msecs / 1000;
    tv.tv_usec = (msecs % 1000) * 1000;

    // Select (FIXME: use pselect() instead if it is available?  interface?)
    do
	// FIXME: Must recompute timeout!
	m_nready = ::select (m_maxfd + 1,
			     (SELECT_ARG_TYPE) &m_rstatus,
			     (SELECT_ARG_TYPE) &m_wstatus,
			     (SELECT_ARG_TYPE) &m_estatus,
			     msecs >= 0 ? &tv : 0);
    while (m_nready == -1 && errno == EINTR);

    if (m_nready == -1)
	throw IOError ("select()", errno);

    // Zap the current file descriptor position so we'll reststart on
    // the next call to #next().  It is preincremented in #next().
    m_current = -1;

    // Indicate if any descriptor is ready for activity.
    return m_nready > 0;
}

/** Get the next ready descriptor, as determined by last #wait().  */
IOFD
IOSelectSelect::next (IOSelectEvent &event, IOSelectCB &cb)
{
    if (m_nready < 1)
	return IOFD_INVALID;

    unsigned events = 0;
    while (! events && m_current++ < m_maxfd)
    {
	if (FD_ISSET (m_current, &m_rstatus))
	    events |= IORead | IOAccept;
	if (FD_ISSET (m_current, &m_wstatus))
	    events |= IOWrite | IOConnect;
	if (FD_ISSET (m_current, &m_estatus))
	    events |= IOUrgent;
    }

    if (! events)
	return IOFD_INVALID;

    if (events & IORead)
	VERIFY (m_nready--);
    if (events & IOWrite)
	VERIFY (m_nready--);
    if (events & IOUrgent)
	VERIFY (m_nready--);

    // FIXME: Use poll-like fd2index?
    IOSelectRequest &req = request (fdindex (m_current));
    cb = req.cb;
    event.source = req.channel;
    event.events = req.mask & events;
    return m_current;
}

} // namespace lat
#endif // !_WIN32
