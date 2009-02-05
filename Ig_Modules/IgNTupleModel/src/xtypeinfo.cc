//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgNTupleModel/interface/IgNTupleModel.h"
#include "Ig_Modules/IgNTupleModel/interface/IgNTupleRep.h"
#include "Ig_Modules/IgNTupleModel/interface/xtypeinfo.h"
#include "Ig_Framework/IgObjectBrowser/interface/xtypeinfo.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgBrowserMethods.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepContext.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepSet.h"
#include "Ig_Modules/IgCommonData/interface/IgTwig.h"
#include "Ig_Modules/IgCommonData/interface/xtypeinfo.h"
#undef emit // zap qt macro
#include <classlib/utils/DebugAids.h>
#include <classlib/utils/Log.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

XTYPEINFO_DEF_1 (IgNTupleModel, IgModel, false);
XTYPEINFO_DEF_1 (IgNTupleRep,   IgRep, false);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>

lat::logflag LFcommonNTuple = { 0, "commonNTuple", true, -1 };

//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

MMM_DEFUN_FUNC(IgRepContext *, IgBrowserMethods::,doRepresent,
	       (IgTwig *twig, IgNTupleModel *model))
{
    LOG (0, trace, LFcommonNTuple, "doRepresent(IgTwig[" << twig->name ()
	 << "],IgNTupleModel)\n" << lat::indent);

    // Create the rep and associate it to a context.
    IgRepContext *context
	= new IgRepContext (twig, new IgNTupleRep (model));
    
    // Redirect actual fill to doUpdate().
    IgRepSet::update (twig, model, IgTwig::SELF_MASK);
    
    LOG (0, trace, LFcommonNTuple, lat::undent);
    return context;
}

MMM_DEFUN_FUNC(void,IgBrowserMethods::,doUpdate,
               (IgTwig *twig, IgNTupleRep */*rep*/, unsigned mask))
{
    // The default implementation does nothing
    
    LOG (0, trace, LFcommonNTuple, "doUpdate(IgTwig[" << twig->name ()
	 << "],IgNTupleRep," << mask << ")\n" << lat::indent);
    LOG (0, trace, LFcommonNTuple, lat::undent);
}
