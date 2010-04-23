#ifndef QT_GUI_ISPY_CONSUMER_THREAD_H
# define QT_GUI_ISPY_CONSUMER_THREAD_H

# include <QThread>
# include <QMutex>
# include <QWaitCondition>
# include "Iguana/Framework/interface/IgNet.h"
# include <deque>

class IgDataStorage;
class ISpyConsumer;

class ISpyConsumerThread : public QThread
{
  Q_OBJECT

public:
  ISpyConsumerThread(QObject *parent = 0);
  ~ISpyConsumerThread(void);

  void  listenTo(bool verbose, const std::string &host, int port);
  bool hasNewEvent(void);
  std::string nextEvent(IgDataStorage *storage);
  std::string previousEvent(IgDataStorage *storage);
  void finalize(void);

protected:
  void 	run(void);

private:
  QMutex     		m_mutex;
  QWaitCondition 	m_condition;
  ISpyConsumer 	       *m_consumer;
  bool 			m_restart;    
  bool			m_abort;
  bool			m_debug;
  std::string 		m_host;
  int 			m_port;
  unsigned int		m_bufferSize;	    
  bool			m_newEvent;
  size_t		m_eventIndex;
  std::deque<IgNet::Object> m_events;
};

#endif // QT_GUI_ISPY_CONSUMER_THREAD_H
