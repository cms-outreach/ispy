#ifndef CLASSLIB_IO_SELECTOR_H
# define CLASSLIB_IO_SELECTOR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/IOTypes.h"
# include "classlib/iobase/IOSelectCB.h"
# include <vector>

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IOChannel;
class IOSelectMethod;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Multi-strategy I/O channel listener.

    #IOSelector is a class for listening changes in the status of
    multiple I/O channels simultaneously.  It is intended to be used
    when an application has multiple I/O channels whose activity it
    wants to monitor and to whose states it wants to respond to.
    Typical uses include server applications that want to wait and
    respond to client activity, or clients that have more than one
    server to talk to.

    #IOSelector defines a basic platform-specific strategy for
    listening activity on the channels (#S_DEFAULT), sufficient for
    most basic needs.  Applications with more specific demands, such
    as very large number of clients or high performance requirements
    can customise the selector to particular platform-specific system
    calls.

    The general usage model of this class is to first attach one or
    more #IOChannel objects, perform a wait, respond to the updated
    channel states, perform further waits, and finally deregister the
    channels.  Channels can be added or removed at any time (FIXME:
    will changes erase current wait update status?).

    It is more efficient to register channels once and do multiple
    waits on them than to register them all, do one wait and then
    register all again.  While channels can be added and removed at
    any time that suits the application, changing the list as rarely
    as possible enables significant optimisation with some strategies
    (e.g. #S_DEVPOLL).  Clients that can sustain descriptor interest
    for a long time should use #dispatch().  Since #pump() always
    removes the descriptor before invoking the handler, it is by
    definition not able to sustain high performance, but may still
    suit certain I/O processing models.

    If the application needs to maintain connection to more than just
    a handful of clients, it is in general better to use this class
    than to use several threads with blocking I/O.  (FIXME: explain
    why or at least link to the documentation.)

    Limitations:

    - It is not possible to #attach() the same channel several times.

    - Selecting on the same I/O channel in multiple threads may not
      work on all systems and may cause unpredictable results on the
      systems where it works.  It will cause major havoc on Windows.

    - The client should always be prepared to get (FIXME: EWOULDBLOCK)
      when operating on a channel @em even if the selector has marked
      it active for I/O.  (FIXME: Explain why.)  Obviously the clients
      can make no assumptions on how much I/O is possible with the
      channel unless they have guarantees from some other source.  In
      general most clients are best modeled as state machines.

    - Most unix platforms cannot select on files, they will always be
      reported ready.  Some signal files available for use if the file
      pointer is not at the end of the file.  Windows in theory can,
      but this implementation simulates the unix behaviour for
      coherency.  On some systems #IOChannel::transfer() provides
      high-throughput operations that bypass cycling of channel
      contents via memory, for example it may be possible to send file
      contents directly to a socket.  Another possibility is to use
      asynchronous I/O.

    - For a descriptor selected for urgent data (out-of-band socket
      data), urgent trigger does not mean the client will be able to
      read the urgent data -- the read attempt may block or throw @c
      EWOULDBLOCK error.  The client may first have to read ordinary
      data to make room in system buffers for the out-of-band data, or
      perform other tricks.  For examples on correct handling of such
      conditions, including use of #Socket::isAtMark(), see GNU libc
      info, node "Out-of-Band Data".
 
    - Windows has only limited ability to listen to I/O handles and
      heterogeneous list of handles in particular.  There is better
      support for asynchronous multi-threaded I/O, somewhat similar to
      POSIX AIO, a rather different and potentially more complicated
      design paradigm.  This library works around most of the Windows
      limitations, but may spawn hidden threads to do its job.
      (FIXME: Can overlapped I/O (posix aio) or I/O completion ports
      (posix aio completion routines notifying a thread pool that uses
      lifo semantics) be combined with this meaningfully?)

    - (FIXME: Explain how this meshes with asynchronous i/o (posix
      aio, w32 overlapped i/o).  Explain the issues with w32,
      especially for handles obtained from outside sources and the
      limitations on simultaneous usage of handle vs. select. */
class IOSelector
{
public:
    /** Selector strategies.  */
    enum Strategy {
	/** Default selector strategy, should always be included as a
	    last-resort fallback if other methods are unavailable.  On
	    unix: select() with automatic switch to poll() if there are
	    too many file descriptors (FIXME not yet, sometimes poll() is
	    just a wrapper for select()).  On windows a multi-threaded
	    custom poller; the only alternative on this platform.  */
	S_DEFAULT,

	/** Use select().  */
	S_SELECT,

	/** Use real-time signals (real-time version of SIGIO) with
	    POSIX signal queueing.  Automatically uses #S_POLL as
	    fallback for handling overflow.  */
	S_RTSIGIO,

	/** #S_RTSIGIO with only one signal queued per file descriptor
	    which can significantly reduce overflow.  */
	S_RTSIGONE,

	/** Use /dev/poll.  */
	S_DEVPOLL,

	/** Use poll().  */
	S_POLL,

	/** Use kqueue().  */
	S_KQUEUE
    };

    /** Default strategy vector.  */
    CLASSLIB_API static const Strategy SV_DEFAULT [];

    /** Default high-load strategy vector.  */
    CLASSLIB_API static const Strategy SV_HIGHPERF [];

    IOSelector (const Strategy *strategy = SV_DEFAULT);
    virtual ~IOSelector (void);

    // virtual void	init (void);
    // virtual void	setOption (int option, int value);
    //// virtual void	setSignal (int sig);
    // virtual void	shutdown (void);

    // Attaching and detaching channels
    virtual void	attach (IOChannel *channel, unsigned mask,
				IOSelectCB c = IOSelectCB());
    virtual void	detach (IOChannel *channel);
    // virtual void	detach (unsigned index);
    virtual void	clear (void);

    // Channel status access
    // virtual unsigned	channels    (void) const;

    // virtual IOChannel *	channel     (unsigned index) const;
    // virtual unsigned	mask        (unsigned index) const;
    virtual void	setMask     (IOChannel *channel, unsigned value);
    virtual void	andMask     (IOChannel *channel, unsigned value);
    virtual void	orMask      (IOChannel *channel, unsigned value);

    // virtual unsigned	status      (unsigned index) const;
    // virtual void	clearStatus (unsigned index, unsigned valueMask);
    // virtual bool	canAccept   (unsigned index) const;
    // virtual bool	canConnect  (unsigned index) const;
    // virtual bool	canRead     (unsigned index) const;
    // virtual bool	canWrite    (unsigned index) const;
    // virtual bool	canUrgent   (unsigned index) const;

    // Selection
    virtual unsigned	dispatch    (long msecs = -1);
    virtual unsigned	pump        (long msecs = -1);
    virtual bool	wait        (long msecs = -1);
    virtual bool	nextReady   (IOSelectEvent &event, IOSelectCB &c);
    //< NB: No guarantees as to the order in which clients are processed!

    // virtual void	initWakeUpPipe (void);
    // virtual int	wakeUp (void);

private:
#if 0
    virtual unsigned	clients   (void) const = 0;
    virtual IOFD	fd        (unsigned index) const = 0;
    virtual unsigned	mask      (unsigned index) const = 0;
    virtual void	setMask   (unsigned index, unsigned value) = 0;
    virtual void	andMask   (unsigned index, unsigned value);
    virtual void	orMask    (unsigned index, unsigned value);
    virtual unsigned	status    (unsigned index) const = 0;
    virtual void	setStatus (unsigned index, unsigned value) = 0;
#endif

    IOSelectMethod	*m_strategy;
    // Pipe		*m_wakeUpPipe;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_IO_SELECTOR_H
