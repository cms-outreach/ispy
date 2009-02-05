//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DCategories/interface/Ig3DClipsCategory.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DPlaneControl.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DVectorControl.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DBoolControl.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DFloatControl.h"
#include "Ig_Modules/Ig3DBaseBrowser/interface/Ig3DBaseBrowser.h"
#include "Ig_Modules/Ig3DBaseModel/interface/Ig3DBaseModel.h"
#include "Ig_Modules/Ig3DBaseModel/interface/Ig3DBaseRep.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppContextService.h"
#include <qinputdialog.h>
#include <qlayout.h>
#include <qstring.h>
#include <qcheckbox.h>
#include <qwhatsthis.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <classlib/utils/DebugAids.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (Ig3DClipsCategory, "/Control Center/Categories/3D Clips");

const char *		Ig3DClipsCategory::s_group	= "Clip Planes";
const char *		Ig3DClipsCategory::s_all	= "All";
const char *		Ig3DClipsCategory::s_active	= "Active";

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

Ig3DClipsCategory::Ig3DClipsCategory (IgState *state,
				      Ig3DBaseBrowser *source)
    : Ig3DObjectCategory<IgSoClipPlane> (state, "Clip Planes"),
      m_source (source),
      m_plane (new Ig3DPlaneControl (this, "Plane")),
      m_on    (new Ig3DBoolControl  (this, "Active")),
      m_manip (new Ig3DBoolControl  (this, "Manipulator"))
{
    QWhatsThis::add (panel (), "3D Clip-Plane Category");
    QWhatsThis::add (m_manip-> widget (), "Trun on/off the 3D Clip Plane's manipulator");
    QWhatsThis::add (m_on->widget (), "Activate/deactivate 3D Clip plane");
    QWhatsThis::add (m_plane->directionControl ()->xControl ()->widget (),
                   "Rotate 3D clip plane around X-axis.");
    QWhatsThis::add (m_plane->directionControl ()->yControl ()->widget (),
                   "Rotate 3D clip plane around Y-axis.");
    QWhatsThis::add (m_plane->directionControl ()->zControl ()->widget (),
                   "Rotate 3D clip plane around Z-axis.");
    QWhatsThis::add (m_plane->distanceControl ()->control ()->widget (),
                   "Distance of 3D Clip plane from origin");

    // Space it up 
    bottomLayout ()->addItem (new QSpacerItem (20, 20, 
        QSizePolicy::Minimum, QSizePolicy::Expanding));

    state->put (s_key, this);	
}

void
Ig3DClipsCategory::attach (void)
{
    IgSoClipPlane *obj = object (currentItem ());
    m_plane->setField (&obj->plane);
    m_on->setField (&obj->on);
    m_manip->setField (&obj->manip);
}

void
Ig3DClipsCategory::detach (void)
{
    m_plane->setField (0);
    m_on->setField (0);
    m_manip->setField (0);
}

void
Ig3DClipsCategory::update (void)
{
    m_plane->update ();
    m_on->update ();
    m_manip->update ();
}

void
Ig3DClipsCategory::apply (void)
{
    m_plane->apply ();
    m_on->apply ();
    m_manip->apply ();
}

//////////////////////////////////////////////////////////////////////
SoSwitch *
Ig3DClipsCategory::repClips (SoGroup *magic, bool make,
		             SoGroup **objects,
		             SoGroup **active)
{
   ASSERT (magic);

    // Find the clip planes switch in `magic'
    SoSwitch	*clips = 0;
    SoNode	*child;
    for (int i = 0; i < magic->getNumChildren () && ! clips; ++i)
    {
	if ((child = magic->getChild (i))->getName ()
	    == SbName (Ig3DBaseModel::encode (s_group)))
	{
	    ASSERT (child->isOfType (SoSwitch::getClassTypeId ()));
	    clips = static_cast<SoSwitch *> (child);
	}
    }
    if (! clips && make)
    {
	SoGroup	*objs = new SoGroup;
	SoGroup *acts = new SoGroup;
	objs->setName (s_all);
	acts->setName (s_active);

	clips = new SoSwitch;
	clips->setName (Ig3DBaseModel::encode (s_group));
	clips->whichChild = 1;
	magic->addChild (clips);
	clips->addChild (objs);
	clips->addChild (acts);
    }
    
    if (clips)
    {
	ASSERT (clips->whichChild.getValue () == 1);
	ASSERT (clips->getChild (0)->isOfType (SoGroup::getClassTypeId ()));
	ASSERT (clips->getChild (1)->isOfType (SoGroup::getClassTypeId ()));
	// ASSERT (active->getNumChildren () <= objects->getNumChildren ());
	if (objects)
	    *objects = static_cast<SoGroup *> (clips->getChild (0));

	if (active)
	    *active = static_cast<SoGroup *> (clips->getChild (1));
    }

    return clips;
}

SoGroup *
Ig3DClipsCategory::repActiveClips (Ig3DBaseRep *rep, bool make)
{
    // Find the active slicers group in the slicer switch in rep's `magic()'
    SoGroup *active = 0;
    repClips (rep->magic (), make, 0, &active);
    return active;
}

SoGroup *
Ig3DClipsCategory::repAllClips (Ig3DBaseRep *rep, bool make)
{
    // Find all slicers group in the slicer switch in rep's `magic()'
    SoGroup *objects = 0;
    repClips (rep->magic (), make, &objects, 0);
    return objects;
}

IgSoClipPlane *
Ig3DClipsCategory::makeClip (QString name, bool on, SbVec3f center)
{
    // Define the clip plane normal as -X axis translated by `center'
    // (i.e. distance by dot product of `center' and -X axis).
    IgSoClipPlane *obj = new IgSoClipPlane;
    obj->setName (Ig3DBaseModel::encode (name.utf8 ().data ()));
    obj->plane = SbPlane (SbVec3f(-1,0,0), SbVec3f (-1,0,0).dot (center));
    obj->on = on;
    obj->manip = false;
    return obj;
}

//////////////////////////////////////////////////////////////////////
void
Ig3DClipsCategory::buildDefaults (void)
{
    QString	name ("Full Clip Plane");
    IgSoClipPlane	*obj  = makeClip (name, false, SbVec3f (0, 0, 0));
    Ig3DBaseRep 	*root = m_source->model ()->attachPoint ();

    repAllClips (root, true)->addChild (obj);
    repActiveClips (root, true)->addChild (obj);
    add (obj, true);
}

void
Ig3DClipsCategory::reconstruct (void)
{
    ASSERT (m_source);
    SoGroup *group = dynamic_cast<SoGroup *>(
	m_source->model ()->attachPoint ()->findMagic (
	   Ig3DBaseModel::encode (s_group)));
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
Ig3DClipsCategory::reconstruct (SoGroup *from)
{
    for (int i = 0, n = from->getNumChildren (); i < n; ++i)
    {
      ASSERT (from->getChild(i)->isOfType(IgSoClipPlane::getClassTypeId()));
      IgSoClipPlane *node = static_cast<IgSoClipPlane *>(from->getChild(i));
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
Ig3DClipsCategory::createNewItem (void)
{ 
    makeOne ();
    setCurrentItem (items ()-1);
    itemChanged (-1);
}

void
Ig3DClipsCategory::deleteItem (void)
{ deleteOne (); }

void
Ig3DClipsCategory::renameItem (const QString &newName)
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
Ig3DClipsCategory::makeOne (void)
{ makeOne (m_source->model ()->attachPoint ());}

void
Ig3DClipsCategory::makeOne (Ig3DBaseRep *rep)
{
    ASSERT (rep);
    ASSERT (rep->node ());
    ASSERT (rep->magic ());

    // Build the base name looking at the number of slicers first by
    // the node's own name, and failing that, the generic name.  Make
    // names unique by counting slicers in that particular rep.
    bool	ok = false;
    SoGroup	*active = repActiveClips (rep, false);
    int		nclips = active ? active->getNumChildren () : 0;
    QString	prefix (!! rep->node ()->getName ()
			? (Ig3DBaseModel::decode (rep->node()->getName()).c_str ())
			: "Clip Plane");
    QString	base (nclips ? (prefix + " #%1").arg (nclips+1) : prefix);
    QString	name (QInputDialog::getText (tr("Name"), QString::null,
					     QLineEdit::Normal,base,&ok, panel ()));

    if (! ok || name.isEmpty ())
	return;

    // OK to construct the clip plane: do so.  Use object's bounding
    // box center to place the clip plane.
    SoGetBoundingBoxAction	bba (m_source->getViewportRegion ());
    SoSearchAction		sa;
    Ig3DBaseRep 		*root = m_source->model ()->attachPoint ();

    sa.setNode (rep);
    sa.apply (root);
    ASSERT (sa.getPath ());
    bba.apply (sa.getPath ());

    IgSoClipPlane *obj = makeClip (name, true, bba.getCenter ());
    repAllClips (root, true)->addChild (obj);
    repActiveClips (rep, true)->addChild (obj);
    add (obj, true);
}

//////////////////////////////////////////////////////////////////////
void
Ig3DClipsCategory::deleteOne (void)
{
    // Search for the object in all reps below the root and remove it.
    int		current  = currentItem ();
    Ig3DBaseRep    *root = m_source->model ()->attachPoint ();
    IgSoClipPlane  *obj  = object (current);

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
Ig3DClipsCategory::registerBrowser (IgState *state, Ig3DBaseBrowser *browser)
{
    IgQtAppContextService *cs = IgQtAppContextService::get (state);
    ASSERT (cs);    
    cs->addOnFocusIn (lat::CreateCallback (this,
					   &Ig3DClipsCategory::browserChanged,
					   browser));    
    IgControlCategory::registerMe (state);
}

//////////////////////////////////////////////////////////////////////
void
Ig3DClipsCategory::browserChanged (Ig3DBaseBrowser *browser)
{
    m_source = browser;
    reconstruct ();
}
