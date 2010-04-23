#ifndef QT_GUI_ISPY_CONSUMER_H
# define QT_GUI_ISPY_CONSUMER_H

# include "Iguana/Framework/interface/IgNet.h"
# include <deque>
# include <sstream>

# include "classlib/utils/TimeInfo.h"

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
  static const uint32_t MAX_EVENT_BUFFER = 10;

  ISpyConsumer(bool verbose, const std::string &host, int port)
    : IgNet("ispy-consumer"),
      m_eventIndex(0),
      m_newEvent(false)
    {
      logme() << "INFO: listening for data from " << host << ':' << port << '\n';
      debug(verbose);
      listenToSource(host, port);
    }

  bool
  hasEvents(void)
    {
      return ! m_events.empty ();
    }
  
  bool
  hasNewEvent(void)
    {
      return m_newEvent;
    }
  
  IgNet::Object&
  newEvent(void)
    {
      if(m_events.back().lastreq == 0)
      {
	m_eventIndex = m_events.size()-1;
	m_newEvent = true;
      }
      m_events[m_eventIndex].lastreq = Time::current();
      m_newEvent = false;
	  
      return m_events[m_eventIndex];
    }
  
  static void
  report(void *arg, uint32_t reason, Object &o)
    {
      ISpyConsumer *self = static_cast<ISpyConsumer *>(arg);
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
	    if (self->m_events.size() > MAX_EVENT_BUFFER)
	    {
	      self->m_events.pop_front();
	    }
	    x << self->m_events.size();
	    self->m_events.push_back(o);
	    self->m_newEvent = true;

	    x << "\n";
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
  size_t			m_eventIndex;
  std::deque<IgNet::Object>	m_events;
  bool				m_newEvent;
};

#endif // QT_GUI_ISPY_CONSUMER_H
