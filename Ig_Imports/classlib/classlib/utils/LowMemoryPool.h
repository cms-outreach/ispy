#ifndef CLASSLIB_LOW_MEMORY_POOL_H
# define CLASSLIB_LOW_MEMORY_POOL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/utils/Callback.h"
# if HAVE_NEW
#  include <new>
# elif HAVE_NEW_H
#  include <new.h>
# endif

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Low memory pool management.

    This class allows the application to allocate memory reserve
    pools.  Whenever a new pool is added, a new memory reserve is
    added to the top of internal stack, and a new handler is
    registered.  When that handler is invoked, the top pool is freed
    and the application callback invoked.  If the last pool is freed
    and it is marked as a last resort pool, #std::bad_alloc is thrown;
    this allows parts of the program allocate last resort reserves
    without knowing whether others parts have already done so.  By
    adding several pools in a row the application can create a cascade
    effect: the later pools will fall back on the earlier ones.

    The application callback should not throw, except perhaps a
    #std::bad_alloc.  This limitation is caused by global @c new
    having a throw specification of #std::bad_alloc.  An attempt to
    throw any other exception will automatically be killed by the
    compiler run-time (and results in a call to #unexpected()).

    If the application is interactive, it should allocate two or three
    low-memory pools.  The first and smallest should be marked as a
    last resort pool.  The pool should be large enough to allow a
    graceful unwind and shutdown back to the main program.  Note that
    exception handling may require some amount of memory, especially
    if no exceptions have yet been thrown in that part of the program
    -- so by the time `operator new' throws `std::bad_alloc', it may
    no longer be possible to process that exception.  Unless the
    program can recover substantial amount of unused resources, it is
    unlikely to make sense to continue execution after the last pool
    is freed.  If the program does continue, it should re-allocate the
    reserve pools before doing anything else so that it can get back
    to a fault tolerant state (if the re-allocation fails, it probably
    is impossible to continue anyway).  Nevertheless, the program
    should attempt to write its state to a recovery file to ensure
    that no work is lost.  The best way to to achieve this is to
    preallocate the resources required (files, memory) when creating
    or opening the document, and them simply writing out the
    information on crash.

    The second and larger pool should not throw any exceptions.  The
    application hook should notify the user that memory is running
    low, and encourage the user to save the application state
    immediately, and if possible, to shut down unnecessary application
    functions (windows, disable allocation-intesive operations such
    opening new documents and other such things).  The hook could also
    reap unused resources or caches if there are any.  If some lengthy
    operation is going on (and consuming substantial amounts of
    memory), the hook should set an indicator that long operations
    inspect regularly such that the operation should be cancelled as
    soon as possible.  Note that it is not possible to throw a special
    warning exception; it is however possible to notify pre-registered
    observers.  The second pool should be large enough for the
    application to be able to save its state if the warning is heeded
    immediately (and provided that allocation-intensive operations are
    cancelled).

    The third and largest pool should act as a first memory-shortage
    warning.  The application hook should advise the user to disable
    unnecessary application features and encourage her to save the
    work as soon as possible.  It may not yet be necessary to disable
    any application features at this point.  Like the second hook,
    this one should set an indicator for allocation-intensive tasks to
    stop.

    If the application uses several layers of pools as described above
    and gracefully returns to normal operation, it should reallocate
    its pools rather quickly.  For an interactive application this can
    be done easily: in event-loop ``idle'' hooks, timer callbacks, or
    combinations of both (e.g. 15 second timer followed by wait until
    all user interface events are drained).  When the second pool is
    released, the application hook should disable application features
    that are likely to be allocation-intensive (such as opening or
    creating new documents).  Once the pool reallocation succeeds
    (this may not be possible on the first attempt as user may not
    have yet had the possibility of responding to the application
    prompt), the disabled features can be restored to normal.  An easy
    way of achieving all this is to check, for all menu items, tool
    bar buttons etc., the state of some global memory allocation
    status indicators.  This will limit the amount direct state
    manipulation that is required.

    Finally, please note that allocating the pools may throw a
    #std::bad_alloc.  You should protect your program logic, and in
    particular the pool cascade allocation against this (for example,
    your last resort pool may get released when you allocate the
    next-larger pool).  */
class LowMemoryPool
{
public:
    static void			add (unsigned kbytes, Callback notifier,
				     bool last = false);
    static void			reset (void);
    static unsigned		pools (void);
    static unsigned		size (unsigned pool);

private:
    LowMemoryPool (unsigned kbytes, Callback notifier, bool last);
    ~LowMemoryPool (void);

    static void			trigger (void);
    static bool			pop (void);

    LowMemoryPool		*m_next;
    char			*m_reserve;
    unsigned			m_size;
    Callback			m_callback;
# if HAVE_STD_NEW_HANDLER
    std::new_handler		m_old_handler;
# else
    new_handler			m_old_handler;
# endif
    bool			m_last;

    static LowMemoryPool	*s_pools;

    // undefined semantics
    LowMemoryPool (const LowMemoryPool &);
    LowMemoryPool &operator= (const LowMemoryPool &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_LOW_MEMORY_POOL_H
