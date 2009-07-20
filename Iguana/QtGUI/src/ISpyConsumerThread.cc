#include "Iguana/QtGUI/interface/ISpyConsumerThread.h"
#include "Iguana/QtGUI/interface/ISpyConsumer.h"
#include "Iguana/Framework/interface/IgCollection.h"
#include "Iguana/Framework/interface/IgParser.h"

ISpyConsumerThread::ISpyConsumerThread(QObject * /*parent*/)
  : m_consumer(0),
    m_restart(false),
    m_abort(false),
    m_bufferSize(10)
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

void
ISpyConsumerThread::nextEvent(IgDataStorage *storage) 
{
  QMutexLocker locker(&m_mutex);
  if(m_consumer != 0)
  {    
    if(m_events.size() > m_bufferSize)
    {
      m_events.pop_front();
    }
    m_events.push_back(m_consumer->newEvent());
    if(! m_events.back().empty())
    {      
      IgParser parser(storage);
      parser.parse(m_events.back().c_str());
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
