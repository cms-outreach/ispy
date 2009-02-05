//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DCategories/interface/Ig3DViewpointsCategory.h"
#include "Ig_Modules/Ig3DBaseBrowser/interface/Ig3DBaseBrowser.h"
#include "Ig_Modules/Ig3DBaseModel/interface/Ig3DBaseModel.h"
#include "Ig_Modules/Ig3DBaseModel/interface/Ig3DBaseRep.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DCameraRotControl.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DFloatControl.h"
#include "Ig_Modules/Ig3DControls/interface/IgQtAxisAlignment.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppContextService.h"
#include "Ig_Modules/IgControlCentre/interface/IgStudioCentre.h"
#include "Ig_Modules/IgControlCentre/interface/IgControlCentre.h"
#include <Inventor/SoInput.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/actions/SoWriteAction.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qfiledialog.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <qdir.h>
#include <qinputdialog.h>
#include <qwhatsthis.h>
#include <classlib/utils/DebugAids.h>
#include <classlib/iobase/Filename.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (Ig3DViewpointsCategory, "/Control Centre/Categories/Viewpoints");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

// FIXME: Camera should come with the objects or we should have some
// sort of context management -- we could have multiple views each
// with their own viewpoints; otoh, things like viewpoints are kind of
// global information.  Need to sort out how the control centre +
// categories interact with multiple views, they could either manage
// info for all of them, or page in/out whenever view changes.

// FIXME: Make IgSoViewpoint with fields indentical to the camera
// settings.  The make a special "current" viewpoint that simply
// links from the values of the current camera (and vice versa).
// Or will the client add that one?  When we want to apply, just
// call viewpoint->applyToCamera (m_camera).
//
//   add (m_current = new IgSoViewpoint (m_camera, "Current Viewpoint"));

Ig3DViewpointsCategory::Ig3DViewpointsCategory (IgState	*state,
					        Ig3DBaseBrowser *source)
    : Ig3DObjectCategory<IgSoViewpoint> (state, "Viewpoints"),
      m_source		(source),
      m_group		(new SoGroup),
      m_previous	(-1),
      m_orientation	(new Ig3DCameraRotControl (this, "Orientation")),
      m_alignment	(0),
      m_focalDistance	(new Ig3DFloatControl     (this, "Focal Distance", 
                                                   IgQtRangeControl::StepperPart
						   | IgQtRangeControl::ValuePart
						   | IgQtRangeControl::SliderPart)),
      m_aspectRatio	(new Ig3DFloatControl     (this, "Aspect Ratio", 
                                                   IgQtRangeControl::StepperPart
						   | IgQtRangeControl::ValuePart
						   | IgQtRangeControl::SliderPart)),
      m_nearDistance	(new Ig3DFloatControl     (this, "Near Clip Plane", 
                                                   IgQtRangeControl::StepperPart
						   | IgQtRangeControl::ValuePart
						   | IgQtRangeControl::SliderPart)),
      m_farDistance	(new Ig3DFloatControl     (this, "Far Clip Plane", 
                                                   IgQtRangeControl::StepperPart
						   | IgQtRangeControl::ValuePart
						   | IgQtRangeControl::SliderPart)),
      m_zoom		(new Ig3DFloatControl     (this, "Zoom", 
                                                   IgQtRangeControl::StepperPart)),
      m_autoClipping	(0),
      m_autoClippings	(0)
{
    QWidget	*area = bodyArea ();
    int		row   = bodyLayout ()->numRows ();

    ASSERT (m_source);
    
    m_group->ref ();
    m_group->setName (Ig3DBaseModel::encode ("Default View Points"));

    makeOne ("Current Viewpoint");
    
    m_aspectRatio->control ()->setMinValue  (0.01);
    m_aspectRatio->control ()->setLongStep  (0.1);
    m_aspectRatio->control ()->setSmallStep (0.01);
    
    m_nearDistance->control ()->setMinValue  (0.);
    m_nearDistance->control ()->setLongStep  (1.0);
    m_nearDistance->control ()->setSmallStep (0.1);
    
    m_farDistance->control ()->setMinValue  (0.);
    m_farDistance->control ()->setLongStep  (1.0);
    m_farDistance->control ()->setSmallStep (0.1);
    
    m_focalDistance->control ()->setMinValue  (1.0);
    m_focalDistance->control ()->setLongStep  (1.0);
    m_focalDistance->control ()->setSmallStep (0.1);
    
    
    //alignment widget
    QWidget 	*vlab = makeIndentedLabel ("Align Camera",
                                            area);
    bodyLayout ()->addWidget (vlab, row, 0);

    m_alignment = new IgQtAxisAlignment (area);
    m_alignment->setMaxAxisAlign (1.);
    m_alignment->setMinAxisAlign (0.);
    bodyLayout ()->addWidget (makeSpacedBox (area,m_alignment->widget ()),
			      row++, 1);
    
    //Zoom control
    m_zoom->control ()->setLongStep  (0.5);
    m_zoom->control ()->setSmallStep (0.01);
    m_zoom->setValue (0);
    m_zoom->setClean ();
    m_zoom->control ()->
      addRevWidget (makeIndentedLabel  ("In", m_zoom->control ()->widget ()));
    m_zoom->control ()->
      addFwdWidget (makeIndentedLabel ("Out", m_zoom->control ()->widget ()));
        
    //Auto clipping option
    m_autoClipping = new QCheckBox ("Auto Clipping", area);
    bodyLayout ()->addWidget (makeBox (area, m_autoClipping), row++, 0);
    
    QHBoxLayout *layout1 = new QHBoxLayout (bottomLayout ());
    
    layout1->addItem (new QSpacerItem (20, 20, 
        QSizePolicy::Expanding, QSizePolicy::Minimum));
    
    QPushButton *saveme = new QPushButton ("Export viewpoints", area);
    layout1->addWidget (saveme);

    layout1->addItem (new QSpacerItem (20, 20, 
        QSizePolicy::Expanding, QSizePolicy::Minimum));
    
    QPushButton *loadme = new QPushButton ("Import viewpoints", area);
    layout1->addWidget (loadme);
    
    layout1->addItem (new QSpacerItem (20, 20, 
        QSizePolicy::Expanding, QSizePolicy::Minimum));
    
    // Space it up 
    bottomLayout ()->addItem (new QSpacerItem (20, 20, 
        QSizePolicy::Minimum, QSizePolicy::Expanding));

    QWhatsThis::add (panel (), "3D Viewpoint Category");
    QWhatsThis::add (m_focalDistance->control ()->widget (), 
                   "Set distance from the view point to the point of focus");
    QWhatsThis::add (m_farDistance->control ()->widget (), 
                   "Set distance of the camera viewpoint to the far clipping plane");
    QWhatsThis::add (m_nearDistance->control ()->widget (), 
                   "Set distance of the camera viewpoint to the far clipping plane");
    QWhatsThis::add (m_aspectRatio->control ()->widget (), 
                   "Set the ratio of camera viewing width to height.\n"
		   "This must be greater than 0.0");
    QWhatsThis::add (m_zoom->control ()->widget (),
                   "Camera Zoom In/Out");
    QWhatsThis::add (m_autoClipping, "Turn on/off continuous automatic adjustments\n"
                                   "of the near and far clipping planes");
    QWhatsThis::add (vlab, "Align camera to x, y or z axis");
    QWhatsThis::add (m_alignment->widget (), "Align camera to x, y or z axis");

    connect (this,			SIGNAL (editable    (bool)), 
	     m_alignment->widget (),	SLOT   (setEnabled   (bool)));
    connect (this,			SIGNAL (editable    (bool)), 
	     saveme,	                SLOT   (setEnabled   (bool)));
    connect (this,			SIGNAL (editable    (bool)), 
	     loadme,	                SLOT   (setEnabled   (bool)));
    connect (m_alignment,	  	SIGNAL (dirty()),
	     this,			SLOT   (alignToAxis ()));
    connect (this,			SIGNAL (clean()),
	     m_alignment,		SLOT   (setClean ()));

    connect (saveme,		SIGNAL (clicked ()),
             this, 		SLOT   (save ()));
    connect (loadme,		SIGNAL (clicked ()),
             this, 		SLOT   (load ()));
    connect (m_zoom,		SIGNAL (dirty ()),
             this, 		SLOT   (zoom ()));
    connect (this,		SIGNAL (tracking (bool)),
             this, 		SLOT   (updateCameraPosition (bool)));
    connect (m_autoClipping,	SIGNAL (clicked ()),
             this,    		SLOT   (autoClipping ()));

    connect (m_autoClipping, 			 SIGNAL (toggled (bool)),
             m_nearDistance->control()->widget(),SLOT(setDisabled(bool)));
    connect (m_autoClipping, 			 SIGNAL (toggled (bool)),
             m_farDistance->control()->widget(), SLOT(setDisabled(bool)));

    disconnect (m_nearDistance, 		    SIGNAL (editable(bool)),
                m_nearDistance->control()->widget(),SLOT (setEnabled(bool)));
    disconnect (m_farDistance,  		    SIGNAL (editable(bool)),
                m_farDistance->control()->widget(), SLOT (setEnabled(bool)));
                
    state->put (s_key, this);    
}

Ig3DViewpointsCategory::~Ig3DViewpointsCategory (void)
{ 
    state ()->detach (s_key);    
    m_group->unref (); 
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
Ig3DViewpointsCategory::update (void)
{
    m_aspectRatio->update ();
    m_nearDistance->update ();
    m_farDistance->update ();
    m_focalDistance->update ();
    m_orientation->update ();
}

void
Ig3DViewpointsCategory::apply (void)
{
    m_aspectRatio->apply ();
    m_nearDistance->apply ();
    m_farDistance->apply ();
    m_focalDistance->apply ();
    updateCameraPosition ();
    m_orientation->apply ();
}

void
Ig3DViewpointsCategory::attach (void)
{
    int		  current = currentItem ();    
    IgSoViewpoint *obj    = object (current);

    if (current > 0)
    {
      if (m_source->isAnimating())
        m_source->stopAnimating ();
      connectCameraFromViewpoint (obj);
      connectViewpointFromCamera (obj);
    }
    else
    {
      connectViewpointFromCamera (obj);
      connectCameraFromViewpoint (obj);
      m_autoClippings [current] = m_source->isAutoClipping ();
    }

    m_orientation->setField (&obj->orientation);
    m_orientation->setCamera (&obj->orientation,
	  	    	      &obj->position,
			      &obj->focalDistance);
    m_focalDistance->setField (&obj->focalDistance);
    m_aspectRatio->setField (&obj->aspectRatio);
    m_nearDistance->setField (&obj->nearDistance);
    m_farDistance->setField (&obj->farDistance);    
    m_autoClipping->setChecked (m_autoClippings [current]);
    autoClipping ();
    
    removeableItem (current > 0);
}

void
Ig3DViewpointsCategory::detach (void)
{
    if (m_previous == -1)
      return;
    
    disconnectCamera ();
    disconnectViewpoint (object (m_previous));

    m_orientation->setField (0);
    m_orientation->setCamera (0, 0, 0);
    m_focalDistance->setField (0);
    m_aspectRatio->setField (0);
    m_nearDistance->setField (0);
    m_farDistance->setField (0);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
Ig3DViewpointsCategory::zoom (void)
{
    float value = m_zoom->value ();

    //move the Zoom value back to 0
    disconnect(m_zoom, SIGNAL (dirty ()), this, SLOT (zoom ()));
    m_zoom->setValue (0.);
    m_zoom->setClean ();
    connect(m_zoom, SIGNAL (dirty ()),this, SLOT (zoom ()));
    
    if (! isTracking () || currentItem () == -1)
	return;
    m_source->zoom (value);
}

void
Ig3DViewpointsCategory::autoClipping ()
{
    bool clipping = m_autoClipping->isChecked ();
    m_autoClippings [currentItem ()] = clipping;
    if (m_source->isAutoClipping () != clipping)
      m_source->setAutoClipping (clipping);
}

void
Ig3DViewpointsCategory::alignToAxis (void)
{
    m_alignment->setClean ();
    if (isTracking () && currentItem () > -1)
    {
      if (m_alignment->xAlign ())
        m_source->viewPlaneX ();
      else if (m_alignment->yAlign ())
        m_source->viewPlaneY ();
      else
        m_source->viewPlaneZ ();
    }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
Ig3DViewpointsCategory::show ()
{
    setCurrentItem (m_previous = 0);    
    IgControlCategory::show ();
}

void
Ig3DViewpointsCategory::focusIn (void)
{
    setCurrentItem (m_previous = 0);
    itemChanged (m_previous);
}

void
Ig3DViewpointsCategory::focusOut (void)
{
    detach ();
    m_previous = -1;
    IgControlCategory::focusOut ();
}

void
Ig3DViewpointsCategory::itemChanged (int index)
{
    IgControlCategory::itemChanged (index);
    m_previous = index;
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
Ig3DViewpointsCategory::newCamera (void)
{
    if (panel ()->isVisible ())
    {
	IgSoViewpoint *obj = object (currentItem ());
	connectViewpointFromCamera (obj);
	connectCameraFromViewpoint (obj);
    }
}

void
Ig3DViewpointsCategory::connectViewpointFromCamera (IgSoViewpoint *obj)
{
    setViewpoint (m_source, obj);
    SoCamera *camera = m_source->getCamera ();
    obj->orientation.connectFrom (&camera->orientation);
    obj->position.connectFrom (&camera->position);
    obj->focalDistance.connectFrom (&camera->focalDistance);
    obj->aspectRatio.connectFrom (&camera->aspectRatio);
    obj->nearDistance.connectFrom (&camera->nearDistance);
    obj->farDistance.connectFrom (&camera->farDistance);
    int cameraType = obj->cameraType.getValue ();
    if (cameraType == IgSoViewpoint::Perspective)
      obj->heightAngle.connectFrom (&((SoPerspectiveCamera *)camera)->heightAngle);
    else if (cameraType == IgSoViewpoint::Orthographic)
      obj->heightAngle.connectFrom (&((SoOrthographicCamera *)camera)->height);
    else
      ASSERT (false);
}

void
Ig3DViewpointsCategory::disconnectViewpoint (IgSoViewpoint *obj)
{
    obj->orientation.disconnect ();
    obj->position.disconnect ();
    obj->focalDistance.disconnect ();
    obj->aspectRatio.disconnect ();
    obj->nearDistance.disconnect ();
    obj->farDistance.disconnect ();
    obj->heightAngle.disconnect ();
}

void
Ig3DViewpointsCategory::connectCameraFromViewpoint (IgSoViewpoint *obj)
{
    setCamera(m_source, obj);
    SoCamera *camera = m_source->getCamera ();
    camera->orientation.connectFrom (&obj->orientation);
    camera->position.connectFrom (&obj->position);
    camera->focalDistance.connectFrom (&obj->focalDistance);
    camera->aspectRatio.connectFrom (&obj->aspectRatio);
    camera->nearDistance.connectFrom (&obj->nearDistance);
    camera->farDistance.connectFrom (&obj->farDistance);
    int cameraType = obj->cameraType.getValue ();
    if (cameraType == IgSoViewpoint::Perspective)
      ((SoPerspectiveCamera *)camera)->
        heightAngle.connectFrom (&obj->heightAngle);
    else if (cameraType == IgSoViewpoint::Orthographic)
      ((SoOrthographicCamera *)camera)->
        height.connectFrom (&obj->heightAngle);
    else
      ASSERT (false);
}

void
Ig3DViewpointsCategory::disconnectCamera (void)
{
    SoCamera *camera = m_source->getCamera ();
    camera->orientation.disconnect ();
    camera->position.disconnect ();
    camera->focalDistance.disconnect ();
    camera->aspectRatio.disconnect ();
    camera->nearDistance.disconnect ();
    camera->farDistance.disconnect ();
    if (camera->isOfType (SoPerspectiveCamera::getClassTypeId ()))
      ((SoPerspectiveCamera *)camera)->heightAngle.disconnect ();
    else if (camera->isOfType (SoOrthographicCamera::getClassTypeId ()))
      ((SoOrthographicCamera *)camera)->height.disconnect ();
    else
      ASSERT (false);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
Ig3DViewpointsCategory::reconstruct (SoGroup *from)
{
    for (int i = 0, n = from->getNumChildren (); i < n; ++i)
    {
      ASSERT (from->getChild(i)->isOfType(IgSoViewpoint::getClassTypeId()));
      IgSoViewpoint *node = static_cast<IgSoViewpoint *>(from->getChild(i));
      bool already_exists = false;
      for (unsigned int j = 0; j < objects () && !already_exists; j++)
      {
	if (node->getName () == object (j)->getName ())
	  already_exists = true;
      }
      if (!already_exists)
      {
        m_group->addChild (node);
        add (node, true);
      }
    }
    update ();
}

//////////////////////////////////////////////////////////////////////
void
Ig3DViewpointsCategory::createNewItem (void)
{ 
    makeOne ();
    //setCurrentItem (items ()-1);
    //itemChanged (items ()-1);
}

void
Ig3DViewpointsCategory::deleteItem (void)
{ deleteOne (); }

void
Ig3DViewpointsCategory::renameItem (const QString &newName)
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
Ig3DViewpointsCategory::makeOne (QString name /* = "" */)
{
    if (name.isEmpty ())
    {
      bool	ok = false;
      QString	base ("Viewpoint #%1");
      name = QInputDialog::getText (tr("Name"), QString::null,
			            QLineEdit::Normal,
			            base.arg (items () + 1),
				    &ok, panel ());

    
      if (! ok || name.isEmpty ())
	return;
    }

    // FIXME: Remember camera type!
    IgSoViewpoint *obj = new IgSoViewpoint;
    obj->setName (Ig3DBaseModel::encode (name.utf8 ().data ()));
    setViewpoint(m_source, obj);

    m_group->addChild (obj);
    add (obj, true);
    m_autoClippings.push_back (m_source->isAutoClipping ());
}

void
Ig3DViewpointsCategory::deleteOne (void)
{
    int		current = currentItem ();

    if (current > 0)
    {
      IgSoViewpoint	*obj    = object (current);
      detach ();
      m_autoClippings.erase (m_autoClippings.begin () + current);
      m_previous = -1 ;
      m_group->removeChild (obj);
      remove (current);
    }
}
//////////////////////////////////////////////////////////////////////
void
Ig3DViewpointsCategory::updateCameraPosition (bool tracking)
{
    if (tracking)
    {
      connect    (m_focalDistance, SIGNAL (dirty ()),
                 this, 		   SLOT   (updateCameraPosition ()));
    }
    else
    {
      disconnect (m_focalDistance,	SIGNAL (dirty ()),
                  this, 		SLOT   (updateCameraPosition ()));
    }
}

void
Ig3DViewpointsCategory::updateCameraPosition (void)
{
    IgSoViewpoint *obj= object (currentItem ());
    obj->position = 
         obj->position.getValue ()  * 
         ( m_focalDistance->value () / obj->position.getValue ().length ());
}

//////////////////////////////////////////////////////////////////////
void
Ig3DViewpointsCategory::registerBrowser (IgState *state, Ig3DBaseBrowser *browser)
{
    m_source = dynamic_cast <Ig3DBaseBrowser *>(browser);
    ASSERT (m_source);
    
    IgQtAppContextService *cs = IgQtAppContextService::get (state);
    ASSERT (cs); 
    cs->addOnFocusIn (lat::CreateCallback (this,
					   &Ig3DViewpointsCategory::browserChanged,
					   m_source));
    
    IgControlCategory::registerMe (state);
    SoGroup *group = dynamic_cast<SoGroup *>(
	m_source->model ()->attachPoint ()->findMagic (
	   Ig3DBaseModel::encode ("Default View Points")));
    if (group)
    {
      reconstruct (group);
      m_source->model ()->attachPoint ()->magic ()->replaceChild (group, m_group);
    }
    else
      m_source->model ()->attachPoint ()->magic ()->addChild (m_group);
}

//////////////////////////////////////////////////////////////////////
void
Ig3DViewpointsCategory::browserChanged (Ig3DBaseBrowser *browser)
{
    detach ();
    m_previous = -1;
    m_source = browser;
}

void
Ig3DViewpointsCategory::save (void)
{
    ASSERT (m_source);
    m_source->saveNode (m_group, "Save Viewpoints As...", panel ());
}

void
Ig3DViewpointsCategory::load (void)
{
    ASSERT (m_source);
    SoNode * node = m_source->openNode (Ig3DBaseModel::encode
                                          ("Default View Points").getString (),
                                        "Select viewpoints to load", panel ());
    if (node) reconstruct (static_cast<SoGroup*>(node));
}

void
Ig3DViewpointsCategory::setViewpoint (Ig3DBaseBrowser *browser, IgSoViewpoint *view)
{
    ASSERT (browser && view);
    SoCamera *camera = browser->getCamera ();
    view->orientation.setValue (camera->orientation.getValue ());   
    view->position.setValue (camera->position.getValue ());
    view->focalDistance.setValue (camera->focalDistance.getValue ());
    view->aspectRatio.setValue (camera->aspectRatio.getValue ());
    view->nearDistance.setValue (camera->nearDistance.getValue ());
    view->farDistance.setValue (camera->farDistance.getValue ());
    if (camera->isOfType (SoPerspectiveCamera::getClassTypeId ()))
    {
      view->heightAngle.setValue (((SoPerspectiveCamera *)camera)->heightAngle.getValue ());
      view->cameraType.setValue(IgSoViewpoint::Perspective);
    }
    else if (camera->isOfType (SoOrthographicCamera::getClassTypeId ()))
    {
      view->heightAngle.setValue (((SoOrthographicCamera *)camera)->height.getValue ());
      view->cameraType.setValue(IgSoViewpoint::Orthographic);
    }
    else
      ASSERT (false);
}

void
Ig3DViewpointsCategory::setCamera (Ig3DBaseBrowser *browser, IgSoViewpoint *view)
{
    ASSERT (browser && view);
    SoCamera *camera = browser->getCamera ();
    
    int cameraType = view->cameraType.getValue ();
    bool toggle = false;
    if ((cameraType == IgSoViewpoint::Perspective) &&
        (!camera->isOfType (SoPerspectiveCamera::getClassTypeId ())))
      toggle = true;
    else if ((cameraType == IgSoViewpoint::Orthographic) &&
             (!camera->isOfType (SoOrthographicCamera::getClassTypeId ())))
      toggle = true;
      
    if (toggle)
    {
      browser->toggleCameraType ();
      camera = browser->getCamera ();
    }
    camera->orientation.connectFrom (&view->orientation);
    camera->position.connectFrom (&view->position);
    camera->focalDistance.connectFrom (&view->focalDistance);
    camera->aspectRatio.connectFrom (&view->aspectRatio);
    camera->nearDistance.connectFrom (&view->nearDistance);
    camera->farDistance.connectFrom (&view->farDistance);
    if (cameraType == IgSoViewpoint::Perspective)
      ((SoPerspectiveCamera *)camera)->
        heightAngle.setValue (view->heightAngle.getValue ());
    else if (cameraType == IgSoViewpoint::Orthographic)
      ((SoOrthographicCamera *)camera)->
        height.setValue (view->heightAngle.getValue ());
    else
      ASSERT(false);
}
