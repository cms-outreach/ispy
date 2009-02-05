#include "classlib/sysapi/system.h"
#include <iostream>

#ifdef _WIN32
# define TEST_EXPORT __declspec(dllexport)
#else
# define TEST_EXPORT
#endif

extern "C" TEST_EXPORT void libfunc (void)
{
    std::cout << "`libfunc' says hello!\n";
}
