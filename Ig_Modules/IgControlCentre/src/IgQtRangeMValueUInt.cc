//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgControlCentre/interface/IgQtRangeMValueUInt.h"

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

IgQtRangeMValueUInt::IgQtRangeMValueUInt (unsigned int value /* = 0 */,
                                        unsigned int min /* = 0 */,
				        unsigned int max /* = SHRT_MAX */)
    : inherited (value, min, max)
{ }

IgQtRangeMValueUInt::IgQtRangeMValueUInt (unsigned int size,
				  const unsigned int* values,
                                  unsigned int min /* = 0 */,
				  unsigned int max /* = SHRT_MAX */)
    : inherited (size, values, min, max)
{ }

IgQtRangeMValueUInt::IgQtRangeMValueUInt (const rangeMValues& values,
                                  unsigned int min /* = 0 */,
				  unsigned int max /* = SHRT_MAX */)
    : inherited (values, min, max)
{ }
