#include "test/Test.h" 
#include "classlib/utils/AutoLoad.h"
#include "classlib/utils/Signal.h"

using namespace lat;
int TEST (int, char **argv)
{
    Signal::handleFatal (argv[0]);

    AutoLoadLib			lib ("SharedLibrary01");
    AutoLoad<void (void)>	existing (lib, "libfunc");
    AutoLoad<void (void)>	nonexistent (lib, "no such thing");
    bool			trapped = false;

    (*existing) ();

    try {
	(*nonexistent) ();
    } catch (AutoLoadError &e) {
	std::cout << "AutoLoadError: " << e.explain () << "\n";
	trapped = true;
    }
    CLASSLIB_TEST_ASSERT (trapped);

    return EXIT_SUCCESS;
}
