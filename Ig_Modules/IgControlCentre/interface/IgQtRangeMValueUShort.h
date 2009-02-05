#ifndef IG_CONTROL_CENTRE_IG_QT_RANGE_MVALUE_USHORT_H
# define IG_CONTROL_CENTRE_IG_QT_RANGE_MVALUE_USHORT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgControlCentre/interface/IgQtRangeMValue.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_CONTROL_CENTRE_API IgQtRangeMValueUShort : public IgQtRangeMValue<unsigned short>
{
public:
    IgQtRangeMValueUShort (unsigned short value = 0,
                        unsigned short min = 0,
		        unsigned short max = SHRT_MAX);
    
    IgQtRangeMValueUShort (unsigned int size,
                        const unsigned short* values,
                        unsigned short min = 0,
		        unsigned short max = SHRT_MAX);
    
    IgQtRangeMValueUShort (const rangeMValues& values,
                        unsigned short min = 0,
		        unsigned short max = SHRT_MAX);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_CONTROL_CENTRE_IG_QT_RANGE_MVALUE_USHORT_H
