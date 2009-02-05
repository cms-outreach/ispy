#ifndef IG_CONTROL_CENTRE_IG_QT_RANGE_VALUE_H
# define IG_CONTROL_CENTRE_IG_QT_RANGE_VALUE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgControlCentre/interface/config.h"
# include <qstring.h>
# include <climits>
# include <algorithm>
# include <cmath>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

template<class T>
class IG_CONTROL_CENTRE_API IgQtRangeValue
{
public:
    typedef IgQtRangeValue<T> inherited;
    
    enum BoundaryHandling {
	ACCUMULATE,
	MODULATE
    };

    IgQtRangeValue (T value = 0,
		    T min = 0,
		    T max = SHRT_MAX);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual T		value (void) const
    { return m_value; }
    
    virtual void	setValue (T value)
    {
      value = adjustValue (value);
      m_change = value - m_value;
      m_value = value;
    }
    
    virtual T		valueChange (void) const
    { return m_change; }
    
    virtual bool	isValueChanged (void) const
    { return valueChange () ? true : false; }

    virtual void	setRange (T min, T max)
    {
      if (min > max)
	std::swap (min, max);
      m_max = max; m_min = min;
      setValue (m_value);
    }

    virtual T		minValue (void) const
    { return m_min; }
    
    virtual void	setMinValue (T value)
    { setRange (value, maxValue ()); }
    
    virtual T		maxValue (void) const
    { return m_max; }
    
    virtual void	setMaxValue (T value)
    { setRange (minValue (), value); }

    virtual BoundaryHandling boundaryHandling (void) const
    { return m_boundary; }
    
    virtual void	setBoundaryHandling (BoundaryHandling mode)
    { m_boundary = mode; setValue (m_value); }
    
protected:
    T	modulate (T value) const
    {
      T width = m_max - m_min;
      value -= int ((value - m_min) / width) * width;
      value += int ((m_max - value) / width) * width;
      return value;
    }
    
    T	accumulate (T value) const
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

    void   forceValue (T value)
    { m_value = value; }
    
    void   		forceValueChanged (T delta)
    { m_change = delta; }

private:
    T			m_value;
    T			m_change;
    T			m_min;
    T			m_max;
    BoundaryHandling	m_boundary;
};

template<class T>
IgQtRangeValue<T>::IgQtRangeValue (T value /* = 0 */,
				   T min /* = 0 */,
				   T max /* = SHRT_MAX */)
    : m_value (value),
      m_change (0),
      m_min (min),
      m_max (max),
      m_boundary (ACCUMULATE)
{ setValue (value); }

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_CONTROL_CENTRE_IG_QT_RANGE_VALUE_H
