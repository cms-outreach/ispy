#include "test/Test.h" 
#include "classlib/utils/SystemInfo.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    std::cout << "system information\n"
	      << "  pagesize  : " << SystemInfo::pagesize () << std::endl
	      << "  allocgran : " << SystemInfo::allocGranularity () << std::endl
	      << "  physical  : " << SystemInfo::physicalPagesAvailable ()
	      << "/" << SystemInfo::physicalPages () << std::endl
	      << "  virtual   : " << SystemInfo::virtualPagesAvailable ()
	      << "/" << SystemInfo::virtualPages () << std::endl
	      << "  processors: " << SystemInfo::processors ()
	      << "/" << SystemInfo::processorsOnline () << std::endl

	      << "  node name : " << SystemInfo::nodename () << std::endl
	      << "  dom name  : " << SystemInfo::domainname () << std::endl
	      << "  os name   : " << SystemInfo::osname () << std::endl
	      << "  os release: " << SystemInfo::osrelease () << std::endl
	      << "  os version: " << SystemInfo::osversion () << std::endl
	      << "  machine   : " << SystemInfo::machine () << std::endl
	      << "  processor : " << SystemInfo::processor () << "\n";

    return 0;
}
