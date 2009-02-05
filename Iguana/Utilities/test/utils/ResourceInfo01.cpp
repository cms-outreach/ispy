#include "test/Test.h" 
#include "classlib/utils/ResourceInfo.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>

using namespace lat;

static void showlimit (ResourceInfo::Limit which, const char *name)
{
    ResourceInfo::Value soft;
    ResourceInfo::Value hard;
    ResourceInfo::getlimits (which, soft, hard);
    std::cout << "  " << name
	      << "\tsoft = " << std::setw (10) << soft
	      << " hard = " << std::setw (10) << hard << "\n";
}

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    ResourceInfo::Status		stats (ResourceInfo::info_self());
    ResourceInfo::Status::iterator	i = stats.begin ();
    char				buf [512];

    std::cout << "resource limits\n";
    showlimit (ResourceInfo::LIMIT_CPU_TIME,		"cpu time    ");
    showlimit (ResourceInfo::LIMIT_FILE_SIZE,		"file size   ");
    showlimit (ResourceInfo::LIMIT_CORE_SIZE,		"core size   ");
    showlimit (ResourceInfo::LIMIT_N_PROCESSES,		"# processes ");
    showlimit (ResourceInfo::LIMIT_N_OPEN_FILES,	"# files     ");
    showlimit (ResourceInfo::LIMIT_VMEMORY,		"vmem        ");
    showlimit (ResourceInfo::LIMIT_VMEMORY_RSS,		"vmem rss    ");
    showlimit (ResourceInfo::LIMIT_VMEMORY_LOCKED,	"vmem locked ");
    showlimit (ResourceInfo::LIMIT_VMEMORY_DATA,	"vmem data   ");
    showlimit (ResourceInfo::LIMIT_VMEMORY_STACK,	"vmem stack  ");

    std::cout << "\nresource usage\n";
    for ( ; i != stats.end (); ++i)
	std::cout << "  " << ResourceInfo::name (i->first) << ": "
		  << ResourceInfo::format (i->first, i->second, buf, sizeof (buf))
		  << " " << ResourceInfo::unit (i->first) << "\n";
    std::cout << "\n";

    return 0;
}
