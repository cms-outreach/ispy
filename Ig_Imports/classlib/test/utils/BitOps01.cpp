#include "test/Test.h" 
#include "classlib/utils/BitOps.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    std::cout << "zero bits on the right of:\n"
	      << "    0: " << BitOps<unsigned int>::rzero (0) << std::endl
	      << "    1: " << BitOps<unsigned int>::rzero (1) << std::endl
	      << "    8: " << BitOps<unsigned int>::rzero (8) << std::endl
	      << "  781: " << BitOps<unsigned int>::rzero (781) << std::endl
	      << " 4096: " << BitOps<unsigned int>::rzero (4096) << std::endl;

    std::cout << std::endl;
    std::cout << "log 2 of:\n"
	      << "    0: " << BitOps<unsigned int>::log2 (0) << std::endl
	      << "    1: " << BitOps<unsigned int>::log2 (1) << std::endl
	      << "    2: " << BitOps<unsigned int>::log2 (2) << std::endl
	      << "  781: " << BitOps<unsigned int>::log2 (781) << std::endl
	      << " 4096: " << BitOps<unsigned int>::log2 (4096) << std::endl;

    std::cout << std::endl;
    std::cout << "power of 2?:\n"
	      << "    0: " << BitOps<unsigned int>::islog2 (0) << std::endl
	      << "    1: " << BitOps<unsigned int>::islog2 (1) << std::endl
	      << "    2: " << BitOps<unsigned int>::islog2 (2) << std::endl
	      << "  781: " << BitOps<unsigned int>::islog2 (781) << std::endl
	      << " 4096: " << BitOps<unsigned int>::islog2 (4096) << std::endl;

    std::cout << std::endl;
    std::cout << "next power of 2:\n"
	      << "    0: " << BitOps<unsigned int>::ceil2 (0) << std::endl
	      << "    1: " << BitOps<unsigned int>::ceil2 (1) << std::endl
	      << "   15: " << BitOps<unsigned int>::ceil2 (15) << std::endl
	      << "  781: " << BitOps<unsigned int>::ceil2 (781) << std::endl
	      << " 4096: " << BitOps<unsigned int>::ceil2 (4096) << std::endl;

    std::cout << std::endl;
    std::cout << "parity:\n"
	      << "    0: " << BitOps<unsigned int>::parity (0) << std::endl
	      << "    1: " << BitOps<unsigned int>::parity (1) << std::endl
	      << "    3: " << BitOps<unsigned int>::parity (3) << std::endl
	      << "  781: " << BitOps<unsigned int>::parity (781) << std::endl
	      << " 4096: " << BitOps<unsigned int>::parity (4096) << std::endl;

    unsigned int swapa = 958;
    unsigned int swapb = 155;
    BitOps<unsigned int>::swap (swapa, swapb);
    std::cout << std::endl;
    std::cout << "swap (958, 155) = (" << swapa << ", " << swapb << ")\n";

    std::cout << std::endl;
    std::cout << "interleaved morton numbers:\n   y\\x ";
    for (unsigned int x = 0; x < 8; ++x)
	std::cout << std::setw (3) << x << " ";
    for (unsigned int y = 0; y < 8; ++y)
    {
	std::cout << "\n" << std::setw (5) << y << ": ";
	for (unsigned int x = 0; x < 8; ++x)
	    std::cout << std::setw (3)
		      << BitOps<unsigned int>::mortonInterleave (x, y)
		      << " ";
    }
    std::cout << std::endl;

    std::cout << std::endl;
    std::cout << "uninterleaved morton numbers:\n";
    for (unsigned int n = 0; n < 64; ++n)
    {
	unsigned int x, y;
	BitOps<unsigned int>::mortonSplit (n, x, y);
	std::cout << std::setw (5) << n << ": <" << x << "," << y << ">";

	if ((n+1) % 4 == 0)
	    std::cout << std::endl;
    }
    return 0;
}
