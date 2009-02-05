//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DCategories/interface/Ig3DSlicersCategory.h"
#include "Ig_Modules/Ig3DBaseBrowser/interface/Ig3DBaseBrowser.h"
#include "Ig_Modules/Ig3DBaseModel/interface/Ig3DBaseModel.h"
#include "Ig_Modules/Ig3DBaseModel/interface/Ig3DBaseRep.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DPlaneControl.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DVectorControl.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DFloatControl.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DBoolControl.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppContextService.h"
#include <qinputdialog.h>
#include <qlayout.h>
#include <qstring.h>
#include <qcheckbox.h>
#include <qwhatsthis.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <climits>
#include <classlib/utils/DebugAids.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

const char *		Ig3DSlicersCategory::s_group	= "Z Slicers";
const char *		Ig3DSlicersCategory::s_all	= "All";
const char *		Ig3DSlicersCategory::s_active	= "Active";

IG_DEFINE_STATE_ELEMENT (Ig3DSlicersCategory, "/Control Centre/Categories/Slicers");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

Ig3DSlicersCategory::Ig3DSlicersCategory (IgState *state,
				          Ig3DBaseBrowser *source)
    : Ig3DObjectCategory<IgSoSlicer> (state, "Slicers"),
      m_source (source),
      m_plane     (new Ig3DPlaneControl (this, "Plane")),
      m_thickness (new Ig3DFloatControl (this, "Thickness",
					 IgQtRangeControl::StepperPart
					 | IgQtRangeControl::ValuePart
					 | IgQtRangeControl::SliderPart)),
      m_on        (new Ig3DBoolControl  (this, "Active")),
      m_manip     (new Ig3DBoolControl  (this, "Manipulator"))
{
    // FIXME: set limits to thickness from world bounding box?
    
    QWhatsThis::add (panel (), "3D Slicer Category");
    QWhatsThis::add (m_manip-> widget (), "Trun on/off the 3D Slicer's manipulator");
    QWhatsThis::add (m_on->widget (), "Activate/deactivate 3D Slicer");
    QWhatsThis::add (m_plane->directionControl ()->xControl ()->widget (),
                   "Rorate 3D slicers around X-axis.");
    QWhatsThis::add (m_plane->directionControl ()->yControl ()->widget (),
                   "Rorate 3D slicers around Y-axis.");
    QWhatsThis::add (m_plane->directionControl ()->zControl ()->widget (),
                   "Rorate 3D slicers around Z-axis.");
    QWhatsThis::add (m_plane->distanceControl ()->control ()->widget (),
                   "Distance of 3D Slicer from origin");
    QWhatsThis::add (m_thickness->control ()->widget (),
                   "Thickness of 3D Slicer");

    m_thickness->control()->setRange (0, SHRT_MAX);
    
    // Space it up 
    bottomLayout ()->addItem (new QSpacerItem (20, 20, 
        QSizePolicy::Minimum, QSizePolicy::Expanding));

    state->put (s_key, this);
}

void
Ig3DSlicersCategory::attach (void)
{
    IgSoSlicer *obj = object (currentItem ());
    m_thickness->setField (&obj->thickness);
    m_plane->setField (&obj->plane);
    m_manip->setField (&obj->manip);
    m_on->setField (&obj->on);
}

void
Ig3DSlicersCategory::detach (void)
{
    m_thickness->setField (0);
    m_plane->setField (0);
    m_manip->setField (0);
    m_on->setField (0);
}

void
Ig3DSlicersCategory::update (void)
{
    m_thickness->update ();
    m_plane->update ();
    m_manip->update ();
    m_on->update ();
}

void
Ig3DSlicersCategory::apply (void)
{
    m_thickness->apply ();
    m_plane->apply ();
    m_manip->apply ();
    m_on->apply ();
}
//////////////////////////////////////////////////////////////////////
SoSwitch *
Ig3DSlicersCategory::repSlicers (SoGroup *magic, bool make,
			 SoGroup **objects,
			 SoGroup **active)
{
    ASSERT (magic);

    // Find the slicer switch in `magic'
    SoSwitch	*slicers = 0;
    SoNode	*child;
    for (int i = 0; i < magic->getNumChildren () && ! slicers; ++i)
	if ((child = magic->getChild (i))->getName ()
	    == SbName (Ig3DBaseModel::encode (s_group)))
	{
	    ASSERT (child->isOfType (SoSwitch::getClassTypeId ()));
	    slicers = static_cast<SoSwitch *> (child);
	}

    if (! slicers && make)
    {
	SoGroup	*objs = new SoGroup;
	SoGroup *acts = new SoGroup;
	objs->setName (s_all);
	acts->setName (s_active);

	slicers = new SoSwitch;
	slicers->setName (Ig3DBaseModel::encode (s_group));
	slicers->whichChild = 1;
	magic->addChild (slicers);
	slicers->addChild (objs);
	slicers->addChild (acts);
    }
    
    if (slicers)
    {
	ASSERT (slicers->whichChild.getValue () == 1);
	ASSERT (slicers->getChild (0)->isOfType (SoGroup::getClassTypeId ()));
	ASSERT (slicers->getChild (1)->isOfType (SoGroup::getClassTypeId ()));
	// ASSERT (active->getNumChildren () <= objects->getNumChildren ());
	if (objects)
	    *objects = static_cast<SoGroup *> (slicers->getChild (0));

	if (active)
	    *active = static_cast<SoGroup *> (slicers->getChild (1));
    }

    return slicers;
}

SoGroup *
Ig3DSlicersCategory::repActiveSlicers (Ig3DBaseRep *rep, bool make)
{
    // Find the active slicers group in the slicer switch in rep's `magic()'
    SoGroup *active = 0;
    repSlicers (rep->magic (), make, 0, &active);
    return active;
}

SoGroup *
Ig3DSlicersCategory::repAllSlicers (Ig3DBaseRep *rep, bool make)
{
    // Find all slicers group in the slicer switch in rep's `magic()'
    SoGroup *objs = 0;
    repSlicers (rep->magic (), make, &objs, 0);
    return objs;
}

IgSoSlicer *
Ig3DSlicersCategory::makeSlicer (QString name, bool on, SbVec3f center)
{
    // Define the front clip plane normal as Z axis translated by
    // `center' (i.e. distance by dot product of `center' and Z axis).
    IgSoSlicer *obj = new IgSoSlicer;

    obj->setName (Ig3DBaseModel::encode (name.utf8 ().data ()));
    obj->plane = SbPlane (SbVec3f (0, 0, 1),
				  SbVec3f (0, 0, 1).dot (center));
    obj->manip = false;
    obj->on = on;
    return obj;
}

//////////////////////////////////////////////////////////////////////
void
Ig3DSlicersCategory::buildDefaults (void)
{
    QString	name ("Full Slicer");
    IgSoSlicer	*obj  = makeSlicer (name, false, SbVec3f (0, 0, 0));
    Ig3DBaseRep *root = m_source->model ()->attachPoint ();

    repAllSlicers (root, true)->addChild (obj);
    repActiveSlicers (root, true)->addChild (obj);
    add (obj, true);
}

void
Ig3DSlicersCategory::reconstruct (void)
{
    ASSERT (m_source);
    SoGroup *group = dynamic_cast<SoGroup *>(
	m_source->model ()->attachPoint ()->findMagic (
	   Ig3DBaseModel::encode ("Z Slicers")));
    clear ();
    if (group)
    {
      ASSERT (group->getChild (0)->isOfType(SoGroup::getClassTypeId()));
      reconstruct (static_cast <SoGroup *>(group->getChild (0)));
    }
    else
      buildDefaults ();
}

void
Ig3DSlicersCategory::reconstruct (SoGroup *from)
{
    for (int i = 0, n = from->getNumChildren (); i < n; ++i)
    {
      ASSERT (from->getChild(i)->isOfType(IgSoSlicer::getClassTypeId()));
      IgSoSlicer *node = static_cast<IgSoSlicer *>(from->getChild(i));
      add (node, true);
    }
    detach ();
    if (object (currentItem ()))
    {
        attach ();
        update ();
    }
}
//////////////////////////////////////////////////////////////////////
void
Ig3DSlicersCategory::createNewItem (void)
{ 
    makeOne ();
    setCurrentItem (items ()-1);
    itemChanged (-1);
}

void
Ig3DSlicersCategory::deleteItem (void)
{ deleteOne (); }

void
Ig3DSlicersCategory::renameItem (const QString &newName)
{
    int current		= currentItem ();
    QString oldName (Ig3DBaseModel::decode(object (current)->getName ()).c_str ());
    if (!newName.isEmpty() && newName != oldName)
    {
      IgControlCategory:: renameItem(newName);
      rename (current, currentText ());
    }
}

//////////////////////////////////////////////////////////////////////
void
Ig3DSlicersCategory::makeOne (void)
{ makeOne (m_source->model ()->attachPoint ()); }

void
Ig3DSlicersCategory::makeOne (Ig3DBaseRep *rep)
{
    ASSERT (rep);
    ASSERT (rep->node ());
    ASSERT (rep->magic ());

    // Build the base name looking at the number of slicers first by
    // the node's own name, and failing that, the generic name.  Make
    // names unique by counting slicers in that particular rep.
    bool	ok = false;
    SoGroup	*active = repActiveSlicers (rep, false);
    int		nslicers = active ? active->getNumChildren () : 0;
    QString	prefix (!! rep->node ()->getName ()
			? (Ig3DBaseModel::decode (rep->node()->getName()).c_str ())
			: "Slicer");
    QString	base (nslicers ? (prefix + " #%1").arg (nslicers+1) : prefix);
    QString	name (QInputDialog::getText (tr("Name"), QString::null,
					     QLineEdit::Normal,base,&ok,panel ()));

    if (! ok || name.isEmpty ())
	return;

    // OK to construct the clip plane pair: do so.  Use obj's
    // bounding box center to place the front clip plane.
    SoGetBoundingBoxAction	bba (m_source->getViewportRegion ());
    SoSearchAction		sa;
    Ig3DBaseRep 		*root = m_source->model ()->attachPoint ();

    sa.setNode (rep);
    sa.apply (root);
    ASSERT (sa.getPath ());
    bba.apply (sa.getPath ());

    IgSoSlicer *obj = makeSlicer (name, true, bba.getCenter ());
    repAllSlicers (root, true)->addChild (obj);
    repActiveSlicers (rep, true)->addChild (obj);
    add (obj, true);
}

void
Ig3DSlicersCategory::deleteOne (void)
{
    // Search for the object in all reps below the root and remove it.
    int current = currentItem ();
    IgSoSlicer	*obj  = object (current);
    Ig3DBaseRep *root = m_source->model ()->attachPoint ();

    SoPath		*location;
    SoSearchAction	searcher;

    searcher.setNode (obj);
    searcher.setInterest (SoSearchAction::ALL);
    searcher.setSearchingAll (TRUE);
    searcher.apply (root);

    for (int i = 0; i < searcher.getPaths ().getLength (); ++i)
    {
	VERIFY (location = searcher.getPaths () [i]);
	ASSERT (location->getLength () >= 2);
	SoNode *parent = location->getNode (location->getLength () - 2);
	ASSERT (parent->isOfType (SoGroup::getClassTypeId ()));
	static_cast<SoGroup *> (parent)->removeChild (obj);
    }

    detach();
    remove (current);
}
//////////////////////////////////////////////////////////////////////
void
Ig3DSlicersCategory::registerBrowser (IgState *state, Ig3DBaseBrowser *browser)
{
    IgQtAppContextService *cs = IgQtAppContextService::get (state);
    ASSERT (cs);    
    cs->addOnFocusIn (lat::CreateCallback (this,
					   &Ig3DSlicersCategory::browserChanged,
					   browser));    
    IgControlCategory::registerMe (state);    
}

//////////////////////////////////////////////////////////////////////
void
Ig3DSlicersCategory::browserChanged (Ig3DBaseBrowser *browser)
{
    m_source = browser;
    reconstruct ();
}
