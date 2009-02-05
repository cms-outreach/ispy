#include "Ig_Modules/IgPython/interface/IgPythonInterpreter.h"
#include <iostream>
#include <cassert>
int main()
{
    IgPythonInterpreter *tPy = new IgPythonInterpreter();

    tPy->redirectOutput(std::cerr);    
    tPy->interpret("assert(0==0)\n");
    tPy->interpret("print 'Hello World'");
    tPy->interpret("import sys");
    assert( tPy->completeName("sys.stde") == "sys.stderr");
    assert( tPy->completeName("sys.stdo") == "sys.stdout");
    assert( tPy->completeName("IgPythonC") == "IgPythonCompleter");
    assert( tPy->completeName("IgPythonS") == "IgPythonState");
}

