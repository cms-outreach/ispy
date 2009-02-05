//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Framework/IgPluginManager/interface/IgPluginManager.h"
#include "PluginFactoryArgs.h"
#include "PluginFactoryArgsType.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/utils/Signal.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

using namespace lat;
int main (int, char **argv)
{
    Signal::handleFatal (argv [0]);
    TestPluginType0 *p0;
    TestPluginType1 *p1;
    TestPluginType2 *p2;
    TestPluginType3 *p3;
    TestPluginType4 *p4;
   
    IgPluginManager::get ()->initialise ();

    TestPluginFactoryArgsV::get ()->load ("Sample");
    VERIFY (p0 = TestPluginFactoryArgs0::get ()->create ("Sample")); delete p0;
    VERIFY (p1 = TestPluginFactoryArgs1::get ()->create ("Sample", 1)); delete p1;
    VERIFY (p2 = TestPluginFactoryArgs2::get ()->create ("Sample", 1, 2)); delete p2;
    VERIFY (p3 = TestPluginFactoryArgs3::get ()->create ("Sample", 1, 2, 3)); delete p3;
    VERIFY (p4 = TestPluginFactoryArgs4::get ()->create ("Sample", 1, 2, 3, 4)); delete p4;

    return EXIT_SUCCESS;
}
