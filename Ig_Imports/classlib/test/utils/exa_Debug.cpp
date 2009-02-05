#include "classlib/utils/DebugAids.h"
#include "classlib/utils/Signal.h"
#include <iostream>

/**   
 *      Example on how to use Seal DebugAids 
 */

using namespace lat;



static void h (void);
static void g (void);
static void f (void);


int main (int, char **argv)
{
    Signal::handleFatal (argv[0]);

    // calls f which calls g which calls h
    f(); 
    return 0;
}

void f (void)
{
    g ();
}

void g (void)
{
    h ();
}


void h (void)
{
   
    int * p = 0; 
    // test if p is valid

    std::cout << "Testing ASSERT_VALID  ... " << std::endl; 
    //ASSERT_VALID(p); 

    // assert (not done if NDEBUG is defined) 
    std::cout << "Testing ASSERT  ... " << std::endl; 
    ASSERT( p != 0); 

    // like assert but evaluates regardless of NDEBUG
    std::cout << "Testing ASSERT_ALWAYS  ... " << std::endl; 
    ASSERT_ALWAYS ( p!= 0);

    // like assert_always but in case of NDEBUG but without failed message
    std::cout << "Testing VERIFY  ... " << std::endl; 
    VERIFY( p != 0); 


}

