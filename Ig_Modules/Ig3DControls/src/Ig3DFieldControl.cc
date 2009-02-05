//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DControls/interface/Ig3DFieldControl.h"
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/fields/SoField.h>
#include <classlib/utils/DebugAids.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

Ig3DFieldControl::Ig3DFieldControl (IgControlCategory *pane)
    : IgControlBase (pane),
      m_field (0),
      m_sensor (new SoFieldSensor (&fieldChanged, this))
{}

Ig3DFieldControl::~Ig3DFieldControl (void)
{ delete m_sensor; }

void
Ig3DFieldControl::setField (SoField *field)
{
    detach ();
    m_field = field;
    attach ();
}

void
Ig3DFieldControl::clearField (void)
{ setField (0); }

SoField *
Ig3DFieldControl::field (void) const
{ return m_field; }

void
Ig3DFieldControl::attach (void)
{
    IgControlBase::attach ();
    if (m_field)
	m_sensor->attach (m_field);
}

void
Ig3DFieldControl::detach (void)
{
    IgControlBase::detach ();
    if (m_field)
	m_sensor->detach ();
}

bool
Ig3DFieldControl::hasValue (void) const
{ return IgControlBase::hasValue () && m_field; }

void
Ig3DFieldControl::fieldChanged (void *data, SoSensor * /*sensor*/)
{ ASSERT (data); static_cast<Ig3DFieldControl *> (data)->update (); }

void
Ig3DFieldControl::fieldSettingsChanged(void)
{
    emit settingsChanged (m_field);
    emit settingsChanged ();
}
