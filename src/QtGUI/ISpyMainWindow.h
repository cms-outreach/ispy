#ifndef IGGI_IGGI_MAIN_WINDOW_H
# define IGGI_IGGI_MAIN_WINDOW_H

# include "ui/ui_IgMainWindowMDI.h"
# include <QObject>

class IgLocationDialog;
class IgSettingsEditor;
class QSettings;
class QMdiArea;

class ISpyMainWindow : public QMainWindow, public Ui::IgMainWindow
{
  Q_OBJECT
public:
  ISpyMainWindow(QWidget *parent = 0);
  ~ISpyMainWindow();

  void          restoreSettings(void);
  void          saveSettings(void);
  QWidget *     workspace(void);

public slots:
  void          writeSettings(bool value);
  void          showSettingsEditor(void);
  void          maximize(void);
  void          fullScreen(void);

signals:
  void          open(void);
  void          nextEvent(void);
  void          previousEvent(void);
  void          rewind(void);
  void          print(void);
  void          save(void);
  void		showAbout(void);

protected:
  void          setupActions(void);

private:
  IgSettingsEditor   *m_settingsEditor;
  void          setSettingsObject(QSettings *settings);
};

#endif // IGGI_IGGI_MAIN_WINDOW_H
