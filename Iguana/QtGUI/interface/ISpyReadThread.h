#ifndef QT_GUI_ISPY_READ_THREAD_H
# define QT_GUI_ISPY_READ_THREAD_H

//<<<<<< INCLUDES                                                       >>>>>>

# include <QThread>
# include <QMutex>
# include <QWaitCondition>
# include "classlib/zip/ZipArchive.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgDataStorage;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class ISpyReadThread : public QThread
{
    Q_OBJECT

public:
    ISpyReadThread (QObject *parent = 0);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    ~ISpyReadThread (void);
    
    void read (lat::ZipArchive *fromArchive, lat::ZipArchive::Iterator member, IgDataStorage *toStorage);

signals:
    void readMember (const QString &name);

protected:
    void run (void);

private:    
    QMutex 		m_mutex;
    QWaitCondition 	m_condition;
    lat::ZipArchive    *m_archive;
    lat::ZipArchive::Iterator 	m_it; 
    IgDataStorage      *m_storage;
    bool 		m_restart;
    bool 		m_abort;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // QT_GUI_ISPY_READ_THREAD_H
