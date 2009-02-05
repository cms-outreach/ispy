//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgCommon3DReps/interface/config.h"
#include "Ig_Modules/IgCommon3DReps/interface/IgCommon3DReps.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgCapabilityDB.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgBrowserMethods.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepContext.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepSet.h"
#include "Ig_Modules/IgCommonData/interface/IgTwig.h"
#include "Ig_Modules/IgCommonData/interface/xtypeinfo.h"
#include "Ig_Modules/Ig3DBaseModel/interface/xtypeinfo.h"
#include "Ig_Modules/Ig3DModel/interface/Ig3DModel.h"
#include "Ig_Modules/Ig3DModel/interface/Ig3DRep.h"
#include "Ig_Modules/Ig3DModel/interface/xtypeinfo.h"
#include "Ig_Modules/Ig2DModel/interface/Ig2DModel.h"
#include "Ig_Modules/Ig2DModel/interface/Ig2DRep.h"
#include "Ig_Modules/Ig2DModel/interface/xtypeinfo.h"
#include "Ig_Modules/IgLegoModel/interface/IgLegoModel.h"
#include "Ig_Modules/IgLegoModel/interface/IgLegoRep.h"
#include "Ig_Modules/IgLegoModel/interface/xtypeinfo.h"
#include "Ig_Modules/IgRZModel/interface/IgRZModel.h"
#include "Ig_Modules/IgRZModel/interface/IgRZRep.h"
#include "Ig_Modules/IgRZModel/interface/xtypeinfo.h"
#include "Ig_Modules/IgRPhiModel/interface/IgRPhiModel.h"
#include "Ig_Modules/IgRPhiModel/interface/IgRPhiRep.h"
#include "Ig_Modules/IgRPhiModel/interface/xtypeinfo.h"
#include "Ig_Modules/IgTextModel/interface/IgSimpleTextRep.h"
#include "Ig_Modules/IgTextModel/interface/IgTextModel.h"
#include "Ig_Modules/IgTextModel/interface/xtypeinfo.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSurfaceOperation.h"
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoResetTransform.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoCube.h>
#include <iostream>
#include <vector>
#include <utility>
#include <cmath>

#include <qapplication.h>
#undef emit // zap qt macro
#include <classlib/utils/DebugAids.h>
#include <classlib/utils/Log.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>

/// #logstream flag for this package.
lat::logflag LFcommon3d = { 0, "common3d", true, -1 };

//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
MMM_DEFUN_FUNC(void,IgBrowserMethods::,doInvalidate,
               (IgTwig *twig, IgModel *model, unsigned mask))
{
    LOG (0, trace, LFcommon3d, "doInvalidate(IgTwig[" << twig->name ()
	 << "]," << mask << ")\n" << lat::indent);

    // Need to lock
    qApp->lock ();
 
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

    qApp->unlock (false);
    LOG (0, trace, LFcommon3d, lat::undent);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
MMM_DEFUN_FUNC(IgRepContext *,IgBrowserMethods::,doRepresent,
               (IgTwig *twig, IgTextModel *model))
{
    LOG (0, trace, LFcommon3d, "doRepresent(IgTwig[" << twig->name ()
	 << "],IgTextModel)\n" << lat::indent);

    qApp->lock ();

    // Create an empty place-holder rep.
    IgTextRep *rep = new IgSimpleTextRep (model, "");
    IgRepContext *context = new IgRepContext (twig, rep);

    // Redirect actual fill to doUpdate().
    IgRepSet::update (twig, model, IgTwig::SELF_MASK);

    // Return
    qApp->unlock (false);
    LOG (0, trace, LFcommon3d, lat::undent);
    return context;
}

MMM_DEFUN_FUNC(void,IgBrowserMethods::,doUpdate,
               (IgTwig *twig, IgTextRep *rep, unsigned mask))
{
    LOG (0, trace, LFcommon3d, "doUpdate(IgTwig[" << twig->name ()
	 << "],IgTextRep," << mask << ")\n" << lat::indent);

    // Need to lock
    qApp->lock ();
    if (mask & IgTwig::SELF_MASK)
	rep->setText ("<i>(No information)</i>");

    qApp->unlock (false);
    LOG (0, trace, LFcommon3d, lat::undent);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
IgRepContext *
preMake3DRep (IgTwig		*twig,
	      Ig3DBaseModel	*model,
	      Ig3DBaseRep	**prep = 0,
	      Ig3DBaseRep	**prev = 0)
{
    ASSERT (! IgRepSet::lookup (twig, model));
    LOG (1, trace, LFcommon3d, "preMake3DRep(["
	 << twig->name () << "],[" << typeid(*model).name ()
	 << "]\n");

    // Create no real representation here, only the hierarchy.  Make
    // sure that if any ancestor of this twig is represented, all the
    // intermediate ancestors get there as well: we may come in here
    // from another browser that has a fully expanded view.  Expand
    // only up to the first visible ancestor; the browsing may not
    // have started at the top of the tree.  If no ancestor is
    // visible, this twig goes top level.
    IgTwig	*parent = twig->parent ();
    Ig3DBaseRep	*parentrep = 0;
    Ig3DBaseRep	*prevrep = 0;
    unsigned	index;

    for (IgTwig *t = parent; t; t = t->parent ())
        if (IgRepSet::lookup (t, model))
	{
	    // Found an ancestor represented in the same model.

	    // Create parent representation if we found an ancestor
	    // higher than our direct parent.  That may force this
	    // child representation to be created, so if that happens,
	    // we have nothing more to do.  It will not happen if the
	    // parent is not being traversed, in which case we still
	    // need to create representation.  (FIXME: This really
	    // needs simplification!)
	    IgRepSet::lookup (parent, model, true);
	    if (IgRep *myrep = IgRepSet::lookup (twig, model))
		return myrep->context ();

	    break;
	}

    parentrep = static_cast<Ig3DBaseRep *> (IgRepSet::lookup (parent, model));

    // If this is not the first child, force representations for
    // previous siblings so that model has the same child order as the
    // twigs.  This matters only in that we want to save and read back
    // the scene graph and get back the same twig tree; nothing else
    // depends on the model child order.
    if (parentrep && (index = parent->indexOf (twig)) > 0)
        prevrep = static_cast<Ig3DBaseRep *>
		  (IgRepSet::lookup (parent->child (index-1), model, true));

    // FIXME: Can sibling creation make this one appear?
    ASSERT (! IgRepSet::lookup (twig, model));

    // Pass out values if they are wanted
    if (prep) *prep = parentrep;
    if (prev) *prev = prevrep;

    // Indicate the context still needs creating
    return 0;
}

void
postMake3DRep (IgTwig *twig, Ig3DBaseModel *model)
{
    LOG (1, trace, LFcommon3d, "postMake3DRep(["
	 << twig->name () << "],[" << typeid(*model).name ()
	 << "]\n");

    // Create child representations if I am visible; if I am, ensure
    // we have children to traverse.  Note that the children cannot
    // have a representation yet since we always create them in the
    // tree order.
    if (twig->traverse ())
    {
        twig->expand ();
        for (unsigned i = 0, n = twig->children (); i < n; ++i)
        {
	    ASSERT (! IgRepSet::lookup (twig->child (i), model));
	    IgRepSet::lookup (twig->child (i), model, true);
        }
    }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
MMM_DEFUN_FUNC(IgRepContext *, IgBrowserMethods::,doRepresent,
	       (IgTwig *twig, Ig3DModel *model))
{
    LOG (0, trace, LFcommon3d, "doRepresent(IgTwig[" << twig->name ()
	 << "],Ig3DModel)\n" << lat::indent);

    // Need to lock: inventor is not thread safe.
    qApp->lock ();

    // Make sure hierarchy above is created correctly
    Ig3DBaseRep *prep = 0;
    Ig3DBaseRep	*prev = 0;
    if (IgRepContext *existing = preMake3DRep (twig, model, &prep, &prev))
    {
	qApp->unlock (false);
	return existing;
    }

    // Now create the payload.  This is just an empty switch.  Derived
    // classes should add the actual representation.
    SoSwitch	*node = new SoSwitch;
    node->setName (Ig3DModel::encode (twig->name ()));
    node->whichChild = twig->visible () ? SO_SWITCH_ALL : SO_SWITCH_NONE;

    IgRepContext *context
	= new IgRepContext (twig, new Ig3DRep (model,
					       static_cast<Ig3DRep *> (prep),
					       static_cast<Ig3DRep *> (prev),
					       node));

    // Redirect actual fill to doUpdate().
    IgRepSet::update (twig, model, IgTwig::SELF_MASK);

    // Now make sure hierarchy below is created correctly
    postMake3DRep (twig, model);

    // Release lock and return
    qApp->unlock (false);
    LOG (0, trace, LFcommon3d, lat::undent);
    return context;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
MMM_DEFUN_FUNC(void,IgBrowserMethods::,doUpdate,
               (IgTwig *twig, Ig3DBaseRep *rep, unsigned mask))
{
    LOG (0, trace, LFcommon3d, "doUpdate(IgTwig[" << twig->name ()
	 << "],Ig3DBaseRep," << mask << ")\n" << lat::indent);

    // Need to lock, inventor is not thread-safe
    qApp->lock ();

    // The derived class must act on rep->node() if it added to it, we
    // only flip the visibility and handle the children.
    ASSERT (dynamic_cast<Ig3DBaseModel *> (rep->model ()));
    ASSERT (rep->node ());
    ASSERT (rep->node ()->isOfType (SoSwitch::getClassTypeId ()));
    SoSwitch *node = static_cast<SoSwitch *> (rep->node ());

    if (mask & (IgTwig::FLAGS_MASK | IgTwig::SELF_MASK))
	// Hide/show the payload.  Some objects have a large payload
	// that can be regenerated quickly, so it would be better to
	// have a hint indicating preference between blasting away
	// nodes or just hiding them.  For now, just override
	// doUpdate() to blast away the nodes.
	node->whichChild = twig->visible () ? SO_SWITCH_ALL : SO_SWITCH_NONE;

    if (mask & IgTwig::SELF_MASK)
	node->setName (Ig3DBaseModel::encode (twig->name ())); // FIXME: Needed?

    // Always clear the 3D representation.  This is necessary so
    // that the old contents are not left in if the twig changes
    // while invisible, and is subsequently made visible again (we
    // have no past knowledge about invalidations when making the
    // twig visible again).
    node->removeAllChildren ();

    // Update children
    if (mask & IgTwig::STRUCTURE_MASK)
    {
	// Update children.  The actual child list may have changed,
	// but in that case the old children must have been destroyed
	// and their reps are gone (we don't support reparenting).
	// Thus we don't need to check for actual structural change,
	// just react to traversal setting changes.  Structural walk
	// is done in doInvalidate(), after this method is done, so
	// all we need to do here is make sure representations exist
	// for the children where required.
	IgModel *model = rep->model ();
	if (twig->traverse ())
	{
	    // Ensure expansion; the make sure there is a rep. FIXME:
	    // we should somehow mark newly created children valid so
	    // subsequent invalidate() won't immediately recreate them!
	    twig->expand ();
	    for (unsigned i = 0, n = twig->children (); i < n; ++i)
		IgRepSet::lookup (twig->child (i), model, true);
	}
    }

    qApp->unlock (false);
    LOG (0, trace, LFcommon3d, lat::undent);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
MMM_DEFUN_FUNC(IgRepContext *, IgBrowserMethods::,doRepresent,
	       (IgTwig *twig, Ig2DModel *model))
{
    LOG (0, trace, LFcommon3d, "doRepresent(IgTwig[" << twig->name ()
	 << "],Ig2DModel)\n" << lat::indent);

    // Need to lock: inventor is not thread safe.
    qApp->lock ();

    // Make sure hierarchy above is created correctly
    if (IgRepContext *existing = preMake3DRep (twig, model))
    {
	qApp->unlock (false);
	return existing;
    }

    // Get layer
    IgRepContext *context
	= new IgRepContext (twig, model->getLayer (twig->fullName ()));

    // Redirect actual fill to doUpdate().
    IgRepSet::update (twig, model, IgTwig::SELF_MASK);

    // Make sure hierarchy below is created correctly
    postMake3DRep (twig, model);

    // Release lock and return
    qApp->unlock (false);
    LOG (0, trace, LFcommon3d, lat::undent);
    return context;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

MMM_DEFUN_FUNC(void,IgBrowserMethods::,doUpdate,
               (IgTwig *twig, Ig2DRep *rep, unsigned mask))
{
    LOG (0, trace, LFcommon3d, "doUpdate(IgTwig[" << twig->name ()
	 << "],Ig2DModel)\n" << lat::indent);


    // Need to lock: inventor is not thread safe.
    qApp->lock ();
    
    mmm_doUpdate (twig, static_cast<Ig3DBaseRep *> (rep), mask);

    if (twig->visible ())
    {
	Ig2DModel *model = dynamic_cast <Ig2DModel *> (rep->model ());
	ASSERT (model);        
	ASSERT (model->sourceModel ());

	Ig3DBaseRep *sourceRep
	    = static_cast<Ig3DBaseRep *>
	    (IgRepSet::lookup (twig, model->sourceModel ()));
	if (sourceRep)
	    IgRepSet::update (twig, model->sourceModel (), 
			      mask | IgTwig::SELF_MASK);
	else
	    sourceRep = static_cast<Ig3DBaseRep *>
			(IgRepSet::lookup (twig, model->sourceModel (), true));

	rep->clear ();
	if (sourceRep)
	{
	    Ig2DModel *model = dynamic_cast<Ig2DModel *> (rep->model ());
	    ASSERT (model);
	    
	    if (model->isToBeCut (twig->fullName ()))
	    {
		static SbViewportRegion viewportRegion (640, 480);	    
		static SoGetBoundingBoxAction bbAction(viewportRegion);

		// Gets the material for the twig
		SoSearchAction getMaterialAction;
		getMaterialAction.setType (SoMaterial::getClassTypeId ());
		getMaterialAction.apply (sourceRep->node ());	
		if (getMaterialAction.getPath () != 0)
		    rep->node ()->addChild
			(getMaterialAction.getPath ()->getTail ());
		
		SoSeparator *resultSep = new SoSeparator;
		rep->node ()->addChild (resultSep);
			
		// Cut the original rep with a plane 
		
		// First we rotate the object so that we are cutting it
		// with the correct orientation.
		SoSeparator *sep = new SoSeparator;
		sep->ref ();
		
		sep->addChild (model->cutTransform ());	    
		sep->addChild (sourceRep->node ());
		
		// Then we calculate its 2D projected bounding box, so
		// that the plane we are using to cut it is actually
		// rescaled to better match the position and size of the
		// object.
		bbAction.apply (sep);
		SbBox3f sourceBox = bbAction.getBoundingBox ();
		
		if (! sourceBox.isEmpty ())
		{
		    SbVec3f sourceCenter = sourceBox.getCenter ();
		    SbVec3f dimensions = sourceBox.getMax () - sourceCenter;
		    float sourceSizeWidth = fabs (dimensions [0]);
		    float sourceSizeHeight = fabs (dimensions [1]);
		
		    model->setCutPlanePosition (sourceCenter[0], sourceCenter[1]);
		    model->setCutPlaneSize (sourceSizeWidth*2,
					    sourceSizeHeight*2);
		
		    // Then we perform the cut operation and add the result to
		    // the rep.
		    IgSurfaceOperation cutOperation (IgSurfaceOperation::SURFACE_1_IN_2);
		    SoNode *result = cutOperation.apply (model->cutPlane (), sep);
		    resultSep->addChild (result);
		}
		sep->unref ();
	    }
	    else
	    {
		SoSeparator *resultSep = new SoSeparator;
		resultSep->addChild (model->cutTransform ());		
		resultSep->addChild (sourceRep);
		rep->node ()->addChild (resultSep);	
	    }	    
	}	
    }
    
    qApp->unlock (false);
    LOG (0, trace, LFcommon3d, lat::undent);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
MMM_DEFUN_FUNC(IgRepContext *, IgBrowserMethods::,doRepresent,
	       (IgTwig *twig, IgLegoModel *model))
{
    LOG (0, trace, LFcommon3d, "doRepresent(IgTwig[" << twig->name ()
	 << "],IgLegoModel)\n" << lat::indent);

    // Need to lock: inventor is not thread safe.
    qApp->lock ();

    // Make sure hierarchy above is created correctly
    Ig3DBaseRep *prep = 0;
    Ig3DBaseRep	*prev = 0;
    if (IgRepContext *existing = preMake3DRep (twig, model, &prep, &prev))
    {
	qApp->unlock (false);
	return existing;
    }

    // Now create the payload.  This is just an empty switch.  Derived
    // classes should add the actual representation.
    SoSwitch	*node = new SoSwitch;
    node->setName (IgLegoModel::encode (twig->name ()));
    node->whichChild = twig->visible () ? SO_SWITCH_ALL : SO_SWITCH_NONE;

    IgRepContext *context
	= new IgRepContext (twig, new IgLegoRep (model,
						 static_cast<IgLegoRep *> (prep),
						 static_cast<IgLegoRep *> (prev),
						 node));

    // Redirect actual fill to doUpdate().
    IgRepSet::update (twig, model, IgTwig::SELF_MASK);

    // Now make sure hierarchy below is created correctly
    postMake3DRep (twig, model);

    // Release lock and return
    qApp->unlock (false);
    LOG (0, trace, LFcommon3d, lat::undent);
    return context;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
MMM_DEFUN_FUNC(IgRepContext *, IgBrowserMethods::,doRepresent,
	       (IgTwig *twig, IgRZModel *model))
{
    LOG (0, trace, LFcommon3d, "doRepresent(IgTwig[" << twig->name ()
	 << "],IgRZModel)\n" << lat::indent);

    // Need to lock: inventor is not thread safe.
    qApp->lock ();

    // Make sure hierarchy above is created correctly
    Ig3DBaseRep *prep = 0;
    Ig3DBaseRep	*prev = 0;
    if (IgRepContext *existing = preMake3DRep (twig, model, &prep, &prev))
    {
	qApp->unlock (false);
	return existing;
    }

    // Now create the payload.  This is just an empty switch.  Derived
    // classes should add the actual representation.
    SoSwitch	*node = new SoSwitch;
    node->setName (IgRZModel::encode (twig->name ()));
    node->whichChild = twig->visible () ? SO_SWITCH_ALL : SO_SWITCH_NONE;

    IgRepContext *context
	= new IgRepContext (twig, new IgRZRep (model,
					       static_cast<IgRZRep *> (prep),
					       static_cast<IgRZRep *> (prev),
					       node));

    // Redirect actual fill to doUpdate().
    IgRepSet::update (twig, model, IgTwig::SELF_MASK);

    // Now make sure hierarchy below is created correctly
    postMake3DRep (twig, model);

    // Release lock and return
    qApp->unlock (false);
    LOG (0, trace, LFcommon3d, lat::undent);
    return context;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
MMM_DEFUN_FUNC(IgRepContext *, IgBrowserMethods::,doRepresent,
	       (IgTwig *twig, IgRPhiModel *model))
{
    LOG (0, trace, LFcommon3d, "doRepresent(IgTwig[" << twig->name ()
	 << "],IgRPhiModel)\n" << lat::indent);

    // Need to lock: inventor is not thread safe.
    qApp->lock ();

    // Make sure hierarchy above is created correctly
    Ig3DBaseRep *prep = 0;
    Ig3DBaseRep	*prev = 0;
    if (IgRepContext *existing = preMake3DRep (twig, model, &prep, &prev))
    {
	qApp->unlock (false);
	return existing;
    }

    // Now create the payload.  This is just an empty switch.  Derived
    // classes should add the actual representation.
    SoSwitch	*node = new SoSwitch;
    node->setName (IgRPhiModel::encode (twig->name ()));
    node->whichChild = twig->visible () ? SO_SWITCH_ALL : SO_SWITCH_NONE;

    IgRepContext *context
	= new IgRepContext (twig, new IgRPhiRep (model,
						 static_cast<IgRPhiRep *> (prep),
						 static_cast<IgRPhiRep *> (prev),
						 node));

    // Redirect actual fill to doUpdate().
    IgRepSet::update (twig, model, IgTwig::SELF_MASK);

    // Now make sure hierarchy below is created correctly
    postMake3DRep (twig, model);

    // Release lock and return
    qApp->unlock (false);
    LOG (0, trace, LFcommon3d, lat::undent);
    return context;
}
