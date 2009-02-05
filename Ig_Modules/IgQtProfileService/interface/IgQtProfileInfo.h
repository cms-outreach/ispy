#ifndef IG_QT_PROFILE_SERVICE_IG_QT_PROFILE_INFO_H
# define IG_QT_PROFILE_SERVICE_IG_QT_PROFILE_INFO_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtProfileService/interface/config.h"
# include <qdialog.h>

class QVBoxLayout;
class QMenuBar;
class QWidget;
class QStatusBar;

class IgOProfConfiguration;
class IgOProfMainWindow;
class IgState;

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/**This class is a QDialog which calls the IgQtProfileService to populate itself with 
   the profiler GUI widget.
 */
class IgQtProfileInfo : public QDialog
{
public:
    IgQtProfileInfo (IgState *state);
    /** Checks if the GUI is ready before showing it.*/
    void show(void);        
public slots:
/**updates the GUI getting new profiling information from oprofile.
 */
void update(const char *filter);    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
private:
    IgState *m_state;
    /** Contains the configuration for the profiler and for the GUI.
     */
    IgOProfConfiguration *m_config;
    QVBoxLayout *m_layout;
    /** The menubar associated to the widget.
     */
    QMenuBar *m_menuBar;
    /** The qdialog widget*/
    QWidget *m_mainWidget;
    /** The status bar of the dialog.*/ 
    QStatusBar *m_statusBar;
    /** The profiler widget*/
    IgOProfMainWindow *m_profiler_widget;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_PROFILE_SERVICE_IG_QT_PROFILE_INFO_H
