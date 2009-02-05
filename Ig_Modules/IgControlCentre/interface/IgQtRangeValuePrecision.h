#ifndef IG_CONTROL_CENTRE_IG_QT_RANGE_VALUE_PRECISION_H
# define IG_CONTROL_CENTRE_IG_QT_RANGE_VALUE_PRECISION_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgControlCentre/interface/config.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_CONTROL_CENTRE_API IgQtRangeValuePrecision
{
public:
    IgQtRangeValuePrecision (float precision = 0.001);
    
    virtual float	precision (void) const;
    virtual void	setPrecision (float precision);
    
    static float 	minimumPrecision (void);

private:
    float		m_precision;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_CONTROL_CENTRE_IG_QT_RANGE_VALUE_PRECISION_H
