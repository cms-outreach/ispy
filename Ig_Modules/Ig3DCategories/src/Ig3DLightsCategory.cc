//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DCategories/interface/Ig3DLightsCategory.h"
#include "Ig_Modules/Ig3DBaseBrowser/interface/Ig3DBaseBrowser.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppContextService.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DFloatControl.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DAngleControl.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DColorControl.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DVec3fControl.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DUnitVec3fControl.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DBoolControl.h"
#include "Ig_Modules/Ig3DBaseModel/interface/Ig3DBaseModel.h"
#include "Ig_Modules/Ig3DBaseModel/interface/Ig3DBaseRep.h"
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoPointLight.h>
#include <Inventor/nodes/SoSpotLight.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/manips/SoDirectionalLightManip.h>
#include <Inventor/manips/SoPointLightManip.h>
#include <Inventor/manips/SoSpotLightManip.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/elements/SoGLLightIdElement.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qstring.h>
#include <qcheckbox.h>
#include <qinputdialog.h>
#include <qwhatsthis.h>
#include <qmessagebox.h>
#include <classlib/utils/DebugAids.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (Ig3DLightsCategory, "/Control Centre/Categories/Lights");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

Ig3DLightsCategory::Ig3DLightsCategory (IgState *state)
    : Ig3DObjectCategory<SoLight> (state, "Lights"),
      m_source (0),
      m_lightsType  (0),
      m_location    (new Ig3DVec3fControl     (this, "Location")),
      m_direction   (new Ig3DUnitVec3fControl (this, "Direction")),
      m_dropOffRate (new Ig3DFloatControl     (this, "Drop-off Rate",
					       IgQtRangeControl::StepperPart
					       | IgQtRangeControl::ValuePart
					       | IgQtRangeControl::SliderPart)),
      m_cutOffAngle (new Ig3DAngleControl     (this, "Cut-off Angle",
					       IgQtRangeControl::StepperPart
					       | IgQtRangeControl::ValuePart
					       | IgQtRangeControl::SliderPart)),
      m_intensity   (new Ig3DFloatControl     (this, "Intensity",
					       IgQtRangeControl::StepperPart
					       | IgQtRangeControl::ValuePart
					       | IgQtRangeControl::SliderPart)),
      m_color       (new Ig3DColorControl     (this, "Color")),
      m_on          (new Ig3DBoolControl      (this, "On")),
      m_manip (0),
      m_nrlights (0)
{
    QWidget	*area = bodyArea ();
    int		row   = bodyLayout ()->numRows ();
    ASSERT (row >= 0);

    // Space it up 
    bottomLayout ()->addItem (new QSpacerItem (20, 20, 
					       QSizePolicy::Minimum, 
					       QSizePolicy::Expanding));

    //Add ComboBox to change the type of selected light
    QHBoxLayout *topRow = new QHBoxLayout (topLayout ());
    topRow->add (m_lightsType = new QComboBox (topArea ()));
    topRow->addItem (new QSpacerItem (20, 20, QSizePolicy::Expanding));
    m_lightsType->insertItem ("Spot Light", SpotLight);
    m_lightsType->insertItem ("Point Light", PointLight);
    m_lightsType->insertItem ("Directional Light", DirectionalLight);
    
    // Add a new row to the grid, value on the left and the label on the right
    m_manip = new QCheckBox ("Manipulator", area);
    bodyLayout ()->addWidget (makeBox (area, m_manip), row, 0);

    m_direction->setMaxAxisAlign (-1.0);
    m_direction->setMinAxisAlign (0.0);
    
    m_intensity->control ()->setRange (0, 1);
    m_intensity->control ()->setSmallStep (0.01);
    m_intensity->control ()->setLongStep (0.1);

    m_dropOffRate->control ()->setRange (0, 1);
    m_dropOffRate->control ()->setSmallStep (0.01);
    m_dropOffRate->control ()->setLongStep (0.1);

    m_cutOffAngle->control ()->setRange (0, 360);
    m_cutOffAngle->control ()->setSmallStep (1);
    m_cutOffAngle->control ()->setLongStep (15);

    QWhatsThis::add (panel (), "3D Light Category");
    QWhatsThis::add (m_location-> xControl ()->widget (),
                     "X-axis of 3D Light location.");
    QWhatsThis::add (m_location-> yControl ()->widget (),
                     "Y-axis of 3D Light location.");
    QWhatsThis::add (m_location-> zControl ()->widget (),
                     "Z-axis of 3D Light location.");
    QWhatsThis::add (m_direction-> xControl ()->widget (),
                     "X-axis of 3D Light direction.");
    QWhatsThis::add (m_direction-> yControl ()->widget (),
                     "Y-axis of 3D Light direction.");
    QWhatsThis::add (m_direction-> zControl ()->widget (),
                     "Z-axis of 3D Light direction.");
    QWhatsThis::add (m_dropOffRate->control ()->widget (),
		     "Drop off Rate of 3D Spot Light");
    QWhatsThis::add (m_cutOffAngle->control ()->widget (),
		     "Cut off Angle of 3D Spot Light");
    QWhatsThis::add (m_intensity->control ()->widget (),
		     "Intensity of 3D Light");
    QWhatsThis::add (m_manip, "Trun on/off the 3D Light manipulator");
    QWhatsThis::add (m_color->widget (), "Color of 3D Light");
    QWhatsThis::add (m_on->widget (), "Turn on/off 3D Light");
    QWhatsThis::add (m_lightsType, "Select type of 3D light");
   
    connect (m_manip, SIGNAL (clicked ()),
             this,    SLOT   (setDirty ()));
    connect (this, SIGNAL (tracking (bool)),
             this, SLOT   (trackingChanged (bool)));
    connect (m_lightsType, SIGNAL (activated (int)),
             this,         SLOT   (lightTypeChanged (int)));
    state->put (s_key, this);
}

Ig3DLightsCategory::~Ig3DLightsCategory (void)
{ 
    state ()->detach (s_key);    
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

SoDirectionalLight *
Ig3DLightsCategory::asDirectionalLight (SoLight *object)
{
    return object->isOfType (SoDirectionalLight::getClassTypeId ())
	? static_cast<SoDirectionalLight *> (object) : 0;
}

SoPointLight *
Ig3DLightsCategory::asPointLight (SoLight *object)
{
    return object->isOfType (SoPointLight::getClassTypeId ())
	? static_cast<SoPointLight *> (object) : 0;
}

SoSpotLight *
Ig3DLightsCategory::asSpotLight (SoLight *object)
{
    return object->isOfType (SoSpotLight::getClassTypeId ())
	? static_cast<SoSpotLight *> (object) : 0;
}

bool
Ig3DLightsCategory::isManipulatorType (SoLight *object)
{
  return (object->isOfType (SoSpotLightManip::getClassTypeId        ()) ||
          object->isOfType (SoPointLightManip::getClassTypeId       ()) ||
	  object->isOfType (SoDirectionalLightManip::getClassTypeId ()));	 
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
Ig3DLightsCategory::attach (void)
{
    // From base light
    int			current = currentItem (); 
    SoLight		*obj = object (current);
    SoDirectionalLight	*d = asDirectionalLight (obj);
    SoPointLight	*p = asPointLight (obj);
    SoSpotLight		*s = asSpotLight (obj);

    m_intensity->setField (&obj->intensity);
    m_color->setField (&obj->color);
    m_on->setField (&obj->on);

    if (d)
    {
	m_direction->setField (&d->direction);
	m_lightsType->setCurrentItem (DirectionalLight);
    }

    else if (p)
    {
	m_location->setField (&p->location);
	m_lightsType->setCurrentItem (PointLight);
    }

    else if (s)
    {
	m_location->setField (&s->location);
	m_direction->setField (&s->direction);
	m_cutOffAngle->setField (&s->cutOffAngle);
	m_dropOffRate->setField (&s->dropOffRate);
	m_lightsType->setCurrentItem (SpotLight);
    }

    m_location->setShown (p || s);
    m_direction->setShown (d || s);
    m_cutOffAngle->setShown (s);
    m_dropOffRate->setShown (s);
    
    m_manip->setEnabled (current > 0);
    removeableItem (current > 0);
    m_lightsType->setEnabled (current > 0);
}

void
Ig3DLightsCategory::detach (void)
{
    m_intensity->setField (0);
    m_color->setField (0);
    m_on->setField (0);
    m_location->setField (0);
    m_direction->setField (0);
    m_cutOffAngle->setField (0);
    m_dropOffRate->setField (0);
}

void
Ig3DLightsCategory::update (void)
{
    m_intensity->update ();
    m_color->update ();
    m_on->update ();

    if (m_location->isShown ())
	m_location->update ();

    if (m_direction->isShown ())
	m_direction->update ();

    if (m_cutOffAngle->isShown ())
	m_cutOffAngle->update ();
    
    if (m_dropOffRate->isShown ())
	m_dropOffRate->update ();
    
    m_manip->setChecked (isManipulatorType (object (currentItem ())));
}

void
Ig3DLightsCategory::apply (void)
{
    m_intensity->apply ();
    m_color->apply ();
    m_on->apply ();

    if (m_location->isShown ())
	m_location->apply ();

    if (m_direction->isShown ())
	m_direction->apply ();

    if (m_cutOffAngle->isShown ())
	m_cutOffAngle->apply ();
    
    if (m_dropOffRate->isShown ())
	m_dropOffRate->apply ();
    
    manipulators (m_manip->isChecked ());
}

//////////////////////////////////////////////////////////////////////
void
Ig3DLightsCategory::manipulators (bool on)
{
    SoLight *obj = object (currentItem ());
    if ((m_source->getHeadlight () != obj) && (isManipulatorType (obj) != on))
      toggleOne (on, obj);
}

void
Ig3DLightsCategory::trackingChanged (bool mode)
{
    if (mode)
	connect (m_manip, SIGNAL (toggled (bool)),
		 this,    SLOT   (manipulators (bool)));
    else
	disconnect (m_manip, SIGNAL (toggled (bool)),
		    this,    SLOT   (manipulators (bool)));      
}

void
Ig3DLightsCategory::lightTypeChanged (int index)
{
    int current = currentItem ();
    if (current != -1)
    {      
	SoLight			*oldLight = object (current);
	SoLight			*newLight = 0;
	SoDirectionalLight	*d        = asDirectionalLight (oldLight);
	SoPointLight		*p        = asPointLight (oldLight);
	SoSpotLight		*s        = asSpotLight (oldLight);
      
	switch (index)
	{
        case SpotLight:
	    if (!s)
	    {
		SoSpotLight *light = m_manip->isChecked () ? 
				     new SoSpotLightManip :
				     new SoSpotLight;
		newLight = light;
		if (p)
		{
		    light->location    = p->location;
		    light->direction.setValue (0, 0, -1);
		}
		else if (d)
		{
		    light->direction   = d->direction;
		    light->location.setValue (0, 0, 0);	     
		}
		else
		    ASSERT (false);
	    }
	    break;      
        case PointLight:
	    if (!p)
	    {
		SoPointLight *light = m_manip->isChecked () ? 
				      new SoPointLightManip :
				      new SoPointLight;
		newLight = light;
		if (s)
		    light->location = s->location;
		else if (d)
		    light->location.setValue (0, 0, 0);
		else
		    ASSERT (false);
	    }
	    break;
        case DirectionalLight:	  
	    if (!d)
	    {
		SoDirectionalLight *light = m_manip->isChecked () ? 
					    new SoDirectionalLightManip :
					    new SoDirectionalLight;
		newLight = light;
		if (s)
		    light->direction = s->direction;
		else if (p)
		    light->direction.setValue (0, 0, -1);
		else
		    ASSERT (false);
	    }
	    break;
        default:
	    ASSERT (false);
	}
	if (newLight)
	{
	    newLight->intensity = oldLight->intensity;
	    newLight->color     = oldLight->color;
	    newLight->on        = oldLight->on;
	    newLight->setName (oldLight->getName ());
	    detach ();
	    SoGroup * group = dynamic_cast<SoGroup *>(
		m_source->model ()->attachPoint ()->findMagic (
		    Ig3DBaseModel::encode ("Default Light Group")));
	    ASSERT (group);
	    group->removeChild (oldLight);
	    group->addChild (newLight);
	    replace (current, newLight);
	    attach ();
	}
    }
}

//////////////////////////////////////////////////////////////////////
void
Ig3DLightsCategory::reconstruct (void)
{
    ASSERT (m_source);
    SoGroup * group = dynamic_cast<SoGroup *>(
	m_source->model ()->attachPoint ()->findMagic (
	    Ig3DBaseModel::encode ("Default Light Group")));
    clear ();
    add (m_source->getHeadlight (), true);
    if (group)
	reconstruct (group);
    else
	buildDefaults ();
}

void
Ig3DLightsCategory::reconstruct (SoGroup *from)
{
    for (int i = 0, n = from->getNumChildren (); i < n; ++i)
    {
	ASSERT (from->getChild (i)->isOfType (SoLight::getClassTypeId ()));
	SoLight *node = static_cast<SoLight *> (from->getChild (i));
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
Ig3DLightsCategory::createNewItem (void)
{ 
    m_nrlights = SoGLLightIdElement::getMaxGLSources ();
    int numLights = items () + 1;
    
    QString status = QString ("Create a new %1th light?\n"
			      "Number of concurrent light sources\n"
			      "in the scene will exceed maximum %2.\n"
			      "Some of the lights will be ignored.\n")
		     .arg (numLights)    // current number of lights
		     .arg (m_nrlights);  // maximun number of lights

    switch ((numLights > m_nrlights) ? (QMessageBox::warning (0, "Ig3DLightsCategory", 
							      status,
							      "&Create", "Cancel",
							      0, 1)) : 0)
    {
    case 0:
	makeOne ();
	setCurrentItem (items () - 1);
	itemChanged (items () - 1);
	break;
    case 1:
	break;
    }
}

void
Ig3DLightsCategory::deleteItem (void)
{ deleteOne (); }

void
Ig3DLightsCategory::renameItem (const QString &newName)
{
    int current	= currentItem ();
    QString oldName (Ig3DBaseModel::decode(object (current)->getName ()).c_str ());
    if (!newName.isEmpty () && newName != oldName)
    {
	IgControlCategory::renameItem (newName);
	rename (current, currentText ());
    }
}

//////////////////////////////////////////////////////////////////////
void
Ig3DLightsCategory::makeOne (void)
{
    bool	ok = false;
    
    QString	base (m_lightsType->currentText () + " #%1");
    QString	name (QInputDialog::getText (tr("Name"), QString::null,
					     QLineEdit::Normal,
					     base.arg (items () + 1),
					     &ok, panel ()));

    if (! ok || name.isEmpty ())
	return;

    int 	index = m_lightsType->currentItem ();
    SoLight	*light = (index == SpotLight)        ? new SoSpotLight        :
                         (index == PointLight)       ? new SoPointLight       :
			 (index == DirectionalLight) ? new SoDirectionalLight :
			 (SoLight*)(0);

    ASSERT (light);
    light->on = true;
    light->setName (Ig3DBaseModel::encode (name.utf8 ().data ()));

    SoGroup * group = dynamic_cast<SoGroup *>(
	m_source->model ()->attachPoint ()->findMagic (
	    Ig3DBaseModel::encode ("Default Light Group")));
    ASSERT (group);
    group->addChild (light);
    add (light, true);
}

void
Ig3DLightsCategory::deleteOne (void)
{
    int		current = currentItem ();

    if (current > 0)
    {
	SoLight	*obj = object (current);
	detach ();
        SoGroup * group = dynamic_cast<SoGroup *>(
	    m_source->model ()->attachPoint ()->findMagic (
		Ig3DBaseModel::encode ("Default Light Group")));
        ASSERT (group);
	group->removeChild (obj);
	remove (current);
    }
}

//////////////////////////////////////////////////////////////////////
void
Ig3DLightsCategory::toggleOne (bool show, SoLight *obj)
{
    // Hide or show the light manipulator: (hide) replace the
    // manipulator with the light; (show) replace the light
    // with manipulator.  

    SoPath		*location;
    SoSearchAction	searcher;

    SoGroup * group = dynamic_cast<SoGroup *>(
	m_source->model ()->attachPoint ()->findMagic (
	    Ig3DBaseModel::encode ("Default Light Group")));
    ASSERT (group);
    searcher.setNode (obj);
    searcher.apply (group);
    location = searcher.getPath ();
    ASSERT (location);

    SoLight *newLight = 0;
    QString name (obj->getName ().getString ());
    if (show && ! isManipulatorType (obj))
    {      
	if (asSpotLight (obj))
	{
	    SoSpotLightManip *replacement = new SoSpotLightManip;
	    newLight = replacement;
	    replacement->replaceNode (location);
	}
	else if (asPointLight (obj))
	{
	    SoPointLightManip *replacement = new SoPointLightManip;
	    newLight = replacement;
	    replacement->replaceNode (location);
	}
	else if (asDirectionalLight (obj))
	{
	    SoDirectionalLightManip *replacement = new SoDirectionalLightManip;
	    newLight = replacement;
	    replacement->replaceNode (location);
	}
	else
	    ASSERT (false);
    }
    else if(! show && isManipulatorType (obj))
    {
	if (asSpotLight (obj))
	{
	    SoSpotLightManip *m
		= static_cast<SoSpotLightManip *> (obj);
	    SoSpotLight *light = new SoSpotLight;
	    newLight = light;
	    m->replaceManip (location, light);
	}
	else if (asPointLight (obj))
	{
	    SoPointLightManip *m
		= static_cast<SoPointLightManip *> (obj);
	    SoPointLight *light = new SoPointLight;
	    newLight = light;
	    m->replaceManip (location, light);
	}
	else if (asDirectionalLight (obj))
	{
	    SoDirectionalLightManip *m
		= static_cast<SoDirectionalLightManip *> (obj);
	    SoDirectionalLight *light = new SoDirectionalLight;
	    newLight = light;	
	    m->replaceManip (location, light);
	}
	else
	    ASSERT (false);
    }
    if (newLight)
    {
	newLight->setName (name.latin1 ());
	detach ();
	replace (obj, newLight);
	attach ();
    }
}

// FIXME: this is a kludge. There should be a way to get a list of
// browsers from the state and to register the correct one.
void
Ig3DLightsCategory::registerBrowser (IgState *state, Ig3DBaseBrowser *browser)
{
    IgQtAppContextService *cs = IgQtAppContextService::get (state);
    ASSERT (cs);    
    cs->addOnFocusIn (lat::CreateCallback (this,
					   &Ig3DLightsCategory::browserChanged,
					   browser));    
    Ig3DObjectCategory<SoLight>::registerBrowser (state, browser);
    browser->getHeadlight ()->setName (Ig3DBaseModel::encode ("Head Light"));
}

void
Ig3DLightsCategory::browserChanged (Ig3DBaseBrowser *browser)
{
    m_source = browser;
    reconstruct ();
}

void
Ig3DLightsCategory::buildDefaults (void)
{
    SoGroup *group = new SoGroup;
    group->setName (Ig3DBaseModel::encode ("Default Light Group"));
    m_source->model ()->attachPoint ()->magic ()->addChild (group);	
}

void
Ig3DLightsCategory::addExtraLights (SoGroup *group)
{   
    SoDirectionalLight *flood = new SoDirectionalLight;
    flood->direction.setValue (-1, 0, -1);
    flood->intensity.setValue (0.8);
    flood->color.setValue (1.0, 1.0, 1.0);
    flood->setName (Ig3DBaseModel::encode ("Top Left Flood"));
    group->addChild (flood);
    add (flood, true);
	
    flood = new SoDirectionalLight;
    flood->direction.setValue (1, 0, -1);
    flood->intensity.setValue (0.8);
    flood->color.setValue (1.0, 1.0, 1.0);
    flood->setName (Ig3DBaseModel::encode ("Top Right Flood"));
    group->addChild (flood);
    add (flood, true);
    
    flood = new SoDirectionalLight;
    flood->direction.setValue (-1, 0, 1);
    flood->intensity.setValue (0.8);
    flood->color.setValue (1.0, 1.0, 1.0);
    flood->setName (Ig3DBaseModel::encode ("Bottom Left Flood"));
    group->addChild (flood);
    add (flood, true);
	
    flood = new SoDirectionalLight;
    flood->direction.setValue (1, 0, 1);
    flood->intensity.setValue (0.8);
    flood->color.setValue (1.0, 1.0, 1.0);
    flood->setName (Ig3DBaseModel::encode ("Bottom Right Flood"));
    group->addChild (flood);
    add (flood, true);
	
    SoPointLight *point = new SoPointLight;
    point->location.setValue (0.0, 0.0, 0.0);
    point->intensity.setValue (0.8);
    point->color.setValue (1.0, 1.0, 1.0);
    point->setName (Ig3DBaseModel::encode ("Point"));
    group->addChild (point);
    add (point, true);
}
