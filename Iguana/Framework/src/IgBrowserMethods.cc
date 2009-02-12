//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Framework/interface/IgBrowserMethods.h"
#include "Iguana/Framework/interface/IgCapabilityDB.h"
#include "Iguana/Framework/interface/IgRepresentable.h"
#include "Iguana/Framework/interface/IgModel.h"
#include "Iguana/Framework/interface/IgRep.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Framework/interface/xtypeinfo.h"
#include "Iguana/Framework/src/debug.h"
#include <cstdlib>
#include <set>
#include <utility>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

MULTIMETHOD_DEFINE(IgBrowserMethods::,doRepresent);
MULTIMETHOD_DEFINE(IgBrowserMethods::,doCommit);
MULTIMETHOD_DEFINE(IgBrowserMethods::,doUpdate);
MULTIMETHOD_DEFINE(IgBrowserMethods::,doExpand);
MULTIMETHOD_DEFINE(IgBrowserMethods::,doInvalidate);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

MMM_DEFUN_FUNC(IgRepContext *,IgBrowserMethods::,
	       doRepresent,(IgRepresentable *object,IgModel *model))
{
    LOG(0, trace, LFbrowser, "  doRepresent(IgRepresentable["
	<< typeid(*object).name () << "],IgModel["
	<< typeid(*model).name () << "])\n");
    return 0;
}

MMM_DEFUN_FUNC(void,IgBrowserMethods::,
	       doCommit,(IgRep *rep, IgRepresentable *object, unsigned))
{
    LOG(0, error, LFbrowser, "  doCommit(IgRep[" << typeid(*rep).name ()
	<< "],IgRepresentable[" << typeid(*object).name () << "]):\n"
	"  This is not a valid combination of arguments.  Please do not\n"
	"  allow browsers to edit objects when the changes cannot be\n"
	"  committed back to the objects!\n");
    // FIXME: throw an exception?
    abort ();
}

MMM_DEFUN_FUNC(void,IgBrowserMethods::,doUpdate,
	       (IgRepresentable *object, IgRep *rep, unsigned mask))
{
    LOG(0, trace, LFbrowser, "  doUpdate(IgRepresentable["
	<< typeid(*object).name () << "],IgRep["
	<< typeid(*rep).name () << "]," << mask << ")\n");
}

MMM_DEFUN_FUNC(void,IgBrowserMethods::,doExpand,
	       (IgRepresentable *object, IgRep *rep))
{
    LOG(0, trace, LFbrowser, "  doExpand(IgRepresentable["
	<< typeid(*object).name () << "],IgRep["
	<< typeid(*rep).name () << "])\n");
}

MMM_DEFUN_FUNC(void,IgBrowserMethods::,doInvalidate,
	       (IgRepresentable *object, IgModel *model, unsigned mask))
{
    LOG(0, trace, LFbrowser, "  doInvalidate(IgRepresentable["
	<< typeid(*object).name () << "],IgModel["
	<< (model ? "(none)" : typeid(*model).name ())
	<< "]," << mask << ")\n");

    if (! model)
	IgRepSet::update (object, mask);
    else if (IgRepSet::lookup (object, model))
	IgRepSet::update (object, model, mask);
    else
	IgRepSet::lookup (object, model, true);
}

//////////////////////////////////////////////////////////////////////
std::string
IgBrowserMethods::key (const char *model, const char *object)
{
    return std::string ("Runtime/Reps/") + model + "/" + object;
}

// FIXME: This needs to be outside functions due to mutually incompatible
// bugs in GCC 3.1 and Solaris CC 5.3.  GCC can't grok the first form in
// a function-local scope but does accept a "using namespace std::rel_ops;".
// However, Solaris CC doesn't find the operator with the latter.
#if HAVE_NAMESPACE_STD_REL_OPS
    using std::rel_ops::operator!=;
#elif HAVE_REL_OPS_IN_STD
    using std::operator!=;
#endif

void
IgBrowserMethods::preload (const lat::XTypeInfo *objtype,
			   const lat::XTypeInfo *modtype)
{
#if HAVE_NAMESPACE_STD_REL_OPS
    // FIXME (see above): using namespace std::rel_ops;
#elif HAVE_REL_OPS_IN_STD
    // FIXME (see above): using std::operator!=;
#endif

    typedef std::pair<const lat::XTypeInfo *, const lat::XTypeInfo *> MarkerPair;
    static std::set<MarkerPair>	done;
    MarkerPair			marker (objtype, modtype);

    // Load rep/model modules if we haven't seen this combination yet.
    // We use capabilities with a simple naming convention to mark the
    // plug-ins that have the appropriate mappings.
    if (done.find (marker) == done.end ())
    {
	LOG (0, trace, LFbrowser, "loading reps for "
	     << objtype->standard ().name () << " -> "
	     << modtype->standard ().name () << '\n');

	// Load the capability
	IgCapabilityDB::get ()->load (key (modtype->standard ().name (),
					   objtype->standard ().name ()));

	// Walk all base class combinations too
	lat::XTypeInfo::BaseIterator	objbase = objtype->beginBases ();
	lat::XTypeInfo::BaseIterator	objlast = objtype->endBases ();
	lat::XTypeInfo::BaseIterator	modbase = modtype->beginBases ();
	lat::XTypeInfo::BaseIterator	modlast = modtype->endBases ();

	for ( ; modbase != modlast; ++modbase)
	    if (modbase->distance () == 1)
		preload (objtype, modbase->base ());

	for ( ; objbase != objlast; ++objbase)
	    if (objbase->distance () == 1)
	    {
		preload (objbase->base (), modtype);
		modbase = modtype->beginBases ();
		for ( ; modbase != modlast; ++modbase)
		    if (modbase->distance () == 1)
			preload (objbase->base (), modbase->base ());
	    }

	// Mark the combination loaded
	done.insert (marker);
    }
}

IgRepContext *
IgBrowserMethods::represent (IgRepresentable *object, IgModel *model)
{
    preload (xtypeid (*object), xtypeid (*model));
    return doRepresent (object, model);
}

void
IgBrowserMethods::commit (IgRep *rep, IgRepresentable *object, unsigned field)
{
    preload (xtypeid (*object), xtypeid (*rep->model ()));
    doCommit (rep, object, field);
}

void
IgBrowserMethods::update (IgRepresentable *object, IgRep *rep, unsigned field)
{
    preload (xtypeid (*object), xtypeid (*rep->model ()));
    doUpdate (object, rep, field);
}

void
IgBrowserMethods::expand (IgRepresentable *parent, IgRep *rep)
{
    preload (xtypeid (*parent), xtypeid (*rep->model ()));
    doExpand (parent, rep);
}

void
IgBrowserMethods::invalidate (IgRepresentable *object, IgModel *model, unsigned field)
{ doInvalidate (object, model, field); }
