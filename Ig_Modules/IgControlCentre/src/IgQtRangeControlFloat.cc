//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgControlCentre/interface/IgQtRangeControlFloat.h"
#include "Ig_Extensions/IgQt/interface/IgQtLineEdit.h"
#include "Ig_Modules/IgControlCentre/interface/IgQtRangeSetting.h"
#include <qslider.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQtRangeControlFloat::IgQtRangeControlFloat (QWidget *parent, int parts,
				              float value /* = 0 */,
		      		              const char *name /* = 0 */)
    : IgQtRangeControl  (parent, parts, name),
      inherited (value),
      m_longStep   (0.1),
      m_smallStep  (0.01)
{ initialize (parts); }

void
IgQtRangeControlFloat::initialize (int /*parts*/)
{
    // Pretend to step into the value to get it properly bounded
    setPrecision (.01);
    float oldValue = value ();
    setRange (SHRT_MIN, SHRT_MAX);
    forceValue (value () + m_smallStep);
    setValue (oldValue);
    forceValueChanged (0);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
IgQtRangeControlFloat::setPrecision (float value)
{
    IgQtRangeValuePrecision::setPrecision (value);
    editSettingsChanged ();
}

/** Get the size of the small step.  */
float
IgQtRangeControlFloat::getSmallStep ()const
{ return m_smallStep; }

/** Set the small step size to @a value.  */
void
IgQtRangeControlFloat::setSmallStep (float value)
{
    IgQtRangeValuePrecision::setPrecision (value);
    m_smallStep = precision ();
    if (m_longStep < m_smallStep)
	m_longStep = m_smallStep;

    adjustSliderSteps ();
    editSettingsChanged ();
}

/** Get the size of the long step.  */
float
IgQtRangeControlFloat::getLongStep (void) const
{ return m_longStep; }

/** Set the length of the long step to @a value.  */
void
IgQtRangeControlFloat::setLongStep (float value)
{
    m_longStep = fabs (value);
    if (m_smallStep > m_longStep)
    {
	IgQtRangeValuePrecision::setPrecision (m_longStep);
	m_smallStep = precision ();
    }
    adjustSliderSteps ();
    editSettingsChanged ();
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Adjust the slider to go from minimum to the maximum with as many
    discrete values as we have small steps, and the page size to the
    ratio of long step from the small one.  */
void
IgQtRangeControlFloat::adjustSliderSteps (void)
{
    if (m_slider)
    {
	disconnect (m_slider, SIGNAL (valueChanged (int)),
		    this,     SLOT (sliderChanged (int)));
	m_slider->setRange (0, int (round ((maxValue()-minValue()) / m_smallStep)));
	m_slider->setPageStep (int (round (m_longStep / m_smallStep)));
        m_slider->setValue (int (round ((value () - minValue ()) / m_smallStep)));
	connect (m_slider, SIGNAL (valueChanged (int)),
	         this,     SLOT (sliderChanged (int)));
    }
}

/** Slider has changed to position @a value small steps from the
    minimum.  Adjust current control value. */
void
IgQtRangeControlFloat::sliderChanged (int value)
{ setValue ((value * m_smallStep) + minValue ()); } 

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
IgQtRangeControlFloat::revMin (void)
{ setValue (minValue ()); }

void
IgQtRangeControlFloat::revLong (void)
{ moveValue (m_longStep,  MoveREV); }

void
IgQtRangeControlFloat::revSmall (void)
{ moveValue (m_smallStep, MoveREV); }

void
IgQtRangeControlFloat::fwdSmall (void)
{ moveValue (m_smallStep, MoveFWD); }

void
IgQtRangeControlFloat::fwdLong (void)
{ moveValue (m_longStep,  MoveFWD); }

void
IgQtRangeControlFloat::fwdMax (void)
{ setValue (maxValue ()); }

void
IgQtRangeControlFloat::emitSignals (void)
{
    emit newValue (value ());
    emit valueChanged (valueChange ());
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
IgQtRangeControlFloat::moveValue (float delta, MoveDirection direction)
{
    float oldPrecision = precision ();
    IgQtRangeValuePrecision::setPrecision (delta);

    switch (m_stepType)
    {
    case ADD:
        setValue (value () + (direction == MoveREV ? -delta : delta));
        break;

    case MULTIPLE:
        setValue (value () * (direction == MoveREV ? 1/delta : delta));
        break;
    }

    IgQtRangeValuePrecision::setPrecision (oldPrecision);
    setValue (value ());
}

void
IgQtRangeControlFloat::textChanged (const QString &newval)
{ setValue (stringToNumber (newval)); }

void
IgQtRangeControlFloat::setValue (float newval)
{
    // First let the base class handle it.  If the value doesn't
    // actually change, we have nothing further to do.
    //If tracking was not on then keep the accumulate the change value
    float prevChangedValue = valueChange ();
    inherited::setValue (newval);
    if (! valueChange ())
    {
	forceValueChanged (prevChangedValue);
	return;
    }
    forceValueChanged (prevChangedValue + valueChange ());

    // Adjust the various display.
    if (m_text)
    {	
        int prec = precision () ? int (fabs (log10 (precision ())) + .5) : 6;
        m_text->setText (QString::number (value (), 'f', prec));
	m_text->setChanged (false);
    }

    if (m_slider)
    {
	disconnect (m_slider, SIGNAL (valueChanged (int)),
		    this, SLOT (sliderChanged (int)));

	m_slider->setValue (int (round ((value () - minValue ()) / m_smallStep)));

	connect (m_slider, SIGNAL (valueChanged (int)),
		 this, SLOT (sliderChanged (int)));
    }

    // Indicate where we are for various buttons and then set myself
    // dirty so they'll get updated.
    fwdEnable (maxValue () - value () >= m_smallStep);
    revEnable (value () - minValue () >= m_smallStep);
    setDirty ();
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Set the range of the control to be from @a min to @a max,
    inclusive.  If the current value is outside that range, it is
    adjusted to fall into it.  */
void
IgQtRangeControlFloat::setRange (float min, float max)
{
    // Like base class, just adjust the number of slider steps.
    inherited::setRange (min, max);
    adjustSliderSteps ();
    editSettingsChanged ();
}

void
IgQtRangeControlFloat::setDirty (bool dirty)
{
    if (dirty)
    {
	emitSignals ();
	IgQtRangeControl::setDirty (dirty);
	if (m_resetChangedValue)
	  forceValueChanged (0);
    }
    else
    {
	forceValueChanged (0);
	IgQtRangeControl::setDirty (dirty);
    }
}

void
IgQtRangeControlFloat::fwdEnable (bool value)
{
    if (!(!value && boundaryHandling () == MODULATE))
	IgQtRangeControl::fwdEnable (value);
}

void
IgQtRangeControlFloat::revEnable (bool value)
{
    if (!(!value && boundaryHandling () == MODULATE))
	IgQtRangeControl::revEnable (value);
}

void
IgQtRangeControlFloat::editSettings (void)
{
    IgQtRangeSetting* setting = new IgQtRangeSetting (widget (), "setting");
    setting->setValue (QString::number (value ()));
    setting->setPrecision (QString::number (precision ()));
    setting->setMin (QString::number (minValue ()));
    setting->setMax (QString::number (maxValue ()));
    setting->setSmall (QString::number (getSmallStep ()));
    setting->setLong (QString::number (getLongStep ()));
    setting->enableValue (isValueEditable ());
    setting->enableStep (isStepEditable ());
    setting->enableRange (isRangeEditable ());
    setting->enablePrecision (isPrecisionEditable ());
    setting->clear ();
    setting->exec ();
    bool changed = false;
    setEmitSettingsChanged (false);
    if (setting->isApplied ())
    {
      if (setting->isPrecisionChanged ())
      {
        setPrecision (stringToNumber (setting->getPrecision ()));
	changed = true;
      }
      if (setting->isRangeChanged ())
      {
        setRange (stringToNumber (setting->getMin ()),
	          stringToNumber (setting->getMax ()));
        changed = true;
      }
      if (setting->isStepChanged ())
      {
	setSmallStep (stringToNumber (setting->getSmall ()));
        setLongStep  (stringToNumber (setting->getLong ()));
        changed = true;
      }
      if (setting->isValueChanged ())
        textChanged (setting->getValue ());
    }
    setEmitSettingsChanged (true);
    if (changed)
      editSettingsChanged ();
    delete setting;
}

QString
IgQtRangeControlFloat::getSettingString (void) const
{
    QString setting ("");
    addSettingItem (QString::number(minValue ()), setting);
    addSettingItem (QString::number(maxValue ()), setting);
    addSettingItem (QString::number(getSmallStep ()), setting);
    addSettingItem (QString::number(getLongStep ()), setting);
    addSettingItem (QString::number(precision ()), setting);
    return setting;
}
    
void
IgQtRangeControlFloat::setSettingString (QString &setting)
{
    bool oldValue = isEmitSettingsChanged ();
    setEmitSettingsChanged (false);
    
    if (!setting.isEmpty ())
    {
      float min = stringToNumber (getSettingItem (setting));
      if (!setting.isEmpty ())
        setRange (min , stringToNumber (getSettingItem (setting)));
      else
        setRange (min , maxValue ());
    }
    
    if (!setting.isEmpty ())
      setSmallStep (stringToNumber (getSettingItem (setting)));

    if (!setting.isEmpty ())
      setLongStep (stringToNumber (getSettingItem (setting)));

    if (!setting.isEmpty ())
      setPrecision (stringToNumber (getSettingItem (setting)));
    
    setEmitSettingsChanged (oldValue);
    editSettingsChanged ();
}
    
