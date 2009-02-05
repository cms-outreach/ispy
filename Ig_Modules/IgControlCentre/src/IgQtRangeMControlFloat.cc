//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgControlCentre/interface/IgQtRangeMControlFloat.h"
#include "Ig_Modules/IgControlCentre/interface/IgQtRangeSetting.h"
#include "Ig_Extensions/IgQt/interface/IgQtLineEdit.h"
#include <qslider.h>
#include <qcombobox.h>
#include <math.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQtRangeMControlFloat::IgQtRangeMControlFloat (QWidget *parent, int parts,
                                               float value /* = 0.*/,
		                               const char *name /*= 0*/)
    : IgQtRangeMControl  (parent, parts, 1, name),
      inherited (value),
      m_longStep   (0.1),
      m_smallStep  (0.01)
{ initialize (parts); }

IgQtRangeMControlFloat::IgQtRangeMControlFloat (QWidget *parent, int parts,
                                               unsigned int size,
		                               const float* values,
		                               const char *name /*= 0*/)
    : IgQtRangeMControl  (parent, parts, size, name),
      inherited (size, values),
      m_longStep   (0.1),
      m_smallStep  (0.01)
{ initialize (parts); }

IgQtRangeMControlFloat::IgQtRangeMControlFloat (QWidget *parent, int parts,
                          const rangeMValues& values,
		          const char *name /*= 0*/)
    : IgQtRangeMControl  (parent, parts, values.size (), name),
      inherited (values),
      m_longStep   (0.1),
      m_smallStep  (0.01)
{ initialize (parts); }

void
IgQtRangeMControlFloat::initialize (int /*parts*/)
{
    // Pretend to step into the value to get it properly bounded
    setPrecision (.01);
    float oldValue = inherited::value (0);
    setRange (SHRT_MIN, SHRT_MAX);
    forceValue (0, inherited::value (0) + m_smallStep);
    setValue (0, oldValue);
    inherited::forceValueChanged (0, 0.);
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
IgQtRangeMControlFloat::resize (unsigned int size, float appendedValues)
{
    inherited::resize (size, appendedValues);
    IgQtRangeMControl::resizeIndex(size);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
IgQtRangeMControlFloat::setPrecision (float value)
{
    IgQtRangeValuePrecision::setPrecision (value);
    editSettingsChanged ();
}

/** Get the size of the small step.  */
float
IgQtRangeMControlFloat::getSmallStep ()const
{ return m_smallStep; }

/** Set the small step size to @a value.  */
void
IgQtRangeMControlFloat::setSmallStep (float value)
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
IgQtRangeMControlFloat::getLongStep (void) const
{ return m_longStep; }

/** Set the length of the long step to @a value.  */
void
IgQtRangeMControlFloat::setLongStep (float value)
{
    m_longStep = ::fabs (value);
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
/** Set the range of the control to be from @a min to @a max,
    inclusive.  If the current value is outside that range, it is
    adjusted to fall into it.  */
void
IgQtRangeMControlFloat::setRange (float min, float max)
{
    // Like base class, just adjust the number of slider steps.
    inherited::setRange (min, max);
    adjustSliderSteps ();
    editSettingsChanged ();
}

/** Adjust the slider to go from minimum to the maximum with as many
    discrete values as we have small steps, and the page size to the
    ratio of long step from the small one.  */
void
IgQtRangeMControlFloat::adjustSliderSteps (void)
{
    if (m_slider)
    {
	disconnect (m_slider, SIGNAL (valueChanged (int)),
		    this,     SLOT (sliderChanged (int)));
	m_slider->setRange (0, int (round ((maxValue()-minValue()) / m_smallStep)));
	m_slider->setPageStep (int (round (m_longStep / m_smallStep)));
        m_slider->setValue (int (round ((inherited::value (currentIndex ()) - minValue ()) / m_smallStep)));
	connect (m_slider, SIGNAL (valueChanged (int)),
	         this,     SLOT (sliderChanged (int)));
    }
}

/** Slider has changed to position @a value small steps from the
    minimum.  Adjust current control value. */
void
IgQtRangeMControlFloat::sliderChanged (int value)
{ setValue (currentIndex (), (value * m_smallStep) + minValue ()); } 
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
IgQtRangeMControlFloat::revMin (void)
{ setValue (currentIndex(), minValue ()); }

void
IgQtRangeMControlFloat::revLong (void)
{ moveValue (m_longStep,  MoveREV); }

void
IgQtRangeMControlFloat::revSmall (void)
{ moveValue (m_smallStep, MoveREV); }

void
IgQtRangeMControlFloat::fwdSmall (void)
{ moveValue (m_smallStep, MoveFWD); }

void
IgQtRangeMControlFloat::fwdLong (void)
{ moveValue (m_longStep,  MoveFWD); }

void
IgQtRangeMControlFloat::fwdMax (void)
{ setValue (currentIndex(), maxValue ()); }

void
IgQtRangeMControlFloat::emitSignals (void)
{
    emit newValue (inherited::value ());
    emit valueChanged (valueChange ());
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
IgQtRangeMControlFloat::setValue (float newval)
{  setValue (currentIndex (), newval); }
    
void
IgQtRangeMControlFloat::setValue (unsigned int index, float newval)
{
    inherited::setValue (index, newval);
    if (isValueChanged (index)){
      if (int(index) == currentIndex ())
        updateTextValue ();
      setDirty ();
    }
}
    
void
IgQtRangeMControlFloat::setValue (unsigned int size, const float* newvals)
{
    inherited::setValue (size, newvals);
    if (isValueChanged ()){
      unsigned int index = (unsigned int)(currentIndex());        
      if (isValueChanged (index))
        updateTextValue ();
      setDirty ();
    }
}
    
void
IgQtRangeMControlFloat::setValue (const rangeMValues& newvals)
{ setValue (newvals.size (), &newvals[0]); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
IgQtRangeMControlFloat::moveValue (float delta, MoveDirection direction)
{
    float oldPrecision = precision ();
    IgQtRangeValuePrecision::setPrecision (delta);
    unsigned int index = (unsigned int)(currentIndex());
    
    switch (m_stepType)
    {
    case ADD:
	setValue (index, inherited::value (index) + (direction == MoveREV ? -delta : delta));
	break;

    case MULTIPLE:
	setValue (index, inherited::value (index) * (direction == MoveREV ? 1/delta : delta));
	break;
    }

    IgQtRangeValuePrecision::setPrecision (oldPrecision);
}

void
IgQtRangeMControlFloat::textChanged (const QString &newval)
{ setValue (stringToNumber (newval)); }

void
IgQtRangeMControlFloat::updateTextValue (void)
{
    // Adjust the various display.
    float newval = inherited::value (currentIndex());

    if (m_text)
    {	
	int prec = precision () ? int (::fabs (::log10 (precision ())) + .5) : 6;
	m_text->setText (QString::number (newval, 'f', prec));
	m_text->setChanged (false);
    }

    if (m_slider)
    {
	disconnect (m_slider, SIGNAL (valueChanged (int)),
		    this, SLOT (sliderChanged (int)));

	m_slider->setValue (int (::round ((newval - minValue ()) / m_smallStep)));

	connect (m_slider, SIGNAL (inherited::valueChanged (int)),
		 this, SLOT (sliderChanged (int)));
    }

    // Indicate where we are for various buttons and then set myself
    // dirty so they'll get updated.
    fwdEnable (maxValue () - newval >= m_smallStep);
    revEnable (newval - minValue () >= m_smallStep);
}

void
IgQtRangeMControlFloat::setDirty (bool dirty)
{
    if (dirty)
    {
	emitSignals ();
	IgQtRangeMControl::setDirty (dirty);
	if (m_resetChangedValue)
	  inherited::forceValueChanged (0);
    }
    else
    {
        inherited::forceValueChanged (0);
	IgQtRangeMControl::setDirty (dirty);
    }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
IgQtRangeMControlFloat::fwdEnable (bool value)
{
    if (!(!value && boundaryHandling () == MODULATE))
	IgQtRangeMControl::fwdEnable (value);
}

void
IgQtRangeMControlFloat::revEnable (bool value)
{
    if (!(!value && boundaryHandling () == MODULATE))
	IgQtRangeMControl::revEnable (value);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
IgQtRangeMControlFloat::editSettings (void)
{
    IgQtRangeSetting* setting = new IgQtRangeSetting (widget (), "setting");
    setting->setValue (QString::number (inherited::value (currentIndex ())));
    setting->setPrecision (QString::number (precision ()));
    setting->setMin (QString::number (inherited::minValue ()));
    setting->setMax (QString::number (inherited::maxValue ()));
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
IgQtRangeMControlFloat::getSettingString (void) const
{
    QString setting ("");
    addSettingItem (QString::number(inherited::minValue ()), setting);
    addSettingItem (QString::number(inherited::maxValue ()), setting);
    addSettingItem (QString::number(getSmallStep ()), setting);
    addSettingItem (QString::number(getLongStep ()), setting);
    addSettingItem (QString::number(precision ()), setting);
    return setting;
}
    
void
IgQtRangeMControlFloat::setSettingString (QString &setting)
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
    
