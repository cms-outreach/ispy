#include "test/Test.h" 
#include "classlib/utils/HostInfo.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    std::cout << "host information\n"
	      << "  name      : " << HostInfo::name () << std::endl
	      << "  dnsname   : " << HostInfo::dnsname () << std::endl
	      << "  dnsdomain : " << HostInfo::dnsdomain () << std::endl
	      << "  fqdn      : " << HostInfo::fqdn () << "\n";

    return 0;
}
