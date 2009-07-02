#ifndef QT_GUI_ISPY_SPLASH_SCREEN_H
# define QT_GUI_ISPY_SPLASH_SCREEN_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "ui_ISpySplashScreen.h"
# include <QObject>
# include <QUrl>
# include <QtNetwork>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgNetworkReplyHandler;
class QNetworkAccessManager;
class ISpyApplication;
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Responsible for handling the behavior of the splash screen / wizard */
class ISpySplashScreen : public QWidget, private Ui::ISpySplashScreen
{
  Q_OBJECT
public:
  ISpySplashScreen(ISpyApplication *application, QWidget *parent = 0);
  // implicit copy constructor
  // implicit assignment operator
  // implicit destructor
  ~ISpySplashScreen(void);
  void                setRightPage(const QUrl &url);
  const QUrl &        rightPage(void);
public slots:
  void                showAbout(void);
  void                showWizard(void);
private slots:
  void                updateRight(IgNetworkReplyHandler *reply);
  void                newsDownloaded(IgNetworkReplyHandler *handler);
  void                newsDownloadError(QNetworkReply::NetworkError);
signals:
  void          splashDone(void);

protected:
  virtual void  closeEvent(QCloseEvent *event);
private:
  ISpyApplication             *m_application;
  QNetworkAccessManager       *m_networkManager;
  QUrl                        m_rightPage;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // QT_GUI_ISPY_SPLASH_SCREEN_H
