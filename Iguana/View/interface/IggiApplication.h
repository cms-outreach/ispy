#ifndef IGGI_IGGI_APPLICATION_H
# define IGGI_IGGI_APPLICATION_H

//<<<<<< INCLUDES                                                       >>>>>>

# include <QObject>
# include "classlib/zip/ZipArchive.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;
class IggiMainWindow;
class IggiTreeModel;
class QModelIndex;
class IgDataStorage;
class IgCollectionTableModel;
class IgMultiStorageTreeModel;
class SoSeparator;
class QTreeView;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IggiApplication : public QObject
{ 
    Q_OBJECT
public:
    IggiApplication (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    virtual ~IggiApplication (void);

    virtual int		run (int argc, char *argv[]);

    virtual IgState *	state (void) const;
    virtual int		argc (void) const;
    virtual char **	argv (void) const;
    virtual const char *appname (void) const;

public slots:
    void 		open (void);
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
    virtual void        setupMainWindow (void);
    virtual int         initState (void);

private slots:
    void 		collectionChanged (const QModelIndex &index);
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
    SoSeparator * 	createBackground (void);
    SoSeparator * 	createSuperimposition (std::string label);
    
    IgState	       *m_state;
    int			m_argc;
    char	      **m_argv;
    IggiMainWindow     *m_mainWindow;
    IggiTreeModel      *m_model;
    lat::ZipArchive    *m_archive;
    lat::ZipArchive    *m_geomArchive;
    char               *m_appname;
    lat::ZipArchive::Iterator m_it; 
    lat::ZipArchive::Iterator m_geomIt; 

    IgCollectionTableModel 	*m_collectionModel;
    IgMultiStorageTreeModel 	*m_storageModel; 
    IgDataStorage 		*m_storage;
    IgDataStorage 		*m_geomStorage;
    QTreeView			*m_myTreeView;

    bool	    	m_init;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGGI_IGGI_APPLICATION_H
