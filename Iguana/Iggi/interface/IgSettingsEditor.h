#ifndef IGGI_IG_SETTINGS_EDITOR_H
# define IGGI_IG_SETTINGS_EDITOR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "ui_IgSettingsEditorMainWindow.h"
# include <QObject>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

 class QAction;
 class QMenu;
 class QSettings;

 class IgLocationDialog;
 class IgSettingsTree;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IgSettingsEditor : public QMainWindow, public Ui::IgSettingsEditorMainWindow
{
     Q_OBJECT
public:
    IgSettingsEditor (QWidget *parent = 0);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

private slots:
     void openSettings (void);
     void openIniFile (void);
     void openPropertyList (void);
     void openRegistryPath (void);
     void about (void);

 private:
     void setSettingsObject (QSettings *settings);

     IgSettingsTree 	*settingsTree_;
     IgLocationDialog 	*locationDialog_;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGGI_IG_SETTINGS_EDITOR_H
