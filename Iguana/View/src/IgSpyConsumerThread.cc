//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/View/interface/IgSpyConsumerThread.h"
#include "Iguana/View/interface/IgSpyConsumer.h"
#include <QSettings>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgSpyConsumerThread::IgSpyConsumerThread(QObject * /*parent*/) 
{
    restart_ = false;
    abort_ = false;
}

IgSpyConsumerThread::~IgSpyConsumerThread(void)
{
    mutex_.lock();
    abort_ = true;
    condition_.wakeOne();
    mutex_.unlock();

    // FIXME: wait();
}

void
IgSpyConsumerThread::nextEvent (void) 
{
    QMutexLocker locker(&mutex_);

    if (! isRunning()) 
    {
	start(LowPriority);
    } 
    else 
    {
	restart_ = true;
	condition_.wakeOne();
    }
}

void
IgSpyConsumerThread::run(void)
{
    QSettings settings;
    std::string host = settings.value ("igsource/host").value<QString> ().toStdString();
    int port = settings.value ("igsource/port").value<int> ();
    bool debug = settings.value ("igsource/debug").value<bool> ();

    IgSpyConsumer consumer(debug, host, port);
    consumer.run();

    forever {
	mutex_.lock();
	// Storing anything in local variables
	// goes here
	mutex_.unlock();

	// Long loop
	{	    
	    if (restart_)
		break;
	    if (abort_)
	    {		
		onInterrupt(0);
		consumer.shouldStop();
		consumer.shutdown();
		
		return;
	    }
	}
	
	mutex_.lock();
	if (!restart_)
	    condition_.wait(&mutex_);
	restart_ = false;
	mutex_.unlock();	    
    }    
}
