#include "test/Test.h" 
#include "classlib/utils/IntBits.h"
#include "classlib/utils/Signal.h"
#include <typeinfo>
#include <iostream>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    std::cout << "8 :"
	      << " min=" << CLASSLIB_INT8_MIN
	      << " max=" << CLASSLIB_INT8_MAX
	      << " umax=" << CLASSLIB_UINT8_MAX
	      << "\n";
	
    std::cout << "16 :"
	      << " min=" << CLASSLIB_INT16_MIN
	      << " max=" << CLASSLIB_INT16_MAX
	      << " umax=" << CLASSLIB_UINT16_MAX
	      << "\n";
	
    std::cout << "32 :"
	      << " min=" << CLASSLIB_INT32_MIN
	      << " max=" << CLASSLIB_INT32_MAX
	      << " umax=" << CLASSLIB_UINT32_MAX
	      << "\n";
	
    std::cout << "64 :"
	      << " min=" << CLASSLIB_INT64_MIN
	      << " max=" << CLASSLIB_INT64_MAX
	      << " umax=" << CLASSLIB_UINT64_MAX
	      << "\n";

    return 0;
}

