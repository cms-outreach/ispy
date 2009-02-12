#ifndef IGUANA_FRAMEWORK_IG_REP_SET_H
# define IGUANA_FRAMEWORK_IG_REP_SET_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/config.h"
# include <map>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgRepresentable;
class IgRepContext;
class IgRep;
class IgModel;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** The set of known representations of an object.

  The set consists of a singly linked circular list of #IgRepContext
  objects.  The contexts record the representations that have been
  created for the #IgRepresentable, connecting the object and the
  location of its representation in a browsing model.  The context
  list establishes the back bone of navigation across models and of
  communication across browsers.

  @note #IgRepSet is never created directly.  It comes to existence
        only when a #IgReprsentable is associated with a #IgRep.  */
class IGUANA_FRAMEWORK_API IgRepSet
{
public:
    ~IgRepSet (void);

    static IgRepSet *		associate (IgRepresentable *object,
					   bool create = false);
    static IgRep *		lookup (IgRepresentable *object,
					IgModel *model,
					bool create = false);
    static IgRep *		lookup (IgRepContext *context,
					IgModel *model,
					bool create = false);
    static void			update (IgRepresentable *object,
					unsigned field);
    static void			update (IgRepContext *context,
					unsigned field);
    static void			update (IgRepresentable *object,
					IgModel *model,
					unsigned field);
    static void			invalidate (IgRepresentable *object,
		    			    unsigned field);
    static void			invalidate (IgRepContext *context,
		    			    unsigned field);
    static void			invalidate (IgRepresentable *object,
		    			    IgModel *model,
		    			    unsigned field);

    IgRepresentable *		object (void) const;
    IgRepContext *		contexts (void) const;
    void			add (IgRepContext *context);
    void			remove (IgRepContext *context);
    IgRepContext *		lookup (IgModel *model) const;

protected:
    IgRepSet (IgRepresentable *object);

private:
    // FIXME: should be a hash table
    // FIXME: get this out of this class
    typedef std::map<IgRepresentable *, IgRepSet *> LookupTable;
    static LookupTable &	table (void);

    IgRepresentable		*m_object; /**< Pointer to the
                                                #IgRepresentable this
                                                set describes.  */
    IgRepContext		*m_first;  /**< Pointer to the first
					        context in the
					        circular singly-linked
					        list.  */

    // no semantics
    IgRepSet (const IgRepSet &);
    IgRepSet &operator= (const IgRepSet &);
};


//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

/** Get the object that this set belongs to.  */
inline IgRepresentable *
IgRepSet::object (void) const
{ return m_object; }

/** Get the circular list of rep contexts.  */
inline IgRepContext *
IgRepSet::contexts (void) const
{ return m_first; }

#endif // IGUANA_FRAMEWORK_IG_REP_SET_H
