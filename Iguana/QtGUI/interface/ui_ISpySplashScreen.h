/********************************************************************************
** Form generated from reading ui file 'ISpySplashScreen.ui'
**
** Created: Mon May 11 14:43:46 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_ISPYSPLASHSCREEN_H
#define UI_ISPYSPLASHSCREEN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QTextBrowser>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ISpySplashScreen
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QTextBrowser *textBrowser;
    QDialogButtonBox *buttonBox;

    void setupUi(QWidget *ISpySplashScreen)
    {
        if (ISpySplashScreen->objectName().isEmpty())
            ISpySplashScreen->setObjectName(QString::fromUtf8("ISpySplashScreen"));
        ISpySplashScreen->setWindowModality(Qt::ApplicationModal);
        ISpySplashScreen->resize(538, 477);
        QIcon icon;
        icon.addPixmap(QPixmap(QString::fromUtf8(":/images/IGUANA_SPLASH.xpm")), QIcon::Normal, QIcon::Off);
        ISpySplashScreen->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(ISpySplashScreen);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        textBrowser = new QTextBrowser(ISpySplashScreen);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(textBrowser->sizePolicy().hasHeightForWidth());
        textBrowser->setSizePolicy(sizePolicy);
        textBrowser->setAcceptDrops(false);
        textBrowser->setFrameShape(QFrame::NoFrame);
        textBrowser->setFrameShadow(QFrame::Plain);
        textBrowser->setOpenExternalLinks(true);
        textBrowser->setOpenLinks(true);

        horizontalLayout->addWidget(textBrowser);


        verticalLayout->addLayout(horizontalLayout);

        buttonBox = new QDialogButtonBox(ISpySplashScreen);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Close);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(ISpySplashScreen);
        QObject::connect(buttonBox, SIGNAL(clicked(QAbstractButton*)), ISpySplashScreen, SLOT(close()));

        QMetaObject::connectSlotsByName(ISpySplashScreen);
    } // setupUi

    void retranslateUi(QWidget *ISpySplashScreen)
    {
        ISpySplashScreen->setWindowTitle(QApplication::translate("ISpySplashScreen", "IGUANA iSpy", 0, QApplication::UnicodeUTF8));
        textBrowser->setHtml(QApplication::translate("ISpySplashScreen", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><img src=\":/images/IGUANA_SPLASH.xpm\" /></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt; font-weight:600;\">iSpy 1.0 (beta)</span><span style=\" font-size:12pt;\"> is for displaying physics event data in 3D and 2D, independent of other CMS software. Updates and example data files are available on "
                        "the application web site. See </span><a href=\"http://iguana.cern.ch/ispy/\"><span style=\" font-size:12pt; text-decoration: underline; color:#0000ee;\">http://iguana.cern.ch/ispy</span></a><span style=\" font-size:12pt;\"> for more information. </span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt; font-style:italic;\">Developed by: </span><span style=\" font-size:12pt;\">Giulio Eulisse, Thomas McCauley, Shahzad Muzaffar, Ianna Osborne, Lucas Taylor, and Lassi Tuura.  </span><a href=\"mailto:iglite-developers@cern.ch\"><span style=\" font-size:12pt; text-decoration: underline; color:#0000ee;\">Contact us</span></a><span style=\" font-size:12pt;\">. </span></p></body></html>", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(ISpySplashScreen);
    } // retranslateUi

};

namespace Ui {
    class ISpySplashScreen: public Ui_ISpySplashScreen {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ISPYSPLASHSCREEN_H
