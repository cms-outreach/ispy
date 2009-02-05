#ifndef IG_CONTROL_CENTRE_IG_QT_RANGE_MVALUE_H
# define IG_CONTROL_CENTRE_IG_QT_RANGE_MVALUE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgControlCentre/interface/config.h"
# include <climits>
# include <vector>
# include <classlib/utils/DebugAids.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

template<class T>
class IG_CONTROL_CENTRE_API IgQtRangeMValue
{
public:
    typedef IgQtRangeMValue<T> inherited;
    
    enum BoundaryHandling {
	ACCUMULATE,
	MODULATE
    };

    typedef std::vector<T> rangeMValues;
    
    IgQtRangeMValue (T value = 0,
                     T min = 0,
		     T max = SHRT_MAX);
    
    IgQtRangeMValue (unsigned int size,
                     const T* values,
                     T min = 0,
		     T max = SHRT_MAX);
    
    IgQtRangeMValue (const rangeMValues& values,
                     T min = 0,
		     T max = SHRT_MAX);
    // implicit copy constructor
    // implicit assignment operator
    virtual ~IgQtRangeMValue (void) {}
   
    unsigned int	size (void) const
    { return m_value.size(); }
    
    virtual void	resize (unsigned int size, T appendedValues = 0)
    {
      ASSERT (size);
      if (size == m_value.size ()) return;
      m_value.resize(size, appendedValues);
      m_change.resize(size, 0);
    }
    
    virtual void	setValue (unsigned int index, T value)
    {
      ASSERT ( index < m_value.size() );
      value = adjustValue (value);
      m_change[index] += value - m_value [index];
      m_value[index]  = value;
      m_valueChangeFlag = isValueChanged (index);
    }
    
    virtual void	setValue (unsigned int size, const T* values)
    {
      m_valueChangeFlag = false;
      resize (size);
      for(unsigned int i = 0; i < size; i++)
        setValue (i, values [i]);
    }
    
    virtual void	setValue (const rangeMValues& values)
    { setValue (values.size (), &values [0]); }
    
    virtual T		value (unsigned int index) const
    {
      ASSERT ( index < m_value.size() );
      return m_value [index];
    }
    
    virtual const rangeMValues&	value (void) const
    { return m_value; }
    
    virtual T 		valueChange (unsigned int index) const
    {
      ASSERT ( index < m_value.size() );
      return m_change[index];
    }
    
    virtual const rangeMValues& valueChange (void) const
    { return m_change; }
    
    virtual bool	isValueChanged (void) const
    { return m_valueChangeFlag; }
    
    virtual bool	isValueChanged (unsigned int index) const
    { return valueChange (index) != 0 ? true : false; }

    virtual void	setRange (T min, T max)
    {
      if (min == m_min && max == m_max)
        return;
      else if (min > max)
	std::swap (min, max);
      m_max = max;
      m_min = min;
      setValue (m_value);
    }
    
    virtual T	minValue (void) const
    { return m_min; }
    
    virtual void	setMinValue (T min)
    { setRange (min, maxValue ()); }
    
    virtual T	maxValue (void) const
    { return m_max; }
    
    virtual void	setMaxValue (T max)
    { setRange (max, maxValue ()); }

    virtual BoundaryHandling boundaryHandling (void) const
    { return m_boundary; }

    virtual void	setBoundaryHandling (BoundaryHandling mode)
    { 
      if (m_boundary != mode)
      { m_boundary = mode; setValue (m_value); }
    }

protected:
    T		modulate (T value) const
    {
      T width = m_max - m_min;
      value -= int ((value - m_min) / width) * width;
      value += int ((m_max - value) / width) * width;
      return value;
    }
    
    T		accumulate (T value) const
    {
      if (value > m_max)
	return m_max;
      else if (value < m_min)
	return m_min;
      return value;
    }

    T  adjustValue (T value)
    {
      switch (boundaryHandling ())
      {
      case ACCUMULATE:
	value = accumulate (value);
	break;

      case MODULATE:
	value = modulate (value);
	break;
      }
      return value;
    }

    virtual void   	forceValue (T value)
    {
      unsigned int size = m_value.size ();
      for (unsigned int i = 0; i < size; i++)
        m_value [i] = value;
    }
    
    virtual void   	forceValue (unsigned int index, T value)
    {
      ASSERT ( index < m_value.size() );
      m_value [index] = value;
    }
    
    virtual void   	forceValue (unsigned int size, const T* values)
    {
      for (unsigned int i = 0; i < size; i++)
        forceValue (i, values[i]);
    }
    
    virtual void   	forceValue (const rangeMValues& values)
    {
      unsigned int size = values.size ();
      for (unsigned int i = 0; i < size; i++)
        forceValue (i, values[i]);
    }
    
    virtual void   	forceValueChanged (T delta)
    {
      unsigned int size = m_change.size ();
      for (unsigned int i = 0; i < size; i++)
        m_change [i] = delta;
      m_valueChangeFlag = delta ? true : false;
    }
    
    virtual void   	forceValueChanged (unsigned int index, T delta)
    {
      ASSERT ( index < m_change.size() );
      m_change [index] = delta;
      if(delta != 0)
        m_valueChangeFlag = true;
    }
    
    virtual void   	forceValueChanged (unsigned int size, const T* delta)
    {
      m_valueChangeFlag = false;
      for (unsigned int i = 0; i < size; i++)
        forceValueChanged (i, delta[i]);
    }
    
    virtual void   	forceValueChanged (const rangeMValues& delta)
    {
      unsigned int size = delta.size ();
      m_valueChangeFlag = false;
      for (unsigned int i = 0; i < size; i++)
        forceValueChanged (i, delta[i]);
    }

protected:

    rangeMValues	m_value;
    rangeMValues	m_change;
    T			m_min;
    T			m_max;
    BoundaryHandling	m_boundary;
    bool		m_valueChangeFlag;
};

template<class T>
IgQtRangeMValue<T>::IgQtRangeMValue (T value,
                                     T min /* = 0 */,
				     T max /* = SHRT_MAX */)
    : m_value (1, value),
      m_change (1, value),
      m_min (min),
      m_max (max),
      m_boundary (ACCUMULATE),
      m_valueChangeFlag (false)
{ setValue (m_value); }

template<class T>
IgQtRangeMValue<T>::IgQtRangeMValue (unsigned int size,
				     const T* values,
                                     T min /* = 0 */,
				     T max /* = SHRT_MAX */)
    : m_value (size, 0),
      m_change (size, 0),
      m_min (min),
      m_max (max),
      m_boundary (ACCUMULATE),
      m_valueChangeFlag (false)
{ 
    ASSERT (size);
    setValue (size, values);
}

template<class T>
IgQtRangeMValue<T>::IgQtRangeMValue (const rangeMValues& values,
                                     T min /* = 0 */,
				     T max /* = SHRT_MAX */)
    : m_value (values.size (), 0),
      m_change (values.size (), 0),
      m_min (min),
      m_max (max),
      m_boundary (ACCUMULATE),
      m_valueChangeFlag (false)
{
    ASSERT (values.size());
    setValue (values);
}

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_CONTROL_CENTRE_IG_QT_RANGE_MVALUE_H
