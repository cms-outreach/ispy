//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgControlCentre/interface/IgQtRangeValuePrecision.h"
# include <cmath>

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

IgQtRangeValuePrecision::IgQtRangeValuePrecision (float precision)
{ setPrecision (precision); }

void
IgQtRangeValuePrecision::setPrecision (float value)
{
    if ((value = fabs (value)) > 0 && value < minimumPrecision ())
	value = minimumPrecision ();

    m_precision = value;
}

float
IgQtRangeValuePrecision::precision (void) const
{ return m_precision; }

float
IgQtRangeValuePrecision::minimumPrecision (void)
{ return 0.00001; }
