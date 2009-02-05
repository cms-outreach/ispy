#include "test/Test.h" 
#include "classlib/utils/LowMemoryPool.h"
#include "classlib/utils/ResourceInfo.h"
#include "classlib/utils/Signal.h"
#include "classlib/utils/DebugAids.h"
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <new>

using namespace lat;

bool s_allocated = false;
bool s_level = 10;

void lowmem (int code)
{
    if (! s_allocated) return;
    s_level = code;

    static const char *messages [3] = 
	{ "*** OUT OF MEMORY.  I will try to shut myself down as gracefully\n"
	  "    as the remaining memory permits.  Brace yourself.\n",
	  "*** WARNING: Running very low on memory.\n",
	  "*** WARNING: Memory getting low.\n" 
	};

    ASSERT (code <= 2);
    std::cerr << messages [code];
    std::cerr.flush ();
}

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    // initialise exception handling
    try { throw 1; } catch (...) {}

    // get pools
    s_allocated = false;
    LowMemoryPool::reset ();
    try
    {
	LowMemoryPool::add (10,  CreateCallback (lowmem, 0), true);
	LowMemoryPool::add (100, CreateCallback (lowmem, 1));
	LowMemoryPool::add (200, CreateCallback (lowmem, 2));
    }
    catch (std::bad_alloc &)
    {
	LowMemoryPool::reset ();
	std::cerr << "out of memory -- cannot allocate enough memory\n";
	return EXIT_FAILURE;
    }

    s_allocated = true;

    // exercise: limit memory to 10 MB
    ResourceInfo::setlimit (ResourceInfo::LIMIT_VMEMORY,
			    ResourceInfo::SOFT,
			    10 * 1024 * 1024);

    // exercise: eat memory in 10 kB chunks until last callback fires
    while (s_level)
    {
	try
	{
	    new char [10 * 1024];
	}
	catch (std::bad_alloc &)
	{
	    std::cerr << "new threw, exiting\n";
	    return EXIT_SUCCESS;
	}
	std::cout << "." << std::flush;
    }

    return EXIT_SUCCESS;
}
