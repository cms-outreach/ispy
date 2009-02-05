//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgControlCentre/interface/IgQtRangeMValueFloat.h"
#include <algorithm>
#include <cmath>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQtRangeMValueFloat::IgQtRangeMValueFloat (float value /* = 0. */,
                                           float min /* = SHRT_MIN */,
				           float max /* = SHRT_MAX */)
    : inherited (value, min, max),
      IgQtRangeValuePrecision (.01)
{ }

IgQtRangeMValueFloat::IgQtRangeMValueFloat (unsigned int size,
				  const float* values,
                                  float min /* = SHRT_MIN */,
				  float max /* = SHRT_MAX */)
    : inherited (size, values, min, max),
      IgQtRangeValuePrecision (.01)
{ }

IgQtRangeMValueFloat::IgQtRangeMValueFloat (const rangeMValues& values,
                                  float min /* = SHRT_MIN */,
				  float max /* = SHRT_MAX */)
    : inherited (values, min, max),
      IgQtRangeValuePrecision (.01)
{ }

void
IgQtRangeMValueFloat::setValue (unsigned int index, float value)
{
    ASSERT ( index < m_value.size() );
    value    = roundOff (adjustValue (value), precision ());
    m_change[index] += value - m_value [index];
    m_value[index]  = value;
    m_valueChangeFlag = isValueChanged (index);
}

bool
IgQtRangeMValueFloat::isValueChanged (unsigned int index) const
{ return fabs (valueChange (index)) >= precision (); }

//////////////////////////////////////////////////////////////////////
float
IgQtRangeMValueFloat::roundOff (float number, float precision /* = .0001 */)
{
    if ((precision = fabs (precision)) > 0)
    {
	if (precision < minimumPrecision ())
	    precision = minimumPrecision ();

	if (precision < 1.0)
	  number =  m_min + round ((number - m_min) / precision) * precision;
	else
	  number = round (number);
    }
    return number;
}

//////////////////////////////////////////////////////////////////////
void
IgQtRangeMValueFloat::forceValueChanged (float delta)
{
    unsigned int size = m_change.size ();
    for (unsigned int i = 0; i < size; i++)
      m_change [i] = delta;
    
    if (fabs (delta) >= precision ())
      m_valueChangeFlag = true;
    else
      m_valueChangeFlag = false;
}

void
IgQtRangeMValueFloat::forceValueChanged (unsigned int index, float delta)
{
    ASSERT ( index < m_change.size() );
    m_change [index] = delta;
    if (fabs (delta) >= precision ())
      m_valueChangeFlag = true;
}
