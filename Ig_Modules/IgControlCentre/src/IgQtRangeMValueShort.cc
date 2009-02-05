//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgControlCentre/interface/IgQtRangeMValueShort.h"

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

IgQtRangeMValueShort::IgQtRangeMValueShort (short value /* = 0 */,
                                        short min /* = SHRT_MIN */,
				        short max /* = SHRT_MAX */)
    : inherited (value, min, max)
{ }

IgQtRangeMValueShort::IgQtRangeMValueShort (unsigned int size,
				  const short* values,
                                  short min /* = SHRT_MIN */,
				  short max /* = SHRT_MAX */)
    : inherited (size, values, min, max)
{ }

IgQtRangeMValueShort::IgQtRangeMValueShort (const rangeMValues& values,
                                  short min /* = SHRT_MIN */,
				  short max /* = SHRT_MAX */)
    : inherited (values, min, max)
{ }
