#ifndef IG_CONTROL_CENTRE_IG_QT_RANGE_MVALUE_FLOAT_H
# define IG_CONTROL_CENTRE_IG_QT_RANGE_MVALUE_FLOAT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgControlCentre/interface/IgQtRangeMValue.h"
# include "Ig_Modules/IgControlCentre/interface/IgQtRangeValuePrecision.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_CONTROL_CENTRE_API IgQtRangeMValueFloat : public IgQtRangeMValue<float>,
						   public IgQtRangeValuePrecision
{
public:
    IgQtRangeMValueFloat (float value = 0.,
                          float min = SHRT_MIN,
		          float max = SHRT_MAX);
    
    IgQtRangeMValueFloat (unsigned int size,
                          const float* values,
                          float min = SHRT_MIN,
		          float max = SHRT_MAX);
    
    IgQtRangeMValueFloat (const rangeMValues& values,
                          float min = SHRT_MIN,
		          float max = SHRT_MAX);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    using inherited::setValue;
    virtual void	setValue (unsigned int index, float value);
    
    using inherited::isValueChanged;
    virtual bool	isValueChanged (unsigned int index) const;

    float		roundOff (float number, float precision = .001);

protected:
    using inherited::forceValueChanged;
    void   		forceValueChanged (float delta);
    void   		forceValueChanged (unsigned int index, float delta);
};


//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_CONTROL_CENTRE_IG_QT_RANGE_VALUE_FLOAT_H
