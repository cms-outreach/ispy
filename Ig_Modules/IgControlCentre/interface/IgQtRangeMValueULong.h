#ifndef IG_CONTROL_CENTRE_IG_QT_RANGE_MVALUE_ULONG_H
# define IG_CONTROL_CENTRE_IG_QT_RANGE_MVALUE_ULONG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgControlCentre/interface/IgQtRangeMValue.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_CONTROL_CENTRE_API IgQtRangeMValueULong : public IgQtRangeMValue<unsigned long>
{
public:
    IgQtRangeMValueULong (unsigned long value = 0,
                        unsigned long min = 0,
		        unsigned long max = SHRT_MAX);
    
    IgQtRangeMValueULong (unsigned int size,
                        const unsigned long* values,
                        unsigned long min = 0,
		        unsigned long max = SHRT_MAX);
    
    IgQtRangeMValueULong (const rangeMValues& values,
                        unsigned long min = 0,
		        unsigned long max = SHRT_MAX);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_CONTROL_CENTRE_IG_QT_RANGE_MVALUE_ULONG_H
