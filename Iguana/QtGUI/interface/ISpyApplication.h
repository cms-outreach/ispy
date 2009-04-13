#ifndef IGGI_IGGI_APPLICATION_H
# define IGGI_IGGI_APPLICATION_H

//<<<<<< INCLUDES                                                       >>>>>>

# include <QObject>
# include <QTimer>
# include "Iguana/QtGUI/interface/ISpyConsumerThread.h"
# include "classlib/zip/ZipArchive.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;
class IgCollection;
class ISpyConsumerThread;
class ISpyMainWindow;
class ISpyTreeModel;
class QModelIndex;
class QSplashScreen;
class IgDataStorage;
class IgCollectionTableModel;
class IgMultiStorageTreeModel;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class ISpyApplication : public QObject
{ 
    Q_OBJECT
public:
    ISpyApplication (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    virtual ~ISpyApplication (void);

    virtual int		run (int argc, char *argv[]);

    virtual IgState    *state (void) const;
    virtual int		argc (void) const;
    virtual char      **argv (void) const;
    virtual const char *appname (void) const;

public slots:
    void 		open (void);
    void 		connect (void);
    void 		autoEvents (void);
    void		nextEvent (void);
    void		previousEvent (void);
    void		rewind (void);
    void 		print (void);
    void 		save (void);
    void		processEvent (unsigned int runNum, unsigned int eventNum);
    void		processEventDialog (void);
    void		skipEvent (long num);
    void		skipEventDialog (void);

protected:
    virtual int         usage (void);
    virtual int		version (void);
    virtual void        setupMainWindow (void);
    virtual int         initState (void);

private slots:
    void 		collectionChanged (const QModelIndex &index);
    void		displayCollection (const QModelIndex &index);
    void		displayItem (const QModelIndex &index);
    
    void		onExit (void);
    void		exit (void);

private:
    int 		doRun (void);
    void		closeFile (void);
    void		loadFile (const QString &fileName);
    void 		readZipMember (lat::ZipArchive::Iterator it);
    void		closeGeomFile (void);
    void		loadGeomFile (const QString &fileName);
    void 		readGeomZipMember (lat::ZipArchive::Iterator it);
    void		defaultSettings (void);
    void		restoreSettings (void);
    void		restoreMainWindowSettings (void);
    void		saveSettings (void);    
    void		drawCollection (IgCollection *collection);
    void		cleanScene (void);
    void   		printVector (QString file, QString format, int level);
    int			getGL2PSOptions (void);
    
    IgState	       *m_state;
    int			m_argc;
    char	      **m_argv;
    ISpyMainWindow     *m_mainWindow;
    lat::ZipArchive    *m_archive;
    lat::ZipArchive    *m_geomArchive;
    char               *m_appname;
    lat::ZipArchive::Iterator 	m_it; 
    lat::ZipArchive::Iterator 	m_geomIt; 

    IgCollectionTableModel     *m_collectionModel;
    IgMultiStorageTreeModel    *m_storageModel; 
    IgDataStorage 	       *m_storage;
    IgDataStorage 	       *m_geomStorage;
    QSplashScreen 	       *m_splash;
    ISpyConsumerThread 		m_consumer;
    
    bool	    	m_init;
    QTimer	       *m_timer;
    bool		m_auto;
    int 		m_gl2psOptions;
    int                 m_gl2psFBBufferSize;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGGI_IGGI_APPLICATION_H
