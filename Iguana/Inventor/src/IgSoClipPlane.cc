//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoClipPlane.h"
#include <Inventor/nodes/SoClipPlane.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/manips/SoClipPlaneManip.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/SbBox.h>

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

SO_NODE_SOURCE (IgSoClipPlane);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
IgSoClipPlane::initClass (void)
{
    SO_NODE_INIT_CLASS (IgSoClipPlane, SoNode, "Node");
}

IgSoClipPlane::IgSoClipPlane (void)
    : m_plane        (new SoClipPlane),
      m_shapeHints   (new SoShapeHints),
      m_onSensor     (0),
      m_manipSensor  (0),
      m_planeSensor  (0),
      m_mplaneSensor (0)
{
    SO_NODE_CONSTRUCTOR (IgSoClipPlane);
    SO_NODE_ADD_FIELD (on, (TRUE));
    SO_NODE_ADD_FIELD (manip, (FALSE));
    SO_NODE_ADD_FIELD (plane, (SbPlane (SbVec3f (1, 0, 0), 0)));

    m_shapeHints->setOverride (TRUE);
    m_shapeHints->ref();

    addChild (m_plane);    
    addChild (m_shapeHints);    

    m_onSensor = new SoFieldSensor (&onSensorCB, this);
    m_onSensor->attach (&on);

    m_manipSensor = new SoFieldSensor (&manipSensorCB, this);
    m_manipSensor->attach (&manip);

    m_planeSensor = new SoFieldSensor (&planeSensorCB, this);
    m_planeSensor->attach (&plane);
    
    m_mplaneSensor = new SoFieldSensor (&mplaneSensorCB, this);
    //m_mplaneSensor->attach (&m_plane->plane);
    
    reconnect ();
}

IgSoClipPlane::~IgSoClipPlane (void)
{
    removeChild (m_plane);    
    if (on.getValue ())
      removeChild (m_shapeHints);
    m_shapeHints->unref();
    delete m_onSensor;
    delete m_manipSensor;
    delete m_planeSensor;
    delete m_mplaneSensor;
}

void
IgSoClipPlane::reconnect (void)
{
    m_plane->on    = on;
    syncPlane ();
    m_mplaneSensor->attach (&m_plane->plane);
}

/** This callback is called whenever the `on' field changes value.
   */
void
IgSoClipPlane::onSensorCB (void *me, SoSensor *)
{
    // activate or deactivate the clip plane

    IgSoClipPlane *self = static_cast<IgSoClipPlane *> (me);
    if (self->on.getValue ())
	self->addChild (self->m_shapeHints);
    else
	self->removeChild (self->m_shapeHints);

    self->m_plane->on = self->on;
}

/** This callback is called whenever the `manip' field changes value.
    We may need to switch the clip plane to its manipulator or
    vice versa.  */
void
IgSoClipPlane::manipSensorCB (void *me, SoSensor *)
{
    // Hide or show the clip plane manipulators
    // Hide: replace clip plane manipulator with the normal clip plane.
    // Show: replace clip plane with manipulator.

    IgSoClipPlane *self = static_cast<IgSoClipPlane *> (me);

    if (self->manip.getValue ())
    {
	// Replace the clip plane by a clip plane manipulator.
	self->replaceChild (0, self->m_plane = new SoClipPlaneManip);
	self->reconnect ();
    }
    else
    {
	// Replace the clip plane manipulator by a normal clip plane.
	self->replaceChild (0, self->m_plane = new SoClipPlane);
	self->reconnect ();
    }
}

/** This callback is called whenever the `plane' field changes value.
   */
void
IgSoClipPlane::planeSensorCB (void *me, SoSensor *)
{
    IgSoClipPlane *self = static_cast<IgSoClipPlane *> (me);
    self->m_mplaneSensor->detach ();
    self->syncPlane ();
    self->m_mplaneSensor->attach (&self->m_plane->plane);
}

/** This callback is called whenever the `m_plane' field changes value.
   */
void
IgSoClipPlane::mplaneSensorCB (void *me, SoSensor *)
{
    IgSoClipPlane *self = static_cast<IgSoClipPlane *> (me);
    self->m_planeSensor->detach ();
    self->plane = self->m_plane->plane;
    self->m_planeSensor->attach (&self->plane);
}

void
IgSoClipPlane::write (SoWriteAction *action)
{ SoNode::write (action); }

void
IgSoClipPlane::syncPlane (void)
{
#ifdef __COIN__
    if (dynamic_cast<SoClipPlaneManip *>(m_plane))
    {
	SbBox3f box (-1,-1,-1, 1, 1, 1);
	SbVec3f p (plane.getValue ().getNormal ());
	float d = -1 * plane.getValue ().getDistanceFromOrigin();
	static_cast<SoClipPlaneManip *>(m_plane)->setValue (box, p, 1.02f);
	m_plane->plane.setValue (SbPlane(p, d));
    }
    else
#endif
	m_plane->plane = plane;
}
