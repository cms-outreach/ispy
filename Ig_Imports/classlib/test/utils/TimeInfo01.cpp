#include "test/Test.h" 
#include "classlib/utils/TimeInfo.h"
#include "classlib/utils/LongLong.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>
#include <fstream>


using namespace lat;

int TEST(int, char **argv)
{
    TimeInfo::NanoTicks rstartc = 0;
    TimeInfo::NanoSecs rstartt = 0;
    TimeInfo::init ();    
    if (TimeInfo::features () & TimeInfo::FEATURE_REAL_COUNT_EPOCH)
    {
	rstartc = TimeInfo::realCycles ();
	rstartt = TimeInfo::realNsecs ();
    }

    TimeInfo::init ();

    Signal::handleFatal (argv[0]);

    std::cout << "processor information\n"
	      << "  MHz         : " << TimeInfo::mhz () << "\n\n";

    CLASSLIB_TEST_ASSERT (TimeInfo::mhz () > 100 && TimeInfo::ghz () < 100 ); 

    // Measure the overhead of sampling virtual time
    static const int NSAMPLES = 100000;
    TimeInfo::NanoTicks v0 = TimeInfo::virtualCycles ();
    TimeInfo::NanoTicks v1 = TimeInfo::virtualCycles ();
    TimeInfo::NanoTicks v2 = TimeInfo::virtualCycles ();
    TimeInfo::NanoTicks vend = v2;
    for (int i = 0; i < NSAMPLES; ++i)
	vend = TimeInfo::virtualCycles ();

    // Measure the overhead of sampling real time
    TimeInfo::NanoTicks r0 = TimeInfo::realCycles ();
    TimeInfo::NanoTicks r1 = TimeInfo::realCycles ();
    TimeInfo::NanoTicks r2 = TimeInfo::realCycles ();
    TimeInfo::NanoTicks rend = r2;
    for (int i = 0; i < NSAMPLES; ++i)
	rend = TimeInfo::realCycles ();

    std::cout << "sampling overhead (1 and " << NSAMPLES << " samples)\n"
	      << "  virtual 0   : " << (v1-v0) << "\n"
	      << "  virtual 1   : " << (v2-v1) << "\n"
	      << "  virtual end : " << (vend-v2) << "\n"
	      << "   per sample : " << double (vend-v2) / NSAMPLES << "\n"
	      << "    ns/sample : " << double (vend-v2) / NSAMPLES / TimeInfo::ghz () << "\n\n"

	      << "  real    0   : " << (r1-r0) << "\n"
	      << "  real    1   : " << (r2-r1) << "\n"
	      << "  real    end : " << (rend-r2) << "\n"
	      << "   per sample : " << double (rend-r2) / NSAMPLES << "\n"
	      << "    ns/sample : " << double (rend-r2) / NSAMPLES / TimeInfo::ghz () << "\n\n";

    CLASSLIB_TEST_ASSERT( (r1 - r0) > 0); 
    CLASSLIB_TEST_ASSERT( (r2 - r1) > 0); 
    CLASSLIB_TEST_ASSERT( (rend-r2) / NSAMPLES /  TimeInfo::ghz () > 0
		    && (rend-r2) / NSAMPLES /  TimeInfo::ghz () < 1000 ); 
    CLASSLIB_TEST_ASSERT( (vend-v2) / NSAMPLES /  TimeInfo::ghz () > 10
		    && (vend-v2) / NSAMPLES /  TimeInfo::ghz () < 1000000 ); 



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


    // measure time in  a loop 
    // initialize timer 

    TimeInfo::init ();    
    TimeInfo::NanoSecs realT0 =  TimeInfo::processRealTime (); 
    TimeInfo::NanoSecs idleT0 =  TimeInfo::processIdleTime(); 
    TimeInfo::NanoSecs userT0 =  TimeInfo::processUserTime();
    TimeInfo::NanoSecs sysT0  =  TimeInfo::processSystemTime();
    TimeInfo::NanoSecs cpuT0  =  TimeInfo::processCpuTime();
    TimeInfo::NanoSecs realNsec0 = TimeInfo::realNsecs ();
    TimeInfo::NanoSecs virtNsec0 = TimeInfo::virtualNsecs ();


    std::cout << "measuring time in a loop ...." << std::endl; 
    std::ofstream file("/tmp/TimeInfo01.out");

    // start making a nap to have some idle time
    static const double sleepTime = 1.33;
    static const int n = 100000; 
    TimeInfo::sleep(sleepTime); 
    double s1 = 0,s2=0; 
    for (int i = 0; i < n; ++i) { 
      double x = static_cast<double>( std::rand() ) / RAND_MAX ; 
      double w  =  1; 
      s1 += w*x;
      s2 += w*x*x;
      // write in  a file to have some system time
      file << i << "  " << x << "  " << s1 << "   " << s2 << "  " << std::endl;
    } 

    // measure time 
    TimeInfo::NanoSecs userT1, sysT1, realT1; 
    TimeInfo::processTimes(userT1,sysT1,realT1); 
    TimeInfo::NanoSecs idleT1 =  TimeInfo::processIdleTime(); 
    TimeInfo::NanoSecs cpuT1 =  TimeInfo::processCpuTime(); 
    TimeInfo::NanoSecs realNsec1 = TimeInfo::realNsecs ();
    TimeInfo::NanoSecs virtNsec1 = TimeInfo::virtualNsecs ();

    TimeInfo::NanoSecs userTime = ( userT1-userT0 ) * 1e-9; 
    TimeInfo::NanoSecs realTime = ( realT1-realT0 ) * 1e-9; 
    TimeInfo::NanoSecs sysTime =  ( sysT1-sysT0 )   * 1e-9; 
    TimeInfo::NanoSecs cpuTime =  ( cpuT1-cpuT0 )   * 1e-9; 
    TimeInfo::NanoSecs idleTime = ( idleT1-idleT0 ) * 1e-9; 


    std::cout << " Real (s) = " << realTime 
		   << " CPU  (s) = " << cpuTime 
		   << " User (s) = " << userTime
		   << " Sys. (s) = " << sysTime
		   << " idle (s) = " << idleTime
		   << std::endl; 

    std::cout << " HR Real Time (s) " <<  (realNsec1-realNsec0)*1e-9
		   << " Virtual Time (s) " <<  (virtNsec1-virtNsec0)*1e-9 
		   << std::endl;

    // check that user + sys = cpu 
    CLASSLIB_TEST_ASSERT_DOUBLES_EQUAL( cpuTime, userTime + sysTime, TimeInfo::resolution()/2); 
    CLASSLIB_TEST_ASSERT_DOUBLES_EQUAL(realTime, cpuTime+idleTime,TimeInfo::resolution()); 
    CLASSLIB_TEST_ASSERT_DOUBLES_EQUAL(realTime, (realNsec1-realNsec0)*1e-9, TimeInfo::resolution() ); 
    CLASSLIB_TEST_ASSERT_DOUBLES_EQUAL(cpuTime, (virtNsec1-virtNsec0)*1e-9, TimeInfo::resolution() ); 
    CLASSLIB_TEST_ASSERT( (realNsec1-realNsec0) > (virtNsec1-virtNsec0) ); 

    return 0;
}
