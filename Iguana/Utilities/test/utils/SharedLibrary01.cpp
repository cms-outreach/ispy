#include "test/Test.h" 
#include "classlib/utils/SharedLibrary.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <cstdlib>
#include <string>

using namespace lat;

static void
dumplibs (const SharedLibrary::LibraryInfo &info)
{
    std::cout << "  " << info.m_filename << std::hex << " text = 0x"
	      << info.m_text_start << " - 0x" << info.m_text_end << ", data = 0x"
	      << info.m_data_start << " - 0x" << info.m_data_end << ", bss = 0x"
	      << info.m_bss_start << " - 0x" << info.m_bss_end << "\n";
}

int TEST (int, char **argv)
{
    Signal::handleFatal (argv[0]);

    std::cout << "shared library search path is: "
	      << SharedLibrary::path () << std::endl;

    std::string libname = SharedLibrary::libname ("test_SharedLibrary01");
    std::cout << "loading `" << libname << "'... " << std::flush;
    SharedLibrary *lib;
    try 
    {
	lib = SharedLibrary::load (libname);
    }
    catch (SharedLibraryError &error) 
    {
	std::cout << "failed: " << error.explain () << std::endl;
	return EXIT_FAILURE;
    }
    std::cout << "done!\n";

    static const char symname [] = "libfunc";
    std::cout << "retrieving `" << symname << "'... " << std::flush;
    SharedLibrary::Function address;

    try
    {
	address = lib->function (symname);
    }
    catch (SharedLibraryError &error)
    {
	std::cout << "failed: " << error.explain () << std::endl;
	return EXIT_FAILURE;
    }
    std::cout << "done!\n";

    std::cout << "calling `" << symname << "'... " << std::flush;
    (*address) ();
    std::cout << "done!\n";

    std::cout << "loaded shared libraries list:\n";
    try {
	SharedLibrary::loaded (CreateCallback (&dumplibs));
    }
    catch (SharedLibraryError &error)
    {
	std::cout << "failed: " << error.explain () << std::endl;
	return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
