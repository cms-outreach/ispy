#ifndef IGGI_IG_SETTINGS_EDITOR_H
# define IGGI_IG_SETTINGS_EDITOR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "ui_IgSettingsEditorMainWindow.h"
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

class IgSettingsEditor : public QMainWindow, private Ui::IgSettingsEditorMainWindow
{
  Q_OBJECT
public:
  IgSettingsEditor(QWidget *parent = 0);
  // implicit copy constructor
  // implicit assignment operator
  // implicit destructor

  void          setSettingsObject(QSettings *settings);

public slots:
  void          about(void);
  void          openSettings(void);
  void          openIniFile(void);
  void          openPropertyList(void);
  void          openRegistryPath(void);
  void          refresh(void);
  void          setAutoRefresh(bool value);
  void          setFallbacksEnabled(bool value);

private:
  void          setupActions(void);

  IgSettingsTree        *m_settingsTree;
  IgLocationDialog      *m_locationDialog;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGGI_IG_SETTINGS_EDITOR_H
