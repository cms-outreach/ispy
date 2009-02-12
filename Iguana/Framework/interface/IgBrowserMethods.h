#ifndef IGUANA_FRAMEWORK_IG_BROWSER_METHODS_H
# define IGUANA_FRAMEWORK_IG_BROWSER_METHODS_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/config.h"
# include "classlib/utils/MultiMethod.h"
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgRepContext;
class IgRepresentable;
class IgModel;
class IgRep;

namespace lat { class XTypeInfo; }

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Global service for the collection of object browsing multi-methods.

  FIXME -- All uses should go through the static methods, not the
  multi-methods.  Automatically preloads all necessary shared
  libraries before actually invoking the multi-method.  */
class IGUANA_FRAMEWORK_API IgBrowserMethods
{
public:
    // Construct keys for preload capability load mechanism
    static std::string	key (const char *model, const char *object);

    static IgRepContext	*represent (IgRepresentable *, IgModel *);
    static void		commit (IgRep *, IgRepresentable *, unsigned);
    static void		update (IgRepresentable *, IgRep *, unsigned);
    static void		expand (IgRepresentable *, IgRep *);
    static void		invalidate (IgRepresentable *, IgModel *, unsigned);

    // IgRepContext *	doRepresent (virtual IgRepresentable *,
    //				     virtual IgModel *);
    static MMM_DECLARE	(IgRepContext *,doRepresent,
			 (IgRepresentable *, IgModel *));
    MULTIMETHOD_DECLARE	(static,IgRepContext *,doRepresent,2,0,
			 (IgRepresentable,IgModel));

    // void		doCommit (virtual IgRep *, virtual IgRepresentable *,
    //				  unsigned subFieldIndex);
    static MMM_DECLARE	(void,doCommit,(IgRep *, IgRepresentable *, unsigned));
    MULTIMETHOD_DECLARE	(static,void,doCommit,2,1,(IgRep,IgRepresentable,unsigned));

    // void		doUpdate (virtual IgRepresentable *, virtual IgRep *,
    //				  unsigned subFieldIndex);
    static MMM_DECLARE	(void,doUpdate,(IgRepresentable *, IgRep *, unsigned));
    MULTIMETHOD_DECLARE	(static,void,doUpdate,2,1,(IgRepresentable,IgRep,unsigned));

    // void		doExpand (virtual IgRepresentable *, virtual IgRep *);
    static MMM_DECLARE	(void,doExpand,(IgRepresentable *, IgRep *));
    MULTIMETHOD_DECLARE	(static,void,doExpand,2,0,(IgRepresentable,IgRep));

    // void		doInvalidate (virtual IgRepresentable *, IgModel *model,
    //				      unsigned subFieldIndex);
    static MMM_DECLARE	(void,doInvalidate,(IgRepresentable *, IgModel *, unsigned));
    MULTIMETHOD_DECLARE	(static,void,doInvalidate,1,2,(IgRepresentable,IgModel *,unsigned));

private:
    static void		preload (const lat::XTypeInfo *objtype,
				 const lat::XTypeInfo *modtype);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_FRAMEWORK_IG_BROWSER_METHODS_H
