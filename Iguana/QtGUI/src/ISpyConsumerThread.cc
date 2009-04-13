//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/ISpyConsumerThread.h"
#include "Iguana/QtGUI/interface/ISpyConsumer.h"
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

ISpyConsumerThread::ISpyConsumerThread(QObject * /*parent*/) 
{
    restart_ = false;
    abort_ = false;
}

ISpyConsumerThread::~ISpyConsumerThread(void)
{
    mutex_.lock();
    abort_ = true;
    condition_.wakeOne();
    mutex_.unlock();

    // FIXME: wait();
}

void
ISpyConsumerThread::nextEvent (void) 
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
ISpyConsumerThread::run(void)
{
    QSettings settings;
    std::string host = settings.value ("igsource/host").value<QString> ().toStdString();
    int port = settings.value ("igsource/port").value<int> ();
    bool debug = settings.value ("igsource/debug").value<bool> ();

    ISpyConsumer consumer(debug, host, port);
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
