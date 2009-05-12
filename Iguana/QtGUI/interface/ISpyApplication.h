#ifndef IGGI_IGGI_APPLICATION_H
# define IGGI_IGGI_APPLICATION_H

//<<<<<< INCLUDES                                                       >>>>>>

# include <QObject>
# include <QStringList> 
# include <QTimer>
# include "Iguana/QtGUI/interface/ISpyConsumerThread.h"
# include "Iguana/QtGUI/interface/ISpyReadThread.h"
# include "classlib/zip/ZipArchive.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;
class IgCollection;
class ISpyMainWindow;
class ISpyTreeModel;
class QModelIndex;
class IgDataStorage;
class IgCollectionTableModel;
class IgMultiStorageTreeModel;
class Ig3DBaseModel;
class QTreeWidget;
class QTreeWidgetItem;
class SoSwitch;
class ISpySplashScreen;

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
    void 		openFileDialog (void);
    void 		open (const QString &fileName);
    void 		openFile (const QString &fileName);
    void 		connect (void);
    void 		autoEvents (void);
    void		nextEvent (void);
    void		previousEvent (void);
    void		rewind (void);
    void		processEvent (unsigned int runNum, unsigned int eventNum);
    void		processEventDialog (void);
    void		skipEvent (long num);
    void		skipEventDialog (void);

signals:
    void 		modelChanged (void);
    void 		showMessage (const QString &fileName);
    void 		print (void);
    void 		save (void);
    void 		firstEvent (void);
    void 		lastEvent (void);
    void		splashDone (void);

protected:
    virtual int         usage (void);
    virtual int		version (void);
    virtual void        setupMainWindow (void);
    virtual int         initState (void);

private slots:
    void		cleanSelection (void);
    void		displayCollection (const QModelIndex &index);
    void		displayItem (const QModelIndex &index);
    void		newMember (const QString& name);
    void		initTreeItems (IgDataStorage *storage);
    void		updateTreeItems (IgDataStorage *storage);
    void 		twigChanged (const QModelIndex &index);
    void		displayTwigCollection (IgDataStorage *storage);
    void		cleanSplash (void);
    
    void		onExit (void);
    void		exit (void);

private:
    void		createCollectionModel (IgDataStorage *storage);    
    int 		doRun (void);
    void		closeFile (void);
    void		loadFile (const QString &fileName);
    void 		parseZipMember (lat::ZipArchive::Iterator it);
    void 		readZipMember (lat::ZipArchive::Iterator it);
    void		closeGeomFile (void);
    void		loadGeomFile (const QString &fileName);
    void 		readGeomZipMember (lat::ZipArchive::Iterator it);
    void		defaultSettings (void);
    void		restoreSettings (void);
    void		drawCollection (IgCollection *collection);
    
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
    Ig3DBaseModel	       *m_3DModel;
    ISpyTreeModel	       *m_model;
    QTreeWidget 	       *m_treeWidget;    
    IgDataStorage 	       *m_storage;
    IgDataStorage 	       *m_geomStorage;
    ISpySplashScreen 	       *m_splash;
    ISpyConsumerThread 		m_consumer;
    ISpyReadThread		m_readThread;
    QList<QTreeWidgetItem *> 	m_items;
    QStringList			m_fileNames;

    typedef std::map<const std::string, SoSwitch *> CollDrawMap;
    CollDrawMap		m_collDraw;
    
    bool	    	m_onExit;
    QTimer	       *m_timer;
    bool		m_auto;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGGI_IGGI_APPLICATION_H
