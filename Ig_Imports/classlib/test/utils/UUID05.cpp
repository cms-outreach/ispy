#include "test/Test.h" 
#include "classlib/utils/UUID.h"
#include "classlib/utils/Time.h"
#include "classlib/utils/Signal.h"
#include <iostream>

using namespace lat;

int TEST(int, char **argv)
{
    Signal::handleFatal (argv[0]);
    const char *text = "7087E77F-8789-D911-9FCB-0030482B37E8";
    UUID t (text);
    std::cout << "value = " << text << "\n";
    std::cout << "uuid = " << t << "\n";
    std::cout << "time = " << t.time ().ns ()
		   << " (UTC " << t.time ().format (false, "%Y-%m-%d %H:%M:%S.")
		   << t.time ().nanoformat() << ")\n";
    std::cout << "type = " << t.type () << "\n";
    std::cout << "variant = " << t.type () << "\n";

    t = UUID::generateTime();
    std::cout << "uuid = " << t << "\n";
    std::cout << "time = " << t.time ().ns ()
		   << " (UTC " << t.time ().format (false, "%Y-%m-%d %H:%M:%S.")
		   << t.time ().nanoformat() << ")\n";
    std::cout << "type = " << t.type () << "\n";
    std::cout << "variant = " << t.type () << "\n";

    t = UUID::generate();
    std::cout << "uuid = " << t << "\n";
    std::cout << "time = " << t.time ().ns ()
		   << " (UTC " << t.time ().format (false, "%Y-%m-%d %H:%M:%S.")
		   << t.time ().nanoformat() << ")\n";
    std::cout << "type = " << t.type () << "\n";
    std::cout << "variant = " << t.type () << "\n";
    return 0;
}
