//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoGridPlane.h"
#include "Iguana/Inventor/interface/IgSoGridPlaneMap.h"
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoResetTransform.h>
#include <Inventor/SbString.h>
#include <assert.h>

#ifdef __APPLE__
# include <OpenGL/gl.h>
#else
# include <GL/gl.h>
#endif
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_NODE_SOURCE (IgSoGridPlane);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
IgSoGridPlane::initClass (void)
{
    SO_NODE_INIT_CLASS (IgSoGridPlane, SoGroup, "Group");
}

IgSoGridPlane::IgSoGridPlane (void)
    : m_plane            (new IgSoGridPlaneMap),
      m_shapeHints       (new SoShapeHints),      
      m_planeSensor      (0),
      m_xOriginSensor    (0),
      m_zOriginSensor    (0),
      m_xLenSensor       (0),
      m_zLenSensor       (0),
      m_spacingSensor    (0),
      m_onSensor         (0),
      m_colorSensor      (0),
      m_labelColorSensor (0),
      m_manip            (0),
      m_manipSensor      (0),
      m_mplaneSensor     (0),
	  m_scaleSensor      (0),
	  m_scale            (0),
	  m_gridPlane        (0)
{
	SO_NODE_CONSTRUCTOR (IgSoGridPlane);	
	SO_NODE_ADD_FIELD (plane, (SbPlane (SbVec3f (0.0, 1.0, 0.0), 0.0)));
	SO_NODE_ADD_FIELD (xOrigin, (0.0f));
	SO_NODE_ADD_FIELD (zOrigin, (0.0f));
	SO_NODE_ADD_FIELD (xLen, (7.0f));
	SO_NODE_ADD_FIELD (zLen, (16.0f));
	SO_NODE_ADD_FIELD (spacing, (1000.0f));
	SO_NODE_ADD_FIELD (on, (FALSE));
	SO_NODE_ADD_FIELD (color, (SbVec3f (1,1,1)));
	SO_NODE_ADD_FIELD (labelColor, (SbVec3f (1,1,1)));
	SO_NODE_ADD_FIELD (manip, (FALSE));
	
	// the reason we are setting the caller is that we need a way to get the
	// most recent values for the scale without disturbing the order of the
	// rendered elements, as soon as a value changes which is important to 
	// the scale, we set a flag on the GridPlaneMap which then calls the 
	// updateScale method in this class
	m_plane->setCaller (this);
	// reset previous transformations
	addChild (new SoResetTransform);

	m_gridPlane = new SoGroup;
	m_gridPlane->setName ("Grid_Plane_Composite");
	addChild (m_gridPlane);

	m_shapeHints->ref();
	m_shapeHints->setName ("Shape_Hints");
	m_gridPlane->addChild (m_shapeHints);
	
	m_plane->setName ("GridPlaneMap");
	m_gridPlane->addChild (m_plane);
	addScale ();
	
	attachSensors ();
}

void
IgSoGridPlane::addScale ()
{
	const bool hasChildren = m_gridPlane->getNumChildren () == 2;
	assert (hasChildren && "check the scene graph elements!");
		
	if (hasChildren)
	{
		// using 'ref ()' because we dont want the scale to be deleted,
		// when the grid is turned off
		m_scale = new SoLevelOfDetail;
		m_scale->screenArea.set1Value (0, 5000);
		m_scale->setName ("Grid_Scale");
		m_scale->ref ();
		
		// add different levels of complexity: until now only 2
		SoSeparator* complexLabels = new SoSeparator;
		SoSeparator* noLabels = new SoSeparator;
		m_scale->addChild (complexLabels);
		m_scale->addChild (noLabels);
		
		if (on.getValue ())
		{
			m_gridPlane->addChild (m_scale);
		}
	}
}

void
IgSoGridPlane::updateScale ()
{
	// is called by gridplanemap
	if (on.getValue ())
	{
		assert (m_scale);
		const float stepLength = spacing.getValue () / 1000.0f;
		const float xLength = ceil (xLen.getValue () / stepLength) * stepLength;
		const float zLength = ceil (zLen.getValue () / stepLength) * stepLength;
		
		SoSeparator* labelGroup = static_cast<SoSeparator*> (m_scale->getChild (0));
		labelGroup->removeAllChildren ();
		
		SbVec3f xDir = m_plane->labelSpot[1] - m_plane->labelSpot[0];
		xDir.normalize ();
		SbVec3f zDir = m_plane->labelSpot[2] - m_plane->labelSpot[0];
		zDir.normalize ();
		
		// if the color is any other than white, change it
		const SbColor c = labelColor.getValue ();
		if (c[0] * c[1] * c[2] < 1.0f) 
		{
			SoMaterial* labelColor = new SoMaterial;
			labelColor->diffuseColor.setValue (c[0], c[1], c[2]);
			labelGroup->addChild (labelColor);
		}
		// set 0
		setLabelMove (labelGroup, m_plane->labelSpot[0]);
		setLabelText (labelGroup, 0, true);
		// set lower right
		setLabelMove (labelGroup, m_plane->labelSpot[1] - m_plane->labelSpot[0]);
		setLabelText (labelGroup, xLength, true);
		// set upper left
		setLabelMove (labelGroup, m_plane->labelSpot[2] - m_plane->labelSpot[1]);
		setLabelText (labelGroup, zLength);
		// back to center
		setLabelMove (labelGroup, m_plane->labelSpot[0] - m_plane->labelSpot[2]);
		
		const float tenStepLengths = stepLength * 10;
		SbVec3f step = xDir * tenStepLengths;
		
		float length = xLength;
		float stepSum = tenStepLengths;
		SbVec3f movedSteps (0.f, 0.f, 0.f);
		bool changed = false;
		
		// steps inbetween
		for (unsigned i = 0; i < 2; i++)
		{
			while (stepSum < length)
			{
				setLabelMove (labelGroup, step);
				movedSteps += step;

				setLabelText (labelGroup, stepSum, (i == 0));
				stepSum += tenStepLengths;
			}
			
			// change direction
			if (!changed)
			{
				if (movedSteps.length () > 0) 
				{
					setLabelMove (labelGroup, -movedSteps);
				}
				step = zDir * tenStepLengths;
				stepSum = tenStepLengths;
				length = zLength;
				changed = true;
			}
		}
	}
}

void
IgSoGridPlane::setLabelMove (SoSeparator* labelGroup, SbVec3f step)
{
	SoTranslation* move = new SoTranslation;
	move->translation = step;
	labelGroup->addChild (move);
}

void
IgSoGridPlane::setLabelText (SoSeparator* labelGroup, float position, bool right)
{
	char textLabel[20];
	SoText2* text = new SoText2;
	unsigned nbr = sprintf (textLabel, "%.2f", position);
	textLabel[nbr+1] = textLabel[nbr];
	textLabel[nbr] = 'm';
	text->string = textLabel;
	
	if (right)
	{
		text->justification = SoText2::RIGHT;
	}
	labelGroup->addChild (text);
}

void
IgSoGridPlane::attachSensors ()
{
	m_planeSensor = new SoFieldSensor (&planeSensorCB, this);
	m_xOriginSensor = new SoFieldSensor (&xOriginSensorCB, this);
	m_zOriginSensor = new SoFieldSensor (&zOriginSensorCB, this);
	m_xLenSensor = new SoFieldSensor (&xLenSensorCB, this);
	m_zLenSensor = new SoFieldSensor (&zLenSensorCB, this);
	m_spacingSensor = new SoFieldSensor (&spacingSensorCB, this);
	m_onSensor = new SoFieldSensor (&onSensorCB, this);
	m_colorSensor = new SoFieldSensor (&colorSensorCB, this);
	m_labelColorSensor = new SoFieldSensor (&labelColorSensorCB, this);
	m_manipSensor = new SoFieldSensor (&manipSensorCB, this);
	m_mplaneSensor = new SoFieldSensor (&planeSensorCB, this);
	
	m_planeSensor->attach (&plane);
	m_xOriginSensor->attach (&xOrigin);
	m_zOriginSensor->attach (&zOrigin);
	m_xLenSensor->attach (&xLen);
	m_zLenSensor->attach (&zLen); 
	m_spacingSensor->attach (&spacing);
	m_onSensor->attach (&on);
	m_colorSensor->attach (&color);
	m_labelColorSensor->attach (&labelColor);
	m_manipSensor->attach (&manip);
}

IgSoGridPlane::~IgSoGridPlane (void)
{
	m_shapeHints->unref ();
	m_scale->unref ();
	m_gridPlane->removeAllChildren ();
	removeChild (m_gridPlane);
	
	delete m_onSensor;
	delete m_planeSensor;
	delete m_xOriginSensor;
	delete m_zOriginSensor;
	delete m_xLenSensor;
    delete m_zLenSensor;
    delete m_spacingSensor;
	delete m_colorSensor;
	delete m_labelColorSensor;
	delete m_manipSensor;
}

void
IgSoGridPlane::xOriginSensorCB (void *me, SoSensor *)
{
	IgSoGridPlane* self = static_cast<IgSoGridPlane *> (me);
	
	self->m_plane->xOrigin = self->xOrigin;
	self->m_plane->refreshScale = true;
}

void
IgSoGridPlane::zOriginSensorCB (void *me, SoSensor *)
{
	IgSoGridPlane* self = static_cast<IgSoGridPlane *> (me);
	
	self->m_plane->zOrigin = self->zOrigin;
	self->m_plane->refreshScale = true;
}

void
IgSoGridPlane::xLenSensorCB (void *me, SoSensor *)
{
	IgSoGridPlane* self = static_cast<IgSoGridPlane *> (me);
	
	self->m_plane->xLen = self->xLen;
	self->m_plane->refreshScale = true;
}

void
IgSoGridPlane::zLenSensorCB (void *me, SoSensor *)
{
	IgSoGridPlane* self = static_cast<IgSoGridPlane *> (me);
	
	self->m_plane->zLen = self->zLen;
	self->m_plane->refreshScale = true;
}

void
IgSoGridPlane::spacingSensorCB (void *me, SoSensor *)
{
	IgSoGridPlane* self = static_cast<IgSoGridPlane *> (me);
	
	self->m_plane->spacing = self->spacing;
	self->m_plane->refreshScale = true;
}

void
IgSoGridPlane::colorSensorCB (void *me, SoSensor *)
{
	IgSoGridPlane* self = static_cast<IgSoGridPlane *> (me);
	
	self->m_plane->color = self->color;
}

void
IgSoGridPlane::labelColorSensorCB (void *me, SoSensor *)
{
	IgSoGridPlane* self = static_cast<IgSoGridPlane *> (me);
	
	self->updateScale ();
}

/** This callback is called whenever the `on' field changes value.
   */
void
IgSoGridPlane::onSensorCB (void *me, SoSensor *)
{
	// activate or deactivate the grid plane
	IgSoGridPlane *self = static_cast<IgSoGridPlane *> (me);
	
	const bool foundHints = (self->m_gridPlane->findChild (self->m_shapeHints) != -1);
	const bool foundScale = (self->m_gridPlane->findChild (self->m_scale) != -1);
	
	if (self->on.getValue ())
	{
		if (!foundHints)
		{
			// 'm_scale' is added in 'updateScale ()'
			self->m_gridPlane->addChild (self->m_shapeHints);
		}
		
		if (!foundScale)
		{
			self->m_gridPlane->addChild (self->m_scale);
		}
	}
	else
	{
		if (foundHints && foundScale)
		{
			self->m_gridPlane->removeChild (self->m_shapeHints);
			self->m_gridPlane->removeChild (self->m_scale);
		}
	}
	self->m_plane->on = self->on;
	self->m_plane->refreshScale = true;
}

/** This callback is called whenever the `manip' field is turned on or off 
	value. 
	*/
void
IgSoGridPlane::manipSensorCB (void *me, SoSensor *)
{	// Hide or show the grid plane manipulators
	
	IgSoGridPlane *self = static_cast<IgSoGridPlane *> (me);
	const bool isOn = self->manip.getValue ();
	
	// check if the manipulator is activated
	if (isOn)
	{
		// if the manipulator doesnt exist yet, create it, activate its plane
		// adjust its plane to the current plane orientation, attach it to a 
		// sensor and add it to the scene graph
		if (!self->m_manip)
		{
			self->m_manip = new IgSoPlaneManip;
			self->m_manip->manip = isOn;
			self->m_manip->plane = self->plane;
			self->m_mplaneSensor->attach (&self->m_manip->plane);
			self->m_gridPlane->addChild (self->m_manip);
		}
	}
	else
	{
		// since the manipulator is turned off, we remove it from the 
		// scene graph and set it to zero
		if (self->m_manip)
		{
			self->m_gridPlane->removeChild (self->m_manip);
			self->m_manip = 0;
		}
	}
	self->m_plane->refreshScale = true;
}

/** This callback is called whenever the `plane' or `m_plane->plane' field
	change value.
   */
void
IgSoGridPlane::planeSensorCB (void *me, SoSensor *)
{
	IgSoGridPlane *self = static_cast<IgSoGridPlane *> (me);
	
	// disable the sensors for both planes in order to avoid an infinite 
	// chain of callbacks
	self->m_mplaneSensor->detach ();
	self->m_planeSensor->detach();
	
	// if the manipulator is turned on, we use the manipulators plane 
	// otherwise the standard plane
	if (self->manip.getValue ())
	{
		self->m_plane->plane = self->m_manip->plane;
		self->plane = self->m_manip->plane;
	}
	else
	{
		// no need to update the manipulator's plane because it 
		// doesn't exist
		self->m_plane->plane = self->plane;
	}
	
	// enable the sensors again
	self->m_planeSensor->attach (&self->plane);
	self->m_mplaneSensor->attach (&self->m_plane->plane);
	
	// only attach the manipulator if it is active (i.e. not null)
	if (self->m_manip)
	{
		self->m_mplaneSensor->attach (&self->m_manip->plane);
	}
	self->m_plane->refreshScale = true;
}
