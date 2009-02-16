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

private:
    int 		doRun (int argc, char *argv[]);
    void		closeFile (void);
    void		loadFile (const QString &fileName);
    void 		readZipMember (lat::ZipArchive::Iterator it);
    
    IgState	       *m_state;
    int			m_argc;
    char	      **m_argv;
    IggiMainWindow     *m_mainWindow;
    IggiTreeModel      *m_model;
    lat::ZipArchive    *m_archive;
    char               *m_appname;
    lat::ZipArchive::Iterator m_it; 
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGGI_IGGI_APPLICATION_H
