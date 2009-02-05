#include "test/Test.h" 
#include "classlib/utils/Time.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    Time t = Time::current ();
    Time u;
    for (int i = 0; i < 10; ++i)
    {
	u = t + i * 3456 + i;
	std::cout << "time:                " << u << std::endl
		  << "seconds:             " << TimeSpan (u).seconds () << std::endl
		  << "fraction (default):  " << u.nanoformat () << std::endl
		  << "fraction (3-wide):   " << u.nanoformat (3) << std::endl
		  << "fraction (3.3-wide): " << u.nanoformat (3, 3) << std::endl
		  << "fraction (3.6-wide): " << u.nanoformat (3, 6) << std::endl
		  << "fraction (9.9-wide): " << u.nanoformat (9, 9) << std::endl
		  << std::endl;

	 CLASSLIB_TEST_ASSERT( u.nanoformat().size() >= 1
			 &&   u.nanoformat().size() <= 9); 
	 CLASSLIB_TEST_ASSERT( u.nanoformat(3).size() >= 3 &&   u.nanoformat(3).size() <= 9); 
	 CLASSLIB_TEST_ASSERT( u.nanoformat(3,3).size() == 3 ); 
	 CLASSLIB_TEST_ASSERT( u.nanoformat(3,6).size() >= 3 &&   u.nanoformat(3,6).size() <= 6); 
	 CLASSLIB_TEST_ASSERT( u.nanoformat(9,9).size() == 9 ); 

    }

    return 0;
}
