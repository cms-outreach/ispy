#include "Iguana/Framework/interface/IgNet.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/utils/StringOps.h"
#include "classlib/utils/Signal.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <signal.h>
#include <unistd.h>

static sig_atomic_t s_stop = 0;

class IgNetTestConsumer : public IgNet
{
public:
  IgNetTestConsumer(bool verbose, const std::string &host, int port)
    : IgNet("test-consumer")
    {
      logme() << "INFO: listening for data from " << host << ':' << port << '\n';
      debug(verbose);
      delay(100);
      listenToSource(host, port);
    }

  static void
  report(void *arg, uint32_t reason, Object &o)
    {
      IgNetTestConsumer *self = static_cast<IgNetTestConsumer *>(arg);
      if (reason == VIS_FLAG_RECEIVED)
      {
	if (o.flags & VIS_FLAG_NEW)
	{
	  o.flags &= ~VIS_FLAG_NEW;
	  std::ostream &x = self->logme();
	  x << "UPDATED: " << o.name << ", version " << o.version << ", "
	    << o.rawdata.size() << " bytes data, flags 0x"
	    << std::hex << o.flags << std::dec;
	  if (o.flags & VIS_FLAG_SCALAR)
	  {
	    std::vector<char> data;
	    data.reserve(o.rawdata.size() + 1);
	    data.insert(data.end(), o.rawdata.begin(), o.rawdata.end());
	    data.push_back(0);

	    x << "; text: '" << &data[0] << "'\n";
	  }
	  else
	    x << '\n';
	}
      }
      else if (reason == VIS_FLAG_DEAD)
	self->logme()
	  << "DELETED: " << o.name << ", version " << o.version
	  << ", flags 0x" << std::hex << o.flags << std::dec << '\n';
      else
	self->logme()
	  << "ERROR: unrecognised object update code " << reason << '\n';
    }

  virtual bool
  shouldStop(void)
    {
      receive(&report, this);
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
    if (i < argc-1 && ! strcmp(argv[i], "--connect"))
    {
      StringList parts = StringOps::split(argv[++i], ':');
      if (parts.size() > 2
	  || (parts.size() == 2
	      && parts[1].find_first_not_of(digits) != std::string::npos))
      {
	std::cerr << argv[0]
		  << ": malformed --connect parameter, "
		  << "expected HOST[:PORT]\n";
	bad = true;
	break;
      }
      host = parts[0];
      if (parts.size() == 2)
	port = atoi(parts[1].c_str());
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
    std::cerr << "Usage: " << argv[0] << " --connect HOST[:PORT] [--[no-]debug]\n";
    return 1;
  }

  IgNetTestConsumer test(debug, host, port);
  test.run();
  return 0;
}
