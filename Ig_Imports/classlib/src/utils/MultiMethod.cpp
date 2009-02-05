//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/utils/MultiMethod.h"
#include "classlib/utils/PODVector.h"
#include "classlib/utils/Log.h"
#include <algorithm>
#include <utility>

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>

/** The multi-method logging flag. */
logflag		LFmultimethod = { 0, "multimethod", true, -1 };


//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Add a new member @a function.

    The formal argument types of @a function must be @a formalTypes.
    This should be a vector exactly as long as @a m_formal.  The
    vector should be statically allocated, and in any case valid as
    long as this function is a registered member of this family.

    Also marks the method dirty.  */
void
MultiMethod::Definition::extend (MemberFunction function,
				 XTypeInfo::ClassDef **formalTypes)
{
    ASSERT (function);
    ASSERT (formalTypes);

    Member item = { function, formalTypes };
    PODVector<Member>::reserve (m_family, INITIAL_FAMILY_SIZE);
    PODVector<Member>::push_back (m_family, item);
    m_dirty = true;
#ifndef NLOG
    if (PODVector<Member>::size (m_family) > m_familyMaxSize)
	m_familyMaxSize = PODVector<Member>::size (m_family);
    if (PODVector<Member>::capacity (m_family) > m_familyMaxCapacity)
	m_familyMaxCapacity = PODVector<Member>::capacity (m_family);
#endif
}

/** Remove a member @a function from the family.  */
void
MultiMethod::Definition::remove (MemberFunction function)
{
    ASSERT (function);

    PODVector<Member>::iterator pos = PODVector<Member>::begin (m_family);
    PODVector<Member>::iterator end = PODVector<Member>::end (m_family);
    while (pos != end && pos->m_function != function)
	++pos;

    ASSERT (pos != end);
    PODVector<Member>::erase (m_family, pos);
    m_dirty = true;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/** Allocate a new hunk of scores.  We allocate a vector of @a min or
    #SCORE_HUNK_SIZE contiguous scores, which ever is more.  */
inline MultiMethod::ScoreHunk *
MultiMethod::createScoreHunk (STDC::size_t min) const
{
    if (min < SCORE_HUNK_SIZE)
	min = SCORE_HUNK_SIZE;
    ScoreHunk *hunk = new ScoreHunk;
    hunk->m_free = hunk->m_scores = new Score [min];
    hunk->m_capacity = min;
    hunk->m_next = m_data->m_scoreHunks;
    m_data->m_scoreHunks = hunk;
    return hunk;
}

/** Ditch all scores.  */
void
MultiMethod::freeScoreHunks (void) const
{
    ScoreHunk *hunk = m_data->m_scoreHunks;
    ScoreHunk *next;

    while (hunk)
    {
	next = hunk->m_next;
	delete [] hunk->m_scores;
	delete hunk;
	hunk = next;
    }

    m_data->m_scoreHunks = 0;
}

/** Allocate a contiguous vector of at least @a n scores.  If we can't
    fit the requested scores in the last hunk we have, we allocate a
    new hunk.  This leaves a hole at the end of the previous hunk, but
    on average the amount waste should be small (#SCORE_HUNK_SIZE
    should be large compared to an average allocation request).  Note
    that scores are never freed individually, we always invalidate all
    scores for this method family (see #regenerate() and
    #freeScoreHunks()).  */
inline MultiMethod::Score *
MultiMethod::allocateScores (STDC::size_t n) const
{
    ScoreHunk	*hunk = m_data->m_scoreHunks
			? m_data->m_scoreHunks
			: createScoreHunk (n);
    Score	*scores;
    if (hunk->m_capacity < n)
	hunk = createScoreHunk (n);

    scores = hunk->m_free;
    hunk->m_free += n;
    hunk->m_capacity -= n;
    return scores;
}

//////////////////////////////////////////////////////////////////////
/** Allocate a new hunk of method type entries.  Since entries are
    allocated individually, there is no waste and no need to indicate
    how many are required.  We allocate them #ENTRY_HUNK_SIZE at a
    time.  */
inline MultiMethod::EntryHunk *
MultiMethod::createEntryHunk (void) const
{
    EntryHunk *hunk = new EntryHunk;
    hunk->m_next = m_data->m_entryHunks;
    hunk->m_free = hunk->m_entries = new Entry [ENTRY_HUNK_SIZE];
    hunk->m_capacity = ENTRY_HUNK_SIZE;
    m_data->m_entryHunks = hunk;
    return hunk;
}

/** Ditch all entries.  Note that the entries on the free list just
    point to entries within hunks, this is where the free list is
    actually getting cleared and freed back to the system.  */
void
MultiMethod::freeEntryHunks (void) const
{
    EntryHunk *hunk = m_data->m_entryHunks;
    EntryHunk *next;

    while (hunk)
    {
	next = hunk->m_next;
	delete [] hunk->m_entries;
	delete hunk;
	hunk = next;
    }

    m_data->m_entryHunks = 0;
    m_data->m_freeEntries = 0;
}

/** Allocate a new #Entry.  Picks one off the free list if that list
    is non-empty (see #freeEntry()), failing that one from the last
    entry hunk, and failing that allocates a new hunk and allocates
    one from it.  */
inline MultiMethod::Entry *
MultiMethod::allocateEntry (void) const
{
    Entry *item = m_data->m_freeEntries;
    if (item)
	m_data->m_freeEntries = item->m_next;
    else
    {
	EntryHunk *hunk = m_data->m_entryHunks
			  ? m_data->m_entryHunks
			  : createEntryHunk ();

	if (hunk->m_capacity == 0)
	    hunk = createEntryHunk ();

	hunk->m_capacity--;
	item = hunk->m_free++;
    }
    return item;
}

/** Mark an #Entry @a item free.

    Note that we allocate the records in hunks, so we never really
    free the #Entry until #freeEntryHunks() is called.  All we do is
    to put the record on a free list, where we can pick it up when we
    need a a new one.  We use the chains in the #Entry record to chain
    the free list, hence won't use any extra space for the list.
   
    Note that this freeing behaviour is deliberate---new entries are
    allocated for new types and freed when the type leaves the system
    (e.g. when loading and unloading shared libraries, respectively).
    When we begin to free entry records, it is more than likely that
    we are about to shut down and hence we will probably free all
    hunks shortly anyway, and hence there is not much of a point in
    spending a lot of effort here.  We just want to get the entries
    off the types as quickly as possible, but since they can be
    chained and are not allocated in blocks, we might just as well put
    them in a free list (which has uses when a system is legitimately
    loading and unloading shared libraries, e.g. to cache plug-in
    features).  */
inline void
MultiMethod::freeEntry (Entry *item) const
{
    item->m_next = m_data->m_freeEntries;
    m_data->m_freeEntries = item;
}

//////////////////////////////////////////////////////////////////////

/** Get the multi-method extension key to extended type information
    (#XTypeInfo).

    All multi-methods share the key and then chain their data via
    #Entry records.  We could allocate an extension key for each
    individual method (or even each individual method and argument
    position), which would speed up the look up during dispatch.  That
    would however also create longer extension vectors in @em every
    type, not just the ones we touch, and hence the space optimisation
    here at the expense of speed.
   
    This method exists solely to avoid initialisation ordering issues.
    The key will simply get allocated the first time something
    somewhere needs it; it is safe to be used in static constructors
    regardless of the order in which objects are initialised.  */
XTypeInfo::ExtensionKey
MultiMethod::extensionKey (void)
{
    // FIXME: thread safety!
    static XTypeInfo::ExtensionKey key = XTypeInfo::allocateExtension ();

    return key;
}

/** Allocate a key (an index) for this multi-method.  The key is used
    to disambiguate our #Entry records from those of other
    multi-methods, as we are chaining all records for all the methods
    in the same #XTypeInfo extension (see #extensionKey() and #Entry).
    Eeach method in the running system hence gets a unique key in the
    order in which the method constructors happen to run.  */
STDC::size_t
MultiMethod::methodKey (void)
{
    static STDC::size_t key = 0;
    return ++key;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Construct a multi-method.  We don't actually do anything here.
    The derived class will invoke #initialise() since only it has all
    the required information but has to prepare the information in
    ways more complex than what is possible in the constructor
    invocation.  */
MultiMethod::MultiMethod (void)
    : m_data (0)
{}

/* Destruct the multi-method.  Disengage myself from the #XTypeInfo
   monitoring hooks (and clean up #Entry records allocated for
   participating types), print out debugging statistics about the
   usage of this method, free all our allocated data and restore the
   static #Definition data to a state where it can be reinitialised.  */
MultiMethod::~MultiMethod (void)
{
    if (m_data && m_data->m_hooked)
    {
	// Mark us disengaged to avoid recursion inside typeUnhook.
	m_data->m_hooked = false;
	XTypeInfo::removeMonitor (this, true);
    }

#ifndef NLOG
    if (m_data)
    {
	std::pair<STDC::size_t,STDC::size_t>	scores (0, 0);
	std::pair<STDC::size_t,STDC::size_t>	entries (0, 0);
	STDC::size_t				free = 0;

	for (ScoreHunk *h = m_data->m_scoreHunks; h; h = h->m_next)
	{
	    scores.first += (h->m_free + h->m_capacity - h->m_scores);
	    scores.second += (h->m_free - h->m_scores);
	}
	for (EntryHunk *h = m_data->m_entryHunks; h; h = h->m_next)
	{
	    entries.first += (h->m_free + h->m_capacity - h->m_entries);
	    entries.second += (h->m_free - h->m_entries);
	}
	for (Entry *f = m_data->m_freeEntries; f; f = f->m_next)
	    ++free;

	LOG (0, trace, LFmultimethod,
	     "statistics for `" << m_data->m_name << '\'' << indent
	     << "\nscores:      allocated = " << scores.first
	     << " used = " << scores.second
	     << "\nentries:     allocated = " << entries.first
	     << " used = " << entries.second
	     << " free = " << free
	     << "\nmax members: allocated = " << m_data->m_familyMaxCapacity
	     << " used = " << m_data->m_familyMaxSize
	     << '\n' << undent);
    }
#endif // !NLOG

    freeEntryHunks ();
    freeScoreHunks ();

    if (m_data)
    {
	m_data->m_generation = 0;
	m_data->m_key = 0;
	m_data->m_formals = 0;
	m_data->m_formalTypes = 0;
	m_data->m_familyMaxSize = 0;
	m_data->m_familyMaxCapacity = 0;
    }
}

/** Initialise the multi-method.

    This method must be called directly after constructor by the
    derived class that knows the formal virtual argument types.  It
    must hand the statically allocated #Definition structure that
    constitutes the run-time data of this method (@a data) and a
    vector of formal virtual argument types (array of length @a
    formals starting at @a formalTypes).  We initialise the static
    data and mark it dirty so that the first #dispatch() will create
    the necessary scores.

    (FIXME: Fix XTypeInfo to dynamically create (possibly in another
    static pool already inside its data) a XTypeInfo that is
    perpetually valid so that the following can be done away with and
    we can just use XTypeInfo.  XTypeInfo would be created by the
    lazy xtypeid_type (for types only) from the static data, and
    registration would just add another info into the chain, and we
    would no longer have the problem of having to manufacture an
    invalid XTypeInfo on xtypeid, nor the ugliness of exposing the
    internal details to use ClassDefs.)
   
    Note that the formal argument types may still be incomplete at
    this point (as in not initialised in #XTypeInfo yet), hence we
    are not yet doing anything with them at this point.  Neither are
    we registering ourselves as a type monitor; that will take place
    on the first #dispatch() call.  This avoids creating the per-type
    #Entry records until we actually need them, especially in the case
    we don't get called at all.  */
void
MultiMethod::initialise (Definition *data, STDC::size_t formals,
			 XTypeInfo::ClassDef **formalTypes)
{
    ASSERT (! m_data);

    ASSERT (data);
    ASSERT (formals > 0);
    ASSERT (formalTypes);

    ASSERT (data->m_key == 0);
    ASSERT (data->m_hooked == false);
    ASSERT (data->m_generation == 0);
    ASSERT (data->m_formals == 0);
    ASSERT (data->m_formalTypes == 0);
    ASSERT (data->m_scoreHunks == 0);
    ASSERT (data->m_entryHunks == 0);
    ASSERT (data->m_freeEntries == 0);

    m_data = data;
    data->m_key = methodKey ();
    data->m_formals = formals;
    data->m_formalTypes = formalTypes;
    data->m_dirty = true;

    for (STDC::size_t formal = 0; formal < formals; ++formal)
	ASSERT (data->m_formalTypes [formal]);
}

//////////////////////////////////////////////////////////////////////

/** Check if @a type is related to any of the formal argument types,
    that is, either is or inherits from one of them.  Returns the
    index of the first formal argument position in which the type is
    related, or #Definition::m_formals plus one.  */
inline STDC::size_t
MultiMethod::relatedFormal (const XTypeInfo *type) const
{
    // FIXME: when destructors run, type might not be complete any
    // more?  happens on sun, assertion below triggers

    bool related = false;
    STDC::size_t formal = 0;
    for ( ; !related && formal < m_data->m_formals; ++formal)
    {
	// formal XTypeInfo must be complete now
	ASSERT (m_data->m_formalTypes [formal]->m_types);
	// FIXME: Does this work in the presence of several XTypeInfos
	//        registered for the same type?  We must be sure to always
	//        deal with the single instance, or accept them all; in
	//        particular, if a shared library A is loaded, then B, then
	//        A unloaded, we must be able to still do the right thing...
	related = (type == m_data->m_formalTypes [formal]->m_types
		   || type->hasBase (m_data->m_formalTypes [formal]->m_types));
    }

    return related ? formal-1 : formal;
}

/** Find the #Entry chain for this method in the @a type.

    Returns the pointer to the first #Entry record if found, null
    otherwise; in either case @a previous will point to the previous
    entry, or null if there is no previous record (e.g., for the first
    entry in the type's list).  @a previous should be initialised to
    null prior to the call.  Note that if @a previous is null on exit,
    it does not mean that there are no entries at all in this type;
    the caller should invoke @c type->getExtension(extensionKey())
    in that case to determine if there are other entries; the entry
    for the method should in that case be first in the extension's
    list.

    If @a formal is given, we search for the first entry in that or
    greater formal position.  If the entry found has the desired
    formal position, we return that entry; otherwise we return null
    and @a previous points to the previous record in the chain.  In
    the latter case the caller should then insert the record for that
    formal argument position after @a previous and before its
    #Entry::m_next.  If no @a formal is given, we look for the first
    #Entry for @a type in this method, no matter what its formal
    argument position is.

    The return value is the pointer to the first entry for this method
    that fulfils the @a formal requirement.  Other entries for this
    method are chained from that one via the #Entry::m_next links, in
    increasing formal argument order.  The chain includes entries for
    (and only for) the argument positions where @a type is related
    (i.e. is, or is inherited from) to the method formal argument at
    that position.  */
inline MultiMethod::Entry *
MultiMethod::findTypeEntries (const XTypeInfo *type,
			      Entry		*&previous,
			      STDC::size_t	key,
			      STDC::size_t	formal /* = STDC::size_t(-1) */)
{
    Entry *item = static_cast<Entry *> (type->getExtension (extensionKey()));
    while (item
	   && (item->m_key < key
	       || (item->m_key == key
		   && item->m_formal < formal
		   && formal != STDC::size_t(-1))))
    {
	previous = item;
	item = item->m_next;
    }

    return (item
	    && item->m_key == key
	    && (formal == STDC::size_t(-1)
		|| item->m_formal == formal))
	? item : 0;
}

/** Create an #Entry chain in @a type for all the formal argument
    positions in which it participates in this multi-method.

    We assume that the first argument position of interest is @a
    formal as returned by #relatedFormal() (a search optimisation in
    case #relatedFormal() has already been called).  In each #Entry
    record created we set the scores to null and the generation to
    zero to indicate that the scores should be rebuilt on the first
    use of this entry in dispatching.  That is, we don't provide the
    scores at this point, only the #Entry to simplify look up logic
    during #dispatch().  */
void
MultiMethod::typePrepare (STDC::size_t formal, XTypeInfo *type) const
{
    ASSERT (type);
    ASSERT (formal < m_data->m_formals);

    Entry *previous = 0;
    Entry *first = findTypeEntries (type, previous, m_data->m_key);
    ASSERT (! first);

    if (first)
    {
	// FIXME: can this ever happen?  shouldn't we prepare the type only
	// once ever?
#ifndef NDEBUG
        ASSERT (! previous || previous->m_next == first);
        ASSERT (first->m_key == m_data->m_key);
	ASSERT (first->m_formal == formal);
	ASSERT (first->m_generation <= m_data->m_generation);
	while (++formal < m_data->m_formals)
	{
	    // FIXME: Does this work in the presence of several XTypeInfos
	    //        registered for the same type?  We must be sure to always
	    //        deal with the single instance, or accept them all; in
	    //        particular, if a shared library A is loaded, then B, then
	    //        A unloaded, we must be able to still do the right thing...
	    if (type == m_data->m_formalTypes [formal]->m_types
		|| type->hasBase (m_data->m_formalTypes [formal]->m_types))
	    {
		first = first->m_next;
	        ASSERT (first);
		ASSERT (first->m_key == m_data->m_key);
		ASSERT (first->m_formal == formal);
		ASSERT (first->m_generation <= m_data->m_generation);
	    }
	}
	ASSERT (! first->m_next || first->m_next->m_key > m_data->m_key);
#endif // !NDEBUG
    }
    else
    {
        first = allocateEntry ();
        if (! previous)
	{
	    first->m_next = static_cast<Entry *> (type->getExtension (extensionKey ()));
	    VERIFY (type->setExtension (extensionKey (), first) == first->m_next);
	}
	else
	{
	    first->m_next = previous->m_next;
	    previous->m_next = first;
	}

	first->m_key = m_data->m_key;
	first->m_formal = formal;
	first->m_generation = 0;
	first->m_scores = 0; 
	previous = first;

	while (++formal < m_data->m_formals)
	{
	    // FIXME: Does this work in the presence of several XTypeInfos
	    //        registered for the same type?  We must be sure to always
	    //        deal with the single instance, or accept them all; in
	    //        particular, if a shared library A is loaded, then B, then
	    //        A unloaded, we must be able to still do the right thing...
	    if (type == m_data->m_formalTypes [formal]->m_types
		|| type->hasBase (m_data->m_formalTypes [formal]->m_types))
	    {
	        Entry *item = allocateEntry ();
		item->m_next = previous->m_next;
		item->m_key = m_data->m_key;
		item->m_formal = formal;
		item->m_generation = 0;
		item->m_scores = 0;

		previous->m_next = item;
		previous = item;	
	    }
	}
    }
}

/** Remove all #Entry records chained in @a type for this multi-method.

    We assume that the first formal argument position in which the
    type participates is @a formal.  We do not deallocate the scores
    here but instead just drop them on the floor.  They will be
    reclaimed when this multi-method is destructed or regenerates
    itself.

    Returns true if the type was one of the formal argument types.  */
bool
MultiMethod::typeClean (STDC::size_t formal, XTypeInfo *type) const
{
    ASSERT (type);
    ASSERT (formal < m_data->m_formals);

    Entry *previous = 0;
    Entry *first = findTypeEntries (type, previous, m_data->m_key);

    if (! first)
    {
	// We've already cleaned this type---we are in recursive clean.
	ASSERT (! m_data->m_hooked);
	return false;
    }

    ASSERT (! previous || previous->m_next == first);
    ASSERT (first->m_key == m_data->m_key);
    ASSERT (first->m_formal == formal);
    ASSERT (first->m_generation <= m_data->m_generation);

    bool argtype = false;
    for (Entry *next; first && first->m_key == m_data->m_key; first = next)
    {
	// FIXME: Does this work in the presence of several XTypeInfos
	//        registered for the same type?  We must be sure to always
	//        deal with the single instance, or accept them all; in
	//        particular, if a shared library A is loaded, then B, then
	//        A unloaded, we must be able to still do the right thing...
	argtype = (argtype || type == m_data->m_formalTypes[formal]->m_types);
	next = first->m_next;
	freeEntry (first);
    }

    if (! previous)
	VERIFY (type->setExtension (extensionKey (), first) != 0);
    else
	previous->m_next = first;

    return argtype;
}

/** Make sure @a type is ready to participate in this multi-method:
    that it has an #Entry chain allocated for each formal argument
    type in which it relates to this method.

    We do this hooking to simplify and speed up the logic in
    #dispatch().  This method will be called by #XTypeInfo once we
    have registered ourselves as a type monitor on first #dispatch():
    once for each type known to the system, and thereafter whenever
    new types enter the running system (e.g. when shared libraries are
    loaded).  We add the chain only to the types that are related to
    the formal arguments of this multi-method.  */
void
MultiMethod::typeHook (XTypeInfo *type)
{
    STDC::size_t formal = relatedFormal (type);
    if (formal != m_data->m_formals)
	typePrepare (formal, type);
}

/** Disable the @a type from participation in this multi-method: free
    the chain of its #Entry records for this multi-method.  Done to
    clean up a type that is about to exit the system.  */
void
MultiMethod::typeUnhook (XTypeInfo *type)
{
    STDC::size_t formal = relatedFormal (type);
    if (formal != m_data->m_formals
	&& typeClean (formal, type)
	&& m_data->m_hooked)
    {
	// We are still hooked as a monitor (and not yet even in the
	// process of disengaging), and one of our formal arguments
	// went away.  So unhook ourselves from all types right now.
	// Mark us unhooked first to notify recursive calls of this
	// function that we are already in the process if disengaging.
	m_data->m_hooked = false;
	XTypeInfo::removeMonitor (this, true);
    }
}

// FIXME: This needs to be outside functions due to mutually incompatible
// bugs in GCC 3.1 and Solaris CC 5.3.  GCC can't grok the first form in
// a function-local scope but does accept a "using namespace std::rel_ops;".
// However, Solaris CC doesn't find the operator with the latter.
#   if HAVE_NAMESPACE_STD_REL_OPS
    using std::rel_ops::operator!=;
#   elif HAVE_REL_OPS_IN_STD
    using std::operator!=;
#   endif

/** Clean up the dispatch scores for @a type and anything inheriting
    from it.

    The "clean up" amounts to dropping the scores on the floor; all
    scores are freed in one go elsewhere already anyway.  This method
    is only responsible for marking the scores in the #Entry records
    outdated and zeroing the generation marker.  This method is called
    only when the generation counter wraps, making the counter in the
    #Entry records unreliable.  So what we do here is a master reset
    to avoid reusing old scores.  */
void
MultiMethod::cleanScores (const XTypeInfo *type) const
{
    Entry *previous = 0;
    Entry *first = findTypeEntries (type, previous, m_data->m_key);
    
    ASSERT (first);
    ASSERT (! previous || previous->m_next == first);
    ASSERT (first->m_key == m_data->m_key);

    for ( ; first && first->m_key == m_data->m_key; first = first->m_next)
    {
	first->m_generation = 0;
	first->m_scores = 0;
    }

#if HAVE_NAMESPACE_STD_REL_OPS
    // FIXME (see above): using std::rel_ops::operator!=;
#elif HAVE_REL_OPS_IN_STD
    // FIXME (see above): using std::operator!=;
#endif

    XTypeInfo::DerivedIterator start = type->beginDerived ();
    XTypeInfo::DerivedIterator end = type->endDerived ();
    for ( ; start != end; ++start)
	cleanScores (*start);
}

/** Compare scores for sorting (by inheritance distance).  */
inline bool
MultiMethod::orderScores (const Score &x, const Score &y)
{ return x.m_distance < y.m_distance; }

/** Build the dispatch score vector for @a type for each argument
    position where @a type is related to the formal arguments.

    This method assumes that the scores for the base classes have
    already been built, and reuses that information by adding to the
    base class information the members that apply to this type.  We
    optimise away the scores table if we can (i.e. there is none, or
    there is only one base class with scores and no member functions
    are specific to this @a type).  If that happens, the dispatch
    vector distances will be off by some inheritance distance offset,
    but that doesn't actually affect the decisions we will make at
    #dispatch() time---but it will skew some of the debugging output
    to show incorrect distance values.

    If the score vectors would be empty, we make the pointer in the
    #Entry to be null.  Otherwise there will be a score for each
    member function that applies in the argument position to @a type
    (that is, @a type either is or inherits from the argument type
    accepted by the member function in that argument position).  The
    score vector is sorted by #Score::m_distance, that is, how far @a
    type is from the member function's accepted argument in
    inheritance (zero if @a type is the member's argument type).  If
    the vector exists, its last entry will be an extra sentinel with
    #LAST_SCORE member index and #INFINITE_DISTANCE inheritance
    distance.  */
void
MultiMethod::buildScores (const XTypeInfo *type) const
{
    LOG (0, trace, LFmultimethod, " " << type->standard ().name ());

    Entry *item = 0;
    Entry *previous = 0;

    // NB: all the base classes have there scores built by now

    // This type's entries are in a linked list, ordered by key and
    // formal.  Start building score tables for each formal position
    // in which this type participates.  We do this in two passes:
    // first to count how many scores will be need and the second time
    // to allocate, collect and sort them.  We also remember whether
    // there were fuctions specific to this type; if not we can try to
    // optimise the simple case where the class has just one base type
    // participating in this method -- and we can just use the scores
    // from the base.
    //
    // The last score in the score array will be given LAST_SCORE
    // index, INFINITE_DISTANCE distance.  The method may not have
    // that many functions and the type not that distant base classes,
    // respectively.  However, we only build the score tables if there
    // is a function that applies in that formal argument position.
    item = findTypeEntries (type, previous, m_data->m_key);

    for ( ; item && item->m_key == m_data->m_key; item = item->m_next)
    {
	// The type is known to be related to this argument position
	LOG (0, trace, LFmultimethod, ":" << item->m_formal);

	// Skip this type if we have already processed it
	if (item->m_generation == m_data->m_generation)
	    continue;

	// Remember the current generation and zap the scores.  The
	// m_scores may be non-null in the case this class was already
	// used in dispatch, but is now dirty through a later load of
	// a derived class used in actual arguments.
	item->m_generation = m_data->m_generation;
	item->m_scores = 0;

	STDC::size_t			  formal = item->m_formal;
	STDC::size_t			  related = 0;
	bool				  specific = false;
	PODVector<Member>::const_iterator memfun
	    = PODVector<Member>::begin (m_data->m_family);
	PODVector<Member>::const_iterator last
	    = PODVector<Member>::end (m_data->m_family);

	for ( ; memfun != last; ++memfun)
	{
	    // The member formal types must be complete by now and
	    // they must be known to inherit (or be) the formal
	    // arguments in the argument position.

	    // FIXME: Does this work in the presence of several XTypeInfos
	    //        registered for the same type?  We must be sure to always
	    //        deal with the single instance, or accept them all; in
	    //        particular, if a shared library A is loaded, then B, then
	    //        A unloaded, we must be able to still do the right thing...
	    ASSERT (memfun->m_formalTypes [formal]->m_types);
	    ASSERT (memfun->m_formalTypes [formal]->m_types
		    == m_data->m_formalTypes [formal]->m_types
		    || memfun->m_formalTypes [formal]->m_types->hasBase
		       (m_data->m_formalTypes [formal]->m_types));

	    specific = (specific || type == memfun->m_formalTypes [formal]->m_types);
	    related += (type == memfun->m_formalTypes [formal]->m_types
			|| type->hasBase (memfun->m_formalTypes [formal]->m_types));
	}

	// if none are related, do not build scores at all.
	LOG (0, trace, LFmultimethod, "/" << related);
	if (! related)
	    continue;

#if HAVE_NAMESPACE_STD_REL_OPS
	// FIXME (see above): using std::rel_ops::operator!=;
#elif HAVE_REL_OPS_IN_STD
	// FIXME (see above): using std::operator!=;
#endif

	// see if we can optimise the score table away
	if (! specific)
	{
	    STDC::size_t		relatedBases = 0;
	    XTypeInfo::BaseIterator	base = type->beginBases ();
	    XTypeInfo::BaseIterator	end = type->endBases ();
	    Entry			*saved = 0;
	    Entry			*baseinfo = 0;

	    for ( ; base != end && relatedBases <= 1; ++base)
		if ((baseinfo = findTypeEntries (base->base (), previous,
						 m_data->m_key, formal)))
		{
		    saved = baseinfo;
		    ++relatedBases;
		}

	    if (base == end && relatedBases == 1)
	    {
		// optimise scores away: use base class information
		LOG (0, trace, LFmultimethod, "o");
		item->m_scores = (saved ? saved->m_scores : 0);
		continue;
	    }
	}

	// build the scores
	XTypeInfo::BaseIterator base;

	item->m_scores = allocateScores (related+1);
	memfun = PODVector<Member>::begin (m_data->m_family);
	for (STDC::size_t score = 0, index = 0; memfun != last; ++memfun, ++index)
	{
	    // the actual argument types must be complete by now
	    // FIXME: Does this work in the presence of several XTypeInfos
	    //        registered for the same type?  We must be sure to always
	    //        deal with the single instance, or accept them all; in
	    //        particular, if a shared library A is loaded, then B, then
	    //        A unloaded, we must be able to still do the right thing...
	    ASSERT (memfun->m_formalTypes [formal]->m_types);
	    if (type == memfun->m_formalTypes [formal]->m_types)
	    {
		LOG (0, trace, LFmultimethod, "(" << type->standard ().name () << "@" << 0 << ")");
		ASSERT (score < related);
		ASSERT (index < LAST_SCORE);
		item->m_scores [score].m_index = index;
		item->m_scores [score].m_distance = 0;
		++score;
	    }
	    else if ((base = type->base (memfun->m_formalTypes [formal]->m_types))
		     != type->endBases ())
	    {
		LOG (0, trace, LFmultimethod, "(" << base->base ()->standard ().name ()
		     << "@" << base->distance () << ")");
		ASSERT (score < related);
		ASSERT (index < LAST_SCORE);
		item->m_scores [score].m_index = index;
		item->m_scores [score].m_distance = base->distance ();
		++score;
	    }
	}

	// ... add the sentinel
	item->m_scores [related].m_index = LAST_SCORE;
	item->m_scores [related].m_distance = INFINITE_DISTANCE;

	// ... and sort them
	std::sort (item->m_scores, item->m_scores+related, orderScores);

	// ... and we are done!
	LOG (0, trace, LFmultimethod, "c");
    }
}

/** The method is has new member functions: step the generation index
    up.

    This means that there is a new member function and all scores
    generated so far are invalid.  We just drop them on the floor.
    This will leave dangling score pointers in #Entry records---hence
    the scores of an #Entry are valid only if its generation equals to
    the method's generation.

    If this is the first time we step the generation, we hook this
    method as a #XTypeInfo type monitor.  This will immediately
    prepare all the related types to have their #Entry record
    chains as required by #dispatch().

    If the generation index wraps around to zero, we invalidate all
    #Entry records to avoid false generation matches (see
    #cleanScores()).  */
void
MultiMethod::newGeneration (void) const
{
    // FIXME: exception safety: block now so that multiple threads
    //        dispatching at the same time won't mess each other up.

    if (! m_data->m_hooked)
    {
	m_data->m_hooked = true;
	XTypeInfo::addMonitor (const_cast<MultiMethod *> (this), true);
    }

    // free all scores and get ready to allocate some more
    freeScoreHunks ();
    m_data->m_generation++;
    m_data->m_dirty = false;

    if (! m_data->m_generation)
    {
	// zap all scores -- we wrapped around
	for (STDC::size_t formal = 0; formal < m_data->m_formals; ++formal)
	{
	    // the XTypeInfo must be complete by now
	    ASSERT (m_data->m_formalTypes [formal]->m_types);
	    cleanScores (m_data->m_formalTypes [formal]->m_types);
        }

	// step one more to avoid zero generation index, that's what all
	// the entries now have, and all uninitalised entries always have
	m_data->m_generation++;
    }
}

/** @a type is about to be used in dispatch and doesn't have valid
    (generation-wise) entry yet; regenerate scores for the type now.

    As a little extra work we also update the scores for all the base
    types too, as that's how #buildScores() expects the world to be.  */
inline void
MultiMethod::regenerate (const XTypeInfo *type) const
{
    LOG (0, trace, LFmultimethod, "regenerating scores for "
	 << type->standard ().name () << " (" << m_data->m_name << "):");

    // FIXME: exception safety: block now so that multiple threads
    //        dispatching at the same time won't mess each other up.

    // Rebuild scores for this type, in reverse class inheritance order.
    // XTypeInfo already knows all the classes we need to know about,
    // and we just walk the base list in reverse order.  #buildScores()
    // automatically ignores unrelated types, so we won't waste time
    // checking for that here.

#if HAVE_NAMESPACE_STD_REL_OPS
    // FIXME (see above): using std::rel_ops::operator!=;
#elif HAVE_REL_OPS_IN_STD
    // FIXME (see above): using std::operator!=;
#endif

    ASSERT (type);
    XTypeInfo::BaseIterator	first = type->beginBases ();
    XTypeInfo::BaseIterator	last = type->endBases ();
    while (last != first)
	buildScores ((--last)->base ());

    buildScores (type);
    LOG (0, trace, LFmultimethod, '\n');
}

//////////////////////////////////////////////////////////////////////

/** Dispatch the method with actual virtual argument types as given in
    @a actualTypes.

    @a actualTypes should be as long a vector as there are formal
    virtual argument types in this multi-method.  The caller must have
    also allocated as working space a vector of pointers to scores (@a
    candidates) and a vector of scores (@a best), both the length of
    the number of formal virtual arguments in this method.  The
    contents of these scratch areas need not be initialised in any
    manner, and they won't contain any useful information on exit.
    Normally they would be allocated on stack, as the length is known
    to the caller statically and can easily be done by the compiler,
    but to this method the information is available only dynamically
    and would require the use of #alloca() or heap allocation.

    The return value is a pointer to the closest-matching member
    function.  The function never returns if no canidadate applies or
    if there is an ambiguity; both of these situations result in call
    to #abort().

    If the multi-method is dirty (the member functions list has
    changed) or @a actualTypes do not yet have their score vectors,
    the required information is (re)created automatically.

    The dispatching logic is built around the construction where each
    type in @a actualTypes has, for each argument position, a vector
    of member functions that apply (an #Entry and #Entry::m_scores).
    The vector is sorted by the distance of the actual type to the
    formal argument of that member function.  This method simply looks
    up the candidates in each argument position, and looks for a
    member that is best in one and no worse than the other candidates
    in other argument positions---exactly as C++ overloading rules
    work.  Both of these steps translate to very compact loops, which
    are not quite constant time look-ups, but very fast at any rate.
    The more specific member functions we have, the faster the lookup
    converges and the less the ambiguity check has work to do.  */
MultiMethod::MemberFunction
MultiMethod::dispatch (XTypeInfo **actualTypes,
		       Score **candidates,
		       Score *best) const
{
    ASSERT (m_data);
    ASSERT (m_data->m_key);

    if (m_data->m_dirty)
	newGeneration ();

    // Locate scoring information for the actual argument types.
    Entry *previous = 0;
    for (STDC::size_t formal = 0; formal < m_data->m_formals; ++formal)
    {
	Entry *data = findTypeEntries (actualTypes [formal], previous,
				       m_data->m_key, formal);

	if (data && data->m_generation < m_data->m_generation)
	    regenerate (actualTypes [formal]);

	if (! data || ! data->m_scores)
	    noViableAlt (actualTypes);

	ASSERT (data->m_generation == m_data->m_generation);
	candidates [formal] = data->m_scores;
    }

#ifndef NLOG
    LOG (1, trace, LFmultimethod, m_data->m_name
	 << " dispatch candidates are:\n" << indent);
    for (STDC::size_t formal = 0; formal < m_data->m_formals; ++formal)
    {
	LOG (1, trace, LFmultimethod, "[" << formal << "]: ");
	for (Score *s = candidates [formal]; s->m_index != LAST_SCORE; ++s)
	    LOG (1, trace, LFmultimethod, " <" << s->m_index << ","
		 << s->m_distance << ">");
	LOG (1, trace, LFmultimethod, "\n");
    }
    LOG (1, trace, LFmultimethod, undent);
#endif

    // Select a match; we only end up here if we do have something to
    // process.  There is no danger that the lists are empty with only
    // sentinels in them -- they will contain at least one with index
    // different from LAST_SCORE.
    bool found = true;
    best [0] = *candidates [0];
    do
    {
	found = true;
	for (STDC::size_t formal = 1; found && formal < m_data->m_formals; ++formal)
	{
	    STDC::size_t cand = 0;
	    do
		found = (best [0].m_index == candidates [formal][cand].m_index);
	    while (! found && candidates [formal][++cand].m_index != LAST_SCORE);

	    if (found)
		best [formal] = candidates [formal][cand];
	}
    } while (! found && (best [0] = *++(candidates[0])).m_index != LAST_SCORE);

    if (! found)
	noViableAlt (actualTypes);

    // We have a candidate in best now; make sure it is the only one.
    // If not, we have an ambiguity.  The situation can only arise for
    // the remaining members in candidates[0] with the same distance,
    // so we'll keep moving that one and looking up in the other
    // argument positions' lists to see if it the ambiguity candidate
    // has the same or better distance in all of them.  Note that we
    // can find a better candidate among those that we inspect if the
    // distance in the first argument is no worse and better in some
    // of the others.  In that case we change our best candidate.
    bool ambiguous = false;
    while (! ambiguous && (++candidates[0])->m_distance == best [0].m_distance)
    {
	bool betterInAll = true;
	bool better = false;

	found = true;
	for (STDC::size_t formal = 1; found && formal < m_data->m_formals; ++formal)
	{
	    STDC::size_t cand = 0;
	    do
		found = candidates[0][0].m_index == candidates[formal][cand].m_index;
	    while (! found
		   && candidates [formal][++cand].m_index != LAST_SCORE
		   && candidates [formal][cand].m_distance <= best [formal].m_distance);

	    if (found && candidates[formal][cand].m_distance < best[formal].m_distance)
		better = true;
	    else
		betterInAll = false;
	}

	if (betterInAll && better)
	{
	    best[0] = *candidates[0];
#if !defined NDEBUG && !defined NLOG
	    // update the information so we will report the distances right
	    for (STDC::size_t formal = 1; formal < m_data->m_formals; ++formal)
	    {
		STDC::size_t cand = 0;
		while (candidates [0][0].m_index != candidates [formal][cand].m_index)
		    ++cand;
		best [formal] = candidates [formal][cand];
	    }
#endif
	}
	else if (better)
	    ambiguous = true;
    }

    if (ambiguous)
	ambiguity (actualTypes, candidates, best);

    ASSERT (best [0].m_index < PODVector<Member>::size (m_data->m_family));
#if !NDEBUG
    LOG (1, trace, LFmultimethod, m_data->m_name << " (#"
	 << m_data->m_key << ") (");
    for (STDC::size_t formal = 0; formal < m_data->m_formals; ++formal)
	LOG (1, trace, LFmultimethod,
	     "virtual "
	     << m_data->m_formalTypes [formal]->m_types->standard ().name ()
	     << (formal == m_data->m_formals-1 ? " *)\n" : " *, "));
    LOG (1, trace, LFmultimethod, "  --> #" << best [0].m_index << " (");
    for (STDC::size_t formal = 0; formal < m_data->m_formals; ++formal)
	LOG (1, trace, LFmultimethod,
	     "virtual "
	     << (m_data->m_family.begin[best[0].m_index].m_formalTypes[formal]
		 ->m_types->standard ().name ())
	     << " * [d=" << best [formal].m_distance
	     << (formal == m_data->m_formals-1 ? "])\n" : "], "));
#endif // !NDEBUG
    return m_data->m_family.begin [best [0].m_index].m_function;
}

//////////////////////////////////////////////////////////////////////

/** Dead-end: declare that no possible match exists.

    The actual arguments of the call were @a actualTypes.  Dump out
    some debugging information that may help to diagnose the
    situation, including the actual argument types and possible
    candidates, and #abort().  Note that we will not throw an
    exception.  Theoretically, the application might be able to
    recover by loading some extra shared libraries on the exception,
    but for now we make this a hard error just like a compiler would
    do on static overload resolution.  */
void
MultiMethod::noViableAlt (XTypeInfo **actualTypes) const
{
#ifndef NDEBUG
    LOG (0, error, LFmultimethod, m_data->m_name << " (#"
	 << m_data->m_key << ") (");
    for (STDC::size_t formal = 0; formal < m_data->m_formals; ++formal)
	LOG (0, error, LFmultimethod,
	     "virtual "
	     << m_data->m_formalTypes [formal]->m_types->standard ().name ()
	     << (formal == m_data->m_formals-1 ? " *)\n" : " *, "));

    LOG (0, error, LFmultimethod,
	 "  no viable alternative for arguments (");
    for (STDC::size_t formal = 0; formal < m_data->m_formals; ++formal)
	LOG (0, error, LFmultimethod,
	     "virtual "
	     << actualTypes [formal]->standard ().name ()
	     << (formal == m_data->m_formals-1 ? " *)\n" : " *, "));

    LOG (0, error, LFmultimethod, "  candidates are:\n");
    PODVector<Member>::const_iterator memfun
	= PODVector<Member>::begin (m_data->m_family);
    PODVector<Member>::const_iterator last
	= PODVector<Member>::end (m_data->m_family);
    for (STDC::size_t n = 0; memfun != last; ++memfun, ++n)
    {
	LOG (0, error, LFmultimethod, "  [#" << n << "] (");
	for (STDC::size_t formal = 0; formal < m_data->m_formals; ++formal)
	    LOG (0, error, LFmultimethod,
		 "virtual "
		 << memfun->m_formalTypes [formal]->m_types->standard ().name ()
		 << (formal == m_data->m_formals-1 ? " *)\n" : " *, "));
    }
#endif // !NDEBUG
    abort ();
}

/** Dead-end: declare that there are several possible matches.

    The actual call arguments were @a actualTypes, and the last
    search state is in @a canidadates and @a best (see #dispatch()).
    Produce debugging output by continuing the search for other
    possible matches (#dispatch() stops on the first ambiguity, we
    have to find the rest here).  Then #abort().  Note that we will
    not throw an exception.  Theoretically the application might be
    able to recover by either unloading something, or by loading
    something more specific from a shared library.  For now we just
    make this a hard error just like the compiler would do on static
    overload resolution.  */
void
MultiMethod::ambiguity (XTypeInfo **actualTypes,
			Score **candidates,
			Score *best) const
{
#ifndef NDEBUG
    LOG (0, error, LFmultimethod, m_data->m_name << " (#"
	 << m_data->m_key << ") (");
    for (STDC::size_t formal = 0; formal < m_data->m_formals; ++formal)
	LOG (0, error, LFmultimethod,
	     "virtual "
	     << m_data->m_formalTypes [formal]->m_types->standard ().name ()
	     << (formal == m_data->m_formals-1 ? " *)\n" : " *, "));
    LOG (0, error, LFmultimethod,
	 "  ambiguous call (");
    for (STDC::size_t formal = 0; formal < m_data->m_formals; ++formal)
	LOG (0, error, LFmultimethod,
	     "virtual "
	     << actualTypes [formal]->standard ().name ()
	     << (formal == m_data->m_formals-1 ? " *)\n" : " *, "));
    LOG (0, error, LFmultimethod, "  candidates are:\n    (");
    ASSERT (best [0].m_index < PODVector<Member>::size (m_data->m_family));
    for (STDC::size_t formal = 0; formal < m_data->m_formals; ++formal)
    {
	const XTypeInfo *type
	    = m_data->m_family.begin [best [0].m_index].m_formalTypes[formal]->m_types;
	LOG (0, error, LFmultimethod,
	     "virtual "
	     << type->standard ().name ()
	     << " [d=" << best [formal].m_distance << "] "
	     << (formal == m_data->m_formals-1 ? " *)\n" : " *, "));
    }
    bool found;
    do {
	// Dump out an ambiguous candidate; The first time around this loop
	// the ambiguity is the one `candidates[0]' points to.  We look for other
	// ambiguities below in the second loop.
	best [0] = *candidates [0];
	ASSERT (best [0].m_index < PODVector<Member>::size (m_data->m_family));
	for (STDC::size_t formal = 0; formal < m_data->m_formals; ++formal)
	{
	    const XTypeInfo *type
		= m_data->m_family.begin[best[0].m_index].m_formalTypes[formal]->m_types;
	    LOG (0, error, LFmultimethod,
		 (formal == 0 ? "    (" : "")
		 << "virtual "
		 << type->standard ().name ()
		 << " [d=" << candidates [formal]->m_distance << "] "
		 << (formal == m_data->m_formals-1 ? " *)\n" : " *, "));
	}

	// Find the next ambiguity
	found = false;
	while (!found && (++candidates [0])->m_distance == best [0].m_distance)
	{
	    found = true;
	    for (STDC::size_t formal = 1; found && formal < m_data->m_formals; ++formal)
	    {
		STDC::size_t cand = 0;
		found = false;
		do
		    found = (candidates [0]->m_index == candidates [formal][cand].m_index);
		while (! found
		       && candidates [formal][++cand].m_index != LAST_SCORE
		       && candidates [formal][cand].m_distance <= best [formal].m_distance);
	    }
	}
    } while (found);
#endif // !NDEBUG
    abort ();
}

} // namespace lat
