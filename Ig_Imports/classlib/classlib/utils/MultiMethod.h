#ifndef CLASSLIB_MULTIMETHOD_H
# define CLASSLIB_MULTIMETHOD_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/utils/XTypeInfo.h"
# include "classlib/utils/PODVector.h"
# include "classlib/utils/Macros.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>

// Usage:
//   x.h:
//     MULTIMETHOD_DECLARE (extern API,Point,intersect,2,0,
//                          (Trajectory,Surface));
//   x.cc:
//     MULTIMETHOD_DEFINE (::, intersect);
//      NB: `::' because the multi-method is at global scope!
//
//   y.cc:
//     MMM_DEFUN_FUNC (Point,::,intersect,(Line *line, Plane *plane))
//     { Point p = /* use `line' and `plane' */; return p; }
//
//      NB: `::' is the scope of the multi-method, in this case global.
//
// or:
//   X.h:
//     class X {
//     public:
//	  MULTIMETHOD_DECLARE(static,Point,intersect,2,0,(Trajectory,Surface));
//     };
//   X.cc:
//     MULTIMETHOD_DEFINE (X::,intersect);
//      NB: `X::' is the scope of the multi-method.
//
//   Y.h:
//     class Y : public X {
//     public: // <-- multimethod members *must* be public
//	  static MMM_DECLARE (Point,intersect,(Line *,Plane *));
//     };
//   Y.cc:
//     MMM_DEFUN_METHOD(Point,X::,Y::,intersect, (Line *line, Plane *plane))
//     { Point p = /* use `line' and `plane' */; return p; }
//
//      NB: `X::' is the scope of the multi-method.  `Y::' is the
//          scope of the family member.

// All this would be so simple if we had GNU or ISO C 9x vararg macros...
#  define MULTIMETHOD_ARGS_1_0(V1)		V1
#  define MULTIMETHOD_ARGS_1_1(V1,A1)		V1,		A1
#  define MULTIMETHOD_ARGS_1_2(V1,A1,A2)	V1,		A1, A2
#  define MULTIMETHOD_ARGS_2_0(V1,V2)		V1, V2
#  define MULTIMETHOD_ARGS_2_1(V1,V2,A1)	V1, V2,		A1
#  define MULTIMETHOD_ARGS_2_2(V1,V2,A1,A2)	V1, V2,		A1, A2
#  define MULTIMETHOD_ARGS_3_0(V1,V2,V3)	V1, V2, V3
#  define MULTIMETHOD_ARGS_3_1(V1,V2,V3,A1)	V1, V2, V3,	A1
#  define MULTIMETHOD_ARGS_3_2(V1,V2,V3,A1,A2)	V1, V2, V3,	A1, A2

#  define MULTIMETHOD_DECLARE(ext,rettype,name,nvirt,nnonvirt,args)	\
    ext lat::MultiMethod::Definition name ## Def;			\
    typedef lat::MultiMethod_##nvirt##_##nnonvirt<			\
      rettype, MULTIMETHOD_ARGS_##nvirt##_##nnonvirt args		\
    > name##Type;							\
    ext name##Type name

#  define MULTIMETHOD_DEFINE(scope,name)				\
    lat::MultiMethod::Definition scope name ## Def = 		\
      { #name, 0, false, false, 0, 0, 0, { 0, 0, 0 }, 0, 0, 0, 0, 0 };	\
    scope name ## Type scope name (&name ##Def)

#  define MMM_SYM(pre,line,post)		CLASSLIB_name3(pre,line,post)
#  define MMM_DECLARE(ret,name,proto)		ret mmm_##name proto
#  define MMM_DECLARE_STUB(mscope,scope,name,proto,regsym)		\
    static lat::MultiMethodMember< mscope name##Type,proto,	\
	&scope mmm_##name, &mscope name##Def> regsym
#  define MMM_DEFUN_METHOD(ret,mscope,scope,name,proto)			\
    MMM_DECLARE_STUB(mscope,scope,name,ret (*) proto,			\
		     MMM_SYM(name,Init,__LINE__));			\
    ret scope mmm_##name proto
#  define MMM_DEFUN_FUNC(ret,mscope,name,proto)				\
    ret mmm_##name proto;						\
    MMM_DECLARE_STUB(mscope,::,name,ret (*) proto,			\
		     MMM_SYM(name,Init,__LINE__));			\
    ret mmm_##name proto

//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Base class for multi-methods.

    This class implements most of the multi-method maintenance and
    dispatching functionality.  Classes derived from this provide the
    actual interface layer needed for any real use.

    @em Implementation

    It can be very space consuming to store the full member matrix
    when the number of involved classes is large.  For example, if
    there are fifty classes that may participate for each argument
    position, a three-virtual multi-method results in 125000-cell
    grid---and the grid is likely to be sparsely populated.  It is
    also expensive in time to store the functions in the family just
    as a plain list and search on each call (not that computing the
    full matrix as described above would be cheap either).

    This implementation aims for a compromise that is efficient both
    in space and time.  We store only the grid cells that exist and
    are needed, and in a manner that minimises the search time at
    method dispatch time.  The call overhead is not constant as it
    would be in a direct matrix lookup, but frequently called family
    members get close.

    For each formal argument position and type involved in the
    dispatch (and hence inheriting from the virtual argument types of
    the multi-method), we record a dispatch vector of viable candidate
    functions: an indication of how far the member's accepted argument
    is from that actual type in that argument position.  During a
    call, knowing the actual argument types, we look up the dispatch
    vectors for each argument position and traverse the candidates,
    looking for a member that is the best candidate in at least one
    argument position and no worse in any other argument position.

    If a winning candidate is found, it is called.  If no winning
    candidate is found, a fatal error is signalled either for no
    viable alternative or for ambiguity.  The rules for these
    situations are the same as in C++ overload resolution, except that
    the error will arise only at run-time---it is not possible to
    diagnose the errors statically.  Signalling the error means here
    printing a diagnostic and calling #abort(); there is no way to
    recover.  In particular, no exceptions will be thrown.

    Another important feature is that members and participating types
    can be added to and removed from the method.  The method
    recalculates the dispatch vectors described above only when
    called, and only for the types (and base types) in the actual
    arguments given.  This on-demand update method therefore never
    recalculates the call "matrix" if the method is not actually
    called after the type or member changes.  On the other hand, the
    behaviour of the method can be changed at run-time by dynamically
    loading and unloading shared libraries.

    This implementation is not fully thread-safe.  Introducing thread
    safety should be a matter of only a small amount of work: the
    methods that initialise the multi-method or alter the member lists
    need to be protected, in addition to protecting the operation of
    updating candidate vectors for participating types.  Calling
    multi-methods is thread-safe, although it should be guarded with
    locks that prohibit information from being changed while dispatch
    is underway.  */
class MultiMethod : protected XTypeInfo::Monitor
{
public:
    struct Definition;
#if ! HAVE_BROKEN_CLASS_ACCESS
protected:
#endif
    struct Score;
#if ! HAVE_BROKEN_CLASS_ACCESS
private:
#endif
    struct Member;
    struct ScoreHunk;
    struct EntryHunk;
    struct Entry;

    friend struct Definition;
    friend struct Member;
    friend struct ScoreHunk;
    friend struct EntryHunk;
    friend struct Entry;
public:
    /** Placeholder type for actual method implementations.

	Any function pointer type will do; the function will never be
	actually called through this pointer.  ISO C++ guarantees that
	the function pointers can be casted safely back and forth. */
    typedef void (*MemberFunction) (void);

    /** Actual data structure defining the multi-method.

	This structure is the actual data for a multi-method.  It
	needs to be separate from the multi-method object due to the
	lack of initialisation order guarantees: the data for the
	method is stored separately so that members can be registered
	regardless of whether or not the multi-method object itself
	has been initialised.  This is necessary since the static
	storage duration objects registering members can execute in a
	pretty much random orderd, and nothing guarantees that the
	multi-method object is initialised before the members are.  */

    struct Definition
    {
	/** Method name captured by the multi-method definition
	    macros.  Used only for debugging output.  */
	const char		*m_name;

	/** Unique method key.  Starts at one and incremented for each
	    multi-method in the application.  */
	STDC::size_t		m_key;

	/** Flag set by the first #regenerate() call to indicate that
	    this method has been hooked as a #XTypeInfo type monitor.  */
	bool			m_hooked;

	/** Indicates whether #Entry structures are out of date.  Set
	    when members are added or removed.  The structures are
	    recomputed lazily (in #dispatch()) only when needed.  */
	bool			m_dirty;

	/** Indicates the method generation: index bumped up each time
	    #m_dirty is cleared in #dispatch().  #Entry has a similar
	    field to record which generation its scores are valid for.  */
	STDC::size_t		m_generation;

	/** The number of virtual arguments.  */
	STDC::size_t		m_formals;

	/** An array of formal types of size #m_formals.  Each element
	    points to the #XTypeInfo for that formal argument.  The
	    actual parameters can be classes derived from the formals.  */
	XTypeInfo::ClassDef	**m_formalTypes;


	/** The functions registered to this multi-method.  To make
	    this a POD type we use #PODVector and not @c std::vector.  */
	PODVector<Member>::Data	m_family;

	/** Maximum size of #m_family ever.  Used only to report statistics. */
	STDC::size_t		m_familyMaxSize;

	/** Maximum capacity of #m_family.  Used only to report statistics.  */
	STDC::size_t		m_familyMaxCapacity;

	/** A singly linked list of hunks of #Score arrays.  This is a
	    custom #Score memory allocator: as they are very small, it
	    is beneficial to allocate them in larger quantities (see
	    #SCORE_HUNK_SIZE) and pick one from the list when needed.
	    When regenerating the multimethod tables we invalidate and
	    free all the hunks at once instead of cleaning up each
	    class separately (see #m_generation).  Allocating in hunks
	    also improves the #dispatch() data locality by keeping the
	    scores in contiguous blocks. */
	ScoreHunk		*m_scoreHunks;

	/** Like #m_scorehunks, but for #Entry objects.  Not freed and
	    reallocated as often, but still the same principle. */
	EntryHunk		*m_entryHunks;

	/** Freed and unused #Entry structs, chained here when types
	    are unregistered and we no longer need the #Entry.  Each
	    list member is already part of the #m_entryhunks.  */
	Entry			*m_freeEntries;

	void			extend (MemberFunction function,
					XTypeInfo::ClassDef **formalTypes);
	void			remove (MemberFunction function);
    };

#if ! HAVE_BROKEN_CLASS_ACCESS
protected:
#endif
    /** Method scoring information.

	Each type used in #dispatch() has an #Entry pointing to an
	array of scores to rank the functions available.  That is, the
	vector describes which functions could take as an argument the
	type for which the #Entry is recorded; the score tells how far
	in inheritance the type is from the formal function argument
	type.  There is a separate #Score vector for every argument
	position in which the type participates.

	The #Score arrays are sorted by increasing #m_distance for a
	partial ranking of the methods in best match order.  The
	#dispatch() is then reduced to looking up the #Score array for
	every argument position, and then picking the method that is
	best (#m_index) in all of them.

	Each sorted #Score vector has a trailing sentinel record with
	#m_index equal to #LAST_SCORE.  If the #Score array would be
	empty except for the sentinel, the whole array is suppressed.  */
    struct Score
    {
	/** The inheritance distance between the type in which the
	    entry is recorded and the formal argument type of the
	    member function this score ranks.  This may be off by some
	    bias in case a derived class shares the scores of its base
	    class.  That will happen only under circumstances where it
	    cannot affect the dispatching decision.  Only the debug
	    output will show incorrect (too small) distances. */
	unsigned short		m_distance;

	/** The index of the member function in the members array or
	    #LAST_SCORE for the sentinel record.  */
	unsigned short		m_index;
    };

    MultiMethod (void);
    ~MultiMethod (void);
    
    void			initialise (Definition *data,
					    STDC::size_t formals,
					    XTypeInfo::ClassDef **formalTypes);

    MemberFunction		dispatch (XTypeInfo **actualTypes,
					  Score **candidates,
					  Score *best) const;

#if ! HAVE_BROKEN_CLASS_ACCESS
private:
#endif
    /** Initial size of the number of member function allocation.

	An array of this many pointers to functions will be allocated
	when the first family member is registered.  Thereafter the
	array will be doubled in size every time it overgrows beyond
	its previous allocation.  */
    enum { INITIAL_FAMILY_SIZE = 32 };		// FIXME: STDC::size_t

    /** The number of #Score objects to allocate per #ScoreHunk.  */
    enum { SCORE_HUNK_SIZE = 64 };		// FIXME: STDC::size_t

    /** The number of #Entry objects to allocate per #EntryHunk.  */
    enum { ENTRY_HUNK_SIZE = 32 };		// FIXME: STDC::size_t

    /** The #Score::m_index of the sentinel #Score.  The number of
	member functions in the family must be less than this.  */
    enum { LAST_SCORE = USHRT_MAX };		// FIXME: unsigned short

    /** The #Score::m_distance of the sentinel #Score.  No actual
	argument object can be this far in inheritance from the
	corresponding formal argument types of any member function.  */
    enum { INFINITE_DISTANCE = USHRT_MAX };	// FIXME: unsigned short

    /** Description of a member belonging to the multi-method.  */
    struct Member
    {
	/** The function.  */
	MemberFunction		m_function;
	/** Actual types accepted in virtual argument positions by
	    this function.  The array has as many elements as the
	    multi-method has virtual arguments.  */
	XTypeInfo::ClassDef	**m_formalTypes;
    };

    /** A hunk of #Score objects. */
    struct ScoreHunk
    {
	ScoreHunk		*m_next;	//< Next hunk
	Score			*m_scores;	//< The score vector
	Score			*m_free;	//< List of free scores
	STDC::size_t		m_capacity;	//< Number of scores allocated
    };

    /** A hunk of #Entry objects. */
    struct EntryHunk
    {
	EntryHunk		*m_next;	//< Next hunk
	Entry			*m_entries;	//< The entry vector
	Entry			*m_free;	//< List of free entries
	STDC::size_t		m_capacity;	//< Number of entries allocated
    };

    /** A record of information chained into extended type
	information.

	We use these records to chain (as a singly linked list) the
	per-type information for this method into the extended type
	information (#XTypeInfo).  For each argument position where
	the type is related to one of the formal arguments of the
	multi-method, we have one of these to hold pointers to the
	dispatch #Score list.  If the type is related in more than one
	argument position, the records are chained in increasing
	argument position.  If the type is related to more than one
	multi-method, the records are ordered by the method keys
	(#m_key).

	The method's #Entry records are allocated in #EntryHunk lists
	and all of them go away at the time the method is destructed.
	The records get cleaned up from the chain in the type at that
	time. */
    struct Entry
    {
	/** Pointer to the next #Entry that applies to this type.  The
	    next one is guaranteed to have #m_key larger than this one
	    has, or the key is equal and #m_formal is larger than one
	    in this one.  */
	Entry			*m_next;

	/** The key of the multi-method owning this record
	    (#Definition::m_key). */
	STDC::size_t		m_key;

	/** The virtual argument position to which #m_scores applies.  */
	STDC::size_t		m_formal;

	/** The method generation for which #m_scores is valid.  Value
	    that is zero or less than #Definition::m_generation means
	    that #m_scores is invalid and needs to be regenerated.  */
	STDC::size_t		m_generation;

	/** The dispatch vector of #Score records for the owner type,
	    method (#m_key) and argument position (#m_formal).  The
	    validity of this pointer is determined by #m_generation:
	    the pointer may be non-null but invalid.  The pointer can
	    be null for a valid generation if no functions apply for
	    these conditions.  If the pointer is valid and non-null,
	    it ranks at least one applicable member function.  The
	    array is sorted by inheritance depth (#Score::m_distance).
	    The last #Score in the vector is a sentinel with method
	    index #LAST_SCORE and #INFINITE_DISTANCE inheritance
	    distance.  */
	Score			*m_scores;
    };

    // global data
    static XTypeInfo::ExtensionKey
    				extensionKey (void);
    static STDC::size_t		methodKey (void);

    // memory management
    ScoreHunk *			createScoreHunk (STDC::size_t min) const;
    void			freeScoreHunks (void) const;
    Score *			allocateScores (STDC::size_t n) const;

    EntryHunk *			createEntryHunk (void) const;
    void			freeEntryHunks (void) const;
    Entry *			allocateEntry (void) const;
    void			freeEntry (Entry *item) const;

    // dynamic type management
    STDC::size_t		relatedFormal (const XTypeInfo *type) const;
    static Entry *		findTypeEntries (const XTypeInfo *type,
						 Entry *&previous,
						 STDC::size_t key,
						 STDC::size_t formal
						 = STDC::size_t(-1));

    void			typePrepare (STDC::size_t formal,
					     XTypeInfo *type) const;
    bool			typeClean (STDC::size_t formal,
					   XTypeInfo *type) const;
    virtual void		typeHook (XTypeInfo *type);
    virtual void		typeUnhook (XTypeInfo *type);

    static bool			orderScores (const Score &x, const Score &y);
    void			cleanScores (const XTypeInfo *type) const;
    void			buildScores (const XTypeInfo *type) const;
    void			newGeneration (void) const;
    void			regenerate (const XTypeInfo *type) const;

    // dispatching
    void			noViableAlt (XTypeInfo **actuals) const;
    void			ambiguity (XTypeInfo **actuals,
					   Score **candidates,
					   Score *best) const;

    // actual data members -- forwarded
    Definition			*m_data;

    // undefined, no semantics
    MultiMethod (const MultiMethod &);
    MultiMethod &operator= (const MultiMethod &);
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Classes for the real multi-methods the user will end up declaring;
// see MULTIMETHOD_DECLARE on how these end up getting used.  These
// classes know the actual arguments used for that specific method,
// and take care of initialising the method.  They also record just
// enough information for the black magic below to recover enough
// details when members for this family are registered.
template <class R, class V1>
class MultiMethod_1_0 : public MultiMethod
{
public:
    static const int	NVIRT = 1;
    static const int	NNONVIRT = 0;
    typedef R		RTYPE;
    typedef V1		VTYPE1;

    MultiMethod_1_0 (Definition *def);
    R operator() (V1 *v1);

private:
    XTypeInfo::ClassDef *m_formals [1];
};

template <class R, class V1, class T1>
class MultiMethod_1_1 : public MultiMethod
{
public:
    static const int	NVIRT = 1;
    static const int	NNONVIRT = 1;
    typedef R		RTYPE;
    typedef V1		VTYPE1;
    typedef T1		NONVTYPE1;

    MultiMethod_1_1 (Definition *def);
    R operator() (V1 *v1, T1 t1);

private:
    XTypeInfo::ClassDef *m_formals [1];
};

template <class R, class V1, class T1, class T2>
class MultiMethod_1_2 : public MultiMethod
{
public:
    static const int	NVIRT = 1;
    static const int	NNONVIRT = 2;
    typedef R		RTYPE;
    typedef V1		VTYPE1;
    typedef T1		NONVTYPE1;
    typedef T2		NONVTYPE2;

    MultiMethod_1_2 (Definition *def);
    R operator() (V1 *v1, T1 t1, T2 t2);

private:
    XTypeInfo::ClassDef *m_formals [1];
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
template <class R, class V1, class V2>
class MultiMethod_2_0 : public MultiMethod
{
public:
    static const int	NVIRT = 2;
    static const int	NNONVIRT = 0;
    typedef R		RTYPE;
    typedef V1		VTYPE1;
    typedef V2		VTYPE2;

    MultiMethod_2_0 (Definition *def);
    R operator() (V1 *v1, V2 *v2);

private:
    XTypeInfo::ClassDef *m_formals [2];
};

template <class R, class V1, class V2, class T1>
class MultiMethod_2_1 : public MultiMethod
{
public:
    static const int	NVIRT = 2;
    static const int	NNONVIRT = 1;
    typedef R		RTYPE;
    typedef V1		VTYPE1;
    typedef V2		VTYPE2;
    typedef T1		NONVTYPE1;

    MultiMethod_2_1 (Definition *def);
    R operator() (V1 *v1, V2 *v2, T1 t1);

private:
    XTypeInfo::ClassDef *m_formals [2];
};

template <class R, class V1, class V2, class T1, class T2>
class MultiMethod_2_2 : public MultiMethod
{
public:
    static const int	NVIRT = 2;
    static const int	NNONVIRT = 2;
    typedef R		RTYPE;
    typedef V1		VTYPE1;
    typedef V2		VTYPE2;
    typedef T1		NONVTYPE1;
    typedef T2		NONVTYPE2;

    MultiMethod_2_2 (Definition *def);
    R operator() (V1 *v1, V2 *v2, T1 t1, T2 t2);

private:
    XTypeInfo::ClassDef *m_formals [2];
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
template <class R, class V1, class V2, class V3>
class MultiMethod_3_0 : public MultiMethod
{
public:
    static const int	NVIRT = 3;
    static const int	NNONVIRT = 0;
    typedef R		RTYPE;
    typedef V1		VTYPE1;
    typedef V2		VTYPE2;
    typedef V3		VTYPE3;

    MultiMethod_3_0 (Definition *def);
    R operator() (V1 *v1, V2 *v2, V3 *v3);

private:
    XTypeInfo::ClassDef *m_formals [3];
};

template <class R, class V1, class V2, class V3, class T1>
class MultiMethod_3_1 : public MultiMethod
{
public:
    static const int	NVIRT = 3;
    static const int	NNONVIRT = 1;
    typedef R		RTYPE;
    typedef V1		VTYPE1;
    typedef V2		VTYPE2;
    typedef V3		VTYPE3;
    typedef T1		NONVTYPE1;

    MultiMethod_3_1 (Definition *def);
    R operator() (V1 *v1, V2 *v2, V3 *v3, T1 t1);

private:
    XTypeInfo::ClassDef *m_formals [3];
};

template <class R, class V1, class V2, class V3, class T1, class T2>
class MultiMethod_3_2 : public MultiMethod
{
public:
    static const int	NVIRT = 3;
    static const int	NNONVIRT = 2;
    typedef R		RTYPE;
    typedef V1		VTYPE1;
    typedef V2		VTYPE2;
    typedef V3		VTYPE3;
    typedef T1		NONVTYPE1;
    typedef T2		NONVTYPE2;

    MultiMethod_3_2 (Definition *def);
    R operator() (V1 *v1, V2 *v2, V3 *v3, T1 t1, T2 t2);

private:
    XTypeInfo::ClassDef *m_formals [3];
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/** Magic class that carries information in its template arguments
    through #MultiMethodStub into the MultiMethodMemberStub_x_y
    classes.  Used in #MultiMethodMember.

    The first two template arguments specify the number of virtual
    and nonvirtual arguments in the multi-method (M).  These are
    important -- they allow #MultiMethodStub to lock in on the
    right candidate when it could get confused by the argument
    list of the method (whose prototype is P).

    Note that we are not able to carry through the actual address
    of the method member.  See #MultiMethodMember for details.  */
template <int NV, int NNV, class M, class R, class P> class MultiMethodCarrier {};

/** Class that manages the registration of a multi-method member.

    We use ugly black magic to deduce what the arguments for the
    multi-method really were and how they should be converted to the
    real arguments this family member takes.  We use overloading of
    template functions (= overloading and template unification) to
    select a stub that matches the arguments from the function the
    user supplied (F, prototyped as P), and the definitions in M, the
    multi-method type.

    Unfortunately we cannot carry the address of the user function F
    through this mechanism -- not all compilers (at least gcc 2.95.2)
    do not seem to be able to deduce the template arguments for the
    `MultiMethodStub' call if F is present.  So we initialise a class
    static pointer in the stub class so that when the stub function is
    called, it can jump to F.  This of course assumes the template
    arguments create a unique class, which should be the case -- and
    if it isn't, the call will die from `ambiguity' first anyway.

    We do all initialisation in the stub class.  Some compilers don't
    like to take to address of the "stub" function in the expression,
    but of course let us take the address within the stub itself.

    (Note: It might actually be possible to carry F through by having
    a yet another set of stub classes -- but then we would have to use
    to levels of function calls and rely on compiler optimising one of
    them away (when the stub is called after dispatch).  It is not
    obvious that that would be any better than the present hack.)

    An instance of this class is created for every family member
    defined with #MMM_DEFUN_FUNC or #MMM_DEFUN_METHOD.  */
template <class M, class P, P F, MultiMethod::Definition *D>
struct MultiMethodMember {
    typedef MultiMethodCarrier<M::NVIRT, M::NNONVIRT, M, typename M::RTYPE, P> Carrier;
    MultiMethodMember (void)  { MultiMethodStub ((Carrier *)0)->init(D,F); }
    ~MultiMethodMember (void) { MultiMethodStub ((Carrier *)0)->reset(D); }
};

//////////////////////////////////////////////////////////////////////
// classes for internal template magic ugliness; see MultiMethodStub
// and MultiMethodMember for the details on how these are used.
template <class M, class V1>
struct MultiMethodMemberStub_1_0 {
    typedef typename M::RTYPE (*F) (V1 *);
    static F s_f;
    static void init (MultiMethod::Definition *d, F f)
	{ static XTypeInfo::ClassDef *actuals [M::NVIRT] = { xtypedata(V1) };
	  s_f = f; d->extend ((MultiMethod::MemberFunction) &stub, actuals); }
    static void reset (MultiMethod::Definition *d)
	{ d->remove ((MultiMethod::MemberFunction) &stub); }
    static typename M::RTYPE stub (typename M::VTYPE1 *v1)
	{ return s_f (dynamic_cast<V1 *> (v1)); }
};

template <class M, class V1>
struct MultiMethodMemberStub_1_1 {
    typedef typename M::RTYPE (*F) (V1 *, typename M::NONVTYPE1);
    static F s_f;
    static void init (MultiMethod::Definition *d, F f)
	{ static XTypeInfo::ClassDef *actuals [M::NVIRT] = { xtypedata(V1) };
	  s_f = f; d->extend ((MultiMethod::MemberFunction) &stub, actuals); }
    static void reset (MultiMethod::Definition *d)
	{ d->remove ((MultiMethod::MemberFunction) &stub); }
    static typename M::RTYPE stub (typename M::VTYPE1 *v1,
				   typename M::NONVTYPE1 a1)
	{ return s_f (dynamic_cast<V1 *> (v1), a1); }
};

template <class M, class V1>
struct MultiMethodMemberStub_1_2 {
    typedef typename M::RTYPE (*F) (V1 *, typename M::NONVTYPE1,
				    typename M::NONVTYPE2);
    static F s_f;
    static void init (MultiMethod::Definition *d, F f)
	{ static XTypeInfo::ClassDef *actuals [M::NVIRT] = { xtypedata(V1) };
	  s_f = f; d->extend ((MultiMethod::MemberFunction) &stub, actuals); }
    static void reset (MultiMethod::Definition *d)
	{ d->remove ((MultiMethod::MemberFunction) &stub); }
    static typename M::RTYPE stub (typename M::VTYPE1 *v1,
				   typename M::NONVTYPE1 a1,
				   typename M::NONVTYPE2 a2)
	{ return s_f (dynamic_cast<V1 *> (v1), a1, a2); }
};

//////////////////////////////////////////////////////////////////////
template <class M, class V1, class V2>
struct MultiMethodMemberStub_2_0 {
    typedef typename M::RTYPE (*F) (V1 *, V2 *);
    static F s_f;
    static void init (MultiMethod::Definition *d, F f)
	{ static XTypeInfo::ClassDef *actuals [M::NVIRT]
	      = { xtypedata(V1), xtypedata(V2) };
	  s_f = f; d->extend ((MultiMethod::MemberFunction) &stub, actuals); }
    static void reset (MultiMethod::Definition *d)
	{ d->remove ((MultiMethod::MemberFunction) &stub); }
    static typename M::RTYPE stub (typename M::VTYPE1 *v1,
				   typename M::VTYPE2 *v2)
	{ return s_f (dynamic_cast<V1 *> (v1), dynamic_cast<V2 *> (v2)); }
};

template <class M, class V1, class V2>
struct MultiMethodMemberStub_2_1 {
    typedef typename M::RTYPE (*F) (V1 *, V2 *, typename M::NONVTYPE1);
    static F s_f;
    static void init (MultiMethod::Definition *d, F f)
	{ static XTypeInfo::ClassDef *actuals [M::NVIRT]
	      = { xtypedata(V1), xtypedata(V2) };
	  s_f = f; d->extend ((MultiMethod::MemberFunction) &stub, actuals); }
    static void reset (MultiMethod::Definition *d)
	{ d->remove ((MultiMethod::MemberFunction) &stub); }
    static typename M::RTYPE stub (typename M::VTYPE1 *v1,
				   typename M::VTYPE2 *v2,
				   typename M::NONVTYPE1 a1)
	{ return s_f (dynamic_cast<V1 *> (v1), dynamic_cast<V2 *> (v2), a1); }
};

template <class M, class V1, class V2>
struct MultiMethodMemberStub_2_2 {
    typedef typename M::RTYPE (*F) (V1 *, V2 *, typename M::NONVTYPE1,
				    typename M::NONVTYPE2);
    static F s_f;
    static void init (MultiMethod::Definition *d, F f)
	{ static XTypeInfo::ClassDef *actuals [M::NVIRT]
	      = { xtypedata(V1), xtypedata(V2) };
	  s_f = f; d->extend ((MultiMethod::MemberFunction) &stub, actuals); }
    static void reset (MultiMethod::Definition *d)
	{ d->remove ((MultiMethod::MemberFunction) &stub); }
    static typename M::RTYPE stub (typename M::VTYPE1 *v1,
				   typename M::VTYPE2 *v2,
				   typename M::NONVTYPE1 a1,
				   typename M::NONVTYPE2 a2)
	{ return s_f (dynamic_cast<V1 *> (v1), dynamic_cast<V2 *> (v2),
		      a1, a2); }
};

//////////////////////////////////////////////////////////////////////
template <class M, class V1, class V2, class V3>
struct MultiMethodMemberStub_3_0 {
    typedef typename M::RTYPE (*F) (V1 *, V2 *, V3 *);
    static F s_f;
    static void init (MultiMethod::Definition *d, F f)
	{ static XTypeInfo::ClassDef *actuals [M::NVIRT]
	      = { xtypedata(V1), xtypedata(V2), xtypedata(V3) };
	  s_f = f; d->extend ((MultiMethod::MemberFunction) &stub, actuals); }
    static void reset (MultiMethod::Definition *d)
	{ d->remove ((MultiMethod::MemberFunction) &stub); }
    static typename M::RTYPE stub (typename M::VTYPE1 *v1,
				   typename M::VTYPE2 *v2,
				   typename M::VTYPE3 *v3)
	{ return s_f (dynamic_cast<V1 *> (v1), dynamic_cast<V2 *> (v2),
		      dynamic_cast<V3 *> (v3)); }
};

template <class M, class V1, class V2, class V3>
struct MultiMethodMemberStub_3_1 {
    typedef typename M::RTYPE (*F) (V1 *, V2 *, V3 *, typename M::NONVTYPE1);
    static F s_f;
    static void init (MultiMethod::Definition *d, F f)
	{ static XTypeInfo::ClassDef *actuals [M::NVIRT]
	      = { xtypedata(V1), xtypedata(V2), xtypedata(V3) };
	  s_f = f; d->extend ((MultiMethod::MemberFunction) &stub, actuals); }
    static void reset (MultiMethod::Definition *d)
	{ d->remove ((MultiMethod::MemberFunction) &stub); }
    static typename M::RTYPE stub (typename M::VTYPE1 *v1,
				   typename M::VTYPE2 *v2,
				   typename M::VTYPE3 *v3,
				   typename M::NONVTYPE1 a1)
	{ return s_f (dynamic_cast<V1 *> (v1), dynamic_cast<V2 *> (v2),
		      dynamic_cast<V3 *> (v3), a1); }
};

template <class M, class V1, class V2, class V3>
struct MultiMethodMemberStub_3_2 {
    typedef typename M::RTYPE (*F) (V1 *, V2 *, V3 *, typename M::NONVTYPE1,
				    typename M::NONVTYPE2);
    static F s_f;
    static void init (MultiMethod::Definition *d, F f)
	{ static XTypeInfo::ClassDef *actuals [M::NVIRT]
	      = { xtypedata(V1), xtypedata(V2), xtypedata(V3) };
	  s_f = f; d->extend ((MultiMethod::MemberFunction) &stub, actuals); }
    static void reset (MultiMethod::Definition *d)
	{ d->remove ((MultiMethod::MemberFunction) &stub); }
    static typename M::RTYPE stub (typename M::VTYPE1 *v1,
				   typename M::VTYPE2 *v2,
				   typename M::VTYPE3 *v3,
				   typename M::NONVTYPE1 a1,
				   typename M::NONVTYPE2 a2)
	{ return s_f (dynamic_cast<V1 *> (v1), dynamic_cast<V2 *> (v2),
		      dynamic_cast<V3 *> (v3), a1, a2); }
};

//////////////////////////////////////////////////////////////////////
template <class M, class V1>
typename MultiMethodMemberStub_1_0<M,V1>::F
MultiMethodMemberStub_1_0<M,V1>::s_f = 0;
template <class M, class V1>
typename MultiMethodMemberStub_1_1<M,V1>::F
MultiMethodMemberStub_1_1<M,V1>::s_f = 0;
template <class M, class V1>
typename MultiMethodMemberStub_1_2<M,V1>::F
MultiMethodMemberStub_1_2<M,V1>::s_f = 0;

template <class M, class V1, class V2>
typename MultiMethodMemberStub_2_0<M,V1,V2>::F
MultiMethodMemberStub_2_0<M,V1,V2>::s_f = 0;
template <class M, class V1, class V2>
typename MultiMethodMemberStub_2_1<M,V1,V2>::F
MultiMethodMemberStub_2_1<M,V1,V2>::s_f = 0;
template <class M, class V1, class V2>
typename MultiMethodMemberStub_2_2<M,V1,V2>::F
MultiMethodMemberStub_2_2<M,V1,V2>::s_f = 0;

template <class M, class V1, class V2, class V3>
typename MultiMethodMemberStub_3_0<M,V1,V2,V3>::F
MultiMethodMemberStub_3_0<M,V1,V2,V3>::s_f = 0;
template <class M, class V1, class V2, class V3>
typename MultiMethodMemberStub_3_1<M,V1,V2,V3>::F
MultiMethodMemberStub_3_1<M,V1,V2,V3>::s_f = 0;
template <class M, class V1, class V2, class V3>
typename MultiMethodMemberStub_3_2<M,V1,V2,V3>::F
MultiMethodMemberStub_3_2<M,V1,V2,V3>::s_f = 0;

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

template <class R, class V1>
inline
MultiMethod_1_0<R,V1>::MultiMethod_1_0 (Definition *def)
{
    m_formals [0] = xtypedata (V1);
    initialise (def, 1, m_formals);
}

template <class R, class V1>
inline R
MultiMethod_1_0<R,V1>::operator() (V1 *v1)
{
    XTypeInfo		*actuals [1] = { xtypeid (*v1) };
    Score		*candidates [1];
    Score		best [1];
    MemberFunction	match;

    match = dispatch (actuals, candidates, best);
    return (*reinterpret_cast<R (*) (V1 *)> (match))
	(v1);
}

/////////////////////////////////////////////////////////////////////////////

template <class R, class V1, class T1>
inline
MultiMethod_1_1<R,V1,T1>::MultiMethod_1_1 (Definition *def)
{
    m_formals [0] = xtypedata (V1);
    initialise (def, 1, m_formals);
}

template <class R, class V1, class T1>
inline R
MultiMethod_1_1<R,V1,T1>::operator() (V1 *v1, T1 t1)
{
    XTypeInfo		*actuals [1] = { xtypeid (*v1) };
    Score		*candidates [1];
    Score		best [1];
    MemberFunction	match;

    match = dispatch (actuals, candidates, best);
    return (*reinterpret_cast<R (*) (V1 *, T1)> (match))
	(v1, t1);
}

/////////////////////////////////////////////////////////////////////////////
template <class R, class V1, class T1, class T2>
inline
MultiMethod_1_2<R,V1,T1,T2>::MultiMethod_1_2 (Definition *def)
{
    m_formals [0] = xtypedata (V1);
    initialise (def, 1, m_formals);
}

template <class R, class V1, class T1, class T2>
inline R
MultiMethod_1_2<R,V1,T1,T2>::operator() (V1 *v1, T1 t1, T2 t2)
{
    XTypeInfo		*actuals [1] = { xtypeid (*v1) };
    Score		*candidates [1];
    Score		best [1];
    MemberFunction	match;

    match = dispatch (actuals, candidates, best);
    return (*reinterpret_cast<R (*) (V1 *, T1, T2)> (match))
	(v1, t1, t2);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
template <class R, class V1, class V2>
inline
MultiMethod_2_0<R,V1,V2>::MultiMethod_2_0 (Definition *def)
{
    m_formals [0] = xtypedata (V1);
    m_formals [1] = xtypedata (V2);
    initialise (def, 2, m_formals);
}

template <class R, class V1, class V2>
inline R
MultiMethod_2_0<R,V1,V2>::operator() (V1 *v1, V2 *v2)
{
    XTypeInfo		*actuals [2] = { xtypeid (*v1), xtypeid (*v2) };
    Score		*candidates [2];
    Score		best [2];
    MemberFunction	match;

    match = dispatch (actuals, candidates, best);
    return (*reinterpret_cast<R (*) (V1 *, V2 *)> (match))
	(v1, v2);
}

/////////////////////////////////////////////////////////////////////////////
template <class R, class V1, class V2, class T1>
inline
MultiMethod_2_1<R,V1,V2,T1>::MultiMethod_2_1 (Definition *def)
{
    m_formals [0] = xtypedata (V1);
    m_formals [1] = xtypedata (V2);
    initialise (def, 2, m_formals);
}

template <class R, class V1, class V2, class T1>
inline R
MultiMethod_2_1<R,V1,V2,T1>::operator() (V1 *v1, V2 *v2, T1 t1)
{
    XTypeInfo		*actuals [2] = { xtypeid (*v1), xtypeid (*v2) };
    Score		*candidates [2];
    Score		best [2];
    MemberFunction	match;

    match = dispatch (actuals, candidates, best);
    return (*reinterpret_cast<R (*) (V1 *, V2 *, T1)> (match))
	(v1, v2, t1);
}

/////////////////////////////////////////////////////////////////////////////
template <class R, class V1, class V2, class T1, class T2>
inline
MultiMethod_2_2<R,V1,V2,T1,T2>::MultiMethod_2_2 (Definition *def)
{
    m_formals [0] = xtypedata (V1);
    m_formals [1] = xtypedata (V2);
    initialise (def, 2, m_formals);
}

template <class R, class V1, class V2, class T1, class T2>
inline R
MultiMethod_2_2<R,V1,V2,T1,T2>::operator() (V1 *v1, V2 *v2, T1 t1, T2 t2)
{
    XTypeInfo		*actuals [2] = { xtypeid (*v1), xtypeid (*v2) };
    Score		*candidates [2];
    Score		best [2];
    MemberFunction	match;

    match = dispatch (actuals, candidates, best);
    return (*reinterpret_cast<R (*) (V1 *, V2 *, T1, T2)> (match))
	(v1, v2, t1, t2);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
template <class R, class V1, class V2, class V3>
inline
MultiMethod_3_0<R,V1,V2,V3>::MultiMethod_3_0 (Definition *def)
{
    m_formals [0] = xtypedata (V1);
    m_formals [1] = xtypedata (V2);
    m_formals [3] = xtypedata (V3);
    initialise (def, 3, m_formals);
}

template <class R, class V1, class V2, class V3>
inline R
MultiMethod_3_0<R,V1,V2,V3>::operator() (V1 *v1, V2 *v2, V3 *v3)
{
    XTypeInfo		*actuals [3] = { xtypeid (*v1), xtypeid (*v2), xtypeid (*v3) };
    Score		*candidates [3];
    Score		best [3];
    MemberFunction	match;

    match = dispatch (actuals, candidates, best);
    return (*reinterpret_cast<R (*) (V1 *, V2 *, V3 *)> (match))
	(v1, v2, v3);
}

/////////////////////////////////////////////////////////////////////////////
template <class R, class V1, class V2, class V3, class T1>
inline
MultiMethod_3_1<R,V1,V2,V3,T1>::MultiMethod_3_1 (Definition *def)
{
    m_formals [0] = xtypedata (V1);
    m_formals [1] = xtypedata (V2);
    m_formals [3] = xtypedata (V3);
    initialise (def, 3, m_formals);
}

template <class R, class V1, class V2, class V3, class T1>
inline R
MultiMethod_3_1<R,V1,V2,V3,T1>::operator() (V1 *v1, V2 *v2, V3 *v3, T1 t1)
{
    XTypeInfo		*actuals [3] = { xtypeid (*v1), xtypeid (*v2), xtypeid (*v3) };
    Score		*candidates [3];
    Score		best [3];
    MemberFunction	match;

    match = dispatch (actuals, candidates, best);
    return (*reinterpret_cast<R (*) (V1 *, V2 *, V3 *, T1)> (match))
	(v1, v2, v3, t1);
}

/////////////////////////////////////////////////////////////////////////////
template <class R, class V1, class V2, class V3, class T1, class T2>
inline
MultiMethod_3_2<R,V1,V2,V3,T1,T2>::MultiMethod_3_2 (Definition *def)
{
    m_formals [0] = xtypedata (V1);
    m_formals [1] = xtypedata (V2);
    m_formals [3] = xtypedata (V3);
    initialise (def, 3, m_formals);
}

template <class R, class V1, class V2, class V3, class T1, class T2>
inline R
MultiMethod_3_2<R,V1,V2,V3,T1,T2>::operator() (V1 *v1, V2 *v2, V3 *v3, T1 t1, T2 t2)
{
    XTypeInfo		*actuals [3] = { xtypeid (*v1), xtypeid (*v2), xtypeid (*v3) };
    Score		*candidates [3];
    Score		best [3];
    MemberFunction	match;

    match = dispatch (actuals, candidates, best);
    return (*reinterpret_cast<R (*) (V1 *, V2 *, V3 *, T1, T2)> (match))
	(v1, v2, v3, t1, t2);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Ugly magic template function to select the right stub type based on
// the type of the member function.  The type of the member function
// is the last argument of MultiMethodCarrier; we select the right
// stub class here based on that function prototype (and pass it all
// the relevant information).
//
// Note that the returned null pointer is never used -- we are only
// interested in the return type.  We cannot actually name the return
// type in MultiMethodMember (we can only use an expression, not a
// declaration), so we use a trick: everything interesting in the
// returned stub class type is actually class-static.  So even though
// we nominally dereference the returned pointer, the compiler really
// never does it and the code is safe!  (I told you this was ugly...
// Now you wish you never would have heard.)
//
// Note also that MSVC++ 7 chokes on `typename M::something' when used
// in the prototype argument list, so we have to use ugly magic to get
// M::RTYPE and M::NONVTYPEn.  For return type we dig the actual type
// out in the caller and it gets passed in with MultiMethodCarrier.
// For NONVTYPEn we just accept anything and make sure in the
// function body that we got the right match.
template <class M, class R, class V1>
inline MultiMethodMemberStub_1_0<M,V1> *
MultiMethodStub (MultiMethodCarrier<1,0,M,R, R (*) (V1*)> *)
{ return 0; }

template <class M, class R, class V1, class A1>
inline MultiMethodMemberStub_1_1<M,V1> *
MultiMethodStub (MultiMethodCarrier<1,1,M,R, R (*) (V1 *, A1)> *)
{ (void) static_cast<A1 **> ((typename M::NONVTYPE1 **) 0); return 0; }

template <class M, class R, class V1, class A1, class A2>
inline MultiMethodMemberStub_1_2<M,V1> *
MultiMethodStub (MultiMethodCarrier<1,2,M,R, R (*) (V1 *, A1, A2)> *)
{ (void) static_cast<A1 **> ((typename M::NONVTYPE1 **) 0);
  (void) static_cast<A2 **> ((typename M::NONVTYPE2 **) 0); return 0; }

//////////////////////////////////////////////////////////////////////
template <class M, class R, class V1, class V2>
inline MultiMethodMemberStub_2_0<M,V1,V2> *
MultiMethodStub (MultiMethodCarrier<2,0,M,R, R (*) (V1 *, V2 *)> *)
{ return 0; }

template <class M, class R, class V1, class V2, class A1>
inline MultiMethodMemberStub_2_1<M,V1,V2> *
MultiMethodStub (MultiMethodCarrier<2,1,M,R, R (*) (V1 *, V2 *, A1)> *)
{ (void) static_cast<A1 **> ((typename M::NONVTYPE1 **) 0); return 0; }

template <class M, class R, class V1, class V2, class A1, class A2>
inline MultiMethodMemberStub_2_2<M,V1,V2> *
MultiMethodStub (MultiMethodCarrier<2,2,M,R, R (*) (V1 *, V2 *, A1, A2)> *)
{ (void) static_cast<A1 **> ((typename M::NONVTYPE1 **) 0);
  (void) static_cast<A2 **> ((typename M::NONVTYPE2 **) 0); return 0; }

//////////////////////////////////////////////////////////////////////
template <class M, class R, class V1, class V2, class V3>
inline MultiMethodMemberStub_3_0<M,V1,V2,V3> *
MultiMethodStub (MultiMethodCarrier<3,0,M,R, R (*) (V1 *, V2 *, V3 *)> *)
{ return 0; }

template <class M, class R, class V1, class V2, class V3, class A1>
inline MultiMethodMemberStub_3_1<M,V1,V2,V3> *
MultiMethodStub (MultiMethodCarrier<3,1,M,R, R (*) (V1 *, V2 *, V3 *, A1)> *)
{ (void) static_cast<A1 **> ((typename M::NONVTYPE1 **) 0); return 0; }

template <class M, class R, class V1, class V2, class V3, class A1, class A2>
inline MultiMethodMemberStub_3_2<M,V1,V2,V3> *
MultiMethodStub (MultiMethodCarrier<3,2,M,R, R (*) (V1 *, V2 *, V3 *, A1, A2)> *)
{ (void) static_cast<A1 **> ((typename M::NONVTYPE1 **) 0);
  (void) static_cast<A2 **> ((typename M::NONVTYPE2 **) 0); return 0; }

} // namespace lat
#endif // CLASSLIB_MULTIMETHOD_H
