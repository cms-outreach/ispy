//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgControlCentre/interface/IgQtRangeMValueUShort.h"

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

IgQtRangeMValueUShort::IgQtRangeMValueUShort (unsigned short value /* = 0 */,
                                        unsigned short min /* = 0 */,
				        unsigned short max /* = SHRT_MAX */)
    : inherited (value, min, max)
{ }

IgQtRangeMValueUShort::IgQtRangeMValueUShort (unsigned int size,
				  const unsigned short* values,
                                  unsigned short min /* = 0 */,
				  unsigned short max /* = SHRT_MAX */)
    : inherited (size, values, min, max)
{ }

IgQtRangeMValueUShort::IgQtRangeMValueUShort (const rangeMValues& values,
                                  unsigned short min /* = 0 */,
				  unsigned short max /* = SHRT_MAX */)
    : inherited (values, min, max)
{ }
