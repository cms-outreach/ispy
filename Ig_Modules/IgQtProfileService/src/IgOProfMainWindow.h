#ifndef __IG_OPROF_MAIN_WINDOW_H__
#define __IG_OPROF_MAIN_WINDOW_H__

#include <qobject.h>

class IgState;
class QStatusBar;
class QVBoxLayout;
class QString;
class IgOProfMainListView;
class IgOProfConfiguration;
class IgOProfMenuBar;
class QPushButton;
class QHBox;
class QLineEdit;
class QLabel;
class QMenuBar;

class IgOProfMainWindow : public QObject
{
    Q_OBJECT
private:
    IgOProfConfiguration	*m_config;
    IgOProfMainListView 	*m_listView;
    QMenuBar			*m_parentMenuBar;
    IgOProfMenuBar 		*m_menuBar;
    QPushButton 		*m_updateButton;
    QVBoxLayout		 	*m_layout;
    QWidget			*m_parent;
    QHBox			*m_buttons;
    QHBox			*m_filterBox;
    QLineEdit			*m_filterLineEdit;
    QLabel			*m_filterLabel;
    QStatusBar 			*m_statusBar;
    bool		        m_inDialog;
    bool 			m_readyToRun;
    
    void checkExec (QString *exec,const char *filter);
public:
    /**
       Saves passed QMenuBar, QWidget, QStatusBar in object local
       storage for later retrivial.
     */
    IgOProfMainWindow (QMenuBar *parentMenuBar, QWidget *parent, QStatusBar *statusbar, bool inDialog);
    /**
       Get information about kernel and oprofile counters.  

       \return true on success, false on user cancellation.
     */
    bool init (void);
    /**
       Actually creates the GUI.  
       
       \param disabled if true the gui only allows to load, open a
       remote site or quit.
     */
    void createGUI (bool disabled);
    /**\return the QWidget which embeds the GUI.
     */
    QWidget &mainWindow (void);
    /**\return false if the users decides to abort the initialization
       process*/
    inline bool readyToRun (void){return m_readyToRun;}
    /**Enables/Disables all the GUI elements which are not needed when
      the profiler is off.*/
    void setGUIStatus (bool status);    
public slots:
void askToSave (void);
    void setFilterAndUpdate (void);
};

#endif
