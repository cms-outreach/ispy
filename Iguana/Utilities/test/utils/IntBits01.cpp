#include "test/Test.h" 
#include "classlib/utils/IntBits.h"
#include "classlib/utils/Signal.h"
#include <typeinfo>
#include <iostream>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    std::cout << "8  : "
	      << "fast = (" << typeid(IntBits<8>::UFast).name ()
	      << "), exact = (" << typeid(IntBits<8>::UExact).name ()
	      << "), least = (" << typeid(IntBits<8>::ULeast).name ()
	      << ")\n";

    std::cout << "16 : "
	      << "fast = (" << typeid(IntBits<16>::UFast).name ()
	      << "), exact = (" << typeid(IntBits<16>::UExact).name ()
	      << "), least = (" << typeid(IntBits<16>::ULeast).name ()
	      << ")\n";

    std::cout << "32 : "
	      << "fast = (" << typeid(IntBits<32>::UFast).name ()
	      << "), exact = (" << typeid(IntBits<32>::UExact).name ()
	      << "), least = (" << typeid(IntBits<32>::ULeast).name ()
	      << ")\n";

    // FIXME: conditionalise for systems without a 64-bit type?  most
    // systems have one though -- and we can work harder to find one.
    std::cout << "64 : "
	      << "fast = (" << typeid(IntBits<64>::UFast).name ()
	      << "), exact = (" << typeid(IntBits<64>::UExact).name ()
	      << "), least = (" << typeid(IntBits<64>::ULeast).name ()
	      << ")\n";

    return 0;
}
