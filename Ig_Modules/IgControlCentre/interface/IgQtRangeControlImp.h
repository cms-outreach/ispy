#ifndef IG_CONTROL_CENTRE_IG_QT_RANGE_CONTROL_IMP_H
# define IG_CONTROL_CENTRE_IG_QT_RANGE_CONTROL_IMP_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgControlCentre/interface/IgQtRangeControl.h"
# include "Ig_Modules/IgControlCentre/interface/IgQtRangeValue.h"
# include "Ig_Modules/IgControlCentre/interface/IgQtRangeSetting.h"
# include "Ig_Extensions/IgQt/interface/IgQtLineEdit.h"
# include <qslider.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

template <class T>
class IG_CONTROL_CENTRE_API IgQtRangeControlImp : public IgQtRangeControl, public IgQtRangeValue<T>
{
public:
    typedef IgQtRangeControlImp<T> inherited;

    IgQtRangeControlImp (QWidget *parent, int parts, T value = 0,
		         const char *name = 0);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual T		stringToNumber (const QString& /*val*/) const
    { return (T)(0); }
    
    using IgQtRangeControl::setDirty;
    T			getSmallStep (void) const
    { return m_smallStep; }
    
    T			getLongStep (void) const
    { return m_longStep; }

    void  		setSmallStep (T size)
    {
      m_smallStep = size;
      if (m_longStep < m_smallStep)
	  m_longStep = m_smallStep;
      adjustSliderSteps ();
      editSettingsChanged ();
    }
    
    void		setLongStep (T size)
    {
      m_longStep = size;
      if (m_smallStep > m_longStep)
	m_smallStep = m_longStep;
      adjustSliderSteps ();
      editSettingsChanged ();
    }

    void		setRange (T min, T max)
    {
      // Like base class, just adjust the number of slider steps.
      IgQtRangeValue<T>::setRange (min, max);
      adjustSliderSteps ();
      editSettingsChanged ();
    }

    virtual void	setValue (T newval);

    void		revMin (void)
    { setValue (inherited::minValue ()); }

    void		revLong (void)
    { moveValue (m_longStep,  MoveREV); }

    void		revSmall (void)
    { moveValue (m_smallStep, MoveREV); }

    void		fwdSmall (void)
    { moveValue (m_smallStep, MoveFWD); }

    void		fwdLong (void)
    { moveValue (m_longStep,  MoveFWD); }

    void		fwdMax (void)
    { setValue (inherited::maxValue ()); }

    void		setDirty (bool dirty);

    void		sliderChanged (int value)
    { setValue ((value * m_smallStep) + inherited::minValue ()); }
    
    void	editSettings (void)
    {
      IgQtRangeSetting* setting = new IgQtRangeSetting (widget (), "setting");
      setting->setPrecision ("");
      setting->setValue (QString::number (inherited::value ()));
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
    
    QString	getSettingString (void) const
    {
      QString setting ("");
      addSettingItem (QString::number(inherited::minValue ()), setting);
      addSettingItem (QString::number(inherited::maxValue ()), setting);
      addSettingItem (QString::number(getSmallStep ()), setting);
      addSettingItem (QString::number(getLongStep ()), setting);
      return setting;
    }
    
    void	setSettingString (QString &setting)
    {
      bool oldValue = isEmitSettingsChanged ();
      setEmitSettingsChanged (false);
    
      if (!setting.isEmpty ())
      {
        T min = stringToNumber (getSettingItem (setting));
        if (!setting.isEmpty ())
          setRange (min , stringToNumber (getSettingItem (setting)));
        else
          setRange (min , inherited::maxValue ());
      }
      
      if (!setting.isEmpty ())
        setSmallStep (stringToNumber (getSettingItem (setting)));
	
      if (!setting.isEmpty ())
        setLongStep (stringToNumber (getSettingItem (setting)));
    
      setEmitSettingsChanged (oldValue);
      editSettingsChanged ();
    }
    
protected:
    virtual void	textChanged (const QString &newval)
    { setValue (stringToNumber (newval)); }

    virtual void	adjustSliderSteps (void)
    {
      if (m_slider)
      {
	disconnect (m_slider, SIGNAL (valueChanged (int)),
		    this,     SLOT (sliderChanged (int)));
	m_slider->setRange (0, int (round ((inherited::maxValue()-inherited::minValue()) / m_smallStep)));
	m_slider->setPageStep (int (round (m_longStep / m_smallStep)));
        m_slider->setValue (int (round ((inherited::value () - inherited::minValue ()) / m_smallStep)));
	connect (m_slider, SIGNAL (valueChanged (int)),
		 this,     SLOT (sliderChanged (int)));
      }
    }
    
    virtual void	moveValue (T delta, MoveDirection direction);
    
    void		fwdEnable (bool enable)
    {
      if (!(!enable && inherited::boundaryHandling () == inherited::MODULATE))
	IgQtRangeControl::fwdEnable (enable);
    }
    
    void		revEnable (bool enable)
    {
      if (!(!enable && inherited::boundaryHandling () == inherited::MODULATE))
	IgQtRangeControl::revEnable (enable);
    }

    void		initialize (int parts);

    T			m_longStep;
    T			m_smallStep;
};

template<class T> 
IgQtRangeControlImp<T>::IgQtRangeControlImp (QWidget *parent, int parts,
				             T value /* = 0 */,
		      		             const char *name /* = 0 */)
    : IgQtRangeControl  (parent, parts, name),
      IgQtRangeValue<T> (value),
      m_longStep   (5),
      m_smallStep  (1)
{ initialize (parts);}

template<class T> void
IgQtRangeControlImp<T>::initialize (int /* parts */)
{
    // Pretend to step into the value to get it properly bounded
    T oldValue = inherited::value ();
    inherited::forceValue (inherited::value () + m_smallStep);
    setValue (oldValue);
    inherited::forceValueChanged (0);
    precisionEditable (false);
}

template<class T> void
IgQtRangeControlImp<T>::setValue (T newval)
{
    // First let the base class handle it.  If the value doesn't
    // actually change, we have nothing further to do.
    //If tracking was not on then keep the accumulate the change value
    T prevChangedValue = inherited::valueChange ();
    IgQtRangeValue<T>::setValue (newval);
    if (! inherited::valueChange ())
    {
        inherited::forceValueChanged (prevChangedValue);
	return;
    }
    inherited::forceValueChanged (prevChangedValue + inherited::valueChange ());

    // Adjust the various display.
    if (m_text)
    {	
	m_text->setText (QString::number (inherited::value ()));
	m_text->setChanged (false);
    }

    if (m_slider)
    {
	disconnect (m_slider, SIGNAL (valueChanged (int)),
		    this, SLOT (sliderChanged (int)));

	m_slider->setValue (int (round ((inherited::value () - inherited::minValue ()) / m_smallStep)));

	connect (m_slider, SIGNAL (valueChanged (int)),
		 this, SLOT (sliderChanged (int)));
    }

    // Indicate where we are for various buttons and then set myself
    // dirty so they'll get updated.
    fwdEnable (inherited::maxValue () - inherited::value () >= m_smallStep);
    revEnable (inherited::value () - inherited::minValue () >= m_smallStep);
    setDirty ();
}

template<class T> void
IgQtRangeControlImp<T>::setDirty (bool dirty)
{
    if (dirty)
    {
	emitSignals ();
	IgQtRangeControl::setDirty (dirty);
	if (m_resetChangedValue)
	  inherited::forceValueChanged (0);
    }
    else
    {
        inherited::forceValueChanged (0);
	IgQtRangeControl::setDirty (dirty);
    }
}

template<class T> void
IgQtRangeControlImp<T>::moveValue (T delta, MoveDirection direction)
{
    T oldValue = inherited::value ();
    switch (m_stepType)
    {
    case ADD:
      if(direction == MoveREV)
      {
	T newValue = oldValue - delta;
	if (newValue < oldValue)
	  setValue (newValue < inherited::minValue () ? inherited::minValue (): newValue);
	else
	  setValue (inherited::minValue ());
      }
      else
      {
	T newValue = oldValue + delta;
	if (newValue > oldValue)
	  setValue (newValue > inherited::maxValue () ? inherited::maxValue (): newValue);
	else
	  setValue (inherited::maxValue ());
      }
      break;
    case MULTIPLE:
      setValue (inherited::value () * (direction == MoveREV ? 1/delta : delta));
      break;
    }
}

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_CONTROL_CENTRE_IG_QT_RANGE_CONTROL_IMP_H
