#ifndef IG_CONTROL_CENTRE_IG_QT_RANGE_MVALUE_SHORT_H
# define IG_CONTROL_CENTRE_IG_QT_RANGE_MVALUE_SHORT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgControlCentre/interface/IgQtRangeMValue.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_CONTROL_CENTRE_API IgQtRangeMValueShort : public IgQtRangeMValue<short>
{
public:
    IgQtRangeMValueShort (short value = 0,
                        short min = SHRT_MIN,
		        short max = SHRT_MAX);
    
    IgQtRangeMValueShort (unsigned int size,
                        const short* values,
                        short min = SHRT_MIN,
		        short max = SHRT_MAX);
    
    IgQtRangeMValueShort (const rangeMValues& values,
                        short min = SHRT_MIN,
		        short max = SHRT_MAX);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_CONTROL_CENTRE_IG_QT_RANGE_MVALUE_SHORT_H
