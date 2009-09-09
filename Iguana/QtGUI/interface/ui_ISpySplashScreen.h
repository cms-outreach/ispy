/********************************************************************************
 ** Form generated from reading ui file 'ISpySplashScreen.ui'
 **
 ** Created: Tue Jun 9 21:45:18 2009
 **      by: Qt User Interface Compiler version 4.5.0
 **
 ** WARNING! All changes made in this file will be lost when recompiling ui file!
 ********************************************************************************/

#ifndef ISPYSPLASHSCREEN_H
#define ISPYSPLASHSCREEN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QTabWidget>
#include <QtGui/QTextBrowser>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ISpySplashScreen
{
public:
  QVBoxLayout *verticalLayout;
  QVBoxLayout *verticalLayout_4;
  QTabWidget *tabWidget;
  QWidget *gettingStartedTab;
  QWidget *verticalLayoutWidget;
  QVBoxLayout *verticalLayout_2;
  QLabel *label;
  QHBoxLayout *horizontalLayout_3;
  QTextBrowser *wizardView;
  QTextBrowser *webView;
  QWidget *newsTab;
  QWidget *verticalLayoutWidget_2;
  QVBoxLayout *verticalLayout_3;
  QTextBrowser *newsView;
  QWidget *aboutISpyTab;
  QWidget *horizontalLayoutWidget_2;
  QHBoxLayout *horizontalLayout_4;
  QTextBrowser *ispyLogo;
  QTextBrowser *ispyDescription;
  QCheckBox *showAtLaunchCheckbox;

  void setupUi(QWidget *ISpySplashScreen)
    {
      if (ISpySplashScreen->objectName().isEmpty())
        ISpySplashScreen->setObjectName(QString::fromUtf8("ISpySplashScreen"));
      ISpySplashScreen->setWindowModality(Qt::ApplicationModal);
      ISpySplashScreen->resize(892, 582);
      ISpySplashScreen->setMinimumSize(QSize(892, 582));
      ISpySplashScreen->setMaximumSize(QSize(892, 582));
      ISpySplashScreen->setStyleSheet(QString::fromUtf8("QFrame {\n"
                                                        "border: 0px;\n"
                                                        "}\n"
                                                        "\n"
                                                        "QTabWidget {\n"
                                                        "border: 0px;\n"
                                                        "background-color: white;\n"
                                                        "}\n"
                                                        "\n"
                                                        ""));
      verticalLayout = new QVBoxLayout(ISpySplashScreen);
      verticalLayout->setSpacing(0);
      verticalLayout->setMargin(0);
      verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
      verticalLayout_4 = new QVBoxLayout();
      verticalLayout_4->setSpacing(0);
      verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
      tabWidget = new QTabWidget(ISpySplashScreen);
      tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
      tabWidget->setCursor(QCursor(Qt::ArrowCursor));
      tabWidget->setStyleSheet(QString::fromUtf8("QFrame {\n"
                                                 "border: 0px;\n"
                                                 "}\n"
                                                 "\n"
                                                 "QTabWidget {\n"
                                                 "background-color: white;\n"
                                                 "}\n"
                                                 "\n"
                                                 "QTabWidget QWidget {\n"
                                                 "background-color: white;\n"
                                                 "}\n"
                                                 "QTabBar {\n"
                                                 "     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
                                                 "                                 stop: 0 #E1E1E1, stop: 0.4 #DDDDDD,\n"
                                                 "                                 stop: 0.5 #D8D8D8, stop: 1.0 #FFFFFF);\n"
                                                 "}\n"
                                                 "\n"
                                                 "QTabBar::tab {\n"
                                                 "    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
                                                 "                                 stop: 0 #E1E1E1, stop: 0.4 #DDDDDD,\n"
                                                 "                                 stop: 0.5 #D8D8D8, stop: 1.0 #FFFFFF);\n"
                                                 "     color: #0a5202;\n"
                                                 "     border: 0px;\n"
                                                 "     min-width: 8ex;\n"
                                                 "     padding: 7px;\n"
                                                 "      padding-left: 30px;\n"
                                                 "      padding-right: 30px;\n"
                                                 " }\n"
                                                 "\n"
                                                 "QTabBar::tab:selected {\n"
                                                 "     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
                                                 "                                 stop: 0 #E1E1E1, stop: 0.4 #DDDDDD,\n"
                                                 "                                 stop: 0.5 #D8D8D8, stop: 1.0 #FFFFFF);\n"
                                                 ""
                                                 "     color: #800000;\n"
                                                 "\n"
                                                 "       border: 0px;\n"
                                                 "     min-width: 8ex;\n"
                                                 " }\n"
                                                 ""));
      tabWidget->setDocumentMode(false);
      tabWidget->setTabsClosable(false);
      gettingStartedTab = new QWidget();
      gettingStartedTab->setObjectName(QString::fromUtf8("gettingStartedTab"));
      gettingStartedTab->setCursor(QCursor(Qt::ArrowCursor));
      verticalLayoutWidget = new QWidget(gettingStartedTab);
      verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
      verticalLayoutWidget->setGeometry(QRect(0, 0, 881, 521));
      verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget);
      verticalLayout_2->setSpacing(0);
      verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
      verticalLayout_2->setContentsMargins(0, 0, 0, 0);
      label = new QLabel(verticalLayoutWidget);
      label->setObjectName(QString::fromUtf8("label"));
      QFont font;
      font.setBold(true);
      font.setWeight(75);
      label->setFont(font);
      label->setStyleSheet(QString::fromUtf8("\n"
                                             "font-size: 32px;\n"
                                             "font-weight: bold;\n"
                                             "color: qradialgradient(spread:pad, cx:0.505, cy:1, radius:1.028, fx:0.505, fy:0.381455, stop:0 rgba(9, 41, 4, 255), stop:0.286432 rgba(2, 79, 0, 255), stop:0.703518 rgba(50, 147, 22, 255), stop:1 rgba(236, 191, 49, 255));\n"
                                             "padding: 7px;\n"
                                             "padding-left: 220px;\n"
                                             ""));

      verticalLayout_2->addWidget(label);

      horizontalLayout_3 = new QHBoxLayout();
      horizontalLayout_3->setSpacing(0);
      horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
      wizardView = new QTextBrowser(verticalLayoutWidget);
      wizardView->setObjectName(QString::fromUtf8("wizardView"));
      QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
      sizePolicy.setHorizontalStretch(255);
      sizePolicy.setVerticalStretch(0);
      sizePolicy.setHeightForWidth(wizardView->sizePolicy().hasHeightForWidth());
      wizardView->setSizePolicy(sizePolicy);
      wizardView->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0.00145729 rgba(255, 255, 255, 255), stop:0.0100503 rgba(50, 153, 0, 255), stop:0.0904523 rgba(238, 238, 238, 255), stop:1 rgba(255, 255, 255, 255));\n"
                                                  "padding-left:14px;"));
      wizardView->setLineWidth(0);

      horizontalLayout_3->addWidget(wizardView);

      webView = new QTextBrowser(verticalLayoutWidget);
      webView->setObjectName(QString::fromUtf8("webView"));
      webView->setStyleSheet(QString::fromUtf8("QTextBrowser\n"
                                               "{\n"
                                               "background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0.00145729 rgba(255, 255, 255, 255), stop:0.0100503 rgba(50, 153, 0, 255), stop:0.0904523 rgba(238, 238, 238, 255), stop:1 rgba(255, 255, 255, 255));\n"
                                               "padding-bottom: 40px;\n"
                                               "}"));

      horizontalLayout_3->addWidget(webView);


      verticalLayout_2->addLayout(horizontalLayout_3);

      tabWidget->addTab(gettingStartedTab, QString());
      newsTab = new QWidget();
      newsTab->setObjectName(QString::fromUtf8("newsTab"));
      verticalLayoutWidget_2 = new QWidget(newsTab);
      verticalLayoutWidget_2->setObjectName(QString::fromUtf8("verticalLayoutWidget_2"));
      verticalLayoutWidget_2->setGeometry(QRect(0, 0, 881, 511));
      verticalLayout_3 = new QVBoxLayout(verticalLayoutWidget_2);
      verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
      verticalLayout_3->setContentsMargins(0, 0, 0, 0);
      newsView = new QTextBrowser(verticalLayoutWidget_2);
      newsView->setObjectName(QString::fromUtf8("newsView"));

      verticalLayout_3->addWidget(newsView);

      tabWidget->addTab(newsTab, QString());
      aboutISpyTab = new QWidget();
      aboutISpyTab->setObjectName(QString::fromUtf8("aboutISpyTab"));
      horizontalLayoutWidget_2 = new QWidget(aboutISpyTab);
      horizontalLayoutWidget_2->setObjectName(QString::fromUtf8("horizontalLayoutWidget_2"));
      horizontalLayoutWidget_2->setGeometry(QRect(0, 0, 881, 521));
      horizontalLayout_4 = new QHBoxLayout(horizontalLayoutWidget_2);
      horizontalLayout_4->setSpacing(0);
      horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
      horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
      ispyLogo = new QTextBrowser(horizontalLayoutWidget_2);
      ispyLogo->setObjectName(QString::fromUtf8("ispyLogo"));
      QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
      sizePolicy1.setHorizontalStretch(0);
      sizePolicy1.setVerticalStretch(0);
      sizePolicy1.setHeightForWidth(ispyLogo->sizePolicy().hasHeightForWidth());
      ispyLogo->setSizePolicy(sizePolicy1);
      ispyLogo->setAcceptDrops(false);
      ispyLogo->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0.00145729 rgba(255, 255, 255, 255), stop:0.0100503 rgba(50, 153, 0, 255), stop:0.0904523 rgba(238, 238, 238, 255), stop:1 rgba(255, 255, 255, 255));"));
      ispyLogo->setFrameShape(QFrame::NoFrame);
      ispyLogo->setFrameShadow(QFrame::Plain);
      ispyLogo->setOpenExternalLinks(true);
      ispyLogo->setOpenLinks(true);

      horizontalLayout_4->addWidget(ispyLogo);

      ispyDescription = new QTextBrowser(horizontalLayoutWidget_2);
      ispyDescription->setObjectName(QString::fromUtf8("ispyDescription"));
      ispyDescription->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0.00145729 rgba(255, 255, 255, 255), stop:0.0100503 rgba(50, 153, 0, 255), stop:0.0904523 rgba(238, 238, 238, 255), stop:1 rgba(255, 255, 255, 255))"));
      ispyDescription->setLineWidth(3);

      horizontalLayout_4->addWidget(ispyDescription);

      tabWidget->addTab(aboutISpyTab, QString());

      verticalLayout_4->addWidget(tabWidget);

      showAtLaunchCheckbox = new QCheckBox(ISpySplashScreen);
      showAtLaunchCheckbox->setObjectName(QString::fromUtf8("showAtLaunchCheckbox"));
      showAtLaunchCheckbox->setEnabled(false);
      showAtLaunchCheckbox->setStyleSheet(QString::fromUtf8("padding: 7px;"));
      showAtLaunchCheckbox->setChecked(true);

      verticalLayout_4->addWidget(showAtLaunchCheckbox);


      verticalLayout->addLayout(verticalLayout_4);


      retranslateUi(ISpySplashScreen);

      tabWidget->setCurrentIndex(0);


      QMetaObject::connectSlotsByName(ISpySplashScreen);
    } // setupUi

  void retranslateUi(QWidget *ISpySplashScreen)
    {
      ISpySplashScreen->setWindowTitle(QApplication::translate("ISpySplashScreen", "IGUANA iSpy", 0, QApplication::UnicodeUTF8));
      label->setText(QApplication::translate("ISpySplashScreen", "Welcome to iSpy 1.3.0rc1", 0, QApplication::UnicodeUTF8));
      wizardView->setHtml(QApplication::translate("ISpySplashScreen", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
                                                  "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
                                                  "p, li { white-space: pre-wrap; }\n"
                                                  "</style></head><body style=\" font-family:'Lucida Grande'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
                                                  "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
                                                  "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:14pt; font-weight:600; color:#0a5204;\"></p>\n"
                                                  "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:14pt; font-weight:600; color:#0a5204;\"></p>\n"
                                                  "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent"
                                                  ":0; text-indent:0px; font-size:14pt; font-weight:600; color:#0a5204;\"></p>\n"
                                                  "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:14pt; font-weight:600; color:#0a5204;\"></p>\n"
                                                  "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:14pt; font-weight:600; color:#0a5204;\"></p>\n"
                                                  "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt; font-weight:600; color:#0a5204;\">Please, select:</span></p>\n"
                                                  "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:14pt; font-weight:600; text-decoration: underline; color:#0a5204;\"></p>\n"
                                                  "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block"
                                                  "-indent:0; text-indent:0px;\"><a href=\"OpenFile\"><span style=\" text-decoration: underline; color:#0f9509;\">Open File from Disk...</span></a></p>\n"
                                                  "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; text-decoration: underline; color:#0f9509;\"></p>\n"
                                                  "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><a href=\"OpenWeb\"><span style=\" text-decoration: underline; color:#0f9509;\">Open File from Web...</span></a></p>\n"
                                                  "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
                                                  "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><a href=\"OpenUrl\"><span style=\" text-decoration: underline; color:#0f9509;\">Open Url...</span></a></p></body></html>", 0, QApplication::UnicodeUTF8));
      webView->setHtml(QApplication::translate("ISpySplashScreen", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
                                               "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
                                               "p, li { white-space: pre-wrap; }\n"
                                               "</style></head><body style=\" font-family:'Lucida Grande'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
                                               "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
                                               "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
                                               "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
                                               "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
                                               ""
                                               "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><img src=\":/images/IGUANA_SPLASH.xpm\" /></p></body></html>", 0, QApplication::UnicodeUTF8));
      tabWidget->setTabText(tabWidget->indexOf(gettingStartedTab), QApplication::translate("ISpySplashScreen", "Getting Started", 0, QApplication::UnicodeUTF8));
      newsView->setHtml(QApplication::translate("ISpySplashScreen", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
                                                "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
                                                "p, li { white-space: pre-wrap; }\n"
                                                "</style></head><body style=\" font-family:'Lucida Grande'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
                                                "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
                                                "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">Sorry, no news available at this moment.</span></p></body></html>", 0, QApplication::UnicodeUTF8));
      tabWidget->setTabText(tabWidget->indexOf(newsTab), QApplication::translate("ISpySplashScreen", "Latest News", 0, QApplication::UnicodeUTF8));
      ispyLogo->setHtml(QApplication::translate("ISpySplashScreen", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
                                                "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
                                                "p, li { white-space: pre-wrap; }\n"
                                                "</style></head><body style=\" font-family:'Lucida Grande'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
                                                "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
                                                "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
                                                "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
                                                "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
                                                "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bott"
                                                "om:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
                                                "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
                                                "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><img src=\":/images/IGUANA_SPLASH.xpm\" /></p></body></html>", 0, QApplication::UnicodeUTF8));
      ispyDescription->setHtml(QApplication::translate("ISpySplashScreen", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
                                                       "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
                                                       "p, li { white-space: pre-wrap; }\n"
                                                       "</style></head><body style=\" font-family:'Lucida Grande'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
                                                       "<p style=\"-qt-paragraph-type:empty; margin-top:100px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
                                                       "<p style=\" margin-top:100px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Sans Serif'; font-size:14pt; font-weight:600; color:#065602;\">iSpy 1.3.0rc1</span><span style=\" font-family:'Sans Serif'; font-size:12pt;\"> is for displaying physics event data in 3D and 2D, independent of other CMS software. </span></p>\n"
                                                       "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-in"
                                                       "dent:0px; font-family:'Sans Serif'; font-size:12pt;\"></p>\n"
                                                       "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Sans Serif'; font-size:12pt;\">Updates and example data files are available on the application web site. </span></p>\n"
                                                       "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Sans Serif'; font-size:12pt;\"></p>\n"
                                                       "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Sans Serif'; font-size:12pt;\">See </span><a href=\"http://iguana.cern.ch/ispy/\"><span style=\" font-family:'Sans Serif'; font-size:12pt; text-decoration: underline; color:#109d0a;\">http://iguana.cern.ch/ispy/</span></a><span style=\" font-family:'Sans Serif'; font-size:12pt;\"> for more information. </span></p>\n"
                                                       "<p style=\"-qt-paragraph-t"
                                                       "ype:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
                                                       "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Sans Serif'; font-size:12pt; font-style:italic;\">Developed by: </span><span style=\" font-family:'Sans Serif'; font-size:12pt;\">Giulio Eulisse, Thomas McCauley, Shahzad Muzaffar, Ianna Osborne, Lucas Taylor, and Lassi Tuura.  </span></p>\n"
                                                       "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Sans Serif'; font-size:12pt;\"></p>\n"
                                                       "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><a href=\"mailto:hn-cms-visualization@cern.ch\"><span style=\" font-family:'Sans Serif'; font-size:12pt; text-decoration: underline; color:#109d0a;\">Contact us</span></a><span style=\" fo"
                                                       "nt-family:'Sans Serif'; font-size:12pt;\">. </span></p></body></html>", 0, QApplication::UnicodeUTF8));
      tabWidget->setTabText(tabWidget->indexOf(aboutISpyTab), QApplication::translate("ISpySplashScreen", "About iSpy", 0, QApplication::UnicodeUTF8));
      showAtLaunchCheckbox->setText(QApplication::translate("ISpySplashScreen", "Show at launch", 0, QApplication::UnicodeUTF8));
      Q_UNUSED(ISpySplashScreen);
    } // retranslateUi

};

namespace Ui {
  class ISpySplashScreen: public Ui_ISpySplashScreen {};
} // namespace Ui

QT_END_NAMESPACE

#endif // ISPYSPLASHSCREEN_H
