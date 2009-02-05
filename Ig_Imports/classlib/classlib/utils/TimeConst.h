#ifndef CLASSLIB_TIME_CONST_H
# define CLASSLIB_TIME_CONST_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/utils/IntBits.h"

namespace lat {
namespace TimeConst {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

    /** Symbolic names for months */
    enum Months {
	January		= 0,
	February	= 1,
	March		= 2,
	April		= 3,
	May		= 4,
	June		= 5,
	July		= 6,
	August		= 7,
	September	= 8,
	October		= 9,
	November	= 10,
	December	= 11
    };

    /** The value type of the time and time span.  This is at least 64
        bits which guarantees ~585 years of coverage (actually ~290
    	years since the type is signed).  We use this instead of fixed
    	point arithmetic to minimise nasty code: the compiler already
    	does it anyway.  (FIXME: Bias not 1970?)  */
    typedef IntBits<64>::SLeast TimeValue;

    /** Seconds in 24 hours.  */
    static const int		SECS_PER_DAY = 86400;

    /** Seconds in one hour hour.  */
    static const int		SECS_PER_HOUR = 3600;

    /** Nanoseconds in one second.  */
    static const TimeValue	SEC_NSECS = 1000000000;

//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace TimeConst
} // namespace lat
#endif // CLASSLIB_TIME_CONST_H
