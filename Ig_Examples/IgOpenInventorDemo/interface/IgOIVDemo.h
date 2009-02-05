#ifndef IG_OPEN_INVENTOR_DEMO_IG_OIV_DEMO_H
# define IG_OPEN_INVENTOR_DEMO_IG_OIV_DEMO_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Examples/IgOpenInventorDemo/interface/config.h"
# include <qmainwindow.h>
# include <qprocess.h>
# include <qstring.h>
# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QTextEdit;
class QListView;
class QVBoxLayout;
class QListViewItem;
class IgState;
class QMainWindow;
class QButtonGroup;
class QRadioButton;
class QSplitter;
class QDir;
class QFileInfo;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_OPEN_INVENTOR_DEMO_API IgOIVDemo : public QObject
{
    Q_OBJECT
public:
    IgOIVDemo (IgState *state);
    ~IgOIVDemo (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

private slots:
    void 			load (const QString &fileName);
    void 			demoChanged (QListViewItem *item);
    void 			dirChanged (void);
    void 			flistChanged (QListViewItem *item);
    void 			print (void);
    void 			cleanup (void);

private:
    void 			init (const QString &name);
    QString			getIGUANATop (unsigned int id = 0);
    void 			checkDemoItem (const QDir &dir, const QString &file);
    void 			terminateChild (void);
    void 			startChild (const QString &name);
    
    IgState			*m_state;
    QMainWindow  		*m_mainWindow;
    QButtonGroup		*m_buttonGroup;
    QProcess			*m_proc;
    QPrinter 			*m_printer;
    QTextEdit 			*m_file;
    QListView 			*m_demo;
    QListView 			*m_flist;
    QProcess::PID 		m_pid;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_OPEN_INVENTOR_DEMO_IG_OIV_DEMO_H
