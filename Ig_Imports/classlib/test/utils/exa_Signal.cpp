#include "classlib/utils/Signal.h"
#include "classlib/utils/TimeInfo.h"
#include <iostream>
#include <cstdlib>

#include <vector> 
#include <iostream>
//#include <assert.h>
#include "classlib/utils/DebugAids.h"

using namespace lat;


// Simple example how to handle signals with SealBase::Signal

static void inthandler (int s, siginfo_t *, void *)
{
     std::cout << " got signal " << s << std::endl; 
    TimeInfo::sleep (1);
    std::cout << "  Woke up\n";
}


int main (int, char **argv)
{
  // to handle all fatal signal received in the program
  Signal::handleFatal (argv[0]);


  std::cout << "Handling SIGINT\n";
  Signal::handle (SIGINT, &inthandler);

  std::cout << "Handling SIGTERM\n";
  Signal::handle (SIGTERM, &inthandler);

  
  std::cout << "press control-c to send a SIGINT " << std::endl; 
  std::cout << " waiting..... " << std::endl; 
  bool gotit = Signal::wait(SIGINT,0,100000); 
  if (gotit) 
    std::cout << "SIGINT received " << std::endl; 
  

  /* 
  int i = 0; 
  gotSignal = false; 
  while (!gotSignal) { 
    // loop here
    i++; 
  }
  std::cout << "Continue execution... " << i << std::endl; 
  */


  std::cout << "Send signal SIGTERM " << std::endl; 
  Signal::raise (SIGTERM);

  //send an Abort signal which will be handle by HandleFatal
  std::cout << "Send signal SIGKILL " << std::endl; 
  Signal::raise (SIGABRT);

}
