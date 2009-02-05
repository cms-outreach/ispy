#ifndef IG_CONTROL_CENTRE_IG_QT_RANGE_VALUE_INT_H
# define IG_CONTROL_CENTRE_IG_QT_RANGE_VALUE_INT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgControlCentre/interface/IgQtRangeValue.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_CONTROL_CENTRE_API IgQtRangeValueInt : public IgQtRangeValue<int>
{
public:
    IgQtRangeValueInt  (int value = 0,
		        int min = SHRT_MIN,
		        int max = SHRT_MAX);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_CONTROL_CENTRE_IG_QT_RANGE_VALUE_INT_H
