//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgControlCentre/interface/IgQtRangeMValueLong.h"

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

IgQtRangeMValueLong::IgQtRangeMValueLong (long value /* = 0 */,
                                        long min /* = SHRT_MIN */,
				        long max /* = SHRT_MAX */)
    : inherited (value, min, max)
{ }

IgQtRangeMValueLong::IgQtRangeMValueLong (unsigned int size,
				  const long* values,
                                  long min /* = SHRT_MIN */,
				  long max /* = SHRT_MAX */)
    : inherited (size, values, min, max)
{ }

IgQtRangeMValueLong::IgQtRangeMValueLong (const rangeMValues& values,
                                  long min /* = SHRT_MIN */,
				  long max /* = SHRT_MAX */)
    : inherited (values, min, max)
{ }
