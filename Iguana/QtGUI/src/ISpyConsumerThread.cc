#include "Iguana/QtGUI/interface/ISpyConsumerThread.h"
#include "Iguana/QtGUI/interface/ISpyConsumer.h"
#include <QSettings>

ISpyConsumerThread::ISpyConsumerThread (QObject * /*parent*/) 
{
    m_restart = false;
    m_abort = false;
}

ISpyConsumerThread::~ISpyConsumerThread (void)
{
    m_mutex.lock ();
    m_abort = true;
    m_condition.wakeOne ();
    m_mutex.unlock ();

    // FIXME: wait();
}

void
ISpyConsumerThread::nextEvent (IgDataStorage *toStorage) 
{
    QMutexLocker locker (&m_mutex);
    m_storage = toStorage;

    if (! isRunning ()) 
    {
	start (LowPriority);
    } 
    else 
    {
	m_restart = true;
	m_condition.wakeOne ();
    }
}

void
ISpyConsumerThread::run (void)
{
    QSettings settings;
    std::string host = settings.value ("igsource/host").value<QString> ().toStdString();
    int port = settings.value ("igsource/port").value<int> ();
    bool debug = settings.value ("igsource/debug").value<bool> ();

    ISpyConsumer consumer (debug, host, port);
    consumer.run ();

    forever {
	m_mutex.lock ();
	// Storing anything in local variables
	// goes here
	// FIXME: 
	// IgDataStorage *storage = m_storage;
	m_mutex.unlock ();

	// Long loop
	{	    
	    if (m_restart)
		break;
	    if (m_abort)
	    {		
		onInterrupt (0);
		consumer.shouldStop ();
		consumer.shutdown ();
		
		return;
	    }
	}
	
	m_mutex.lock ();
	if (! m_restart)
	    m_condition.wait (&m_mutex);
	m_restart = false;
	m_mutex.unlock ();	    
    }    
}
