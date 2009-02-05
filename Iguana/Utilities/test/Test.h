#include "classlib/utils/Error.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>

static int failures = 0;
static int successes = 0;

#define CLASSLIB_TEST_ASSERT(x)				\
  do {							\
      std::cout << __FILE__ << ":" << __LINE__		\
	        << ": '" #x "': ";			\
      if (! (x))					\
      {							\
          ++failures;					\
	  std::cout << "FAILED\n";			\
      }							\
      else						\
      {							\
	  ++successes;					\
	  std::cout << "PASSED\n";			\
      }							\
  } while (0)

#define CLASSLIB_TEST_ASSERT_EQUAL(x,y)			\
  do {							\
      std::cout << __FILE__ << ":" << __LINE__		\
	        << ": '" #x "' [value=" << (x)		\
		<< "] vs. '" #y "' [value=" << (y)	\
		<< "]: ";				\
      if ((x) != (y))					\
      {							\
          ++failures;					\
	  std::cout << "FAILED\n";			\
      }							\
      else						\
      {							\
	  ++successes;					\
	  std::cout << "PASSED\n";			\
      }							\
  } while (0)

#define CLASSLIB_TEST_ASSERT_DOUBLES_EQUAL(x,y,e)	\
  do {							\
      std::cout << __FILE__ << ":" << __LINE__		\
	        << ": '" #x "' [value=" << (x)		\
		<< "] vs. '" #y "' [value=" << (y)	\
		<< "]: ";				\
      if (fabs((x) - (y)) > (e))			\
      {							\
          ++failures;					\
	  std::cout << "FAILED\n";			\
      }							\
      else						\
      {							\
	  ++successes;					\
	  std::cout << "PASSED\n";			\
      }							\
  } while (0)

static int TEST (int argc, char **argv);
int main( int argc, char **argv)
{
    lat::Signal::handleFatal (argv[0]);
    try
    {
        int iret = TEST(argc, argv); 
        CLASSLIB_TEST_ASSERT(iret == 0);
    }
    catch (lat::Error &e)
    {
	std::cout << "FAILED: died because of an error " << e.explain () << "\n";
	CLASSLIB_TEST_ASSERT(false);
    }
    return (failures ? 1 : successes ? 0 : 77);
}
