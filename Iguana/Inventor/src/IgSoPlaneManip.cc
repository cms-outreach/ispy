//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoPlaneManip.h"
#include <Inventor/draggers/SoJackDragger.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_NODE_SOURCE (IgSoPlaneManip);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
IgSoPlaneManip::initClass (void)
{
    SO_NODE_INIT_CLASS (IgSoPlaneManip, SoNode, "Node");
}

IgSoPlaneManip::IgSoPlaneManip (void) 
    : m_manip (new SoJackDragger),
      m_manipSensor (0),
      m_planeSensor (0)
{
    SO_NODE_CONSTRUCTOR (IgSoPlaneManip);
    SO_NODE_ADD_FIELD   (plane,    (SbPlane (SbVec3f (0.0f, 1.0f, 0.0f), 0.0f)));
    SO_NODE_ADD_FIELD   (manip,  (FALSE));
 
    m_manip->ref();
    m_manip->addValueChangedCallback(jackChanged, this);
    
    m_manipSensor = new SoFieldSensor (&manipChanged, this);
    m_manipSensor->attach (&manip);
    
    m_planeSensor = new SoFieldSensor (&planeChanged, this);
}

IgSoPlaneManip::~IgSoPlaneManip (void)
{
    m_manip->unref();
    delete m_manipSensor;
    delete m_planeSensor;
}

SoDragger *
IgSoPlaneManip::getDragger(void)
{ return m_manip; }

void
IgSoPlaneManip::getMotionMatrix (SbMatrix &matrix, bool scaling /*=true*/)
{
    matrix = m_manip->getMotionMatrix();
      
    SbVec3f normal = plane.getValue().getNormal();
    SbVec3f t, s;
    SbRotation r, so;      
    matrix.getTransform(t, r, s, so);
    r.setValue(SbVec3f(0.0f, 1.0f, 0.0f), normal);
    t = normal * plane.getValue().getDistanceFromOrigin ();
    if (scaling)
	s.setValue (1.0f, 1.0f, 1.0f);
    matrix.setTransform(t, r, s, so);
}

void
IgSoPlaneManip::jackChanged(void *me, SoDragger *dragger)
{
    IgSoPlaneManip * self = static_cast<IgSoPlaneManip*>(me);
    SbMatrix matrix = dragger->getMotionMatrix();

    SbPlane plane(SbVec3f(0.0f, 1.0f, 0.0f), 0.0f);
    plane.transform(matrix);
    
    if ((plane.getNormal().equals (self->plane.getValue().getNormal(), 0.01f)) &&
        (fabs(plane.getDistanceFromOrigin() - 
	      self->plane.getValue().getDistanceFromOrigin()) < 0.0001))
        return;

    if (self->manip.getValue ())
    {
        self->m_planeSensor->detach();
        self->plane = plane;
        self->m_planeSensor->attach(&self->plane);
    }
    else
        self->plane = plane;
}

void
IgSoPlaneManip::planeChanged(void *me, SoSensor *)
{
    IgSoPlaneManip * self = static_cast<IgSoPlaneManip*>(me);
    
    SbMatrix matrix;
    self->getMotionMatrix (matrix, false);
      
    self->m_manip->enableValueChangedCallbacks(false);
    self->m_manip->setMotionMatrix(matrix);
    self->m_manip->enableValueChangedCallbacks(true);
}

void
IgSoPlaneManip::manipChanged (void *me, SoSensor *)
{
    IgSoPlaneManip * self = static_cast<IgSoPlaneManip*>(me);
    if (self->manip.getValue ())
    {
	if (!self->hasDragger ())
	{
	    self->insertChild (self->m_manip, 0);
	    self->m_planeSensor->attach (&self->plane);
	    self->m_planeSensor->trigger ();
	}
    }
    else if(self->hasDragger ())
    {
	self->m_planeSensor->detach ();
	self->m_manip->enableValueChangedCallbacks(false);
	self->removeChild (self->m_manip);
    }
}

bool
IgSoPlaneManip::hasDragger (void)
{
    return (getNumChildren () > 0) &&
         dynamic_cast<SoJackDragger*>(getChild(0));
}

void
IgSoPlaneManip::write (SoWriteAction *action)
{ SoNode::write (action); }
