#ifndef IG_XML_MODEL_IG_COMMON_XML_REPS_H
# define IG_XML_MODEL_IG_COMMON_XML_REPS_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgXMLModel/interface/config.h"
# include <classlib/utils/MultiMethod.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgRepContext;
class IgTwig;
class IgXMLRep;
class IgXMLModel;
class IgUIDRep;
class IgUIDModel;
class IgAjaxTreeModel;


//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>

IG_XML_MODEL_API MMM_DECLARE(void, doUpdate,
			     (IgTwig *twig, IgXMLRep *rep, unsigned mask));

IG_XML_MODEL_API MMM_DECLARE(IgRepContext *, doRepresent,
			     (IgTwig *twig, IgXMLModel *model));

IG_XML_MODEL_API MMM_DECLARE(void, doUpdate,
			     (IgTwig *twig, IgUIDRep *rep, unsigned mask));

IG_XML_MODEL_API MMM_DECLARE(IgRepContext *, doRepresent,
			     (IgTwig *twig, IgUIDModel *model));

IG_XML_MODEL_API MMM_DECLARE(IgRepContext *, doRepresent,
			     (IgTwig *twig, IgAjaxTreeModel *model));


//<<<<<< CLASS DECLARATIONS                                             >>>>>>
//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_XML_MODEL_IG_COMMON_XML_REPS_H
