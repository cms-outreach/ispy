//<<<<<< INCLUDES                                                       >>>>>>

#include "src/iobase/IOSelectKqueue.h"
#include "classlib/iobase/IOChannel.h"
#include "classlib/iobase/IOSelectEvent.h"
#include "classlib/iobase/IOError.h"
#include "classlib/utils/DebugAids.h"
#if HAVE_KQUEUE

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
IOSelectKqueue::IOSelectKqueue (void)
{
    if ((m_kqueue = kqueue ()) == -1)
	throw IOError ("kqueue()", errno);

    clear ();
}

/** Destruct the strategy object.  Assumes #IOSelector has already
    called #clear().  */
IOSelectKqueue::~IOSelectKqueue (void)
{
    // Queue events are automatically deleted when the descriptor is
    // closed so we don't need to do anything special here.
    ASSERT (m_changes.empty ());
    ASSERT (m_results.empty ());
}

/** Attach the file descriptor @a fd with interest mask @a mask.  */
void
IOSelectKqueue::attach (const IOSelectRequest &req)
{
    // Can't wait for urgent data (FIXME?).
    ASSERT (! (req.mask & IOUrgent));
    ASSERT (! (m_results.size () % 2));
    ASSERT (! (m_changes.size () % 2));

    // Remember an index for this descriptor through a by-descriptor
    // table.  We need it to look up the mask later on.  This assumes
    // nobody listens to the descriptor twice, an assumption shared
    // with several implementations.
    fdmap (req.fd, requests ());

    // Record request into base class tracker.
    addReq (req);

    // Add space to the result array and queue request details.
    m_results.resize (m_results.size () + 2);
    doSet (req.fd, req.mask);
}

/** Remove the client registration at @a index.  */
void
IOSelectKqueue::detach (const IOSelectRequest &req)
{
    ASSERT (fdindex (req.fd) < requests ());
    ASSERT (m_results.size () == 2 * requests ());
    ASSERT (! m_results.empty () > 0);
    ASSERT (! (m_results.size () % 2));
    ASSERT (! (m_changes.size () % 2));

    // Push remove request into the queue.
    struct kevent ke;
    memset (&ke, 0, sizeof (ke));
    ke.ident  = req.fd;
    ke.filter = EVFILT_READ;
    ke.flags  = EV_DELETE;
    m_changes.push_back (ke);
    
    ke.ident  = req.fd;
    ke.filter = EVFILT_WRITE;
    ke.flags  = EV_DELETE;
    m_changes.push_back (ke);

    // Shrink result array and fd map, remove request from base class
    m_results.resize (m_results.size () - 2);
    removeReq (req, purge (req.fd));
}

/** Remove all clients from this selector.  */
void
IOSelectKqueue::clear (void)
{
    ASSERT (! (m_results.size () % 2));
    ASSERT (! (m_changes.size () % 2));

    // Reset tracking indices
    m_current = m_nready = 0;

    // Clear vectors and reclaim memory
    { std::vector<struct kevent> tmp; std::swap (m_changes, tmp); }
    { std::vector<struct kevent> tmp; std::swap (m_results, tmp); }
    compact ();

    // Clear base class request tracker.
    clearReqs ();
}

//////////////////////////////////////////////////////////////////////
/// Set the I/O event mask of registration of @a fd to @a value.
void
IOSelectKqueue::doSet (IOFD fd, unsigned value)
{
    ASSERT (! (m_changes.size () % 2));
    ASSERT (! (m_results.size () % 2));
    request (fdindex (fd)).mask |= value;

    struct kevent ke;
    memset (&ke, 0, sizeof (ke));

    if (value & (IOAccept | IORead))
    {
	ke.ident  = fd;
	ke.filter = EVFILT_READ;
	ke.flags  = EV_ADD | EV_ENABLE;
	// ke.udata = ptr;
	m_changes.push_back (ke);
    }

    if (value & (IOConnect | IOWrite))
    {
	ke.ident  = fd;
	ke.filter = EVFILT_WRITE;
	ke.flags  = EV_ADD | EV_ENABLE;
	// ke.udata = ptr;
	m_changes.push_back (ke);
    }
}

/// Clear in @a fd's selection mask all bits not set in @a value.
void
IOSelectKqueue::doClear (IOFD fd, unsigned value)
{
    ASSERT (! (m_changes.size () % 2));
    ASSERT (! (m_results.size () % 2));
    request (fdindex (fd)).mask &= value;

    struct kevent ke;
    memset (&ke, 0, sizeof (ke));

    if (! (value & (IOAccept | IORead)))
    {
	ke.ident  = fd;
	ke.filter = EVFILT_READ;
	ke.flags  = EV_DELETE;
	m_changes.push_back (ke);
    }

    if (! (value & (IOConnect | IOWrite)))
    {
	ke.ident  = fd;
	ke.filter = EVFILT_WRITE;
	ke.flags  = EV_DELETE;
	m_changes.push_back (ke);
    }
}

/// Set the I/O event mask of registration of @a fd to @a value.
void
IOSelectKqueue::setMask (IOFD fd, unsigned value)
{
    // Simply set and clear unset bits.  The clear might not do
    // anything but avoids us having to keep state.
    doSet (fd, value);
    doClear (fd, value);
}

/// And the I/O event mask of registration at position @a index with @a value.
void
IOSelectKqueue::andMask (IOFD fd, unsigned value)
{ doClear (fd, value); }

/// Or the I/O event mask of registration at position @a index with @a value.
void
IOSelectKqueue::orMask (IOFD fd, unsigned value)
{ doSet (fd, value); }

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
IOSelectKqueue::wait (long msecs)
{
    timespec ts;
    ts.tv_sec = msecs / 1000;
    ts.tv_nsec = (msecs % 1000) * 1000000;

    std::vector<struct kevent> changes;
    std::swap (m_changes, changes);
    m_current = 0;

    int n = kevent (m_kqueue, &changes[0], changes.size (),
		    &m_results[0], m_results.size (),
		    msecs >= 0 ? &ts : 0);
    if (n == -1)
	throw IOError ("kevent()", errno);

    ASSERT (n >= 0);
    m_nready = n;
    return n > 0;
}

/** Get the next ready descriptor, as determined by last #wait().
    This might throw an #IOError if the next descriptor to be
    inspected was invalid.  */
IOFD
IOSelectKqueue::next (IOSelectEvent &event, IOSelectCB &cb)
{
    if (m_current == m_nready)
	return IOFD_INVALID;

    // FIXME: Compact multiple returns for the fd into one?
    struct kevent &ke = m_results [m_current++];
    IOSelectRequest &req = request (fdindex (ke.ident));
    ASSERT (req.channel);
    ASSERT (req.channel->fd () == (int) ke.ident);
    ASSERT (req.fd == (int) ke.ident);
    cb = req.cb;
    event.source = req.channel;
    if (ke.filter == EVFILT_READ)
	event.events = IORead;
    else if (ke.filter == EVFILT_WRITE)
	event.events = IOWrite;
    else // Huh?
	throw IOError ("kevent()", EINVAL);

    return ke.ident;
}

} // namespace lat
#endif // HAVE_KQUEUE
