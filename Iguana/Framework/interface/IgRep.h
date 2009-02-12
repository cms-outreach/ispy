#ifndef IGUANA_FRAMEWORK_IG_REP_H
# define IGUANA_FRAMEWORK_IG_REP_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/config.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgRepContext;
class IgModel;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Base class for all object representations.

  This is the base interface that all #IgRepresentable representations
  must conform to.  It is up to the browser and model to decide what
  needs to go in the derived class.  However, at the very minimum two
  things must be provided: access to the #IgRepContext and pointer to
  the #IgModel the rep belongs to.  The accessors for the former is
  #context(), for the latter #model().

  The derived reps are usually created in the extension of the
  IgBrowserMethods::doRepresent() multi-method.  Upon creation, the
  derived class must set its internal #IgRepContext pointer to null.
  Immediately after the creation a new #IgRepContext will be created
  for this new rep; this will cause the protected #context() method to
  be invoked.  At that point the derived class should set its
  #IgRepContext from the argument.

  The derived classes must be prepared to handle destruction in two
  ways: the #IgRepresentable is going away and the #IgRepContext is
  deleting the rep, or someone else is deleting rep (e.g. the model).
  In the former case the caller will have gotten rid of references to
  the application object before destructing the rep (in fact the
  application object most probably no longer exists); in the latter
  case the object probably still exists.  In the former case the rep
  must take care not to delete the context.  The way to detect this is
  that prior to destruction the #IgRepContext will have set its rep
  to null.  Otherwise the context will still point to this rep and the
  rep should first invoke IgRepContext::erase() with @c false as
  argument, and the @c delete the context.

  (FIXME: Rep garbage collection might delete the rep as well; this
  falls in the second case but the rep must realise that the model
  might not yet know.  But we don't have garbage collection yet...) */
class IGUANA_FRAMEWORK_API IgRep
{
public:
    virtual ~IgRep (void);

    virtual IgRepContext *	context (void) const = 0;
    virtual IgModel *		model (void) const = 0;

protected:
    friend class IgRepContext;
    virtual void		context (IgRepContext *context) = 0;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_FRAMEWORK_IG_REP_H
