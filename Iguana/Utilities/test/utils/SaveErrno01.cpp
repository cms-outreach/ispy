#include "test/Test.h" 
#include "classlib/utils/SaveErrno.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>
#include <cerrno>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    std::cout << "errno (default) = " << errno << std::endl;
    errno = EINVAL;
    std::cout << "errno (EINVAL) = " << errno << std::endl;
    {
	SaveErrno s;
	errno = EDOM;
	std::cout << "errno (EDOM) = " << errno << std::endl;
    }
    std::cout << "errno (EINVAL) = " << errno << std::endl;
    
    return 0;
}
