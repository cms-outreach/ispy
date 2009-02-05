//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgXMLModel/interface/IgXMLRep.h"
#include "Ig_Modules/IgXMLModel/interface/IgXMLModel.h"
#include "Ig_Modules/IgXMLModel/interface/IgUIDRep.h"
#include "Ig_Modules/IgXMLModel/interface/IgUIDModel.h"
#include "Ig_Modules/IgXMLModel/interface/IgAjaxTreeModel.h"
#include "Ig_Modules/IgXMLModel/interface/xtypeinfo.h"
#include "Ig_Framework/IgObjectBrowser/interface/xtypeinfo.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgBrowserMethods.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepContext.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepSet.h"
#include "Ig_Modules/IgCommonData/interface/IgTwig.h"
#include "Ig_Modules/IgCommonData/interface/xtypeinfo.h"
#include <classlib/utils/DebugAids.h>
#include <classlib/utils/Log.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

XTYPEINFO_DEF_1 (IgXMLRep, IgRep, false);
XTYPEINFO_DEF_1 (IgXMLModel, IgModel, false);
XTYPEINFO_DEF_1 (IgUIDRep, IgRep, false);
XTYPEINFO_DEF_1 (IgUIDModel, IgModel, false);
XTYPEINFO_DEF_1 (IgAjaxTreeModel, IgXMLModel, false);

lat::logflag LFcommonXML = { 0, "commonXML", true, -1 };


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
IgRepContext *
preMakeXMLRep (IgTwig		*twig,
	       IgXMLModel	*model,
	       IgXMLRep		**prep = 0,
	       IgXMLRep		**prev = 0)
{
    ASSERT (! IgRepSet::lookup (twig, model));
    LOG (1, trace, LFcommonXML, "preMakeXMLRep(["
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
    IgXMLRep	*parentrep = 0;
    IgXMLRep	*prevrep = 0;
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

    parentrep = static_cast<IgXMLRep *> (IgRepSet::lookup (parent, model));
    
    // If this is not the first child, force representations for
    // previous siblings so that model has the same child order as the
    // twigs.  This matters only in that we want to save and read back
    // the scene graph and get back the same twig tree; nothing else
    // depends on the model child order.
    if (parentrep && (index = parent->indexOf (twig)) > 0)
        prevrep = static_cast<IgXMLRep *>
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
postMakeXMLRep (IgTwig *twig, IgXMLModel *model)
{
    LOG (1, trace, LFcommonXML, "postMakeXMLRep(["
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

MMM_DEFUN_FUNC(IgRepContext *,IgBrowserMethods::,doRepresent,
               (IgTwig *twig, IgXMLModel *model))
{
    LOG (0, trace, LFcommonXML, "doRepresent(IgTwig[" << twig->name ()
	 << "],IgXMLModel)\n" << lat::indent);

    // Make sure hierarchy above is created correctly
    IgXMLRep *prep = 0;
    IgXMLRep *prev = 0;
    if (IgRepContext *existing = preMakeXMLRep (twig, model, &prep, &prev))
    {
	return existing;
    }

    // Create an empty place-holder rep.
    IgXMLRep *rep = new IgXMLRep (model, prep, prev, "TWIG");
    IgRepContext *context = new IgRepContext (twig, rep);

    // Redirect actual fill to doUpdate().
    IgRepSet::update (twig, model, IgTwig::SELF_MASK);

    // Return
    LOG (0, trace, LFcommonXML, lat::undent);
    return context;
}

MMM_DEFUN_FUNC(void,IgBrowserMethods::,doUpdate,
               (IgTwig *twig, IgXMLRep *rep, unsigned mask))
{
    LOG (0, trace, LFcommonXML, "doUpdate(IgTwig[" << twig->name ()
	 << "],IgXMLRep," << mask << ")\n" << lat::indent);
    
    if (mask & IgTwig::SELF_MASK)
	rep->setAttribute ("NAME", twig->name ());

    if (twig->selfVisible ())
	rep->setAttribute ("VIS", "1");    
    
    if (mask & IgTwig::STRUCTURE_MASK)
    {
	for (unsigned int i = 0; i < twig->children (); i++)
	{
	    IgRepSet::lookup (twig->child (i), rep->model (), true);		
	}	    
    }    
        
    LOG (0, trace, LFcommonXML, lat::undent);
}



MMM_DEFUN_FUNC(IgRepContext *,IgBrowserMethods::,doRepresent,
               (IgTwig *twig, IgAjaxTreeModel *model))
{
    LOG (0, trace, LFcommonXML, "doRepresent(IgTwig[" << twig->name ()
	 << "],IgAjaxModel)\n" << lat::indent);

    // Make sure hierarchy above is created correctly
    IgXMLRep *prep = 0;
    IgXMLRep *prev = 0;
    if (IgRepContext *existing = preMakeXMLRep (twig, model, &prep, &prev))
    {
	return existing;
    }

    IgRep *parentRep = IgRepSet::lookup (twig->parent (), model, false);
    
    // Create an empty place-holder rep.
    IgXMLRep *rep = new IgXMLRep (model, prep, prev, "TWIG");
    IgRepContext *context = new IgRepContext (twig, rep);


    // Put the UID in the rep.
    IgUIDRep *uidRep =  dynamic_cast <IgUIDRep *> 	
			(IgRepSet::lookup (twig, model->uidModel (), true));
    ASSERT (uidRep);
    
    rep->setAttribute ("RPID", uidRep->id ());

    // Redirect actual fill to doUpdate().
    IgRepSet::update (twig, model, IgTwig::SELF_MASK);

    if (parentRep == 0)
    {
	// Create 1 level of children.
	for (unsigned int i = twig->children ();
	     i != 0;)
	{
	    --i;	    
	    IgRepSet::lookup (twig->child (i), model, true);
	    //	    IgRepSet::update (twig->child (i), model, IgTwig::SELF_MASK);	
	}
    }
    
    // Return
    LOG (0, trace, LFcommonXML, lat::undent);
    return context;
}



MMM_DEFUN_FUNC(IgRepContext *,IgBrowserMethods::,doRepresent,
               (IgTwig *twig, IgUIDModel *model))
{
    LOG (0, trace, LFcommonXML, "doRepresent(IgTwig[" << twig->name ()
	 << "],IgUIDModel)\n" << lat::indent);

    IgUIDRep *rep = new IgUIDRep (model);
    IgRepContext *context = new IgRepContext (twig, rep);
    
    LOG (0, trace, LFcommonXML, lat::undent);
    return context;
}


MMM_DEFUN_FUNC(void,IgBrowserMethods::,doUpdate,
               (IgTwig */*twig*/, IgUIDRep */*rep*/, unsigned /*mask*/))
{
}
