//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/IOSelector.h"
#include "classlib/iobase/IOSelectEvent.h"
#include "classlib/iobase/IOChannel.h"
#include "classlib/iobase/IOError.h"
#include "classlib/utils/Signal.h"
#include "classlib/utils/DebugAids.h"
#include "src/iobase/IOSelectDevPoll.h"
#include "src/iobase/IOSelectKqueue.h"
#include "src/iobase/IOSelectPoll.h"
#include "src/iobase/IOSelectSelect.h"
#include "src/iobase/IOSelectSig.h"
#include "src/iobase/IOSelectSigFd.h"
#include "src/iobase/IOSelectWindows.h"

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

const IOSelector::Strategy IOSelector::SV_DEFAULT [] = {
    S_KQUEUE,
    S_SELECT,
    S_DEFAULT
};

const IOSelector::Strategy IOSelector::SV_HIGHPERF [] = {
    S_KQUEUE,
    // S_RTSIGONE,// FIXME: Off because client must set real-time signal
    // S_RTSIGIO, // FIXME: Off because client must set real-time signal
    S_DEVPOLL,
    S_POLL,
    S_SELECT,
    S_DEFAULT
};

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Construct a new empty selector using strategy choices from @a strategy,
    which must be a list of the #Strategy methods ending in #S_DEFAULT.  */
IOSelector::IOSelector (const Strategy *strategy /* = SV_DEFAULT */)
    : m_strategy (0)
{
    ASSERT (strategy);
    while (! m_strategy)
	switch (*strategy++)
	{
	case S_DEFAULT:
	case S_SELECT:
#ifdef _WIN32
	    m_strategy = new IOSelectWindows;
#else
	    m_strategy = new IOSelectSelect;
#endif
	    break;

	case S_RTSIGIO:
# if HAVE_POSIX_RT_SIGNALS && HAVE_F_SETSIG
	    // FIXME: Off because client must set real-time signal for this to work
	    m_strategy = new IOSelectSig;
# endif
	    break;

	case S_RTSIGONE:
# if HAVE_POSIX_RT_SIGNALS && HAVE_F_SETSIG && HAVE_F_SETAUXFL
	    // FIXME: Off because client must set real-time signal for this to work
	    m_strategy = new IOSelectSigFd;
# endif
	    break;

	case S_KQUEUE:
# if HAVE_KQUEUE
	    m_strategy = new IOSelectKqueue;
# endif
	    break;

	case S_DEVPOLL:
# if HAVE_DEV_POLL_H && HAVE_POLL
	    m_strategy = new IOSelectDevPoll;
# endif
	    break;

	case S_POLL:
# if HAVE_POLL
	    m_strategy = new IOSelectPoll;
# endif
	    break;

	default:
	    ASSERT (false);
	}
}

/// Destruct a selector.
IOSelector::~IOSelector (void)
{ clear (); delete m_strategy; }

/** Attach the @a channel to the selector with interest mask @a mask.
    Note that it is not possible for multiple clients to listen to the
    same channel with different interest masks.  It is not technically
    supported by most selection methods and in any case would cause
    clients to get events in random order and thus steal data from
    each other.  */
void
IOSelector::attach (IOChannel *channel, unsigned mask,
		    IOSelectCB cb /* = IOSelectCB() */)
{
    ASSERT (channel);
    ASSERT (channel->fd () != IOFD_INVALID);

    IOSelectRequest req;
    req.fd	= channel->fd ();
    req.channel	= channel;
    req.mask	= mask;
    req.cb	= cb;

    channel->select (m_strategy, req);
}

/// Remove all instances of @a channel registered to the selector.
void
IOSelector::detach (IOChannel *channel)
{
    if (! channel || channel->fd () == IOFD_INVALID)
	return;

    IOSelectRequest req;
    req.fd	= channel->fd ();
    req.channel	= channel;
    req.mask	= 0;

    m_strategy->detach (req);
}

void
IOSelector::clear (void)
{ m_strategy->clear (); }

//////////////////////////////////////////////////////////////////////
void
IOSelector::setMask (IOChannel *channel, unsigned value)
{ m_strategy->setMask (channel->fd (), value); }

void
IOSelector::andMask (IOChannel *channel, unsigned value)
{ m_strategy->andMask (channel->fd (), value); }

void
IOSelector::orMask (IOChannel *channel, unsigned value)
{ m_strategy->orMask (channel->fd (), value); }


//////////////////////////////////////////////////////////////////////
/** Wait on the registered channels for available I/O operations and
    dispatch the information to the callbacks.  If the client callback
    returns @c true, it is removed from the selector; all others are
    retained.  Note that since no channel can be subscribed more than
    once, it is not possible for the callback to re-subscribe itself
    and then return false.  If all callbacks are known to be single
    shot handlers (which hampers the performance of the selector but
    may not matter to the client), use #pump() instead.  */
unsigned
IOSelector::dispatch (long msecs /* = -1 */)
{
    if (! m_strategy->requests () || ! wait (msecs))
	return 0;

    unsigned		n = 0;
    IOSelectEvent	event;
    IOSelectCB		cb;

    while (nextReady (event, cb))
    {
	++n;
	if (cb && cb (&event))
	    detach (event.source);
    }

    return n;
}

/** Wait on the registered channels for available I/O operations and
    dispatch the information to the callbacks.  All callbacks are
    automatically deregistered before they are invoked; their return
    value is ignored.  This method is a convenince for implementing
    single-shot I/O handlers.  Since the handlers are removed before
    they are invoked, it is safe for them to re-subscribe themselves.
    Note that removing the handlers from the selector seriously limits
    the possible I/O bandwith; do not use #pump() (or in general,
    handlers that return @c true) when high number of I/O connections
    need to be managed.  #dispatch() has much better performance. */
unsigned
IOSelector::pump (long msecs /* = -1 */)
{
    if (! m_strategy->requests () || ! wait (msecs))
	return 0;

    unsigned		n = 0;
    IOSelectEvent	event;
    IOSelectCB		cb;

    while (nextReady (event, cb))
    {
	++n;
	detach (event.source);
	if (cb) cb (&event);
    }

    return n;
}

/** Wait on the registered channels for available I/O operations.

    If @a msecs is positive, waits at most that long for something to
    happen.  If it is negative, waits until at least one registered
    channel becomes available for I/O.  Setting @a msecs to zero
    specifies that the channels are probed without waiting.

    @return On return, the status' of all registered channels has been
            updated to reflect the current I/O availability status.
            The return value tells the number of channels available
            for I/O.  Use #nextEvent() repeatedly to process ready
	    channels.

    @throw  A #IOError in case of a problem, or any error thrown
    	    by the #IOSelectable or #IOChannel as it is probed.  */
bool
IOSelector::wait (long msecs /* = -1 */)
{ return m_strategy->wait (msecs); }

/** Get the next ready channel, as determined by last #wait().  */
bool
IOSelector::nextReady (IOSelectEvent &event, IOSelectCB &cb)
{ return m_strategy->next (event, cb) != IOFD_INVALID; }

} // namespace lat
