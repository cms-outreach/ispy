//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Framework/interface/IgOnLowMemoryService.h"
#include "classlib/utils/LowMemoryPool.h"
#include <iostream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

bool IgOnLowMemoryService::s_allocated = false;

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

bool
IgOnLowMemoryService::init (const char *appname)
{
    // initialise exception handling
    try { throw 1; } catch (...) {}

    s_allocated = false;
    lat::LowMemoryPool::reset ();
    try
    {
	lat::LowMemoryPool::add (10,  lat::CreateCallback (fatal), true);
	lat::LowMemoryPool::add (100, lat::CreateCallback (quit));
	lat::LowMemoryPool::add (200, lat::CreateCallback (danger));
    }
    catch (std::bad_alloc &)
    {
	lat::LowMemoryPool::reset ();
	std::cerr << appname << ": out of memory -- "
	    "cannot allocate enough memory\n";
	return false;
    }

    s_allocated = true;
    return true;
}

void
IgOnLowMemoryService::fatal (void)
{
    if (! s_allocated) return;

    static const char *message =
	"*** OUT OF MEMORY.  I will try to shut myself down as gracefully\n"
	"    as the remaining memory permits.  Brace yourself.\n";

    std::cerr << message;
    std::cerr.flush ();
}

void
IgOnLowMemoryService::quit (void)
{
    if (! s_allocated) return;

    static const char *message =
	"*** WARNING: Running very low on memory.  Please try to close or\n"
	"    disable any unnecessary application functions, and please save\n"
	"    your work now.  To be safe, you should probably restart the program.\n";

    std::cerr << message;
    std::cerr.flush ();
}

void
IgOnLowMemoryService::danger (void)
{
    if (! s_allocated) return;

    static const char *message =
	"*** WARNING: Memory seems to be getting low.  Please try to disable\n"
	"    application features you do not need right now.  To be safe,\n"
	"    please save your work now.\n";

    std::cerr << message;
    std::cerr.flush ();
}


