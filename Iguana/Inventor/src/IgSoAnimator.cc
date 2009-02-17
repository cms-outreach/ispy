//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoAnimator.h"
#include <Inventor/engines/SoCompose.h>
#include <Inventor/engines/SoTimeCounter.h>
#include <Inventor/engines/SoCalculator.h>
#include <Inventor/engines/SoCounter.h>
#include <Inventor/sensors/SoFieldSensor.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_NODE_SOURCE (IgSoAnimator);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
IgSoAnimator::initClass (void)
{
    SO_NODE_INIT_CLASS (IgSoAnimator, SoRotation, "Node");
}

IgSoAnimator::IgSoAnimator (void)
    : m_crot         (0),
      m_calc         (0),
      m_counter      (0),
      m_stateCounter (0),
      m_onSensor     (0),
      m_resetSensor  (0),
      m_stateSensor  (0)
{
    SO_NODE_CONSTRUCTOR (IgSoAnimator);
    SO_NODE_ADD_FIELD (timeMax, (2000));	// short int ==> mrad / M_PI
    SO_NODE_ADD_FIELD (timeStep, (10));
    SO_NODE_ADD_FIELD (timeFreq, (.05));
    SO_NODE_ADD_FIELD (axis, (SbVec3f (1, 0, 0)));
    SO_NODE_ADD_FIELD (on, (FALSE));
    SO_NODE_ADD_FIELD (reset, (FALSE));
    SO_NODE_ADD_FIELD (state, (0));
    SO_NODE_ADD_FIELD (newAxis, (0));		// output signal
    SO_NODE_ADD_FIELD (complete, (FALSE));	// output signal

    m_stateSensor = new SoFieldSensor (&invokeUpdateAxis, this);
    m_stateSensor->setPriority (SoFieldSensor::getDefaultPriority () - 2);
    m_onSensor = new SoFieldSensor (&onSensorCB, this);
    m_resetSensor = new SoFieldSensor (&resetSensorCB, this);
   
    initAnim ();
}

IgSoAnimator::~IgSoAnimator()
{
    m_crot->unref ();
    m_counter->unref ();
    m_calc->unref();
    m_stateCounter->unref();
    delete m_onSensor;
    delete m_resetSensor;
    delete m_stateSensor;
}

void
IgSoAnimator::updateAxis (void)
{
    if (state.getValue () == 1)			// Rotate about Y
	axis.setValue (0, 1, 0);
    else if (state.getValue () == 2)		// Rotate about Z
	axis.setValue (0, 0, 1);
    else					// Rotate about X
	axis.setValue (1, 0, 0);
    
    newAxis = state.getValue ();
    if (state.getValue () == 0)
	complete = true;
}

void
IgSoAnimator::invokeUpdateAxis (void *data, SoSensor *)
{ static_cast<IgSoAnimator *> (data)->updateAxis (); }

void
IgSoAnimator::resetSensorCB (void *me, SoSensor *)
{
    IgSoAnimator *self = static_cast<IgSoAnimator *> (me);
    if (self->reset.getValue ())
    {
      self->m_stateSensor->detach ();
      self->m_onSensor->detach ();
      self->m_resetSensor->detach ();
      if (self->m_counter->getRefCount())
      {
        self->m_stateCounter->unref();
	self->m_crot->unref ();
        self->m_calc->unref();
        self->m_counter->unref ();
      }
      
      short max  = self->timeMax.getValue();
      short step = self->timeStep.getValue();
      float freq = self->timeFreq.getValue();
      self->setToDefaults ();
      self->reset = true;
      self->timeMax  = max;
      self->timeStep = step;
      self->timeFreq = freq;
      self->initAnim ();
    }
}

void
IgSoAnimator::initAnim (void)
{
    m_crot         = new SoComposeRotation;
    m_counter      = new SoTimeCounter;
    m_calc         = new SoCalculator;
    m_stateCounter = new SoCounter;
      
    m_counter->ref ();
    m_counter->min = 0;
    m_counter->max.connectFrom (&timeMax);
    m_counter->step.connectFrom (&timeStep);
    m_counter->frequency.connectFrom (&timeFreq);
    m_counter->on.connectFrom (&on);

    // Create a calculator to turn the timer into output angle
    m_calc->ref();
    m_calc->a.connectFrom (&m_counter->output);
    m_calc->expression.set1Value (0, "oa = M_PI * a / 1000"); // mrad => rad

    // Connect the rotation angle into the rotation
    m_crot->ref ();
    m_crot->axis.connectFrom (&axis);
    m_crot->angle.connectFrom (&m_calc->oa);
    rotation.connectFrom (&m_crot->rotation);

    // Use the state counter to cycle over axes.  The states are: 0=X, 1=Y, 2=Z
    m_stateCounter->ref ();
    m_stateCounter->min = 0;
    m_stateCounter->max = 2; 
    m_stateCounter->step = 1;
    m_stateCounter->trigger.connectFrom (&m_counter->syncOut);
    m_stateCounter->reset.setValue (0);
    
    // Connect the timer cycle to the state, then connect the state output
    // to the callback sensor.  Thus when the state changes we get called
    // so that we can change the axis.
    state.connectFrom (&m_stateCounter->output);
    m_stateSensor->attach (&state);
    m_onSensor->attach (&on);
    m_resetSensor->attach (&reset);
}

void
IgSoAnimator::onSensorCB (void *me, SoSensor *)
{
    IgSoAnimator *self = static_cast<IgSoAnimator *> (me);
    if (self->on.getValue ())
      self->reset.setValue (false);
}

void
IgSoAnimator::write (SoWriteAction *action)
{
  rotation.disconnect ();
  state.disconnect ();
  SoNode::write (action);
  rotation.connectFrom (&m_crot->rotation);
  state.connectFrom (&m_stateCounter->output);
}
