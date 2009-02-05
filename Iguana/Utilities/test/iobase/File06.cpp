#include "test/Test.h" 
#include "classlib/iobase/File.h"
#include "classlib/iobase/Filename.h"
#include "classlib/utils/ShellEnvironment.h"
#include "classlib/utils/Time.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>

using namespace lat;

static std::string inputFile = "BuildFile";

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    if (ShellEnvironment().has("INPUT_FILE") )
	inputFile = ShellEnvironment().get("INPUT_FILE"); 


    Filename            fileName(inputFile);
    if (!fileName.exists()) { 
      std::cout << "Error : input file " << inputFile << " does not exist ! " << std::endl;
      return -1;
    }

    std::cout << "Input file   : " << inputFile << std::endl;

    File	f (inputFile);
    Time	ctime, mtime;

    f.times (&ctime, &mtime, 0);

    std::cout << "config.log ctime = " << ctime
	      << ", mtime = " << mtime << std::endl;

    return 0;
}
