#ifndef IG_QT_MDI_H
#define IG_QT_MDI_H

#include "Ig_Modules/IgVis/interface/config.h"
#include "qmainwindow.h"
#include <vector>

/*! 
* \class IgQtMDI IgQtMDI.h Ig_Modules/IgVis/interface/IgQtMDI.h
*
* \brief Top level class to contain Multiple Document Interface
* Qt objects for event browsing.
*
*
* \author G. Alverson, Northeastern University
* \date 08-Sep-2000
*
*/

class QWorkspace;
class QToolBox;
class IgQtMDIAbsShell;
class IgQtMDIAbsShellBuilder;

class IG_VIS_API IgQtMDI : public QMainWindow {
    Q_OBJECT
public:
    //! c-tor
    IgQtMDI(QWidget *parent, QString title);
    //! to register a child browser
    virtual void addBrowser( IgQtMDIAbsShell* browser );
    //! to de-register a child browser
    virtual void removeBrowser( IgQtMDIAbsShell* browser );
    //! accessor for the central workspace
    QWorkspace* workspace(void);
    //! accessor for the information toolbar
    QToolBox* infobox(void);
    //! accessor for controls
    QToolBox* twigbox(void);
    //! causes the MDI to query the resourceMgr for the previous status
    bool restoreResources( void );
    //! saves current options
    void saveResources( void );
public slots:
signals:
    //! Signal to quit
    void quit();
protected:
    //! the pointer to the central workspace widget
    QWorkspace  *theWorkspace;
    //! the pointer to information toolbar
    QToolBox *theInfobox;
    //! the pointer to twig control widgets toolbar
    QToolBox *theTwigbox;
    //! to control contained windows
    QPopupMenu* windowsMenu;
    //! the file level menu
    QPopupMenu* topFile;
protected slots:
    //! to create a new scan window
    virtual void newScanSlot( int );
    //! to set the default font
    void fontDialogSlot();
protected:
    //! list of child browsers
    std::vector<IgQtMDIAbsShell*> childBrowsers;
    //! list of possible browsers 1:1 with new browser menu
    std::map< int, IgQtMDIAbsShellBuilder* > browserMenuItems;
private slots:
    //! use quitSlot to forward the quit signal onwards
    void quitSlot();
    //! tell us about yourself...
    void about();
    void windowsMenuAboutToShow();
    void windowsMenuActivated( int id );
};

#endif // IG_QT_MDI_H
