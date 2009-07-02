#include "Iguana/QtGUI/interface/ISpySplashScreen.h"
#include "Iguana/QtGUI/interface/ISpyApplication.h"
#include "Iguana/QtGUI/interface/IgNetworkReplyHandler.h"
#include <QtGui>

ISpySplashScreen::ISpySplashScreen(ISpyApplication *application, QWidget *parent)
  : QWidget(parent),
    m_application(application)
{
  setupUi(this);
  // Completely handle clicking in the GUI.
  this->wizardView->setOpenLinks(false);
  this->webView->setOpenLinks(false);
  this->ispyDescription->setOpenLinks(false);
  QObject::connect(this->wizardView,SIGNAL(anchorClicked(const QUrl&)),
                   application,SLOT(handleWizardLinks(const QUrl &)));
  QObject::connect(this->webView,SIGNAL(anchorClicked(const QUrl&)),
                   application,SLOT(handleWizardLinks(const QUrl &)));
  QObject::connect(this->ispyDescription,SIGNAL(anchorClicked(const QUrl&)),
                   application,SLOT(handleWizardLinks(const QUrl &)));

  // Get the news from http://iguana.web.cern.ch/iguana/ispy/ispy-news.htm
  // and update the news view accordingly once fetching is complete.
  QUrl newsUrl("http://iguana.web.cern.ch/iguana/ispy/ispy-news.htm");
  QNetworkReply *newsReply = m_application->getUrl(newsUrl);
  IgNetworkReplyHandler *handler = new IgNetworkReplyHandler(newsReply,
                                                             new QBuffer);
  QObject::connect(handler, SIGNAL(done(IgNetworkReplyHandler *)),
                   this, SLOT(newsDownloaded(IgNetworkReplyHandler *)));
  QObject::connect(newsReply, SIGNAL(error(QNetworkReply::NetworkError)),
                   this, SLOT(newsDownloadError(QNetworkReply::NetworkError)));
}

ISpySplashScreen::~ISpySplashScreen(void)
{
  if (! isHidden())
  {
    hide();
    splashDone();
  }
}

void
ISpySplashScreen::closeEvent(QCloseEvent *event)
{
  hide();
  splashDone();
  event->accept();
}

void
ISpySplashScreen::showAbout(void)
{
  this->tabWidget->setCurrentIndex(2);
  this->show();
  this->raise();
}

void
ISpySplashScreen::showWizard(void)
{
  this->webView->setHtml(
    "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">"
    "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">"
    "p, li { white-space: pre-wrap; }"
    "</style></head><body style=\" font-family:'Lucida Grande'; font-size:13pt; font-weight:400; font-style:normal;\">"
    "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>"
    "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>"
    "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>"
    "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>"
    "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><img src=\":/images/IGUANA_SPLASH.xpm\" /></p></body></html>"
    );
  this->tabWidget->setCurrentIndex(0);
  this->show();
  this->raise();
}

void
ISpySplashScreen::setRightPage(const QUrl &url)
{
  m_rightPage = url;
  IgNetworkReplyHandler *handler = new IgNetworkReplyHandler(m_application->getUrl(url),
                                                             new QBuffer);
  QObject::connect(handler, SIGNAL(done(IgNetworkReplyHandler *)), this, SLOT(updateRight(IgNetworkReplyHandler *)));
}

const QUrl &
ISpySplashScreen::rightPage(void)
{
  return m_rightPage;
}

void
ISpySplashScreen::updateRight(IgNetworkReplyHandler *handler)
{
  QBuffer *buffer = static_cast<QBuffer *>(handler->device());
  QString html = buffer->data();
  html.replace("<img src=\"/icons/back.gif\" alt=\"[DIR]\"> <a href=\"/iguana/ispy/\">Parent Directory</a>", "");
  html.replace("<hr>", "<br>");
  this->webView->setHtml(html);
}

void
ISpySplashScreen::newsDownloaded(IgNetworkReplyHandler *handler)
{
  // Use our network page in case an error is found.
  if (handler->error() != QNetworkReply::NoError)
  {
    newsDownloadError(handler->error());
    return;
  }

  QBuffer *buffer = static_cast<QBuffer *>(handler->device());

  // In case everything is alright we post the news page in the correct
  // QTextBrowser.
  this->newsView->setHtml(buffer->data());
}

void
ISpySplashScreen::newsDownloadError(QNetworkReply::NetworkError)
{
  this->newsView->setHtml("<h1>Error while fetching news</h1>"
                          "<strong>There was an error while downloading news. No network connection?</strong>");
}
