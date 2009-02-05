#include <iostream>

class ExtendibleClass
{
public:
    virtual void f(void) 
	{
	    std::cerr << "default implementation" << std::endl;
	};
};
