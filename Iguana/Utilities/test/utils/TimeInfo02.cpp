#include "test/Test.h" 
#include "classlib/utils/TimeInfo.h"
#include "classlib/utils/LongLong.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace lat;
int TEST (int, char **argv)
{
    TimeInfo::NanoTicks rstartc = 0;
    TimeInfo::NanoSecs rstartt = 0;
    TimeInfo::init ();    
    if (TimeInfo::features () & TimeInfo::FEATURE_REAL_COUNT_EPOCH)
    {
	rstartc = TimeInfo::realCycles ();
	rstartt = TimeInfo::realNsecs ();
    }

    Signal::handleFatal (argv[0]);

    std::cout << "processor information\n"
	      << "  MHz         : " << TimeInfo::mhz () << "\n\n";

    CLASSLIB_TEST_ASSERT (TimeInfo::mhz () > 100 && TimeInfo::ghz () < 100); 

    // Get run duration so far.  Virtual cycles may appear to be
    // larger than the real cycles: if FEATURE_REAL_COUNT_EPOCH was
    // present, the substraction below removes proces start-up costs.  
    TimeInfo::NanoTicks virtc = TimeInfo::virtualCycles ();
    TimeInfo::NanoSecs virtt = TimeInfo::virtualNsecs ();
    TimeInfo::NanoTicks realc = TimeInfo::realCycles ();
    TimeInfo::NanoSecs realt = TimeInfo::realNsecs ();
    
    std::cout << "------------------------------------------------\n"
	      << "Start    " << rstartc << " ticks, " << rstartt*1e-9 << " secs\n" 
	      << "Real     " << (realc-rstartc) << " ticks, "
	      << (realt-rstartt)*1e-9 << " secs\n"
	      << "Virtual  " << virtc << " ticks, " << virtt*1e-9 << " secs\n"
	      << "Process  " << TimeInfo::processUserTime () * 1e-9 << " user, "
	      << TimeInfo::processSystemTime () * 1e-9 << " system, "
	      << TimeInfo::processRealTime () * 1e-9 << " real ("
	      << TimeInfo::processIdleTime () * 1e-9 << " idle)\n"
	      << "System   " << std::setprecision(9) << TimeInfo::time () * 1e-9
	      << " since epoch, "
	      << TimeInfo::elapsedTime () * 1e-9 << " elapsed; resolution: "
	      << TimeInfo::resolution () * 1e-9 << std::endl;

    std::cout << "Features";

    if (TimeInfo::features () & TimeInfo::FEATURE_EXACT_MHZ)
	std::cout << " EXACT_MHZ";
    if (TimeInfo::features () & TimeInfo::FEATURE_TIME_EPOCH)
	std::cout << " TIME_EPOCH";
    if (TimeInfo::features () & TimeInfo::FEATURE_REAL_COUNT_EPOCH)
	std::cout << " REAL_COUNT_EPOCH";
    if (TimeInfo::features () & TimeInfo::FEATURE_PROCESS_TIMES)
	std::cout << " PROCESS_TIMES";

    std::cout << std::endl << std::endl;
    return 0;
}
