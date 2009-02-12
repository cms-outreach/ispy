//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Framework/interface/IgRepContext.h"
#include "Iguana/Framework/interface/IgBrowserMethods.h"
#include "Iguana/Framework/interface/IgRepresentable.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Return the rep set table. */
IgRepSet::LookupTable &
IgRepSet::table (void)
{
    static LookupTable table;
    return table;
}

/** Create a new empty representation set for the @a object.  */
IgRepSet::IgRepSet (IgRepresentable *object)
    : m_object (object),
      m_first (0)
{}

/** Destroy the representation set, and thus all rep contexts and reps
    attached to it.  */
IgRepSet::~IgRepSet (void)
{
    VERIFY (table ().erase (m_object) == 1);

    // Get rid of contexts
    while (m_first)
	delete m_first;
}

/** Add a new rep context @a context to the set.  Adds @a context to
    the circular list of contexts.  */
void
IgRepSet::add (IgRepContext *context)
{
    ASSERT (context);
    ASSERT (context != m_first);

    if (! m_first)
    {
	context->chain (context);
	m_first = context;
    }
    else
    {
	IgRepContext *last = m_first;
	while (last->next () != m_first)
	{
	    ASSERT (last->next () != context);
	    last = last->next ();
	}
	context->chain (m_first);
	last->chain (context);
    }
}

/** Remove the @a context from this set.  */
void
IgRepSet::remove (IgRepContext *context)
{
    ASSERT (m_first);
    ASSERT (context);

    IgRepContext *previous = m_first;
    IgRepContext *pos = m_first->next ();

    while (pos != context)
    {
	ASSERT (pos);
	previous = pos;
	pos = pos->next ();
    }

    ASSERT (pos == context);
    ASSERT (previous->next () == context);

    IgRepContext *next = pos->next ();
    if (previous == context)
    {
	// the list is only context long, and `context' is its only element
	ASSERT (m_first == context);
	ASSERT (m_first->next () == m_first);
	m_first = 0;
    }
    else
    {
	// unchain this context
	previous->chain (next);
	if (m_first == context)
	    m_first = next;
    }
    context->chain (0);
}

/** Look for a context for the object and @a model in this set.
    Returns the context object if one exists, otherwise null.  */
IgRepContext *
IgRepSet::lookup (IgModel *model) const
{
    ASSERT (model);

    IgRepContext *context = m_first;
    do {
	if (! context)
	    return 0;

	if (context->model () == model)
	    return context;

	context = context->next ();
    } while (context != m_first);

    return 0;
}

/** Get a rep set for the @a object.  Returns the set if it exists.
    Otherwise, if @a create is @c true, makes a new set and returns
    that.  Otherwise returns a null pointer.  */
IgRepSet *
IgRepSet::associate (IgRepresentable *object, bool create /* = false */)
{
    LookupTable &lookup_table = table ();
    LookupTable::iterator pos = lookup_table.find (object);

    if (pos != lookup_table.end ())
    {
	ASSERT (pos->second);
	return pos->second;
    }
    else if (create)
    {
	IgRepSet *&set = lookup_table [object];
	set = new IgRepSet (object);
	return set;
    }
    else
	return 0;
}

/** Look up a representation for @a object in @a model.  Returns the
    #IgRep if one exists.  Otherwise if @a create is @c true, tries
    to make one with #IgBrowserMethods::represent().  If that succeeds,
    returns the #IgRep.  Otherwise returns null.  */
IgRep *
IgRepSet::lookup (IgRepresentable *object, IgModel *model, bool create)
{
    IgRepSet	 *set = IgRepSet::associate (object, create);
    IgRepContext *context = set ? set->lookup (model) : 0;

    if (! context && create)
	context = IgBrowserMethods::represent (object, model);

    return context ? context->rep () : 0;
}

/** Look up a for a context matching @a model, starting from @a context
    but ignoring @a context itself.  Returns the #IgRep if one exists.
    Otherwise if @a create is @c true, tries to make one for the object
    owning @a context with #IgBrowserMethods::represent().  If that
    succeeds, returns the #IgRep.  Otherwise returns null.  */
IgRep *
IgRepSet::lookup (IgRepContext *context, IgModel *model, bool create)
{
    IgRepContext *other = context->next ();

    while (other != context && other->model () != model)
	other = other->next ();

    if (other->model () == model)
	return other->rep ();
    else if (create)
    {
	other = IgBrowserMethods::represent (context->object (), model);
	ASSERT (! other || other->model () == model);
	ASSERT (! other || other->rep ());

    }

    return other ? other->rep () : 0;
}

/** Update all @em existing representations of @a object, using @a field
    as details (arbitrary information).  Every existing rep of the object
    is updated using #IgBrowserMethods::update().  You should normally use
    #invalidate() methods instead of this one. */
void
IgRepSet::update (IgRepresentable *object, unsigned field)
{
    // Check if the object has any representations
    IgRepSet *set = associate (object);
    if (! set)
	return;

    // Update all contexts
    IgRepContext *first = set->contexts ();
    IgRepContext *context = first;

    do
	IgBrowserMethods::update (object, context->rep (), field);
    while ((context = context->next ()) != first);
}

/** Update all @em existing reps in the same chain as @a context, except
    @a context itself.  Each is updated using #IgBrowserMethods::update().
    The @a field is arbitrary information passed down to that method.
    You should normally use #invalidate() methods instead of this one.  */
void
IgRepSet::update (IgRepContext *context, unsigned field)
{
    IgRepContext	*next = context->next ();
    IgRepresentable	*object = context->object ();

    for ( ; next != context; next = next->next ())
	IgBrowserMethods::update (object, next->rep (), field);
}

/** Update the @a object rep for @a model if one exists.  The @a field is
    arbitrary information passed down to #IgBrowserMethods::update().
    You should normally use #invalidate() methods instead of this one.  */
void
IgRepSet::update (IgRepresentable *object,IgModel *model, unsigned field)
{
    if (IgRep *rep = lookup (object, model, false))
	IgBrowserMethods::update (object, rep, field);
}

/** Invalidate all representations of @a object.  Use this method to indicate
    that object has been changed in ways that require reps to be updated.
    Unlike #update(), this method does the right thing by processing the
    object, recursively if necessary, to make sure every existing outdated
    representation is updated, and new ones are created as necessary.  The
    main difference is that #update() only handles reps that already exists
    -- it will not handle hierarchical objects correctly.  #invalidate()
    will call #update() on all objects as necessary.  It is recommended to
    always use this method to flag object updates.  The @a field is arbitrary
    information passed down to #IgBrowserMethods::invalidate() and eventually
    to #IgBrowserMethods::update().  */
void
IgRepSet::invalidate (IgRepresentable *object, unsigned field)
{
    IgBrowserMethods::invalidate (object, 0, field);
}

/** Invalidate representations of @a object in all other models except the
    one @a context belongs to.  See the other #invalidate() methods for more
    details.  */
void
IgRepSet::invalidate (IgRepContext *context, unsigned field)
{
    IgRepContext	*next = context->next ();
    IgRepresentable	*object = context->object ();

    for ( ; next != context; next = next->next ())
	IgBrowserMethods::invalidate (object, next->model (), field);
}

/** Invalidate representations of @a object only in @a model.  See the other
    #invalidate() methods for more details.  */
void
IgRepSet::invalidate (IgRepresentable *object, IgModel *model, unsigned field)
{
    IgBrowserMethods::invalidate (object, model, field);
}
