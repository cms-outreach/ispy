#ifndef IG_CONTROL_CENTRE_IG_QT_RANGE_VALUE_LONG_H
# define IG_CONTROL_CENTRE_IG_QT_RANGE_VALUE_LONG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgControlCentre/interface/IgQtRangeValue.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_CONTROL_CENTRE_API IgQtRangeValueLong : public IgQtRangeValue<long>
{
public:
    IgQtRangeValueLong  (long value = 0,
		        long min = SHRT_MIN,
		        long max = SHRT_MAX);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_CONTROL_CENTRE_IG_QT_RANGE_VALUE_LONG_H
