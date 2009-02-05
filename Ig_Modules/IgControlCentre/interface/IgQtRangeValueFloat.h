#ifndef IG_CONTROL_CENTRE_IG_QT_RANGE_VALUE_FLOAT_H
# define IG_CONTROL_CENTRE_IG_QT_RANGE_VALUE_FLOAT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgControlCentre/interface/IgQtRangeValue.h"
# include "Ig_Modules/IgControlCentre/interface/IgQtRangeValuePrecision.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_CONTROL_CENTRE_API IgQtRangeValueFloat : public IgQtRangeValue<float>,
						  public IgQtRangeValuePrecision
{
public:
    IgQtRangeValueFloat (float value = 0,
		         float min = SHRT_MIN,
		         float max = SHRT_MAX);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    
    virtual void	setValue (float value);
    virtual bool	isValueChanged (void) const;

    float		roundOff (float number, float precision = .001);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_CONTROL_CENTRE_IG_QT_RANGE_VALUE_FLOAT_H
