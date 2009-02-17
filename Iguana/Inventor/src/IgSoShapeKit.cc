//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoShapeKit.h"
#include <Inventor/nodekits/SoNodekitCatalog.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/nodes/SoSeparator.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_KIT_SOURCE (IgSoShapeKit);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgSoShapeKit::IgSoShapeKit (void)
{
    SO_KIT_CONSTRUCTOR (IgSoShapeKit);
    SO_KIT_ADD_CATALOG_ENTRY (separator, SoSeparator, FALSE, this,\x0, TRUE);
    SO_KIT_INIT_INSTANCE ();

    m_sensor = new SoNodeSensor (selfMonitorCB, this);
    m_sensor->setPriority (1);
}

IgSoShapeKit::~IgSoShapeKit (void)
{ delete m_sensor; }

void IgSoShapeKit::initClass (void)
{ SO_KIT_INIT_CLASS (IgSoShapeKit, SoBaseKit, "BaseKit"); }

SbBool
IgSoShapeKit::setUpConnections (SbBool onoff, SbBool doitalways /* = false */)
{
    if (! doitalways && connectionsSetUp == onoff)
	return onoff;

    if (onoff)
    {
	SoBaseKit::setUpConnections (onoff, doitalways);
	selfMonitorCB (this, 0); // automatically attaches sensor
    }
    else
    {
	disableSensor ();
	SoBaseKit::setUpConnections (onoff, doitalways);
    }
    return connectionsSetUp != onoff;
}

void
IgSoShapeKit::setDefaultOnNonWritingFields (void)
{
    // Mark all parts default so they won't be written out.
    // Normal fields will be written out, so don't touch them.
    // The first part seems to always be "this" and has no field.
    SoBaseKit::setDefaultOnNonWritingFields ();
    const SoNodekitCatalog *cat = getNodekitCatalog ();
    for (int i = 0; i < cat->getNumEntries (); ++i)
	if (SoField *field = getField (cat->getName (i)))
	    field->setDefault (TRUE);
}

void
IgSoShapeKit::preAction (void)
{
    // If we've been scheduled for refresh and an action is applied,
    // force the refresh update before applying the action.
    if (m_sensor->isScheduled ())
    {	
	m_sensor->unschedule ();
	refresh ();
    }
}

void
IgSoShapeKit::doAction (SoAction *action)
{ preAction (); SoBaseKit::doAction (action); }

void
IgSoShapeKit::callback (SoCallbackAction *action)
{ preAction (); SoBaseKit::callback (action); }

void
IgSoShapeKit::GLRender (SoGLRenderAction *action)
{ preAction (); SoBaseKit::GLRender (action); }

void
IgSoShapeKit::getBoundingBox (SoGetBoundingBoxAction *action)
{ preAction (); SoBaseKit::getBoundingBox (action); }

void
IgSoShapeKit::getMatrix (SoGetMatrixAction *action)
{ preAction (); SoBaseKit::getMatrix (action); }

void
IgSoShapeKit::handleEvent (SoHandleEventAction *action)
{ preAction (); SoBaseKit::handleEvent (action); }

void
IgSoShapeKit::rayPick (SoRayPickAction *action)
{ preAction (); SoBaseKit::rayPick (action); }

void
IgSoShapeKit::pick (SoPickAction *action)
{ preAction (); SoBaseKit::pick (action); }

void
IgSoShapeKit::search (SoSearchAction *action)
{ preAction (); SoBaseKit::search (action); }

void
IgSoShapeKit::write (SoWriteAction *action)
{ preAction (); SoBaseKit::write (action); }

void
IgSoShapeKit::getPrimitiveCount (SoGetPrimitiveCountAction *action)
{ preAction (); SoBaseKit::getPrimitiveCount (action); }

SbBool
IgSoShapeKit::readInstance (SoInput *in, unsigned short flags)
{ 
    bool result = SoBaseKit::readInstance (in, flags);	
    this->refresh ();    
    return result; 
}

void
IgSoShapeKit::selfMonitorCB (void *data, SoSensor *)
{
    IgSoShapeKit *obj = (IgSoShapeKit *) data;

    // Force sensor on first, it allows "refresh" method to exit early
    obj->disableSensor ();
    obj->refresh ();
    obj->enableSensor ();
}

void
IgSoShapeKit::enableSensor (void)
{
    if (! m_sensor->getAttachedNode ())
	m_sensor->attach (this);
}

void
IgSoShapeKit::disableSensor (void)
{ m_sensor->detach (); }

void
IgSoShapeKit::refresh (void)
{}
