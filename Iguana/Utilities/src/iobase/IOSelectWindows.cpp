//<<<<<< INCLUDES                                                       >>>>>>

#include "src/iobase/IOSelectWindows.h"
#include "classlib/iobase/IOError.h"
#include "classlib/utils/DebugAids.h"
#ifdef _WIN32

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
IOSelectWindows::IOSelectWindows (void)
{ clear (); }

/** Destruct the strategy object.  Assumes #IOSelector has already
    called #clear().  */
IOSelectWindows::~IOSelectWindows (void)
{}

/** Attach the file descriptor @a fd with interest mask @a mask.  */
void
IOSelectWindows::attach (const IOSelectRequest &req)
{
    ASSERT (req.fd != IOFD_INVALID);

    // Record request into base class tracker.
    addReq (req);

    // Some channels can be used directly with WaitForMultipleObjects
    // or MsgWaitForMultipleObjects (e.g. files, consoles, processes
    // and named pipes).  Others (e.g. sockets) can use event objects
    // from for example overlapped or asynchronous I/O to signal and
    // trigger state change.

    // FIXME: Does this work?  How does state->mask () come into all
    // this?  How do we check that the object has been signalled?
    // Don't we need to create an event object from overlapped or
    // asynchronous I/O with the appropriate mask?

    // FIXME: Allow any number of objects, create a cascade of threads
    // to deal with at most 64 objects each.
    ASSERT (m_fds < MAXIMUM_WAIT_OBJECTS);
    m_w4 [m_fds++] = req.fd;
}

/** Remove the client registration at @a index.  */
void
IOSelectWindows::detach (const IOSelectRequest &req)
{
    ASSERT (req.fd != IOFD_INVALID);
}

/** Remove all clients from this selector.  */
void
IOSelectWindows::clear (void)
{
    // Clear base class request tracker
    clearReqs ();
}

//////////////////////////////////////////////////////////////////////
/// Set the I/O event mask of registration at position @a index to @a value.
void
IOSelectWindows::setMask (IOFD fd, unsigned value)
{
    ASSERT (fd != IOFD_INVALID);
}

/// And the I/O event mask of registration at position @a index with @a value.
void
IOSelectWindows::andMask (IOFD fd, unsigned value)
{ setMask (fd, request (findReq (fd)).mask & value); }

/// Or the I/O event mask of registration at position @a index with @a value.
void
IOSelectWindows::orMask (IOFD fd, unsigned value)
{ setMask (fd, request (findReq (fd)).mask | value); }

//////////////////////////////////////////////////////////////////////
size_t
IOSelectWindows::findReq (IOFD fd)
{ ASSERT (false); return 0; }

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
IOSelectWindows::wait (long msecs)
{
    bool gotone;

    // Initialise state
    m_fds = m_nready = m_sig = 0;
    m_ms = msecs >= 0 ? msecs : INFINITE;
    m_msgq = false;

    // Setup all registered descriptors
    for (size_t i = 0; i < m_polls.size (); ++i)
	m_polls [i]->start (this);

    // Now start poll threads
    for (size_t i = 0; i < m_pollers.size (); ++i)
	m_pollers [i]->run ();

    // Select
    m_sig = m_msgq
	    ? MsgWaitForMultipleObjects (m_fds, m_w4, false,
					 m_ms, QS_ALLEVENTS)
	    : WaitForMultipleObjects (m_fds, m_w4, false, m_ms);
    if (m_sig == WAIT_FAILED)
	throw IOError ("WaitForMultipleObjects()", GetLastError ());
    else if (m_sig >= WAIT_OBJECT_0 && m_sig < WAIT_OBJECT_0 + m_fds)
	gotone = true;
    else if (m_sig == WAIT_TIMEOUT)
	gotone = false;
    else
	throw IOError ("WaitForMultipleObjects()", GetLastError ());

    // Stop all poll threads.
    for (size_t i = 0; i < m_pollers.size (); ++i)
	m_pollers [i]->stop ();

    // Update channel status flags if at least one channel was ready
    if (gotone)
        for (size_t i = 0; i < m_polls.size (); ++i)
	    m_polls [i]->update (this);

    // Collect poll threads (FIXME: stop before update() above?)
    for (size_t i = 0; i < m_pollers.size (); ++i)
	m_pollers [i]->finalise ();

    // Zap the current file descriptor position so we'll reststart on
    // the next call to #next().  It is preincremented in #next().
    m_current = -1;

    // Indicate if any descriptor is ready for activity.
    return m_nready > 0;
}

/** Get the next ready descriptor, as determined by last #wait().  */
IOFD
IOSelectWindows::next (IOSelectEvent &event, IOSelectCB &cb)
{
    if (m_nready < 1)
	return IOFD_INVALID;

    unsigned events = 0;
    // FIXME: find next available handle (m_current, m_max, set events)

    if (! events)
	return IOFD_INVALID;

    if (events & IORead)
	VERIFY (m_nready--);
    if (events & IOWrite)
	VERIFY (m_nready--);
    if (events & IOUrgent)
	VERIFY (m_nready--);

    event.source = 0;
    event.events = events;
    // FIXME: return m_current;
    return IOFD_INVALID;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
IOSelectWindows::Poll::~Poll (void)
{}

void
IOSelectWindows::Poll::start (IOSelectWindows *in)
{}

void
IOSelectWindows::Poll::update (IOSelectWindows *in)
{}

void
IOSelectWindows::Poll::close (IOSelectWindows *in)
{}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
IOSelectWindows::Poller::~Poller (void)
{}

HANDLE
IOSelectWindows::Poller::handle (void) const
{ return INVALID_HANDLE_VALUE; }

void
IOSelectWindows::Poller::initialise (void)
{}

void
IOSelectWindows::Poller::run (void)
{}

void
IOSelectWindows::Poller::stop (void)
{}

void
IOSelectWindows::Poller::finalise (void)
{}

} // namespace lat
#endif // _WIN32
