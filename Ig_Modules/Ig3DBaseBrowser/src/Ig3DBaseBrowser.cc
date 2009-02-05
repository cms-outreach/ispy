#include "Ig_Modules/Ig3DBaseBrowser/interface/Ig3DBaseBrowser.h"
#include "Ig_Modules/Ig3DBaseBrowser/interface/xtypeinfo.h"
#include "Ig_Modules/Ig3DBaseModel/interface/Ig3DBaseModel.h"
#include "Ig_Modules/Ig3DBaseModel/interface/Ig3DBaseModelEvent.h"
#include "Ig_Modules/Ig3DBaseModel/interface/Ig3DBaseRep.h"
#include "Ig_Modules/Ig3DSystem/interface/Ig3DSystem.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtSite.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtObjectMenuService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtObjectMenuMessage.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtObjectMenu.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoGrid.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoGL2PSAction.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepSet.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepContext.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgSelectionService.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgSelectionMessage.h"
#include "Ig_Framework/IgObjectBrowser/interface/xtypeinfo.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgExtensionDB.h"
#include <climits>
#include <qhbox.h>
#include <qwidget.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qfiledialog.h>
#include <qcolordialog.h>
#include <qinputdialog.h>
#include <qbuttongroup.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qvbox.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <qdir.h>
#include <qfileinfo.h>
#include <qcursor.h>
#include <Inventor/Qt/SoQtCursor.h>
#include <Inventor/SoPath.h>
#include <Inventor/SbLinear.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoClipPlane.h>
#include <Inventor/actions/SoLineHighlightRenderAction.h>
#include <Inventor/actions/SoBoxHighlightRenderAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/Qt/SoQt.h>
#include <gl2ps.h>

#undef emit /* FIXME: disgusting hack: qt defines `emit' as a macro */
#include <classlib/utils/Callback.h>
#include <classlib/utils/DebugAids.h>
#include <classlib/iobase/Filename.h>
#include <classlib/utils/Log.h>
#include <typeinfo>

// Button index values.
enum {
    INTERACT_BUTTON = 0,
    EXAMINE_BUTTON,
    HOME_BUTTON,
    SET_HOME_BUTTON,
    VIEW_ALL_BUTTON,
    SEEK_BUTTON,
    CAMERA_BUTTON
};

lat::logflag LF3dbrowser = { 0, "3dbrowser", true, -1 };

// NB: We create an intermediate widget for geometry management due
// SoQt's resizing logic.  It seems that SoQt doesn't actually react
// to resize events, but depends on an event filter to spy the parent
// widget resize events.  This means that as a child of a splitter (or
// anything else that subdivides its own area) things will go awfully
// wrong.  Hence we give an intermediate QHBox for layout management
// (a QWidget won't do).

QWidget *
Ig3DBaseBrowser::initialise (IgState *state, IgSite *site)
{
    // Ig3DBaseBrowser inherits SoQtExaminerViewer.  However, we need to
    // initialise the 3D subsystem properly before creating the
    // viewer.  Thus we need to run a special between the entering of
    // the Ig3DBaseBrowser constructor and before running the base class
    // (SoQtExaminerViewer) constructor.  This function is that magic
    // initialiser hook.  The return value is the geometry-managing
    // parent widget for the examiner window (see the comment above
    // for the details).

    // Initialise SoQt if it hasn't been done yet.
    Ig3DSystem::init (state, IgQtSite::selfFrom (site)->topLevelWidget ());

    // Return the magic intermediate widget parent to manage geometry.
    QWidget *w = new QWidget (IgQtSite::hostFrom (site));
    (new QVBoxLayout (w))->setAutoAdd (true);
    QSize size = IgQtSite::hostFrom (site)->size();
    if (size.width () > 1)
      w->resize (size);
    return w;
}

// `init' allows us to use SoQt widgets and Inventor so initialise the
// base class and the data members.
Ig3DBaseBrowser::Ig3DBaseBrowser (IgState *state, 
				  IgSite *site, 
				  Ig3DBaseModel *model)
    : SoQtExaminerViewer (initialise (state, site), "3D Browser",
			  true, BUILD_ALL, BROWSER, false),
      m_state (new IgState (state)),
      m_model (model),
      m_selection (0),
      m_menuRep (0), 
      m_first_time (true),
      m_grid (false),
      m_gl2psOptions (GL2PS_SIMPLE_LINE_OFFSET 
	              | GL2PS_BEST_ROOT
		      | GL2PS_OCCLUSION_CULL),
      m_currentPick (0),
      m_whatsThisPicking (false),
      m_oldView (true),
      m_oldSeek (false),
      m_gl2psFBBufferSize (1024*1024),
      m_farDistanceSensor (0),
      m_nearDistanceSensor (0)
{
    initWidget (site);
    //FIXME: remove when SoQt fixes the continious rendering problem
    //create sersors to sence the near and for clip plane distance and
    //force them to be 0.1 and SHRT_MAX
    SoCamera * const camera = SoQtExaminerViewer::getCamera ();

    ASSERT (camera);
    camera->farDistance  = SHRT_MAX;
    camera->nearDistance  = 0.1;
    m_farDistanceSensor = new SoFieldSensor (&farDistanceSensorCB, this);
    m_farDistanceSensor->attach (&camera->farDistance);
    m_nearDistanceSensor = new SoFieldSensor (&nearDistanceSensorCB, this);
    m_nearDistanceSensor->attach (&camera->nearDistance);

    SoNode *node = model->attachPoint ()->findMagic (
                         Ig3DBaseModel::encode ("Default Grid Group"));
    if (node && dynamic_cast<SoGroup*>(node)->getNumChildren ())
        m_grid = true;
}

SbBool
Ig3DBaseBrowser::eventCallback (void *closure, QEvent *event)
{
    Ig3DBaseBrowser *self = static_cast<Ig3DBaseBrowser *> (closure);
    if (!self->getParentWidget()->hasMouse () ||
        !self->isViewing())
      return false;

    if (QWheelEvent *wheelEvent = dynamic_cast<QWheelEvent *> (event))
    {
	self->zoom (0.1 * (wheelEvent->delta () > 0 ? 1:-1));
	return true;
    }
    else if (QMouseEvent *mEvent = dynamic_cast<QMouseEvent *> (event))
    {
	if (mEvent->button () == Qt::LeftButton
	    &&  mEvent->type () == QEvent::MouseButtonRelease
	    && self->isSeekMode())
	{
	  self->setComponentCursor (SoQtCursor::getRotateCursor ());
	  return true;
	}
	return false;
    }
    return false;
}

void
Ig3DBaseBrowser::initWidget (IgSite *site)
{
    ASSERT (m_model);
    // Host in the site and decorate the viewer with more buttons.
    IgQtSite::host (site, getParentWidget ());
    setBaseWidget (buildWidget (getParentWidget ()));

    // FIXME: from cmscan
    // setBottomWheelString ("Rot Z");
    // parent->setIcon (QPixmap (cmsLogo_xpm));

    // Hook on the selection service
    ASSERT (IgSelectionService::get (m_state));
    IgSelectionService::get (m_state)
	->add (lat::CreateCallback (this, &Ig3DBaseBrowser::selectMessage));
    m_model->listen (Ig3DBaseModel::ModelChanged,
		     lat::CreateCallback (this, &Ig3DBaseBrowser::modelChanged));

    // Make the model the scene graph.  Note that the model always has
    // a hidden root whether or not it contains anything.
    SoGroup *graph = m_model->sceneGraph ();
    ASSERT (graph);
    setSceneGraph (graph);

    // Put selection hooks on the scene graph.  The know that the
    // model's hidden root is always a SoSelection.
    ASSERT (graph->isOfType (SoSelection::getClassTypeId ()));
    SoSelection *top = static_cast<SoSelection *> (graph);
    top->policy.setValue (SoSelection::SINGLE);

    redrawOnSelectionChange (top);
    setGLRenderAction (new SoLineHighlightRenderAction);
    setEventCallback (eventCallback, this);

    // Make myself visible: get SoQt to do some preparations.
    show ();
}

void
Ig3DBaseBrowser::focusIn (void)
{
    // Make the model the scene graph.  Note that the model always has
    // a hidden root whether or not it contains anything.
    SoSelection *top = static_cast<SoSelection *> (m_model->sceneGraph ());
    top->setPickFilterCallback (onPick, this);
    top->addSelectionCallback (onSelect, this);
    top->addDeselectionCallback (onDeselect, this);
}

void
Ig3DBaseBrowser::focusOut (void)
{
    SoSelection *top = static_cast<SoSelection *> (m_model->sceneGraph ());
    top->removeSelectionCallback (onSelect, this);
    top->removeDeselectionCallback (onDeselect, this);
}

/*!
  Overloaded from parent class to add the tool tips
*/
void
Ig3DBaseBrowser::createViewerButtons (QWidget* parent, SbPList* buttonlist)
{
    SoQtExaminerViewer::createViewerButtons (parent, buttonlist);

    // Are there any buttons?
    int nrbuttons = buttonlist->getLength();
    if (nrbuttons == 0) return;

    QToolTip::add ((QWidget*) ((*buttonlist ) [INTERACT_BUTTON]), "Select/Pick");
    QWhatsThis::add ((QWidget*) ((*buttonlist) [INTERACT_BUTTON]), 
		     tr ("<b>Select/Pick Button:</b>\n"
			 "Selects object manipulation or pick mode \n"
			 "(and deselects camera or viewer mode).\n"
			 "The cursor shape will change to an arrow.\n"
			 "In this mode, the user is manipulating objects\n"
			 "in the scene graph."));

    QToolTip::add ((QWidget*) ((*buttonlist ) [EXAMINE_BUTTON]), "View");
    QWhatsThis::add ((QWidget*) ((*buttonlist) [EXAMINE_BUTTON]),
		     tr ("<b>View Button:</b>\n"
			 "Selects camera or viewer mode (and deselects object \n"
			 "manipulation or pick mode). The cursor shape will\n"
			 "change to a circular arrows icon. In this mode,\n"
			 "the user is moving the camera in 3D space."));
        
    QToolTip::add ((QWidget*) ((*buttonlist ) [HOME_BUTTON]), "Home");
    QWhatsThis::add ((QWidget*) ((*buttonlist) [HOME_BUTTON]),
		     tr ("<b>Home Button:</b>\n"
			 "Returns the camera to its home position\n"
			 "(initial position if not reset)."));
    
    QToolTip::add ((QWidget*) ((*buttonlist ) [SET_HOME_BUTTON]), "Set Home");
    QWhatsThis::add ((QWidget*) ((*buttonlist) [SET_HOME_BUTTON]),
		     tr ("<b>Set Home Button:</b>\n"
			 "Resets the home position to the current\n"
			 "camera position."));

    QToolTip::add ((QWidget*) ((*buttonlist ) [VIEW_ALL_BUTTON]), "View All");
    QWhatsThis::add ((QWidget*) ((*buttonlist) [VIEW_ALL_BUTTON]),
		     tr ("<b>View All Button:</b>\n"
			 "Brings the entire scene graph into view."));
    
    QToolTip::add ((QWidget*) ((*buttonlist ) [SEEK_BUTTON]), "Seek");
    QWhatsThis::add ((QWidget*) ((*buttonlist) [SEEK_BUTTON]),
		     tr ("<b>Seek Button:</b>\n"
			 "Allows the user to select a new center\n"
			 "of rotation for the camera. When clicked\n"
			 "on (and in viewer mode) the cursor changes\n"
			 "to a crosshair. The next left mouse\n"
			 "buttonpress causes whatever is underneath\n"
			 "the cursor to be selected as the new center\n"
			 "of rotation. Once the button is released,\n"
			 "the camera either jumps or animates to its\n"
			 "new position depending on the current\n"
			 "setting of the seek time in the preferences\n"
			 "dialog box."));
    
    QToolTip::add ((QWidget*) ((*buttonlist ) [CAMERA_BUTTON]), "Projection");
    QWhatsThis::add ((QWidget*) ((*buttonlist) [CAMERA_BUTTON]),
		     tr ("<b>Projection Button:</b>\n"
			 "Selects the type of camera used\n"
			 "by the viewer. It toggles between\n"
			 "the two available camera types -\n"
			 "perspective and orthographic."));
}

QWidget *
Ig3DBaseBrowser::buildLeftTrim (QWidget *parent)
{
    QWidget	*result = new QWidget (parent);
    result->setFixedWidth (0);
    
    return result;
}

QWidget *
Ig3DBaseBrowser::buildBottomTrim (QWidget *parent)
{
    QWidget	*result = new QWidget (parent);
    result->setFixedHeight (0);
    
    return result;
}

QWidget *
Ig3DBaseBrowser::buildRightTrim (QWidget *parent)
{
    QWidget	*result = new QWidget (parent);
    result->setFixedWidth (0);
    
    return result;
}


Ig3DBaseBrowser::~Ig3DBaseBrowser (void)
{
    // FIXME: remove model selection, picking callbacks?
    IgSelectionService::get (m_state)
	->remove (lat::CreateCallback (this, &Ig3DBaseBrowser::selectMessage));
    m_model->unlisten (Ig3DBaseModel::ModelChanged,
		       lat::CreateCallback (this, &Ig3DBaseBrowser::modelChanged));
}

IgState *
Ig3DBaseBrowser::state (void) const
{ return m_state; }

Ig3DBaseModel *
Ig3DBaseBrowser::model (void) const
{ return m_model; }

void
Ig3DBaseBrowser::setAutoClipping (SbBool enable)
{
    SoQtViewer::setAutoClipping (enable);
    if (enable)
    {
	SoCamera *camera = this->getCamera ();
	camera->farDistance.touch ();
	camera->nearDistance.touch ();
    }
}

void
Ig3DBaseBrowser::setFeedbackVisibility (bool enable)
{
    bool old = isFeedbackVisible ();
    if (enable != old)
	SoQtExaminerViewer::setFeedbackVisibility (enable);
}

void
Ig3DBaseBrowser::setGridVisibility (bool enable)
{     
    Ig3DBaseBrowser::drawGrid (enable);
}

bool
Ig3DBaseBrowser::isGridVisible (void)
{
    return m_grid;
}

void
Ig3DBaseBrowser::drawGrid (const bool enable) 
{
    SoGroup *group = 0;
    SoNode *node = model ()->attachPoint ()->findMagic (
			Ig3DBaseModel::encode ("Default Grid Group"));
    if (!node)
    {
	group = new SoGroup;
 	group->setName (Ig3DBaseModel::encode ("Default Grid Group"));
	model ()->attachPoint ()->magic ()->insertChild (group, 0);
    }
    else
    {
	group = dynamic_cast<SoGroup *> (node);
    }
 
    if (! enable)
    {
	if (group->getNumChildren () > 0)
	    group->removeChild (0);
    }
    else if (group->getNumChildren () == 0)
    {
	IgSoGrid *xGrid = new IgSoGrid;
	xGrid->setName (Ig3DBaseModel::encode ("X Grid"));
	group->addChild (xGrid);
    }
    m_grid = enable;
}

void
Ig3DBaseBrowser::zoomIn (void) 
{    
    // There is no particular meaning in this 
    // number 0.5. It just looks like a reasonable step. 
    zoom (-0.5f);
}

void
Ig3DBaseBrowser::zoomOut (void) 
{    
    // There is no particular meaning in this 
    // number 0.5. It just looks like a reasonable step. 
    zoom (0.5f);
}

void
Ig3DBaseBrowser::zoom (const float diffvalue)
{
    SoCamera *cam = this->getCamera ();
    if (cam == NULL) return; // can happen for empty scenegraph
    SoType t = cam->getTypeId ();

    // This will be in the range of <0, ->>.
    float multiplicator = exp (diffvalue);

    if (t.isDerivedFrom (SoOrthographicCamera::getClassTypeId ())) 
    {
        // Since there's no perspective, "zooming" in the original sense
        // of the word won't have any visible effect. So we just increase
        // or decrease the field-of-view values of the camera instead, to
        // "shrink" the projection size of the model / scene.
        SoOrthographicCamera *oc = (SoOrthographicCamera *) cam;
        oc->height = oc->height.getValue () * multiplicator;
    }
    else if (t.isDerivedFrom (SoPerspectiveCamera::getClassTypeId ())) 
    {
        float oldfocaldist = cam->focalDistance.getValue ();
        cam->focalDistance = oldfocaldist * multiplicator;

        SbVec3f direction;
        cam->orientation.getValue ().multVec (SbVec3f (0, 0, -1), direction);
        cam->position = cam->position.getValue () +
			(cam->focalDistance.getValue () - oldfocaldist) * -direction;

    }
    else 
    {
        static SbBool first = TRUE;
        if (first) 
        {
            SoDebugError::postWarning("Ig3DBaseBrowser::zoom",
                                      "unknown camera type, "
                                      "attempts to zoom will have no effect");
            first = FALSE;
        }
    }
}

void
Ig3DBaseBrowser::toggleCameraType (void)
{
    SoQtExaminerViewer::toggleCameraType ();
    SoCamera * const camera = this->getCamera ();
    camera->farDistance  = SHRT_MAX;
    camera->nearDistance  = 0.1;
    m_farDistanceSensor->attach (&camera->farDistance);
    m_nearDistanceSensor->attach (&camera->nearDistance);
    cameraToggled();
}

void
Ig3DBaseBrowser::invertCamera (void)
{
    SoCamera * const camera = this->getCamera ();
    if (!camera) return; // probably a scene-less viewer

    camera->position = camera->position.getValue () * -1.0F;
    camera->orientation.setValue (
          SbRotation (SbVec3f (0.F,-1.F,0.F), M_PI)
	* camera->orientation.getValue ());
}

void
Ig3DBaseBrowser::farDistanceSensorCB (void *me, SoSensor *)
{
    Ig3DBaseBrowser *self = static_cast<Ig3DBaseBrowser *> (me);
    if (self->isAutoClipping ())
    {
	SoCamera * const camera = self->getCamera ();
	self->m_farDistanceSensor->detach ();
	camera->farDistance  = SHRT_MAX;
	self->m_farDistanceSensor->attach (&camera->farDistance);
    }
}

void
Ig3DBaseBrowser::nearDistanceSensorCB (void *me, SoSensor *)
{
    Ig3DBaseBrowser *self = static_cast<Ig3DBaseBrowser *> (me);
    if (self->isAutoClipping ())
    {
	SoCamera * const camera = self->getCamera ();
	self->m_nearDistanceSensor->detach ();
	camera->nearDistance  = 0.1;
	self->m_nearDistanceSensor->attach (&camera->nearDistance);
    }
}

void
Ig3DBaseBrowser::browse (IgRepresentable *object)
{
    // FIXME: can we just depend on model change event?
    IgRep *rep = IgRepSet::lookup (object, m_model, true);

    // FIXME: terrible kludge; need something more robust...
    if (rep && m_first_time)
    {
	m_first_time = false;
	viewAll ();
    }

    // FIXME: select?
}

void
Ig3DBaseBrowser::select (Ig3DBaseRep *rep)
{
    // FIXME: handle multiple selection?
    if (m_selection != rep)
    {
        m_selection = rep;

	LOG(0, trace, LF3dbrowser, "changing selection to " << rep << '\n');
	ASSERT(m_model->sceneGraph()->isOfType(SoSelection::getClassTypeId()));
	SoSelection	*top = static_cast<SoSelection *>
			       (m_model->sceneGraph ());
	SoNode		*node = rep;
	SoPathList	*selected = const_cast<SoPathList *> (top->getList ());
	bool		alreadySelected = false;
	int		path = 0;

	LOG(0, trace, LF3dbrowser, " -- looping over selected items\n");
	// Check if the selected rep corresponds to an unselected node
	while (node && ! alreadySelected && path < selected->getLength ())
	    alreadySelected = (*selected) [path++]->containsNode (node);

	if (! node)
	    top->deselectAll ();
	else if (! alreadySelected)
	{
	    // Find a path from the selection node to the selected node
	    SoSearchAction finder;
	    finder.setNode (node);
	    finder.setFind (SoSearchAction::NODE);
	    finder.apply (top);

	    if (finder.getPath ())
	    {
		LOG(0, trace, LF3dbrowser, " -- selecting object\n");

		// Set the node as selected.  Selection callbacks will
		// fire on exit, which will result in a message on the
		// bus.  `select' ignores the current selection policy
		// so clear that manually.
		LOG(0, trace, LF3dbrowser, " -- firing callback\n");
		top->deselectAll ();
		top->select (finder.getPath ());
	    }
	}
    }
}

Ig3DBaseRep *
Ig3DBaseBrowser::getCurrentPick (void) const
{
    return m_currentPick;
}

Ig3DBaseRep *
Ig3DBaseBrowser::getSelection (void) const
{
    return m_selection;
}

void
Ig3DBaseBrowser::setCurrentPick (Ig3DBaseRep *rep)
{
    // NB: do not fire any notifications; this method is an internal
    // means to change a state without getting into loops.
    m_selection = rep;
    // FIXME: tell the rep (w/detailed path + point information)?
    // FIXME: how can we pass that extra detail in the selection?
}

const SoPickedPoint *
Ig3DBaseBrowser::getCurrentPickPoint (void) const
{
    return m_pick;
}

void
Ig3DBaseBrowser::setCurrentPickPoint (const SoPickedPoint *pick)
{
    // FIXME: validity time?
    m_pick = pick;
}

//////////////////////////////////////////////////////////////////////
SoPath *
Ig3DBaseBrowser::onPick (void *cb, const SoPickedPoint *pick)
{
    // FIXME: pass pick to the 3d rep to allow it to give more detail
    Ig3DBaseBrowser *self = static_cast<Ig3DBaseBrowser *> (cb);
    //if (self->isWhatsThisPicking ()) return 0;

    self->setCurrentPickPoint (pick);

    // Find a matching rep for the selected node
    SoPath	*selection = pick->getPath ();
    const int	length = selection->getLength ();
    int		index = 0;
    Ig3DBaseRep	*match = 0;
    for ( ; index < length; ++index)
    {
	// skip until we find a representation boundary
	SoNode	*node = selection->getNodeFromTail (index);
	LOG(0, trace, LF3dbrowser, "pick node [-" << index << "] = "
	    << node->getName ().getString () << '\n');

	if ((match = Ig3DBaseRep::asRep (node)))
	    break;
    }

    // select if matched and not already selected; otherwise deselect
    LOG(0, trace, LF3dbrowser, "pick --> " << index << " of " << length);
    if (index < length && self->getSelection () != match)
    {
	LOG(0, trace, LF3dbrowser, ": match and different from previous\n");
	return selection->copy (0, length - index);
    }
    else
    {
	LOG(0, trace, LF3dbrowser, ": no match or same as previously\n");
	return 0;
    }
}

void
Ig3DBaseBrowser::onSelect (void *cb, SoPath *selection)
{
    // FIXME: if 3d rep set more detail on pick, use it here
    // FIXME: handle multiple selection?
    ASSERT (selection);
    ASSERT (selection->getLength () > 0);

    LOG(0, trace, LF3dbrowser, "select: " << selection->getLength () << '\n');
    Ig3DBaseBrowser *self = static_cast<Ig3DBaseBrowser *> (cb);
    Ig3DBaseRep *match = Ig3DBaseRep::asRep (selection->getNodeFromTail (0));
    ASSERT (match);

    self->setCurrentPick (match);
    IgSelectionService::get (self->m_state)
	->broadcast (IgSelectionMessage (match->context ()));
}

void
Ig3DBaseBrowser::onDeselect (void *cb, SoPath *selection)
{
    // FIXME: handle multiple selection?
    LOG(0, trace, LF3dbrowser, "deselect: " << selection->getLength () << '\n');
    Ig3DBaseBrowser *self = static_cast<Ig3DBaseBrowser *> (cb);
    Ig3DBaseRep *match = Ig3DBaseRep::asRep (selection->getNodeFromTail (0));
    ASSERT (match);
    self->setCurrentPick (0);
    IgSelectionService::get (self->m_state)
	->broadcast (IgSelectionMessage (0));
}

//////////////////////////////////////////////////////////////////////
void
Ig3DBaseBrowser::selectMessage (IgSelectionMessage message)
{
    if (! message.context ())
	select (0);
    else if (Ig3DBaseRep *rep = dynamic_cast<Ig3DBaseRep *>
	     (IgRepSet::lookup (message.context (), m_model, true)))
	select (rep);
}

//////////////////////////////////////////////////////////////////////
void
Ig3DBaseBrowser::modelChanged (Ig3DBaseModelEvent)
{
    getSceneManager ()->scheduleRedraw ();
}


//////////////////////////////////////////////////////////////////////
void
Ig3DBaseBrowser::save (void)
{ saveNode (m_model->sceneGraph (), "Save Scene As...", getShellWidget ()); }

void
Ig3DBaseBrowser::print (void)
{
    QString	vector2EPS ("Vector EPS [Level 2] (*.eps)");
    QString	vector3EPS ("Vector EPS [Level 3] (*.eps)");
    QString	vectorPDF  ("Portable Document Format (*.pdf)");
    QStringList filters (vector2EPS);
    filters.append (vector3EPS);
    filters.append (vectorPDF);
    
    SoOffscreenRenderer	*renderer =
        new SoOffscreenRenderer (this->getViewportRegion ());
    
    int num =renderer->getNumWriteFiletypes();

    if (num == 0)
    {
        filters.append ("Encapsulated postscript (*.eps)");
        filters.append ("Encapsulated postscript (*.ps)");
	filters.append ("The SGI RGB file format (*.rgb)");
	filters.append ("The SGI RGB file format (*.rgba)");
	filters.append ("The SGI RGB file format (*.bw)");
	filters.append ("The SGI RGB file format (*.inta)");
	filters.append ("The SGI RGB file format (*.int)");
    }
    else
    {
        for (int i=0; i < num; i++)
	{
            SbPList extlist;
	    SbString fullname, description;
            renderer->getWriteFiletypeInfo(i, extlist, fullname, description);
            QString filter (fullname.getString());
	    filter+=" (*.";
	    for (int j=0; j < extlist.getLength(); j++)
	        filters.append (filter+(const char*)extlist[j]+")");
	}
    }
    delete renderer;

    // Pop up file dialog to as for the name.
    QFileDialog	dialog (getShellWidget (), "Print To File", true);
    dialog.setFilters (filters);
    dialog.setMode (QFileDialog::AnyFile);
    bool tryagain = true;
    QString	f;
    while (tryagain)
    {
      if (dialog.exec () != QDialog::Accepted)
	return;

      f = dialog.selectedFile ();
      if (f.isEmpty ())
	return;
      else
      {
        lat::Filename sealf (f.latin1());
	QString dir (sealf.directory().name());
	if (sealf.exists ())
	{
	  LOG(0, trace, LF3dbrowser, QString(f+": File already exists.\n").latin1());
	  int button = QMessageBox::warning (getShellWidget (), "File already exists",
	                                    "File \""+f+"\" already exists.\n"
					    "Do you want to overwrite it?",
					    "Yes", "No");
          if (button == 0)
	  {
	    if (!sealf.isWritable ())
	    {
	      LOG(0, trace, LF3dbrowser, QString(f+": File not write able.\n").latin1());
	      int button = QMessageBox::warning (getShellWidget (), "Access denied",
	                                    "File \""+f+"\" not write able.\n"
					    "Do you want to select some other file?",
					    "Yes", "No");
	      if (button == 1)
	        return;
	    }
	    else
	      tryagain = false;
	  }
	}
	else if (!lat::Filename(dir.latin1()).isWritable ())
	{
	  LOG(0, trace, LF3dbrowser, QString(dir+": Directory not write able.\n").latin1());
	  int button = QMessageBox::warning (getShellWidget (), "Access denied",
	                                    "You do not have permissions to write in \""+dir+"\" directory.\n"
					    "Do you want to select some other file?",
					    "Yes", "No");
          if (button == 1)
	    return;
	}
	else
	  tryagain = false;
      }
    }
    // Pick format settings.
    float	ppi = SoOffscreenRenderer::getScreenPixelsPerInch ();
    //float	dpi = 300.;
    float	dpi = ppi;
    QString	format ("jpg");

    QString sfilter = dialog.selectedFilter ();
    int extIndexStart = sfilter.findRev(".")+1;
    int extIndexEnd = sfilter.findRev(")");
    format = sfilter.mid (extIndexStart, extIndexEnd-extIndexStart);
    
    // Add suffix.
    QString suffix ("." + format);
    if (f.find (suffix, -(format.length()+1)) == -1)
	f += suffix;

    // FIXME: Use a state element to remeber all file operations(save, open)
    QDir::setCurrent (QFileInfo (f).dirPath ());
    
    QApplication::setOverrideCursor (Qt::waitCursor);
    if (sfilter == vector2EPS)
	printVector (f, format, 2);
    else if (sfilter == vector3EPS)
	printVector (f, format, 3);
    else if (sfilter == vectorPDF)
	printVector (f, format, 0);
    else
        printBitmap (f, ppi, dpi, format);
    QApplication::restoreOverrideCursor ();
}

//////////////////////////////////////////////////////////////////////
void
Ig3DBaseBrowser::printBitmap (QString file, float ppi,
			      float dpi, QString format)
{
    // This is mostly like the *Inventor Mentor* example.
    // Initialise off-screen renderer.
    float		r, g, b;
    SbViewportRegion	outvr = this->getViewportRegion ();
    
    SbVec2s		pixels (outvr.getViewportSizePixels ());
    SbVec2s		size ((short) (pixels [0] * dpi / ppi + 0.5), (short) (pixels [1] * dpi / ppi + 0.5));
    SbVec2s 		origin = outvr.getViewportOriginPixels();
    outvr.setViewportPixels (origin, size);

    // Set up a custom GL render action for the offscreen rendered.
    // Do *not* use the one returned by `getGLRenderAction()': doing
    // so leaves the display in a confused state and doesn't produce
    // an output file.  This way we also avoid issues with having to
    // mess and then later reset antialiasing and related parameters.
    SoGLRenderAction	*ra = new SoGLRenderAction (outvr);
    SoOffscreenRenderer	*renderer = new SoOffscreenRenderer (outvr);

    // FIXME? renderer.setComponents (SoOffscreenRenderer::RGB_TRANSPARENCY);
    getSceneManager ()->getBackgroundColor ().getValue (r, g, b);
    renderer->setBackgroundColor (SbColor (r, g, b));
    renderer->setGLRenderAction (ra);
    ra->setTransparencyType (SoGLRenderAction::SORTED_OBJECT_BLEND);
    ra->setSmoothing (TRUE);
    ra->setNumPasses (8);

    // Want to render from above the SceneGraph so we get what the
    // camera sees; SoQtViewer uses the following code.  (FIXME:
    // do we actually want to just render root, or look for camera?)
    SoNode *root = getSceneManager ()->getSceneGraph ();

    SbBool ok = renderer->render (root);
    if (!ok) 
    {
	QMessageBox::information (0, "IGUANA Print Info",
				  tr ("Printing of the %1 format works only\n"
				      "if you run locally installed software\n"
				      "If iguana is running remotely, please,\n"
				      "print as vector Postscript.").arg (file.right (3)));
    }
    else if (!renderer->writeToFile (file.latin1 (), format.latin1 ()))
    {
	LOG(0, trace, LF3dbrowser, 
	    QString(file + ": Failed to open file for writing.\n").latin1());
	QMessageBox::warning (getShellWidget (), "System Error",
	                      "Failed to open file \""+file+"\" for writing.",
			      "Ok");
    }
    delete renderer;
    delete ra;
}

void
Ig3DBaseBrowser::printVector (QString file, QString format, int level)
{
    // Use gl2ps to print vector graphics output.  To be extended to
    // handle other vector graphics formats (SVG, WMF).
    static IgSoGL2PSAction  *gl2psAction = 0;
    if (FILE *output = fopen (file.utf8 (), "wb+"))
    {
	int type = GL2PS_EPS;
	if (format == "pdf")
	  type = GL2PS_PDF;
	else if (format == "eps")
	  type = GL2PS_EPS;
	else
	  ASSERT (0);

	if (! gl2psAction )
          gl2psAction = new IgSoGL2PSAction (this->getViewportRegion ());
        
	gl2psAction->setViewportRegion (this->getViewportRegion ());
	SoGLRenderAction* prevAction = getGLRenderAction ();
	setGLRenderAction (gl2psAction);
	
	int state = GL2PS_OVERFLOW;
	while (state == GL2PS_OVERFLOW)
	{ 
	    int gl2psOptions = GL2PS_SILENT | GL2PS_USE_CURRENT_VIEWPORT
	                       | (level < 3 ? GL2PS_NO_PS3_SHADING : 0)
			       | getGL2PSOptions ();
	    gl2psBeginPage ("IGUANA Scene Graph", "IGUANA", NULL,
			    type, GL2PS_BSP_SORT,
			    gl2psOptions,
			    GL_RGBA, 0, NULL,0, 0, 0,
			    m_gl2psFBBufferSize, output, NULL);
	    actualRedraw ();
	    state = gl2psEndPage();
	    if (state == GL2PS_OVERFLOW)
	        m_gl2psFBBufferSize += 1024*1024;
	}
	fclose (output);
	setGLRenderAction (prevAction);
    }
    else
    {
	LOG(0, trace, LF3dbrowser, QString(file +": Failed to open file for writing.\n").latin1());
        QMessageBox::warning (getShellWidget (), "System Error",
	                      "Failed to open file \""+file+"\" for writing.",
	   		      "Ok");
    }
}

int
Ig3DBaseBrowser::getGL2PSOptions (void)
{ return m_gl2psOptions; }

void
Ig3DBaseBrowser::setGL2PSOptions (int options)
{ m_gl2psOptions = options; }

//////////////////////////////////////////////////////////////////////
void
Ig3DBaseBrowser::repMenu (IgQtObjectMenuMessage message)
{
    static const IgQtObjectMenu::ItemDef defs [] = {
	{ -1, MENU_3D_OPS, 0, 0, -1 },
	{ -1, MENU_SEEKTO, "Seek To", SLOT(repSeekTo()), -1 },
    };
    static const int	ndefs = sizeof (defs)/sizeof (defs [0]);
    IgRepresentable	*object = message.object ();

    m_menuRep = dynamic_cast<Ig3DBaseRep *>
		(IgRepSet::lookup (object, m_model, false));

    message.menu ()->removeFromDefs (defs, ndefs);
    if (m_menuRep)
	// Add our items if they aren't there yet.
	message.menu ()->insertFromDefs (this, defs, ndefs);
	
/*    if (! m_menuRep)
	// An irrelevant object, remove our items.
	message.menu ()->removeFromDefs (defs, ndefs);
    else 
	// Add our items if they aren't there yet.
	message.menu ()->insertFromDefs (this, defs, ndefs);
*/
}

void
Ig3DBaseBrowser::repSeekTo (void)
{
    ASSERT (m_menuRep);

    // Locate the center point of the object
    SoGetBoundingBoxAction	bbaction (getViewportRegion ());
    SoSearchAction		search;
    search.setNode (m_menuRep);
    search.apply (m_model->sceneGraph ());
    ASSERT (search.getPath ());
    bbaction.apply (search.getPath ());

    // Compute various points and directions; find a rotation that puts
    // the current camera direction into a new one
    SoCamera	*camera = getCamera ();
    SbBox3f	bbox = bbaction.getBoundingBox ();
    SbVec3f	hitpoint = bbox.getCenter ();
    SbVec3f	here = camera->position.getValue ();
    SbVec3f	dir = hitpoint - here;
    SbVec3f	olddir;
    float	fd = (bbox.getMin () - bbox.getMax ()).length () / 2;

    dir.normalize ();

    camera->orientation.getValue ().multVec (SbVec3f (0, 0, -1), olddir);
    SbRotation	diffrot (olddir, dir);

    // Move the camera
    camera->focalDistance = fd;
    camera->orientation.setValue (camera->orientation.getValue () * diffrot);
    camera->position.setValue (hitpoint - fd * dir);
}

void
Ig3DBaseBrowser::leftWheelPressed (void)
{ leftWheelStart (); }

void
Ig3DBaseBrowser::leftWheelChanged (float by)
{ leftWheelMotion (by); }

void
Ig3DBaseBrowser::leftWheelReleased (void)
{ leftWheelFinish (); }



Ig3DBaseRep *
Ig3DBaseBrowser::menuRep (void)
{ return m_menuRep; }

void
Ig3DBaseBrowser::resetToHomePosition (void)
{
    SoQtViewer::resetToHomePosition ();    
}

void
Ig3DBaseBrowser::saveHomePosition (void)
{
    SoQtViewer::saveHomePosition ();    
}

void
Ig3DBaseBrowser::viewAll (void)
{
    SoQtViewer::viewAll ();
}

void
Ig3DBaseBrowser::seek (void)
{
    if (SoQtViewer::isSeekMode())
    {
      SoQtViewer::setSeekMode (false);
      setComponentCursor (SoQtCursor::getRotateCursor ());
    }
    else
    {
      SoQtViewer::setSeekMode (true);
      if (isAnimating ()) { stopAnimating (); }
      setComponentCursor(SoQtCursor(SoQtCursor::CROSSHAIR));
    }
}

void
Ig3DBaseBrowser::view (void)
{
    if (isViewing () != true)
    {
	m_whatsThisPicking = false;
	SoQtViewer::setViewing (true);    
	setComponentCursor (SoQtCursor::getRotateCursor ());    
    }
}

void
Ig3DBaseBrowser::pick (void)
{
    if (isViewing () != false)
    {
	SoQtViewer::setViewing (false);    
	SoQtViewer::setSeekMode (false);
	setComponentCursor(SoQtCursor(SoQtCursor::DEFAULT));
    }    
}

void
Ig3DBaseBrowser::autoPrint (void)
{
     QDateTime dt = QDateTime::currentDateTime ();
     QString fName = "screenShot-" + dt.toString ("hh:mm:ss.zzz-dd.MM.yyyy") + ".png";
     QString dName = "screenShot-" + dt.toString ("hh:mm:ss.zzz-dd.MM.yyyy") + ".date";

     SbColor c = getBackgroundColor ();
     SoOffscreenRenderer osr (this->getViewportRegion ());
     osr.setBackgroundColor (c);
     SoNode *root = getSceneManager ()->getSceneGraph ();
     SbBool ok = osr.render (root);
    
     if (!ok) { return; }
     // ok = osr.writeToRGB (fName);
     ok = osr.writeToFile (fName.latin1 (), "png");
     if (!ok) { return; }
  
     dt = QDateTime::currentDateTime ();
     QFile file (dName);
     if  (file.open (IO_WriteOnly)) 
     {
	 QTextStream stream (&file);
	 stream << dt.toString ("ddd MMM d hh:mm:ss.zzz yyyy") << "\n";
	 file.close ();
     }
}

void
Ig3DBaseBrowser::viewPlaneX (void) 
{
    SoCamera * const camera = this->getCamera ();
    if (!camera) return; // probably a scene-less viewer
    
    camera->position = SbVec3f(-1,0,0) * camera->position.getValue().length();
    camera->orientation = SbRotation(SbVec3f(0,1,0),-M_PI/2.f);
}

void
Ig3DBaseBrowser::viewPlaneY (void)
{
    SoCamera * const camera = this->getCamera ();
    if (!camera) return; // probably a scene-less viewer

    SbVec3f norient = SbVec3f(0,-1,0);
    camera->position = -norient * camera->position.getValue().length();
    camera->orientation = SbRotation(SbVec3f(0,0,1),-M_PI/2.f) * 
                          SbRotation(SbVec3f(0,0,-1),norient);
}

void
Ig3DBaseBrowser::viewPlaneZ (void) 
{
    SoCamera * const camera = this->getCamera ();
    if (!camera) return; // probably a scene-less viewer
  
    camera->position = SbVec3f(0,0,1) * camera->position.getValue().length();
    camera->orientation = SbRotation::identity();
}

bool
Ig3DBaseBrowser::saveNode (SoNode *node, const QString& title,
                           QWidget* parent /* = 0 */, const char* file /*= 0*/)
{
    QString	f="";
    bool binaryfile = false;
    if (file==0)
    {
      QFileDialog	dialog (QString::null, QString::null, parent,
	  		        title, true);
      QString	binary ("Binary OIV Files (*.iv)");
      QString	ascii  ("ASCII OIV Files (*.iv)");
      QStringList	filters (ascii);
      filters.append (binary);
      dialog.setFilters (filters);
      dialog.setMode (QFileDialog::AnyFile);
    
      bool tryagain = true;
      while (tryagain)
      {
        if (dialog.exec () != QDialog::Accepted)
	  return false;

        f = dialog.selectedFile ();
        if (f.isEmpty ())
	  return false;
        else
        {
          lat::Filename sealf (f.latin1());
	  if (sealf.exists ())
	  {
	    int button = QMessageBox::warning (parent, "File already exists",
	                                    "File \""+f+"\" already exists.\n"
					    "Do you want to overwrite it?",
					    "Yes", "No");
            if (button == 0)
	    {
	      if (!sealf.isWritable ())
	      {
	        int button = QMessageBox::warning (parent, "Access denied",
	                                    "File \""+f+"\" not write able.\n"
					    "Do you want to select some other file?",
					    "Yes", "No");
	        if (button == 1)
	          return false;
	      }
	      else
	        tryagain = false;
	    }
	  }
	  else if (!lat::Filename(dialog.dirPath ().latin1()).isWritable ())
	  {
	    int button = QMessageBox::warning (parent, "Access denied",
	                                      "You do not have permissions to write in \""+dialog.dirPath ()+"\" directory.\n"
		  			      "Do you want to select some other file?",
					      "Yes", "No");
            if (button == 1)
	      return false;
	  }
	  else
	    tryagain = false;
        }
      }
      if (dialog.selectedFilter () == binary)
        binaryfile = true;
    }
    else
      f = file;

    if (! (f.length () > 3 && f.find (".iv", -3) != -1))
	f += ".iv";

    QDir::setCurrent (QFileInfo (f).dirPath ());
    return writeNode (node, f.utf8 (), binaryfile);
}

bool
Ig3DBaseBrowser::writeNode (SoNode *node, const QString& file, bool binary,
                           QWidget* parent /* = 0 */)
{
    SoOutput out;
    QApplication::setOverrideCursor (Qt::waitCursor);
    bool ret = false;
    if (out.openFile (file.utf8 ()))
    {
	out.setBinary (binary);
	SoWriteAction writer (&out);
	writer.apply (node);
	ret = true;
    }
    else
       QMessageBox::warning (parent, "System Error",
	                               "Failed to open file \""+file+"\" for writing."
				       "Ok");
    QApplication::restoreOverrideCursor ();
    return ret;
}

SoNode*
Ig3DBaseBrowser::openNode (const QString& nodeName, const QString& title,
                       QWidget* parent /* = 0 */, const char* file /*= 0*/)
{
    QString filename = "";
    if (file!=0)
      filename = file;
    else
      filename = 
	QFileDialog::getOpenFileName ("./",
				      "Open Inventor files (*.iv)",
				      0,
				      QString::null,
				      0,
				      0,
				      title);
    
    if (filename != QString::null && !filename.isEmpty())
    {
      SoNode      *node = 0;
      SoInput     file;
      if (! file.openFile (filename.latin1 ()) 
	  || ! SoDB::read (&file, node) 
	  || ! node)
      {
        QMessageBox::warning (parent,
	    	              "File Access Error", 
			      "Can not open file \""+ filename + "\" for reading.", "Ok");
	return 0;
      }
      QDir::setCurrent (QFileInfo (filename).dirPath ());
      
      if (nodeName.isEmpty ()) return node;
      
      node = findGroup (node, nodeName.latin1 ());
      if (!node)
        QMessageBox::warning (parent,
	    	              "Wrong file", 
			      "Can not find node \""+nodeName+"\" in file \""+ filename + "\".", "Ok");
      return node;
   }
   return 0;
}

SoNode*
Ig3DBaseBrowser::findGroup (SoNode *node, const char* name)
{
  if (node->isOfType(SoGroup::getClassTypeId()))
  {
    if (node->getName () == name)
      return node;
    else
    {
      SoGroup * group = dynamic_cast<SoGroup*>(node);
      int count = group->getNumChildren ();
      for (int i = 0; i < count; i++)
      {
        SoNode *n = findGroup (group->getChild(i), name);
	if (n) return n;
      }
    }
  }
  return 0;
}

bool
Ig3DBaseBrowser::isWhatsThisPicking (void)
{ return m_whatsThisPicking; }

void
Ig3DBaseBrowser::setWhatsThisPicking (bool enable /* = true */)
{
    if (m_whatsThisPicking != enable)
    {
        m_whatsThisPicking = enable;
        if (enable)
        {
            m_oldView = isViewing ();
	    m_oldSeek = isSeekMode();
	    pick ();
	    getGLWidget ()->setCursor (QCursor (Qt::WhatsThisCursor));
	}
	else
	{
	    if (m_oldView)
	    {
	      view ();
	      if (m_oldSeek)
	        seek ();
	    }
	    else
	    {
	      pick ();
	      setComponentCursor(SoQtCursor(SoQtCursor::DEFAULT));
	    }
	}
    }
}
