//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgControlCentre/interface/IgQtRangeValueFloat.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

IgQtRangeValueFloat::IgQtRangeValueFloat (float value /* = 0 */,
				          float min /* = SHRT_MIN */,
				          float max /* = SHRT_MAX */)
    : inherited (value, min, max),
      IgQtRangeValuePrecision (.01)
{ }

void
IgQtRangeValueFloat::setValue (float value)
{
    value    = roundOff (adjustValue (value), precision ());
    forceValueChanged (value - inherited::value ());
    forceValue (value);
}

bool
IgQtRangeValueFloat::isValueChanged (void) const
{ return fabs (valueChange ()) >= precision (); }


//////////////////////////////////////////////////////////////////////
float
IgQtRangeValueFloat::roundOff (float number, float precision /* = .0001 */)
{
    if ((precision = fabs (precision)) > 0)
    {
	if (precision < minimumPrecision ())
	    precision = minimumPrecision ();

	if (precision < 1.0)
	  number =  minValue () + round ((number - minValue ()) / precision) * precision;
	else
	  number = round (number);
    }
    return number;
}
