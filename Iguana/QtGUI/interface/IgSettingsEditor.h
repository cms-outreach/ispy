#ifndef IGGI_IG_SETTINGS_EDITOR_H
# define IGGI_IG_SETTINGS_EDITOR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "ui_IgSettingsEditorDialog.h"
# include <QObject>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QSettings;
class IgLocationDialog;
class IgSettingsTree;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IgSettingsEditor : public QDialog, public Ui::IgSettingsEditorDialog
{
    Q_OBJECT
public:
    IgSettingsEditor (QWidget *parent = 0);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    void		refresh (void);
    void 		setAutoRefresh (bool value);
    void		setFallbacksEnabled (bool value);
    void 		setSettingsObject (QSettings *settings);
    
private:
    IgSettingsTree 	*settingsTree_;
    IgLocationDialog 	*locationDialog_;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGGI_IG_SETTINGS_EDITOR_H
