#include <cassert>
#include <string>
#include <boost/python.hpp>
#include "Ig_Modules/IgPython/interface/IgPythonInterpreter.h"

int main()
{
    IgPythonInterpreter *tPy = new IgPythonInterpreter();
    
    /* We create a string object in python...*/
    tPy->interpret("PyString='test'");
    
    /* and take it from python via the internal python dictionary*/
    object PyString = tPy->getGlobalNamespace()["PyString"];
    
    /* then we transform it to a std::string*/
    std::string CString = extract<std::string>(PyString)();

    assert(CString=="test");
}
