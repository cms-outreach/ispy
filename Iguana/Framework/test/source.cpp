#include "Iguana/Framework/interface/IgNet.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/utils/TimeInfo.h"
#include "classlib/utils/Signal.h"
#include "classlib/utils/Time.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <signal.h>
#include <unistd.h>

static sig_atomic_t s_stop = 0;

class IgNetTestProducer : public IgNet
{
public:
  IgNetTestProducer(bool verbose, int port)
    : IgNet("test-source")
    {
      logme() << "INFO: serving data at port " << port << '\n';
      debug(verbose);
      startLocalServer(port);
    }

  virtual bool
  shouldStop(void)
    {
      return s_stop > 0;
    }
};

static void
onInterrupt(int /* sig */)
{
  s_stop++;
}

static char
onAssertFail(const char *message)
{
  std::cout.flush();
  fflush(stdout);
  std::cerr.flush();
  fflush(stderr);
  std::cerr << message << "ABORTING\n";
  return 'a';
}

int
main(int argc, char **argv)
{
  using namespace lat;
  DebugAids::failHook(&onAssertFail);
  Signal::handleFatal(argv[0]);
  signal(SIGINT, &onInterrupt);

  // Check and process arguments.
  static const char *digits = "0123456789";
  std::string host("localhost");
  int port = 9000;
  bool debug = false;
  bool bad = false;
  for (int i = 1; i < argc; ++i)
    if (i < argc-1 && ! strcmp(argv[i], "--listen"))
    {
      if (std::string(argv[i+1]).find_first_not_of(digits) != std::string::npos)
      {
	std::cerr << argv[0] << ": malformed --listen parameter, expected PORT\n";
	bad = true;
	break;
      }
      port = atoi(argv[++i]);
    }
    else if (! strcmp(argv[i], "--debug"))
      debug = true;
    else if (! strcmp(argv[i], "--no-debug"))
      debug = false;
    else
    {
      bad = true;
      break;
    }

  if (bad || ! port)
  {
    std::cerr << "Usage: " << argv[0] << " --listen PORT [--[no-]debug]\n";
    return 1;
  }

  IgNetTestProducer test(debug, port);
  test.start();

  static const unsigned char data[] = "Hello world!";
  for (int i = 0; ! s_stop; i = (i+1) % 10)
  {
    switch (i)
    {
    case 0: case 1: case 2: case 3: case 4: case 5: case 6:
      {
	IgNet::Object o;
	o.version = Time::current().ns();
	o.flags = IgNet::VIS_FLAG_NEW;
	o.name = "Run 1/Event X";
	o.name[o.name.size()-1] = '0' + i;
	o.rawdata.insert(o.rawdata.end(), &data[0], &data[0]+sizeof(data));
	o.lastreq = 0;
	test.lock();
	test.updateLocalObject(o);
	test.unlock();
	test.sendLocalChanges();
	TimeInfo::msleep(1000);
	break;
      }

    case 7:
      {
	IgNet::Object o;
	o.version = Time::current().ns();
	o.flags = IgNet::VIS_FLAG_NEW | IgNet::VIS_FLAG_SCALAR;
	o.name = "Run 1/Event 7";
	o.rawdata.insert(o.rawdata.end(), &data[0], &data[0]+sizeof(data));
	o.lastreq = 0;
	test.lock();
	test.updateLocalObject(o);
	test.unlock();
	test.sendLocalChanges();
	TimeInfo::msleep(1000);
	break;
      }

    case 8:
      {
	IgNet::Object o;
	o.version = Time::current().ns();
	o.flags = IgNet::VIS_FLAG_NEW;
	o.name = "Run 1/Event 3";
	test.lock();
	test.updateLocalObject(o);
	test.unlock();
	test.sendLocalChanges();
	TimeInfo::msleep(1000);
	break;
      }

    case 9:
      {
	test.lock();
	test.removeLocalObject("Run 1/Event 1");
	test.removeLocalObject("Run 1/Event 2");
	test.removeLocalObject("Run 1/Event 3");
	test.removeLocalObject("Run 1/Event 4");
	test.removeLocalObject("Run 1/Event 5");
	test.removeLocalObject("Run 1/Event 6");
	test.removeLocalObject("Run 1/Event 7");
	test.unlock();
	test.sendLocalChanges();
	TimeInfo::msleep(5000);
	break;
      }
    }
  }
    
  return 0;
}
