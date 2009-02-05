//<<<<<< INCLUDES                                                       >>>>>>
#define QT_NO_EMIT
#include "Iguana/View/interface/IViewQueuedTwig.h"
#include "Iguana/View/interface/IViewDTRecSegment4DTwig.h"
#include "Iguana/View/interface/IViewEventTwig.h"
#include "Iguana/View/interface/IViewTrackTwig.h"
#include "Iguana/View/interface/IViewSiStripDigiTwig.h"
#include "Iguana/View/interface/Ig3DModel.h"
#include "Iguana/View/interface/IgRPhiModel.h"
#include "Iguana/View/interface/IgRZModel.h"
#include "Iguana/View/interface/IgLegoModel.h"
#include "Iguana/View/interface/IgSimpleTextModel.h"
#include "Iguana/View/interface/IgTextModel.h"
#include "Iguana/View/interface/Ig3DRep.h"
#include "Iguana/View/interface/IgRZRep.h"
#include "Iguana/View/interface/IgRPhiRep.h"
#include "Iguana/View/interface/IgLegoRep.h"
#include "Iguana/View/interface/IgTextRep.h"
#include "Iguana/View/interface/IgSimpleTextRep.h"
#include "Iguana/View/interface/debug.h"
#include "Iguana/Framework/interface/IgBrowserMethods.h"
#include "Iguana/Framework/interface/IgBrowserDB.h"
#include "Iguana/Framework/interface/IgRepContext.h"
#include "Iguana/Framework/interface/IgTwig.h"
#include "Iguana/Framework/interface/xtypeinfo.h"
#include "Iguana/View/interface/xtypeinfo.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/// #logstream flag for this package.
lat::logflag LFfwvis = { 0, "fwvis", true, -1 };

XTYPEINFO_DEF_1 (Ig3DModel,   IgModel, false);
XTYPEINFO_DEF_1 (IgRPhiModel, IgModel, false);
XTYPEINFO_DEF_1 (IgRZModel,   IgModel, false);
XTYPEINFO_DEF_1 (IgLegoModel, IgModel, false);
XTYPEINFO_DEF_1 (IgTextModel, IgModel, false);
XTYPEINFO_DEF_1 (IgSimpleTextModel, IgTextModel, false);
XTYPEINFO_DEF_1 (Ig3DRep,   IgRep, false);
XTYPEINFO_DEF_1 (IgRPhiRep, IgRep, false);
XTYPEINFO_DEF_1 (IgRZRep,   IgRep, false);
XTYPEINFO_DEF_1 (IgLegoRep, IgRep, false);
XTYPEINFO_DEF_1 (IgTextRep, IgRep, false);
XTYPEINFO_DEF_1 (IViewQueuedTwig, IgSimpleTwig, false);
XTYPEINFO_DEF_1 (IViewDTRecSegment4DTwig, IViewQueuedTwig, false);
XTYPEINFO_DEF_1 (IViewEventTwig, IViewQueuedTwig, false);
XTYPEINFO_DEF_1 (IViewTrackTwig, IViewQueuedTwig, false);
XTYPEINFO_DEF_1 (IViewSiStripDigiTwig, IViewQueuedTwig, false);

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
MMM_DEFUN_FUNC(void,IgBrowserMethods::,doInvalidate,
               (IgTwig *twig, IgModel *model, unsigned mask))
{
    LOG (0, trace, LFfwvis, "doInvalidate(IgTwig[" << twig->name ()
	 << "]," << mask << ")\n" << lat::indent);

    // Need to lock
    //    qApp->lock ();
 
    // Update all models this twig has a representation in.
    // (FIXME: Queue and pull from the queue in central place?)
    bool needStructureUpdate = mask & IgTwig::STRUCTURE_MASK;
    if (! model)
	IgRepSet::update (twig, mask);
    else if (IgRepSet::lookup (twig, model))
	IgRepSet::update (twig, model, mask);
    else
    {
	IgRepSet::lookup (twig, model, true);
	needStructureUpdate = false;
    }

    // Invalidate all sub-twigs if requested.  This may or may not
    // create children (and the above might have changed the child
    // list!), but try to optimise it away if we can.  In general
    // we don't make new reps, but update everything there is.
    if (needStructureUpdate)
    {
	for (unsigned i = 0, n = twig->children (); i < n; ++i)
	    IgRepSet::invalidate (twig->child (i), model, mask);
    }

    //qApp->unlock (false);
    LOG (0, trace, LFfwvis, lat::undent);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
MMM_DEFUN_FUNC(IgRepContext *,IgBrowserMethods::,doRepresent,
               (IgTwig *twig, IgTextModel *model))
{
    LOG (0, trace, LFfwvis, "doRepresent(IgTwig[" << twig->name ()
	 << "],IgTextModel)\n" << lat::indent);

    //qApp->lock ();

    // Create an empty place-holder rep.
    IgTextRep *rep = new IgSimpleTextRep (model, "");
    IgRepContext *context = new IgRepContext (twig, rep);

    // Redirect actual fill to doUpdate().
    IgRepSet::update (twig, model, IgTwig::SELF_MASK);

    // Return
    //qApp->unlock (false);
    LOG (0, trace, LFfwvis, lat::undent);
    return context;
}

MMM_DEFUN_FUNC(void,IgBrowserMethods::,doUpdate,
               (IgTwig *twig, IgTextRep *rep, unsigned mask))
{
    LOG (0, trace, LFfwvis, "doUpdate(IgTwig[" << twig->name ()
	 << "],IgTextRep," << mask << ")\n" << lat::indent);

    // Need to lock
    //qApp->lock ();
    if (mask & IgTwig::SELF_MASK)
	rep->setText ("<i>(No information)</i>");

    //qApp->unlock (false);
    LOG (0, trace, LFfwvis, lat::undent);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
MMM_DEFUN_FUNC(IgRepContext *, IgBrowserMethods::,doRepresent,
	       (IgTwig *twig, Ig3DModel *model))
{
    LOG (0, trace, LFfwvis, "doRepresent(IgTwig[" << twig->name ()
	 << "],Ig3DModel)\n" << lat::indent);

    // Need to lock: inventor is not thread safe.
    //qApp->lock ();

//     // Make sure hierarchy above is created correctly
//     Ig3DRep *prep = 0;
//     Ig3DRep	*prev = 0;
// //     if (IgRepContext *existing = preMake3DRep (twig, model, &prep, &prev))
// //     {
// // 	qApp->unlock (false);
// // 	return existing;
// //     }

//     // Now create the payload.  This is just an empty switch.  Derived
//     // classes should add the actual representation.
// //     SoSwitch	*node = new SoSwitch;
// //     node->setName (Ig3DModel::encode (twig->name ()));
// //     node->whichChild = twig->visible () ? SO_SWITCH_ALL : SO_SWITCH_NONE;

//     IgRepContext *context
// 	= new IgRepContext (twig, new Ig3DRep);

//     // Redirect actual fill to doUpdate().
//     IgRepSet::update (twig, model, IgTwig::SELF_MASK);

// //     // Now make sure hierarchy below is created correctly
// //     postMake3DRep (twig, model);

// //     // Release lock and return
// //     qApp->unlock (false);
    LOG (0, trace, LFfwvis, lat::undent);
    //    return context;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
MMM_DEFUN_FUNC(IgRepContext *, IgBrowserMethods::,doRepresent,
	       (IgTwig *twig, IgLegoModel *model))
{
    LOG (0, trace, LFfwvis, "doRepresent(IgTwig[" << twig->name ()
	 << "],IgLegoModel)\n" << lat::indent);

    // Need to lock: inventor is not thread safe.
    //    qApp->lock ();

    // Make sure hierarchy above is created correctly
    Ig3DRep *prep = 0;
    Ig3DRep	*prev = 0;
//     if (IgRepContext *existing = preMake3DRep (twig, model, &prep, &prev))
//     {
// 	//qApp->unlock (false);
// 	return existing;
//     }

//     // Now create the payload.  This is just an empty switch.  Derived
//     // classes should add the actual representation.
//     SoSwitch	*node = new SoSwitch;
//     node->setName (IgLegoModel::encode (twig->name ()));
//     node->whichChild = twig->visible () ? SO_SWITCH_ALL : SO_SWITCH_NONE;

//     IgRepContext *context
// 	= new IgRepContext (twig, new IgLegoRep (model,
// 						 static_cast<IgLegoRep *> (prep),
// 						 static_cast<IgLegoRep *> (prev),
// 						 node));

    // Redirect actual fill to doUpdate().
    IgRepSet::update (twig, model, IgTwig::SELF_MASK);

//     // Now make sure hierarchy below is created correctly
//     postMake3DRep (twig, model);

//     // Release lock and return
//     qApp->unlock (false);
    LOG (0, trace, LFfwvis, lat::undent);
    //    return context;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
MMM_DEFUN_FUNC(IgRepContext *, IgBrowserMethods::,doRepresent,
	       (IgTwig *twig, IgRZModel *model))
{
    LOG (0, trace, LFfwvis, "doRepresent(IgTwig[" << twig->name ()
	 << "],IgRZModel)\n" << lat::indent);

    // Need to lock: inventor is not thread safe.
    //    qApp->lock ();

    // Make sure hierarchy above is created correctly
    Ig3DRep *prep = 0;
    Ig3DRep	*prev = 0;
//     if (IgRepContext *existing = preMake3DRep (twig, model, &prep, &prev))
//     {
// 	qApp->unlock (false);
// 	return existing;
//     }

    // Now create the payload.  This is just an empty switch.  Derived
    // classes should add the actual representation.
//     SoSwitch	*node = new SoSwitch;
//     node->setName (IgRZModel::encode (twig->name ()));
//     node->whichChild = twig->visible () ? SO_SWITCH_ALL : SO_SWITCH_NONE;

//     IgRepContext *context
// 	= new IgRepContext (twig, new IgRZRep (model,
// 					       static_cast<IgRZRep *> (prep),
// 					       static_cast<IgRZRep *> (prev),
// 					       node));

    // Redirect actual fill to doUpdate().
    IgRepSet::update (twig, model, IgTwig::SELF_MASK);

//     // Now make sure hierarchy below is created correctly
//     postMake3DRep (twig, model);

//     // Release lock and return
//     qApp->unlock (false);
    LOG (0, trace, LFfwvis, lat::undent);
    //    return context;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
MMM_DEFUN_FUNC(IgRepContext *, IgBrowserMethods::,doRepresent,
	       (IgTwig *twig, IgRPhiModel *model))
{
    LOG (0, trace, LFfwvis, "doRepresent(IgTwig[" << twig->name ()
	 << "],IgRPhiModel)\n" << lat::indent);

    // Need to lock: inventor is not thread safe.
//     qApp->lock ();

    // Make sure hierarchy above is created correctly
    Ig3DRep *prep = 0;
    Ig3DRep	*prev = 0;
//     if (IgRepContext *existing = preMake3DRep (twig, model, &prep, &prev))
//     {
// 	qApp->unlock (false);
// 	return existing;
//     }

    // Now create the payload.  This is just an empty switch.  Derived
    // classes should add the actual representation.
//     SoSwitch	*node = new SoSwitch;
//     node->setName (IgRPhiModel::encode (twig->name ()));
//     node->whichChild = twig->visible () ? SO_SWITCH_ALL : SO_SWITCH_NONE;

//     IgRepContext *context
// 	= new IgRepContext (twig, new IgRPhiRep (model,
// 						 static_cast<IgRPhiRep *> (prep),
// 						 static_cast<IgRPhiRep *> (prev),
// 						 node));

    // Redirect actual fill to doUpdate().
    IgRepSet::update (twig, model, IgTwig::SELF_MASK);

//     // Now make sure hierarchy below is created correctly
//     postMake3DRep (twig, model);

//     // Release lock and return
//     qApp->unlock (false);
    LOG (0, trace, LFfwvis, lat::undent);
    //    return context;
}

