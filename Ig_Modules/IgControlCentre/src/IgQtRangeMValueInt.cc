//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgControlCentre/interface/IgQtRangeMValueInt.h"

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

IgQtRangeMValueInt::IgQtRangeMValueInt (int value /* = 0 */,
                                        int min /* = SHRT_MIN */,
				        int max /* = SHRT_MAX */)
    : inherited (value, min, max)
{ }

IgQtRangeMValueInt::IgQtRangeMValueInt (unsigned int size,
				  const int* values,
                                  int min /* = SHRT_MIN */,
				  int max /* = SHRT_MAX */)
    : inherited (size, values, min, max)
{ }

IgQtRangeMValueInt::IgQtRangeMValueInt (const rangeMValues& values,
                                  int min /* = SHRT_MIN */,
				  int max /* = SHRT_MAX */)
    : inherited (values, min, max)
{ }
