#include "test/Test.h" 
#include "classlib/utils/Time.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    Time	t = Time::current ();
    unsigned	d = Time::toDosDate (t);
    Time        t2 = Time::fromDosDate (d);

    std::cout << "time:  " << t << std::endl
	      << "dos:   " << d << std::endl
	      << "undos: " << t2 << std::endl << std::endl;

    CLASSLIB_TEST_ASSERT_DOUBLES_EQUAL( t.second(true),  t2.second(true), 1 ); 
   
    t += 5 * 30 * TimeConst::SECS_PER_DAY * TimeConst::SEC_NSECS;
    d = Time::toDosDate (t);
    t2 = Time::fromDosDate (d);

    std::cout << "time:  " << t << std::endl
	      << "dos:   " << d << std::endl
	      << "undos: " << t2 << std::endl;

    CLASSLIB_TEST_ASSERT_DOUBLES_EQUAL( t.second(true),  t2.second(true), 1 ); 

    return 0;
}
