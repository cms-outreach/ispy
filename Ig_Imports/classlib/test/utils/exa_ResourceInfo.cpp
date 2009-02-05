#include "classlib/utils/ResourceInfo.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>
#include <vector>

using namespace lat;

void printAllResource(std::ostream & out) { 


    ResourceInfo::Status		stats (ResourceInfo::info_self());
    ResourceInfo::Status::iterator	i = stats.begin ();
    char				buf [512];


    out << "\nresource usage\n";
    for ( ; i != stats.end (); ++i)
      out << "  " << ResourceInfo::name (i->first) << ": "
		  << ResourceInfo::format (i->first, i->second, buf, sizeof (buf))
		  << " " << ResourceInfo::unit (i->first) << "\n";
    out << "\n";

}

void printMemoryUsage(std::ostream & out) { 

  
  ResourceInfo::Status		stats (ResourceInfo::info_self());
  char				buf [512];

  ResourceInfo::Value virtualMem =  stats[ResourceInfo::RES_VMEMORY];    
  ResourceInfo::Value residentMem =  stats[ResourceInfo::RES_VMEMORY_RSS];    
  out << "\n memory usage\n";
  out << "  " << ResourceInfo::name (ResourceInfo::RES_VMEMORY) << "  :  " 
      << ResourceInfo::format (ResourceInfo::RES_VMEMORY, virtualMem , buf, sizeof (buf)) 
      << " " << ResourceInfo::unit (ResourceInfo::RES_VMEMORY) << "\n";
  out << "  " << ResourceInfo::name (ResourceInfo::RES_VMEMORY_RSS) << "  :  " 
      << ResourceInfo::format (ResourceInfo::RES_VMEMORY_RSS, residentMem , buf, sizeof (buf)) 
      << " " << ResourceInfo::unit (ResourceInfo::RES_VMEMORY) << "\n";
  out << "\n";
}


int main(int, char **argv)
{
    Signal::handleFatal (argv[0]);
 
   std::ostream &out = std::cout;

   printAllResource(out);

    // allocate some memory (80 MB) 
   std::vector<double> x(10000000);

   out << "\n\nAfter Allocation of 80MB \n\n" << std::endl;
   printMemoryUsage(out);

   //char a;
   //std::cin >> a;

   x.clear();

   out << "\n\nAfter released memory \n\n" << std::endl;

   printMemoryUsage(out);

   //char b;
   //std::cin >> b;

   return 0;
}
