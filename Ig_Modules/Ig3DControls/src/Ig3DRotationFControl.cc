//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DControls/interface/Ig3DRotationFControl.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DUnitVec3fControl.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DRotationControl.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DFloatControl.h"
#include "Ig_Modules/IgControlCentre/interface/IgQtRangeControl.h"
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

Ig3DRotationFControl::Ig3DRotationFControl (IgControlCategory *pane,
				            QString	      label)
    : Ig3DFieldControl (pane)
{
    ASSERT (pane);
    ASSERT (pane->bodyArea ());
    ASSERT (pane->bodyLayout ());

    QWidget *area = pane->bodyArea ();

    // Add a new row to the grid: just the label for this category
    QWidget 	*vlab = makeIndentedLabel (label, area);
    addGridWidget (vlab, gridRows (), 0);

    // Now add subitems for direction and distance
    pane->pushIndent ();
    m_axis = new Ig3DUnitVec3fControl (pane, "Axis");
    
    m_angle = new Ig3DFloatControl (pane, "Angle",
				    IgQtRangeControl::StepperPart
				    | IgQtRangeControl::ValuePart
				    | IgQtRangeControl::SliderPart);    
    m_angle->control ()->setSmallStep (1); 
    m_angle->control ()->setLongStep (5); 
    m_angle->control ()->setRange (-180.,180.);
    
    pane->popIndent ();

    // Always reflect the state of the real value controls and vice versa.
    connect (m_axis,   SIGNAL(dirty()), this, SLOT(setDirty()));
    connect (m_axis,   SIGNAL(settingsChanged (void)),
             this,     SLOT  (emitSettingsChanged (void)));
    connect (m_angle,  SIGNAL(dirty()), this, SLOT(setDirty()));
    connect (m_angle,  SIGNAL(settingsChanged (void)),
             this,     SLOT  (emitSettingsChanged (void)));
    connect (this, SIGNAL(clean()), m_axis, SLOT(setClean()));
    connect (this, SIGNAL(clean()), m_angle,  SLOT(setClean()));
    connect (this, SIGNAL(editable(bool)),m_axis,SLOT(setEditable(bool)));
    connect (this, SIGNAL(editable(bool)),m_angle, SLOT(setEditable(bool)));
    connect (this, SIGNAL(editable(bool)),vlab,       SLOT(setEnabled(bool)));
}

//////////////////////////////////////////////////////////////////////
SoSFRotation *
Ig3DRotationFControl::field (void) const
{ return static_cast<SoSFRotation *> (Ig3DFieldControl::field ()); }

void
Ig3DRotationFControl::setField (SoSFRotation *field)
{ Ig3DFieldControl::setField (field); }

SbRotation
Ig3DRotationFControl::fieldValue (void) const
{ ASSERT (field ()); return field ()->getValue (); }

//////////////////////////////////////////////////////////////////////
SbRotation
Ig3DRotationFControl::value (void) const
{ return SbRotation (axis (), angle ()); }

SbVec3f
Ig3DRotationFControl::axis (void) const
{ return m_axis->value (); }

float
Ig3DRotationFControl::angle (void) const
{ return Ig3DRotationControl::deg2rad(m_angle->value ()); }

void
Ig3DRotationFControl::setValue (const SbRotation &newval)
{
    SbVec3f axis;
    float angle;
    newval.getValue (axis, angle);
    setValue (axis, Ig3DRotationControl::rad2deg(angle));
}

void
Ig3DRotationFControl::setValue (const SbVec3f &axis, float angle)
{ setAxis (axis); setAngle (angle); }

void
Ig3DRotationFControl::setAxis (const SbVec3f &newval)
{ m_axis->setValue (newval); }

void
Ig3DRotationFControl::setAngle (float newval)
{ m_angle->setValue (newval); }

void
Ig3DRotationFControl::updateValue (void)
{ setValue (fieldValue ()); }

void
Ig3DRotationFControl::applyValue (void)
{ field ()->setValue (value ()); }

//////////////////////////////////////////////////////////////////////
Ig3DVectorControl*
Ig3DRotationFControl::axisControl (void) const
{ return m_axis; }

Ig3DFloatControl*
Ig3DRotationFControl::angleControl  (void) const
{ return m_angle; }

QString
Ig3DRotationFControl::getSettingString (void) const
{

  QString setting (m_axis->control ()->getSettingString ());
  IgQtRangeControlCommon::addSettingItem (m_angle->control ()->
                                          getSettingString (),
                                          setting);
  return setting;
}
    
void
Ig3DRotationFControl::setSettingString (QString &setting)
{
  m_axis->control ()->setSettingString (setting);
  m_angle->control ()->setSettingString (setting);
}

void
Ig3DRotationFControl::emitSettingsChanged (void)
{ fieldSettingsChanged (); }
