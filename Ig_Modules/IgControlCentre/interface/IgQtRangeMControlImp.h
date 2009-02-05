#ifndef IG_CONTROL_CENTRE_IG_QT_RANGE_MCONTROL_IMP_H
# define IG_CONTROL_CENTRE_IG_QT_RANGE_MCONTROL_IMP_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgControlCentre/interface/IgQtRangeMValue.h"
# include "Ig_Modules/IgControlCentre/interface/IgQtRangeMControl.h"
# include "Ig_Modules/IgControlCentre/interface/IgQtRangeSetting.h"
# include "Ig_Extensions/IgQt/interface/IgQtLineEdit.h"
# include <qslider.h>
# include <qcombobox.h>
# include <math.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

template <class T>
class IG_CONTROL_CENTRE_API IgQtRangeMControlImp : public IgQtRangeMControl, public IgQtRangeMValue<T>
{
public:
    typedef std::vector<T> rangeMValues;
    typedef IgQtRangeMControlImp<T> inherited;

    IgQtRangeMControlImp (QWidget *parent, int parts,
                          T value = 0,
		          const char *name = 0);

    IgQtRangeMControlImp (QWidget *parent, int parts,
                          unsigned int size,
		          const T* values,
		          const char *name = 0);

    IgQtRangeMControlImp (QWidget *parent, int parts,
                          const rangeMValues& values,
		          const char *name = 0);

    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual T		stringToNumber (const QString& /*val*/) const
    { return (T)(0); }
    
    using IgQtRangeMControl::setDirty;
    
    void	resize (unsigned int size, T appendedValues = 0)
    {
      IgQtRangeMValue<T>::resize (size, appendedValues);
      IgQtRangeMControl::resizeIndex(size);
    }
    
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
      IgQtRangeMValue<T>::setRange (min, max);
      adjustSliderSteps ();
      editSettingsChanged ();
    }

    virtual void	setValue (T newval)
    {  setValue (m_index->currentItem (), newval); }
    
    virtual void	setValue (unsigned int index, T newval)
    {
      IgQtRangeMValue<T>::setValue (index, newval);
      if (inherited::isValueChanged (index)){
        if (int(index) == currentIndex ())
          updateTextValue ();
        setDirty ();
      }
    }
    
    virtual void	setValue (unsigned int size, const T* newvals)
    {
      IgQtRangeMValue<T>::setValue (size, newvals);
      if (inherited::isValueChanged ()){
        unsigned int index = (unsigned int)(currentIndex ());        
        if (inherited::isValueChanged (index))
          updateTextValue ();
        setDirty ();
      }
    }
    
    virtual void	setValue (const rangeMValues& newvals)
    { setValue (newvals.size (), &newvals[0]); }

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
      setting->setValue (QString::number (inherited::value (currentIndex ())));
      setting->setMin (QString::number (inherited::minValue ()));
      setting->setMax (QString::number (inherited::maxValue ()));
      setting->setSmall (QString::number (getSmallStep ()));
      setting->setLong (QString::number (getLongStep ()));
      setting->enableValue (inherited::isValueEditable ());
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
	disconnect (m_slider, SIGNAL (inherited::valueChanged (int)),
		    this,     SLOT (sliderChanged (int)));
	m_slider->setRange (0, int (::round ((inherited::maxValue()-inherited::minValue()) / m_smallStep)));
	m_slider->setPageStep (int (::round (m_longStep / m_smallStep)));
        m_slider->setValue (int (::round ((inherited::value (currentIndex ()) - inherited::minValue ()) / m_smallStep)));
	connect (m_slider, SIGNAL (inherited::valueChanged (int)),
	         this,     SLOT (sliderChanged (int)));
      }
    }
    
    virtual void	moveValue (T delta, MoveDirection dir);
    
    void		fwdEnable (bool enable)
    {
      if (!(!enable && inherited::boundaryHandling () == inherited::MODULATE))
	IgQtRangeMControl::fwdEnable (enable);
    }
    
    void		revEnable (bool enable)
    {
      if (!(!enable && inherited::boundaryHandling () == inherited::MODULATE))
	IgQtRangeMControl::revEnable (enable);
    }

    void		textValueChanged (const QString &value);
    void		updateTextValue (void);

    void		initialize (int parts);

    T			m_longStep;
    T			m_smallStep;
};

template<class T> 
IgQtRangeMControlImp<T>::IgQtRangeMControlImp (QWidget *parent, int parts,
                                               T value /* = 0 */,
		                               const char *name /*= 0*/)
    : IgQtRangeMControl  (parent, parts, 1, name),
      IgQtRangeMValue<T> (value),
      m_longStep   (5),
      m_smallStep  (1)
{ initialize (parts); }

template<class T> 
IgQtRangeMControlImp<T>::IgQtRangeMControlImp (QWidget *parent, int parts,
                                               unsigned int size,
		                               const T* values,
		                               const char *name /*= 0*/)
    : IgQtRangeMControl  (parent, parts, size, name),
      IgQtRangeMValue<T> (size, values),
      m_longStep   (5),
      m_smallStep  (1)
{ initialize (parts); }

template<class T> 
IgQtRangeMControlImp<T>::IgQtRangeMControlImp (QWidget *parent, int parts,
                          const rangeMValues& values,
		          const char *name /*= 0*/)
    : IgQtRangeMControl  (parent, parts, values.size (), name),
      IgQtRangeMValue<T> (values),
      m_longStep   (5),
      m_smallStep  (1)
{ initialize (parts); }

template<class T>
void
IgQtRangeMControlImp<T>::initialize (int /* parts */)
{
    // Pretend to step into the value to get it properly bounded
    T oldValue = inherited::value (0);
    forceValue (0, inherited::value (0) + m_smallStep);
    setValue (0, oldValue);
    inherited::forceValueChanged (0, (T)(0));
    precisionEditable (false);
}

template<class T>
void
IgQtRangeMControlImp<T>::setDirty (bool dirty)
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

template<class T> void
IgQtRangeMControlImp<T>::moveValue (T delta, MoveDirection direction)
{
    unsigned int index = (unsigned int)(m_index->currentItem());
    T oldValue = inherited::value (index);
    switch (m_stepType)
    {
    case ADD:
      if(direction == MoveREV)
      {
	T newValue = oldValue - delta;
	if (newValue < oldValue)
	  setValue (index, newValue < inherited::minValue () ? inherited::minValue (): newValue);
	else
	  setValue (index, inherited::minValue ());
      }
      else
      {
	T newValue = oldValue + delta;
	if (newValue > oldValue)
	  setValue (index, newValue > inherited::maxValue () ? inherited::maxValue (): newValue);
	else
	  setValue (index, inherited::maxValue ());
      }
      break;
    case MULTIPLE:
      setValue (index, inherited::value (index) * (direction == MoveREV ? 1/delta : delta));
      break;
    }
}

template<class T> void
IgQtRangeMControlImp<T>::updateTextValue (void)
{
    // Adjust the various display.
    T newval = inherited::value (m_index->currentItem());
    
    if (m_text)
    {	
	m_text->setText (QString::number (newval));
	m_text->setChanged (false);
    }

    if (m_slider)
    {
	disconnect (m_slider, SIGNAL (inherited::valueChanged (int)),
		    this, SLOT (sliderChanged (int)));

	m_slider->setValue (int (round ((newval - inherited::minValue ()) / m_smallStep)));

	connect (m_slider, SIGNAL (inherited::valueChanged (int)),
		 this, SLOT (sliderChanged (int)));
    }

    // Indicate where we are for various buttons and then set myself
    // dirty so they'll get updated.
    fwdEnable (inherited::maxValue () - newval >= m_smallStep);
    revEnable (newval - inherited::minValue () >= m_smallStep);
}

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_CONTROL_CENTRE_IG_QT_RANGE_MCONTROL_IMP_H
