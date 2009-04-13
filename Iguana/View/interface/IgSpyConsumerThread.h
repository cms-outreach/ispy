#ifndef VIEW_IG_SPY_CONSUMER_THREAD_H
# define VIEW_IG_SPY_CONSUMER_THREAD_H

//<<<<<< INCLUDES                                                       >>>>>>

# include <QThread>
# include <QMutex>
# include <QWaitCondition>
# include "Iguana/Framework/interface/IgNet.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IgSpyConsumerThread : public QThread
{
    Q_OBJECT

public:
    IgSpyConsumerThread(QObject *parent = 0);
    ~IgSpyConsumerThread (void);
    
    void	nextEvent (void);

signals:
    void 	newEvent(void);

protected:
    void run(void);

private:
    QMutex 		mutex_;
    QWaitCondition 	condition_;
    bool 		restart_;    
    bool		abort_;
    IgNet::Object      *data_;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIEW_IG_SPY_CONSUMER_THREAD_H
