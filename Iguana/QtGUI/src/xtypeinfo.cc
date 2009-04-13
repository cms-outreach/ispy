//<<<<<< INCLUDES                                                       >>>>>>
#define QT_NO_EMIT
#include "Iguana/QtGUI/interface/ISpyQueuedTwig.h"
#include "Iguana/QtGUI/interface/ISpyBasicClusterTwig.h"
#include "Iguana/QtGUI/interface/ISpyCaloTowerTwig.h"
#include "Iguana/QtGUI/interface/ISpyCSCSegmentTwig.h"
#include "Iguana/QtGUI/interface/ISpyDTRecSegment4DTwig.h"
#include "Iguana/QtGUI/interface/ISpyDetectorTwig.h"
#include "Iguana/QtGUI/interface/ISpyEventTwig.h"
#include "Iguana/QtGUI/interface/ISpyEcalRecHitTwig.h"
#include "Iguana/QtGUI/interface/ISpyHBRecHitTwig.h"
#include "Iguana/QtGUI/interface/ISpyHERecHitTwig.h"
#include "Iguana/QtGUI/interface/ISpyHFRecHitTwig.h"
#include "Iguana/QtGUI/interface/ISpyHORecHitTwig.h"
#include "Iguana/QtGUI/interface/ISpyJetTwig.h"
#include "Iguana/QtGUI/interface/ISpyMETTwig.h"
#include "Iguana/QtGUI/interface/ISpyMuonTwig.h"
#include "Iguana/QtGUI/interface/ISpyPFClusterTwig.h"
#include "Iguana/QtGUI/interface/ISpyPFRecHitTwig.h"
#include "Iguana/QtGUI/interface/ISpyPFRecTrackTwig.h"
#include "Iguana/QtGUI/interface/ISpyPixelDigiTwig.h"
#include "Iguana/QtGUI/interface/ISpyRPCRecHitTwig.h"
#include "Iguana/QtGUI/interface/ISpySiPixelClusterTwig.h"
#include "Iguana/QtGUI/interface/ISpySiPixelRecHitTwig.h"
#include "Iguana/QtGUI/interface/ISpySiStripClusterTwig.h"
#include "Iguana/QtGUI/interface/ISpySiStripDigiTwig.h"
#include "Iguana/QtGUI/interface/ISpyTrackingRecHitTwig.h"
#include "Iguana/QtGUI/interface/ISpyTrackTwig.h"
#include "Iguana/QtGUI/interface/Ig3DModel.h"
#include "Iguana/QtGUI/interface/IgRPhiModel.h"
#include "Iguana/QtGUI/interface/IgRZModel.h"
#include "Iguana/QtGUI/interface/IgLegoModel.h"
#include "Iguana/QtGUI/interface/IgSimpleTextModel.h"
#include "Iguana/QtGUI/interface/IgTextModel.h"
#include "Iguana/QtGUI/interface/Ig3DRep.h"
#include "Iguana/QtGUI/interface/IgRZRep.h"
#include "Iguana/QtGUI/interface/IgRPhiRep.h"
#include "Iguana/QtGUI/interface/IgLegoRep.h"
#include "Iguana/QtGUI/interface/IgTextRep.h"
#include "Iguana/QtGUI/interface/IgSimpleTextRep.h"
#include "Iguana/QtGUI/interface/debug.h"
#include "Iguana/Framework/interface/IgBrowserMethods.h"
#include "Iguana/Framework/interface/IgBrowserDB.h"
#include "Iguana/Framework/interface/IgRepContext.h"
#include "Iguana/Framework/interface/IgTwig.h"
#include "Iguana/Framework/interface/xtypeinfo.h"
#include "Iguana/QtGUI/interface/xtypeinfo.h"

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
XTYPEINFO_DEF_1 (ISpyQueuedTwig, IgSimpleTwig, false);
XTYPEINFO_DEF_1 (ISpyBasicClusterTwig, IgSimpleTwig, false);
XTYPEINFO_DEF_1 (ISpyCaloTowerTwig, IgSimpleTwig, false);
XTYPEINFO_DEF_1 (ISpyCSCSegmentTwig, IgSimpleTwig, false);
XTYPEINFO_DEF_1 (ISpyDetectorTwig, IgSimpleTwig, false);
XTYPEINFO_DEF_1 (ISpyDTRecSegment4DTwig, IgSimpleTwig, false);
XTYPEINFO_DEF_1 (ISpyEcalRecHitTwig, IgSimpleTwig, false);
XTYPEINFO_DEF_1 (ISpyEventTwig, IgSimpleTwig, false);
XTYPEINFO_DEF_1 (ISpyHBRecHitTwig, IgSimpleTwig, false);
XTYPEINFO_DEF_1 (ISpyHERecHitTwig, IgSimpleTwig, false);
XTYPEINFO_DEF_1 (ISpyHFRecHitTwig, IgSimpleTwig, false);
XTYPEINFO_DEF_1 (ISpyHORecHitTwig, IgSimpleTwig, false);
XTYPEINFO_DEF_1 (ISpyJetTwig, IgSimpleTwig, false);
XTYPEINFO_DEF_1 (ISpyMETTwig, IgSimpleTwig, false);
XTYPEINFO_DEF_1 (ISpyMuonTwig, IgSimpleTwig, false);
XTYPEINFO_DEF_1 (ISpyPFClusterTwig, IgSimpleTwig, false);
XTYPEINFO_DEF_1 (ISpyPFRecHitTwig, IgSimpleTwig, false);
XTYPEINFO_DEF_1 (ISpyPFRecTrackTwig, IgSimpleTwig, false);
XTYPEINFO_DEF_1 (ISpyPixelDigiTwig, IgSimpleTwig, false);
XTYPEINFO_DEF_1 (ISpyRPCRecHitTwig, IgSimpleTwig, false);
XTYPEINFO_DEF_1 (ISpySiPixelClusterTwig, IgSimpleTwig, false);
XTYPEINFO_DEF_1 (ISpySiPixelRecHitTwig, IgSimpleTwig, false);
XTYPEINFO_DEF_1 (ISpySiStripClusterTwig, IgSimpleTwig, false);
XTYPEINFO_DEF_1 (ISpySiStripDigiTwig, IgSimpleTwig, false);
XTYPEINFO_DEF_1 (ISpyTrackingRecHitTwig, IgSimpleTwig, false);
XTYPEINFO_DEF_1 (ISpyTrackTwig, IgSimpleTwig, false);

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
    return 0;    
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
//     Ig3DRep *prep = 0;
//     Ig3DRep	*prev = 0;
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
    return 0;    
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
//     Ig3DRep *prep = 0;
//     Ig3DRep	*prev = 0;
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
    return 0;    
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
//     Ig3DRep *prep = 0;
//     Ig3DRep	*prev = 0;
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
    return 0;
}

