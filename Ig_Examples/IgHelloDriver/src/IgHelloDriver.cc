//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Examples/IgHelloDriver/interface/IgHelloDriver.h"
#include <classlib/utils/DebugAids.h>
#include <classlib/utils/ResourceInfo.h>
#include <iostream>
#include <cstdlib>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

const char *
IgHelloDriver::catalogLabel (void)
{ return "Hello"; }

IgHelloDriver::IgHelloDriver (IgState *state)
    : m_state (state)
{ ASSERT (m_state); }

int
IgHelloDriver::run (void)
{
    // Test driver
    std::cout << "IGUANA is really cool -- and says hello!\n\n";

    lat::ResourceInfo::Status                stats (lat::ResourceInfo::info_self());
    lat::ResourceInfo::Status::iterator      i = stats.begin ();
    char                                      buf [512];

    std::cout << "Resource usage\n";
    for ( ; i != stats.end (); ++i)
        std::cout << "  " << lat::ResourceInfo::name (i->first) << ": "
                  << lat::ResourceInfo::format (i->first, i->second, buf, sizeof (buf))
                  << " " << lat::ResourceInfo::unit (i->first) << "\n";
    std::cout << "\n";
    return EXIT_SUCCESS;
}
