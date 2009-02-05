#ifndef IG_CONTROL_CENTRE_IG_QT_RANGE_MVALUE_LONG_H
# define IG_CONTROL_CENTRE_IG_QT_RANGE_MVALUE_LONG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgControlCentre/interface/IgQtRangeMValue.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_CONTROL_CENTRE_API IgQtRangeMValueLong : public IgQtRangeMValue<long>
{
public:
    IgQtRangeMValueLong (long value = 0,
                        long min = SHRT_MIN,
		        long max = SHRT_MAX);
    
    IgQtRangeMValueLong (unsigned int size,
                        const long* values,
                        long min = SHRT_MIN,
		        long max = SHRT_MAX);
    
    IgQtRangeMValueLong (const rangeMValues& values,
                        long min = SHRT_MIN,
		        long max = SHRT_MAX);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_CONTROL_CENTRE_IG_QT_RANGE_MVALUE_LONG_H
