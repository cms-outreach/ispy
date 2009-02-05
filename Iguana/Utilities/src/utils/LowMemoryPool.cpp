//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/utils/LowMemoryPool.h"
#include "classlib/utils/DebugAids.h"

// FIXME: Work around non-compliant msvc++ headers.  Remove this and
// switch back to std::* with a newer compiler/system library.
#if HAVE_STD_NEW_HANDLER
using std::new_handler;
using std::set_new_handler;
#endif

// FIXME: msvc++ rtl
//  set_new_mode can make malloc use new_handler

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

LowMemoryPool		*LowMemoryPool::s_pools = 0;

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Create a new pool. */
LowMemoryPool::LowMemoryPool (unsigned kbytes, Callback notifier, bool last)
    : m_next (s_pools),
      m_reserve (new char [kbytes * 1024]),
      m_size (kbytes),
      m_callback (notifier),
      m_old_handler (set_new_handler (&trigger)),
      m_last (last)
{
    // Note the order above: `operator new' may throw, we don't want
    // to do anything that violates the ASSERTs here until we can
    // actually grab the memory.  Callbacks use reference-counting, so
    // copying them won't harm.
    ASSERT (kbytes > 0);
    ASSERT (notifier);
    s_pools = this;
}

/** Destruct a pool.  A no-op, all work is done in #pop() or #reset().  */
LowMemoryPool::~LowMemoryPool (void)
{}

/** Release the top pool, restore the next most recent handler and
    invoke the callback.  Our globals will be reset so that #init()
    will be happy if called again.  */
void
LowMemoryPool::trigger (void)
{ if (pop ()) throw std::bad_alloc (); }

/** Release the top pool, freeing its reserve and invoking the callback.  */
bool
LowMemoryPool::pop (void)
{
    ASSERT (s_pools);
    ASSERT (s_pools->m_reserve);
    LowMemoryPool *pool = s_pools;
    s_pools = s_pools->m_next;

    delete pool->m_reserve;
    VERIFY (set_new_handler (pool->m_old_handler) == &trigger);
    ASSERT (pool->m_callback);
    pool->m_callback ();

    bool last = pool->m_last;
    delete pool;

    return last && ! s_pools;
}

/** Return the size of this pool. */
unsigned
LowMemoryPool::size (unsigned pool)
{
    LowMemoryPool *p = s_pools;
    while (p && pool > 0)
    {
	p = p->m_next;
	--pool;
    }

    ASSERT (p);
    return p->m_size;
}

/** Return the pool status: true if the pool is currently allocated. */
unsigned
LowMemoryPool::pools (void)
{
    LowMemoryPool *p = s_pools;
    unsigned	  n = 0;
    while (p)
    {
	p = p->m_next;
	++n;
    }

    return n;
}

/** Free all pools.  Does not throw an exception even if a last resort
    pool is removed, but does unwind the new handlers.  */
void
LowMemoryPool::reset (void)
{
    while (s_pools)
    {
	ASSERT (s_pools->m_reserve);
	LowMemoryPool *pool = s_pools;
	s_pools = s_pools->m_next;

	delete pool->m_reserve;
	VERIFY (set_new_handler (pool->m_old_handler) == &trigger);
	delete pool;
    }

    ASSERT (! s_pools);
}

/** Add a new pool to the top of the pool stack. */
void
LowMemoryPool::add (unsigned kbytes, Callback notifier, bool last /*= false*/)
{ new LowMemoryPool (kbytes, notifier, last); }

} // namespace lat
