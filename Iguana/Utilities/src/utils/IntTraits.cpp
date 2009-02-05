//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/utils/IntTraits.h"

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

#if !CLASSLIB_BROKEN_TEMPLATE_CONSTANTS
const unsigned char		IntTraits<unsigned char>::Min;
const unsigned char		IntTraits<unsigned char>::Max;
const signed char		IntTraits<signed char>::Min;
const signed char		IntTraits<signed char>::Max;
const char			IntTraits<char>::Min;
const char			IntTraits<char>::Max;

const unsigned short		IntTraits<unsigned short>::Min;
const unsigned short		IntTraits<unsigned short>::Max;
const signed short		IntTraits<signed short>::Min;
const signed short		IntTraits<signed short>::Max;

const unsigned int		IntTraits<unsigned int>::Min;
const unsigned int		IntTraits<unsigned int>::Max;
const signed int		IntTraits<signed int>::Min;
const signed int		IntTraits<signed int>::Max;

const unsigned long		IntTraits<unsigned long>::Min;
const unsigned long		IntTraits<unsigned long>::Max;
const signed long		IntTraits<signed long>::Min;
const signed long		IntTraits<signed long>::Max;

# if HAVE_LONG_LONG
const unsigned long long	IntTraits<unsigned long long>::Min;
const unsigned long long	IntTraits<unsigned long long>::Max;
const signed long long		IntTraits<signed long long>::Min;
const signed long long		IntTraits<signed long long>::Max;
# elif HAVE_INT64
const unsigned __int64		IntTraits<unsigned __int64>::Min;
const unsigned __int64		IntTraits<unsigned __int64>::Max;
const signed __int64		IntTraits<signed __int64>::Min;
const signed __int64		IntTraits<signed __int64>::Max;
# endif
#endif // !CLASSLIB_BROKEN_TEMPLATE_CONSTANTS

} // namespace lat
