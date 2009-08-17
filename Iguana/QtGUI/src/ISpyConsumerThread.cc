#include "Iguana/QtGUI/interface/ISpyConsumerThread.h"
#include "Iguana/QtGUI/interface/ISpyConsumer.h"
#include "Iguana/Framework/interface/IgCollection.h"
#include "Iguana/Framework/interface/IgParser.h"

ISpyConsumerThread::ISpyConsumerThread(QObject * /*parent*/)
  : m_consumer(0),
    m_restart(false),
    m_abort(false),
    m_bufferSize(10),
    m_newEvent(false),
    m_eventIndex(0)
{}

ISpyConsumerThread::~ISpyConsumerThread(void)
{
  m_mutex.lock();
  m_abort = true;
  m_condition.wakeOne();
  m_mutex.unlock();

  wait();
}

void
ISpyConsumerThread::listenTo(bool verbose, const std::string &host, int port)
{
  m_mutex.lock();
  m_debug = verbose;
  m_host = host;
  m_port = port;
  m_mutex.unlock();
}

std::string
ISpyConsumerThread::nextEvent(IgDataStorage *storage) 
{
  QMutexLocker locker(&m_mutex);
  std::string oName;

  if(m_consumer != 0 && m_consumer->hasEvents())
  {    
    if(m_events.size() > m_bufferSize)
    {
      m_events.pop_front();
    }
    IgNet::Object newo = m_consumer->newEvent();
    oName = newo.name;

    // Check whether it is a valid object
    // and is different from the previous one
    if (! oName.empty ())
    {
      std::string lastName;
      if (! m_events.empty())
	lastName = m_events.back().name;
      (oName != lastName) ? m_newEvent = true : m_newEvent = false;
      if (m_newEvent)
      {	
	m_events.push_back(newo);
	m_eventIndex = m_events.size()-1;
      }
      else if (++m_eventIndex > m_events.size()-1)
      {
	m_eventIndex = 0;
      }      
    }
    
    if (! m_events.empty())
    {
      IgNet::Object o = m_events[m_eventIndex];
      
      if (! o.name.empty ())
      {
	std::vector<char> data;	
	data.reserve(o.rawdata.size() + 1);
	data.insert(data.end(), o.rawdata.begin(), o.rawdata.end());
	data.push_back(0);

	IgParser parser(storage);
	parser.parse(&data[0]);
	oName = o.name;
	if (! m_newEvent)
	  oName.append (" (already viewed)");
      }      
    }
  }
  else
    oName.append ("Waiting for online events...");
  
  if(! isRunning()) 
  {
    start(LowPriority);
  } 
  else 
  {
    m_restart = true;
    m_condition.wakeOne();
  }
  return oName;
}

std::string
ISpyConsumerThread::previousEvent(IgDataStorage *storage) 
{
  QMutexLocker locker(&m_mutex);
  std::string oName;
  
  if(! m_events.empty())
  {
    (m_eventIndex > 0 && --m_eventIndex < m_events.size()) ? m_eventIndex : m_eventIndex = 0;
    
    ASSERT(m_eventIndex < m_events.size());
    IgNet::Object o = m_events[m_eventIndex];
       
    if (! o.name.empty ())
    {
      o.lastreq = Time::current();
      std::vector<char> data;	
      data.reserve(o.rawdata.size() + 1);
      data.insert(data.end(), o.rawdata.begin(), o.rawdata.end());
      data.push_back(0);
 
      IgParser parser(storage);
      parser.parse(&data[0]);
      oName = o.name;
      oName.append (" (already viewed)");
    }
  }
   
  if(! isRunning()) 
  {
    start(LowPriority);
  } 
  else 
  {
    m_restart = true;
    m_condition.wakeOne();
  }
  return oName;
}

void
ISpyConsumerThread::finalize(void)
{
  m_mutex.lock();
  interrupt(0);
  m_consumer->shouldStop();
  m_consumer->shutdown();
  m_mutex.unlock();	    
}

void
ISpyConsumerThread::run(void)
{
  m_consumer = new ISpyConsumer(m_debug, m_host, m_port);
  m_consumer->run();
}
