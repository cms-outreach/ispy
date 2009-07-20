#ifndef QT_GUI_ISPY_CONSUMER_H
# define QT_GUI_ISPY_CONSUMER_H

# include "Iguana/Framework/interface/IgNet.h"
# include <deque>
# include <sstream>

# include "classlib/iobase/IOFlags.h"
# include "classlib/iobase/File.h"
# include "classlib/iobase/Filename.h"
# include "classlib/iotools/OutputStream.h"
# include "classlib/utils/ShellEnvironment.h"
# include "classlib/zip/ZipArchive.h"
# include "classlib/zip/ZipMember.h"

using namespace lat;

static sig_atomic_t s_stop = 0;
static void
interrupt(int /* sig */)
{
  s_stop = 1;
}

class ISpyConsumer : public IgNet
{
public:
  static const uint32_t MAX_EVENT_BUFFER = 50;

  ISpyConsumer(bool verbose, const std::string &host, int port)
    : IgNet("ispy-consumer"),
      m_events(MAX_EVENT_BUFFER, "")
    {
      logme() << "INFO: listening for data from " << host << ':' << port << '\n';
      debug(verbose);
      delay(100);
      listenToSource(host, port);
    }

  std::string
  newEvent(void)
    {
      std::string event(m_events.back());
      return event;
    }
  
  static void
  report(void *arg, uint32_t reason, Object &o)
    {
      ISpyConsumer *self = static_cast<ISpyConsumer *>(arg);
      lat::ZipArchive *archive = 0;
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
	    // Save latest event in a file.
	    // FIXME: N events?
	    Filename zipOutputFile(Filename("latest.ig").substitute(ShellEnvironment()));
	    if (zipOutputFile.exists())
	      std::rename(zipOutputFile.name(), "latest-old.ig");
	    
	    archive = new ZipArchive(zipOutputFile, IOFlags::OpenWrite
				     | IOFlags::OpenCreate | IOFlags::OpenTruncate);
	    
	    lat::ZipMember *current = new ZipMember(o.name);
	    current->isDirectory(false);
	    current->time(Time::current());
	    current->method(ZConstants::DEFLATED);
	    current->level(ZConstants::BEST_COMPRESSION);

	    lat::OutputStream *output = archive->output(current);
	    
	    std::vector<char> data;
	    data.reserve(o.rawdata.size() + 1);
	    data.insert(data.end(), o.rawdata.begin(), o.rawdata.end());
	    data.push_back(0);

	    std::stringstream oss;
	    oss << &data[0];
	    output->write(oss.str().c_str(), oss.str().length());
	    output->close();
	    delete output;
	    archive->close();
	    delete archive;

	    if (self->m_events.size() > MAX_EVENT_BUFFER)
	    {
	      self->m_events.pop_front();
	    }
	    x << self->m_events.size();
	    self->m_events.push_back(oss.str());
	    //FIXME: x << "; text: '" << &data[0]
	    x << "'\n";
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
      return s_stop != 0;
    }
private:
  std::deque<std::string> m_events;
};

#endif // QT_GUI_ISPY_CONSUMER_H
