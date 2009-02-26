#ifndef IGGI_IGGI_MAIN_WINDOW_H
# define IGGI_IGGI_MAIN_WINDOW_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "ui_IgMainWindow.h"
# include <QObject>
# include "IgPolarCoordSystem.h"
# include "IgPolarCSControl.h"
# include "IgRZCoordSystem.h"
# include "IgRZCSControl.h"
# include "IggiScene.h"
# include "IggiItem.h"
# include "IggiControlCenter.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgLocationDialog;
class IgSettingsTree;
class QSettings;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IggiMainWindow : public QMainWindow, public Ui::IgMainWindow
{
    Q_OBJECT
public:
    IggiMainWindow (QWidget *parent = 0);
    ~IggiMainWindow ();

    void setZoomFocus (IggiItem* item);
		
public slots:
    void showControlCenter (bool value);
    void showRPhiGrid (bool value);
    void showRZGrid (bool value);
    void zoomIn ();
    void zoomOut ();
    void writeSettings (bool value);
    void showSettingsEditor (void);    
    void about (void);
    void aboutQt (void);
    void openSettings (void);
    void openIniFile (void);
    void openPropertyList (void);
    void openRegistryPath (void);
    void refreshSettingsEditor (void);
    void setAutoRefresh (bool value);
    void setFallbacksEnable (bool value);
    
private:
    IgPolarCoordSystem* m_RPhiGrid;
    IgPolarCSControl*   m_RPhiGridControl;
    IgRZCoordSystem*    m_RZGrid;
    IgRZCSControl*      m_RZGridControl;
    IggiScene*          m_scene;
    IggiItem*           m_focus;
    IggiControlCenter*  m_controlCenter;
    qreal		m_xScale;
    qreal		m_yScale;

    IgLocationDialog 	*locationDialog_;
    IgSettingsTree 	*settingsTree_;

    void 		setSettingsObject (QSettings *settings);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGGI_IGGI_MAIN_WINDOW_H
