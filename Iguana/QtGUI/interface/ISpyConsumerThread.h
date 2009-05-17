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

class IgDataStorage;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class ISpyConsumerThread : public QThread
{
    Q_OBJECT

public:
    ISpyConsumerThread(QObject *parent = 0);
    ~ISpyConsumerThread (void);
    
    void	nextEvent (IgDataStorage *toStorage);

signals:
    void 	newEvent (const QString &name);

protected:
    void 	run (void);

private:
    QMutex 		m_mutex;
    QWaitCondition 	m_condition;
    IgDataStorage      *m_storage;
    bool 		m_restart;    
    bool		m_abort;
    IgNet::Object      *m_data;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIEW_IG_SPY_CONSUMER_THREAD_H
