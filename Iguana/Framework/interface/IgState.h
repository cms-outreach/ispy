#ifndef IGUANA_FRAMEWORK_IG_STATE_H
# define IGUANA_FRAMEWORK_IG_STATE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/config.h"
# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgStateElement;
class IgStateKey;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** A shared application object state.

    #IgState manages a collection of #IgStateElement objects as a
    state that can shared among various clients.  The general idea is
    that #IgState does not know about specific items that need to be
    shared among the clients, but the clients know how to look up
    specific extensions in the state.  #IgState is merely a means up
    keeping the state data together in a way that clients do not need
    to care about its origin.

    State objects can be chained in a tree-like structure.  This
    allows an #IgStateElement setting in the child to override the
    definition in the parent, or to be scoped purely into the child.
    State elements that cannot be found in the child are looked up
    in the parent until a definition is found or the state tree root
    is reached.  Elements can be introduced anywhere on the path,
    which can be used to give them appropriate scope.

    Destroying a state destroys all the elements in it.  */
class IGUANA_FRAMEWORK_API IgState
{
public:
    IgState (IgState *parent = 0);
    ~IgState (void);

    IgStateElement *	get (IgStateKey index, IgState **context = 0) const;
    void		put (IgStateKey index, IgStateElement *element);
    void		erase (IgStateKey index);
    void		detach (IgStateKey index);

    IgState *		root (void) const;
    IgState *		parent (void) const;
    unsigned		children (void) const;
    IgState *		child (unsigned index) const;
    unsigned		elements (void) const;
    IgStateElement *	element (unsigned index) const;

private:
    typedef std::vector<IgState *>	  Children;
    typedef std::vector<IgStateElement *> Elements;

    void		addChild (IgState *child);
    void		detachChild (IgState *child);

    IgState		*m_parent;	//< Parent state, null if this is root
    Children		m_children;	//< Sub-states.
    Elements		m_elements;	//< Presently recorded elements.

    // undefined, no semantics
    IgState (const IgState &x);
    IgState &operator= (const IgState &x);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_FRAMEWORK_IG_SESSION_H
