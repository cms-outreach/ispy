//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Framework/IgPluginManager/interface/IgPluginManager.h"
#include "PluginFactory.h"
#include "PluginFactoryType.h"
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
    TestPluginType *p;

    IgPluginManager::get ()->initialise ();
    VERIFY (p = TestPluginFactory::get ()->create ("Sample", 1, 2.0));
    delete p;

    VERIFY (p = TestPluginFactory::get ()->create ("Init", 3, 4.0));
    delete p;

    return EXIT_SUCCESS;
}
