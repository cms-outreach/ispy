//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoSlicer.h"
#include "Iguana/Inventor/interface/IgSoSlicerEngine.h"
#include "Iguana/Inventor/interface/IgSoClipPlane.h"
#include "Iguana/Inventor/interface/IgSoPlaneManip.h"
#include <Inventor/sensors/SoFieldSensor.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_NODE_SOURCE (IgSoSlicer);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
IgSoSlicer::initClass (void)
{
    SO_NODE_INIT_CLASS (IgSoSlicer, SoNode, "Node");
}

IgSoSlicer::IgSoSlicer (void)
    : m_front        (new IgSoClipPlane),
      m_back         (new IgSoClipPlane),
      m_manipPlane   (new IgSoPlaneManip),
      m_engine       (new IgSoSlicerEngine),
      m_planeSensor  (0),
      m_mplaneSensor (0)
{
    // Construct two clip planes, front, back and a Plane manipulator

    SO_NODE_CONSTRUCTOR (IgSoSlicer);
    SO_NODE_ADD_FIELD (on, (TRUE));
    SO_NODE_ADD_FIELD (manip, (FALSE));
    SO_NODE_ADD_FIELD (thickness, (.5f));
    SO_NODE_ADD_FIELD (plane, (SbPlane (SbVec3f (0, 0, -1), 0)));

    //add clip planes
    m_manipPlane->ref ();
    m_front->ref ();
    m_back->ref ();
    addChild (m_manipPlane);
    addChild (m_front);
    addChild (m_back);

    //set the manipplane same as the plane
    m_manipPlane->plane = plane;
    
    //connect "on" field to the front and back clip plane's 
    m_front->on.connectFrom (&on);
    m_back->on.connectFrom (&on);
    
    //connect engine's outputs to the front and back 
    //clip plane's "plane" feild
    m_front->plane.connectFrom (&m_engine->frontPlane);
    m_back->plane.connectFrom (&m_engine->backPlane);

    //connect the engine's inputs
    m_engine->thickness.connectFrom (&thickness);
    m_engine->plane.connectFrom (&plane);
    m_engine->ref();

    m_manipPlane->manip.connectFrom (&manip);
        
    m_planeSensor = new SoFieldSensor (&planeSensorCB, this);
    m_planeSensor->attach (&plane);
    
    m_mplaneSensor = new SoFieldSensor (&mplaneSensorCB, this);
    m_mplaneSensor->attach (&m_manipPlane->plane);
}

IgSoSlicer::~IgSoSlicer (void)
{
    removeChild (m_manipPlane);
    removeChild (m_front);
    removeChild (m_back);
    m_manipPlane->unref ();
    m_front->unref ();
    m_back->unref ();
    m_engine->unref();
    delete m_planeSensor;
    delete m_mplaneSensor;
}

/** This callback is called whenever the `m_plane' field changes value.
   */
void
IgSoSlicer::mplaneSensorCB (void *me, SoSensor *)
{
    IgSoSlicer *self = static_cast<IgSoSlicer *> (me);
    self->m_planeSensor->detach ();
    self->plane = self->m_manipPlane->plane;
    self->m_planeSensor->attach (&self->plane);
}

/** This callback is called whenever the `plane' field changes value.
   */
void
IgSoSlicer::planeSensorCB (void *me, SoSensor *)
{
    IgSoSlicer *self = static_cast<IgSoSlicer *> (me);
    self->m_mplaneSensor->detach ();
    self->m_manipPlane->plane = self->plane;
    self->m_mplaneSensor->attach (&self->m_manipPlane->plane);
}

void
IgSoSlicer::write (SoWriteAction *action)
{ SoNode::write (action); }
