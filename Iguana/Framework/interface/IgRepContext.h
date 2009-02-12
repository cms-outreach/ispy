#ifndef IGUANA_FRAMEWORK_IG_REP_CONTEXT_H
# define IGUANA_FRAMEWORK_IG_REP_CONTEXT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/config.h"
# include "Iguana/Framework/interface/IgRepSet.h"
# include "Iguana/Framework/interface/IgRep.h"
# include "classlib/utils/DebugAids.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgRepresentable;
class IgRepSet;
class IgRep;
class IgModel;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** A multi-way association to link a representable object and a rep in a
    particular model.

  #IgRepSet for a #IgRepresentable consists of circular list of these
  context objects, one for each distinct #IgModel in which the
  representable object is represented.  This object is the multi-way
  association linking #IgRep, #IgModel and #IgRepSet together.
  Through the #IgRepSet link the original application object is also
  accessible.

  The client must create a rep and a context exactly as follows to
  guarantee that the rep and context are properly registered in the
  #IgRepSet of the application object:
    - it must first create the rep
    - it must then create a new context; it can either pass the rep
      set or the object (in which case a rep set is looked up or
      created as required), and it must also pass the rep and the
      model

  If the #IgRepresentable or the #IgRepSet are destroyed, all their
  #IgRepContext objects are also destroyed.  This will cause the
  #IgRep to be destroyed as well.  If the #IgRep is destroyed for any
  other reason, it must take care to destroy its context object as
  well.  The exact patterns in which these must be done are described
  in #IgRep documentation.

  Note that it is not permissible to create a rep that inherits both
  #IgRep and #IgRepContext.  (FIXME: Killed by non-virtual destructor
  which is invoked by the #IgRepSet -- need to arrange for more
  flexible erasure patterns.)  */
class IGUANA_FRAMEWORK_API IgRepContext
{
public:
    IgRepContext (IgRepSet *set, IgRep *rep);
    IgRepContext (IgRepresentable *object, IgRep *rep);
    ~IgRepContext (void);

    IgRepContext *	next (void) const;

    IgRepresentable *	object (void) const;
    IgRepSet *		set (void) const;
    IgRep *		rep (void) const;
    IgModel *		model (void) const;
    void		erase (bool zap);

private:
    friend class IgRepSet;
    void		chain (IgRepContext *next);

    IgRepContext	*m_next; /**< Pointer to the next context in
				      the chain.  Note that the list
				      is circular so it may point back
				      to this object, meaning that
				      there are no other contexts in
				      the list.  */
    IgRepSet		*m_set;	 /**< Pointer to the set owning this
				      context.  The set is uniquely
				      linked with actual
				      #IgRepresentable.  */
    IgRep		*m_rep;  /**< Pointer to the #IgRep.  */

    // no semantics
    IgRepContext (const IgRepContext &);
    IgRepContext &operator= (const IgRepContext &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

/** Construct a new representation context.  You can specify either
    the object that the @a rep belongs to, or its #IgRepSet.  */
inline
IgRepContext::IgRepContext (IgRepSet *set, IgRep *rep)
    : m_next (0),
      m_set (set),
      m_rep (rep)
{
    ASSERT (set);
    ASSERT (rep);
    ASSERT (rep->model ());

    set->add (this);
    ASSERT (m_next);
    rep->context (this);
}

/** @overload */
inline
IgRepContext::IgRepContext (IgRepresentable *object, IgRep *rep)
    : m_next (0),
      m_set (IgRepSet::associate (object, true)),
      m_rep (rep)
{
    ASSERT (m_set);
    ASSERT (rep);
    ASSERT (rep->model ());

    m_set->add (this);
    ASSERT (m_next);
    rep->context (this);
}

/** Destruct a rep context.

  See the class documentation for the destruction protocol.

  The internals are as follows.  If #IgRepSet is destroying the
  context (the application object is dying), #m_set is set and we
  remove ourselves from it; this will cause #chain() to be invoked in
  a manner that clears #m_set.  We also delete the #IgRep; see the
  destruction protocol documentation in #IgRep for more details on
  writing the destructor correctly.  If the #IgRep is destroying the
  context, it will already have invoked #erase(), which has detached
  myself from the #IgRepSet, #m_set is null and there is nothing to
  do here.  */
inline
IgRepContext::~IgRepContext (void)
{
    if (m_set) erase (true);
    ASSERT (! m_set);
    ASSERT (! m_next);
    ASSERT (! m_rep);
}

/** Disengage the rep context from both the #IgRep and the #IgRepSet,
    optionally deleting the #IgRep.

  This method is used both internally (see the @link #~IgRepContext
  for details) and by #IgRep implementations.  The latter must call
  this method with @c false argument before destroying the context.

  @param zap Pass @c true if the #IgRep should be destroyed.
             Otherwise the context will just disengage from it.  */
inline void
IgRepContext::erase (bool zap)
{
    // If we are being killed by the IgRepresentable, on first call
    // (called from ~IgRepContext) zap will be true, we disengage from
    // the set; on the second call (from ~IgRep) zap will be false, do
    // nothing.
    //
    // If we are getting called from ~IgRep, zap will be false, we
    // disengage from the set and do not destroy the rep.

    // Check that we are indeed in one of the three above cases.
    ASSERT ((m_set && !zap && m_rep)	// From ~IgRep: must still have rep
	    || (m_set && zap && m_rep)	// 1st round: must zap and have rep
	    || (!m_set && !zap && !m_rep)); // 2nd round: no zap, no rep

    // Remove from the set if still necessary
    if (m_set)
	m_set->remove (this);
    ASSERT (! m_set);
    ASSERT (! m_next);

    // Get rid of rep -- null it out and possibly also delete it
    IgRep *rep = m_rep;
    m_rep = 0;
    if (zap)
	delete rep;
}

/** Get the next context in the circular list.  */
inline IgRepContext *
IgRepContext::next (void) const
{
    return m_next;
}

/** Get the set belonging to the object that rep represents.

  The set is the owner of this object.  */
inline IgRepSet *
IgRepContext::set (void) const
{
    return m_set;
}

/** Get the rep owned by the context.  */
inline IgRep *
IgRepContext::rep (void) const
{
    return m_rep;
}

/** Get the model in which the rep belongs to.  */
inline IgModel *
IgRepContext::model (void) const
{
    return m_rep->model ();
}

/** Get the object the rep belongs to.  */
inline IgRepresentable *
IgRepContext::object (void) const
{
    return m_set->object ();
}

/** Chain or unchain the context in the circular list.

  This method is used both internally and by the #IgRepSet.

  @param next If null, indicates that the context is being detached
              from the set.  If set, just sets the next pointer to
              that value.  Note that since the list is singly linked,
              the caller must ensure that previous context is updated
              appropriately.  The caller must also ensure that the
              list remains circular.  */
inline void
IgRepContext::chain (IgRepContext *next)
{
    m_next = next;
    if (! next)
	m_set = 0;
}

#endif // IGUANA_FRAMEWORK_IG_REP_CONTEXT_H
