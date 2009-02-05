#ifndef IG_CONTROL_CENTRE_IG_QT_RANGE_MVALUE_INT_H
# define IG_CONTROL_CENTRE_IG_QT_RANGE_MVALUE_INT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgControlCentre/interface/IgQtRangeMValue.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_CONTROL_CENTRE_API IgQtRangeMValueInt : public IgQtRangeMValue<int>
{
public:
    IgQtRangeMValueInt (int value = 0,
                        int min = SHRT_MIN,
		        int max = SHRT_MAX);
    
    IgQtRangeMValueInt (unsigned int size,
                        const int* values,
                        int min = SHRT_MIN,
		        int max = SHRT_MAX);
    
    IgQtRangeMValueInt (const rangeMValues& values,
                        int min = SHRT_MIN,
		        int max = SHRT_MAX);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_CONTROL_CENTRE_IG_QT_RANGE_MVALUE_INT_H
