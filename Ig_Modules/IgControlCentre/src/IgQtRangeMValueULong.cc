//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgControlCentre/interface/IgQtRangeMValueULong.h"

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

IgQtRangeMValueULong::IgQtRangeMValueULong (unsigned long value /* = 0 */,
                                        unsigned long min /* = 0 */,
				        unsigned long max /* = SHRT_MAX */)
    : inherited (value, min, max)
{ }

IgQtRangeMValueULong::IgQtRangeMValueULong (unsigned int size,
				  const unsigned long* values,
                                  unsigned long min /* = 0 */,
				  unsigned long max /* = SHRT_MAX */)
    : inherited (size, values, min, max)
{ }

IgQtRangeMValueULong::IgQtRangeMValueULong (const rangeMValues& values,
                                  unsigned long min /* = 0 */,
				  unsigned long max /* = SHRT_MAX */)
    : inherited (values, min, max)
{ }
