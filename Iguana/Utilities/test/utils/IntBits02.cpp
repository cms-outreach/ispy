#include "test/Test.h" 
#include "classlib/utils/IntBits.h"
#include "classlib/utils/IntTraits.h"
#include "classlib/utils/Signal.h"
#include <typeinfo>
#include <iostream>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    std::cout << "8  :"
	      << "\n  fast = (" << typeid(IntBits<8>::UFast).name ()
	      << "/" << typeid(IntBits<8>::SFast).name ()
	      << " smin=" << (int) IntTraits<IntBits<8>::SFast>::Min
	      << " smax=" << (int) IntTraits<IntBits<8>::SFast>::Max
	      << " umin=" << (int) IntTraits<IntBits<8>::UFast>::Min
	      << " umax=" << (int) IntTraits<IntBits<8>::UFast>::Max
	      << ")\n  exact = (" << typeid(IntBits<8>::UExact).name ()
	      << "/" << typeid(IntBits<8>::SExact).name ()
	      << " smin=" << (int) IntTraits<IntBits<8>::SExact>::Min
	      << " smax=" << (int) IntTraits<IntBits<8>::SExact>::Max
	      << " umin=" << (int) IntTraits<IntBits<8>::UExact>::Min
	      << " umax=" << (int) IntTraits<IntBits<8>::UExact>::Max
	      << ")\n  least = (" << typeid(IntBits<8>::ULeast).name ()
	      << "/" << typeid(IntBits<8>::SLeast).name ()
	      << " smin=" << (int) IntTraits<IntBits<8>::SLeast>::Min
	      << " smax=" << (int) IntTraits<IntBits<8>::SLeast>::Max
	      << " umin=" << (int) IntTraits<IntBits<8>::ULeast>::Min
	      << " umax=" << (int) IntTraits<IntBits<8>::ULeast>::Max
	      << ")\n";

    std::cout << "16  :"
	      << "\n  fast = (" << typeid(IntBits<16>::UFast).name ()
	      << "/" << typeid(IntBits<16>::SFast).name ()
	      << " smin=" << IntTraits<IntBits<16>::SFast>::Min
	      << " smax=" << IntTraits<IntBits<16>::SFast>::Max
	      << " umin=" << IntTraits<IntBits<16>::UFast>::Min
	      << " umax=" << IntTraits<IntBits<16>::UFast>::Max
	      << ")\n  exact = (" << typeid(IntBits<16>::UExact).name ()
	      << "/" << typeid(IntBits<16>::SExact).name ()
	      << " smin=" << IntTraits<IntBits<16>::SExact>::Min
	      << " smax=" << IntTraits<IntBits<16>::SExact>::Max
	      << " umin=" << IntTraits<IntBits<16>::UExact>::Min
	      << " umax=" << IntTraits<IntBits<16>::UExact>::Max
	      << ")\n  least = (" << typeid(IntBits<16>::ULeast).name ()
	      << "/" << typeid(IntBits<16>::SLeast).name ()
	      << " smin=" << IntTraits<IntBits<16>::SLeast>::Min
	      << " smax=" << IntTraits<IntBits<16>::SLeast>::Max
	      << " umin=" << IntTraits<IntBits<16>::ULeast>::Min
	      << " umax=" << IntTraits<IntBits<16>::ULeast>::Max
	      << ")\n";

    std::cout << "32  :"
	      << "\n  fast = (" << typeid(IntBits<32>::UFast).name ()
	      << "/" << typeid(IntBits<32>::SFast).name ()
	      << " smin=" << IntTraits<IntBits<32>::SFast>::Min
	      << " smax=" << IntTraits<IntBits<32>::SFast>::Max
	      << " umin=" << IntTraits<IntBits<32>::UFast>::Min
	      << " umax=" << IntTraits<IntBits<32>::UFast>::Max
	      << ")\n  exact = (" << typeid(IntBits<32>::UExact).name ()
	      << "/" << typeid(IntBits<32>::SExact).name ()
	      << " smin=" << IntTraits<IntBits<32>::SExact>::Min
	      << " smax=" << IntTraits<IntBits<32>::SExact>::Max
	      << " umin=" << IntTraits<IntBits<32>::UExact>::Min
	      << " umax=" << IntTraits<IntBits<32>::UExact>::Max
	      << ")\n  least = (" << typeid(IntBits<32>::ULeast).name ()
	      << "/" << typeid(IntBits<32>::SLeast).name ()
	      << " smin=" << IntTraits<IntBits<32>::SLeast>::Min
	      << " smax=" << IntTraits<IntBits<32>::SLeast>::Max
	      << " umin=" << IntTraits<IntBits<32>::ULeast>::Min
	      << " umax=" << IntTraits<IntBits<32>::ULeast>::Max
	      << ")\n";

#ifndef _WIN32
    std::cout << "64  :"
	      << "\n  fast = (" << typeid(IntBits<64>::UFast).name ()
	      << "/" << typeid(IntBits<64>::SFast).name ()
	      << " smin=" << IntTraits<IntBits<64>::SFast>::Min
	      << " smax=" << IntTraits<IntBits<64>::SFast>::Max
	      << " umin=" << IntTraits<IntBits<64>::UFast>::Min
	      << " umax=" << IntTraits<IntBits<64>::UFast>::Max
	      << ")\n  exact = (" << typeid(IntBits<64>::UExact).name ()
	      << "/" << typeid(IntBits<64>::SExact).name ()
	      << " smin=" << IntTraits<IntBits<64>::SExact>::Min
	      << " smax=" << IntTraits<IntBits<64>::SExact>::Max
	      << " umin=" << IntTraits<IntBits<64>::UExact>::Min
	      << " umax=" << IntTraits<IntBits<64>::UExact>::Max
	      << ")\n  least = (" << typeid(IntBits<64>::ULeast).name ()
	      << "/" << typeid(IntBits<64>::SLeast).name ()
	      << " smin=" << IntTraits<IntBits<64>::SLeast>::Min
	      << " smax=" << IntTraits<IntBits<64>::SLeast>::Max
	      << " umin=" << IntTraits<IntBits<64>::ULeast>::Min
	      << " umax=" << IntTraits<IntBits<64>::ULeast>::Max
	      << ")\n";
#endif
    return 0;
}

