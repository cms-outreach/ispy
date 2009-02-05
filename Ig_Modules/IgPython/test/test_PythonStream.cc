#include "Ig_Modules/IgPython/interface/IgPythonStream.h"
#include <fstream>
#include <cassert>
#include <string>

int main()
{
    IgPythonStream ps;
    ps << "assert(0==0)" << std::endl
       << "A=10" << std::endl;
    int A=0,B=0;
    ps["A"] >> B;
    assert(B==10);
    
    ps << "A=2\nB=3\n" << std::endl;
    
    /*Even more difficult...*/
    ps >> "A" >> A >> "B" >> B;
    
    assert(A==2);
    assert(B==3);

    std::ifstream testScript ("test/test.py");
    if (! testScript.is_open ()) 
    {
	std::cout << "This test should be run from Ig_Modules/IgPython directory." << std::endl;
    } else 
    {
	assert(testScript);
    
	ps << testScript.rdbuf() << std::endl;
	std::cout << testScript.rdbuf() << std::endl;
	ps.flush();
    }
    
    return 0;
}
