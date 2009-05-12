/********************************************************************************
** Form generated from reading ui file 'IgSettingsEditorMainWindow.ui'
**
** Created: Thu May 7 09:03:58 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_IGSETTINGSEDITORMAINWINDOW_H
#define UI_IGSETTINGSEDITORMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QScrollArea>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_IgSettingsEditorMainWindow
{
public:
    QAction *actionClose;
    QAction *actionOpen_Settings;
    QAction *actionOpen_INI_File;
    QAction *actionOpen_Mac_Property_List;
    QAction *actionOpen_Windows_Registry_Path;
    QAction *actionRefresh;
    QAction *actionAuto_Refresh;
    QAction *actionFallback;
    QAction *actionAbout;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuOptions;
    QMenu *menuHelp;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *IgSettingsEditorMainWindow)
    {
        if (IgSettingsEditorMainWindow->objectName().isEmpty())
            IgSettingsEditorMainWindow->setObjectName(QString::fromUtf8("IgSettingsEditorMainWindow"));
        IgSettingsEditorMainWindow->resize(508, 599);
        actionClose = new QAction(IgSettingsEditorMainWindow);
        actionClose->setObjectName(QString::fromUtf8("actionClose"));
        actionOpen_Settings = new QAction(IgSettingsEditorMainWindow);
        actionOpen_Settings->setObjectName(QString::fromUtf8("actionOpen_Settings"));
        actionOpen_INI_File = new QAction(IgSettingsEditorMainWindow);
        actionOpen_INI_File->setObjectName(QString::fromUtf8("actionOpen_INI_File"));
        actionOpen_Mac_Property_List = new QAction(IgSettingsEditorMainWindow);
        actionOpen_Mac_Property_List->setObjectName(QString::fromUtf8("actionOpen_Mac_Property_List"));
        actionOpen_Windows_Registry_Path = new QAction(IgSettingsEditorMainWindow);
        actionOpen_Windows_Registry_Path->setObjectName(QString::fromUtf8("actionOpen_Windows_Registry_Path"));
        actionRefresh = new QAction(IgSettingsEditorMainWindow);
        actionRefresh->setObjectName(QString::fromUtf8("actionRefresh"));
        actionAuto_Refresh = new QAction(IgSettingsEditorMainWindow);
        actionAuto_Refresh->setObjectName(QString::fromUtf8("actionAuto_Refresh"));
        actionAuto_Refresh->setCheckable(true);
        actionAuto_Refresh->setChecked(true);
        actionFallback = new QAction(IgSettingsEditorMainWindow);
        actionFallback->setObjectName(QString::fromUtf8("actionFallback"));
        actionFallback->setCheckable(true);
        actionFallback->setChecked(true);
        actionAbout = new QAction(IgSettingsEditorMainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        centralwidget = new QWidget(IgSettingsEditorMainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        scrollArea = new QScrollArea(centralwidget);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 486, 537));
        scrollArea->setWidget(scrollAreaWidgetContents);

        gridLayout->addWidget(scrollArea, 0, 0, 1, 1);

        IgSettingsEditorMainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(IgSettingsEditorMainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 508, 22));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuOptions = new QMenu(menubar);
        menuOptions->setObjectName(QString::fromUtf8("menuOptions"));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        IgSettingsEditorMainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(IgSettingsEditorMainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        IgSettingsEditorMainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuOptions->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionOpen_Settings);
        menuFile->addAction(actionOpen_INI_File);
        menuFile->addAction(actionOpen_Mac_Property_List);
        menuFile->addAction(actionOpen_Windows_Registry_Path);
        menuFile->addAction(actionRefresh);
        menuFile->addSeparator();
        menuFile->addAction(actionClose);
        menuOptions->addAction(actionAuto_Refresh);
        menuOptions->addAction(actionFallback);
        menuHelp->addAction(actionAbout);

        retranslateUi(IgSettingsEditorMainWindow);
        QObject::connect(actionClose, SIGNAL(triggered()), IgSettingsEditorMainWindow, SLOT(hide()));
        QObject::connect(actionAbout, SIGNAL(triggered()), IgSettingsEditorMainWindow, SLOT(about()));
        QObject::connect(actionOpen_Settings, SIGNAL(triggered()), IgSettingsEditorMainWindow, SLOT(openSettings()));
        QObject::connect(actionOpen_INI_File, SIGNAL(triggered()), IgSettingsEditorMainWindow, SLOT(openIniFile()));
        QObject::connect(actionOpen_Mac_Property_List, SIGNAL(triggered()), IgSettingsEditorMainWindow, SLOT(openPropertyList()));
        QObject::connect(actionOpen_Windows_Registry_Path, SIGNAL(triggered()), IgSettingsEditorMainWindow, SLOT(openRegistryPath()));
        QObject::connect(actionAuto_Refresh, SIGNAL(triggered(bool)), IgSettingsEditorMainWindow, SLOT(setAutoRefresh(bool)));
        QObject::connect(actionRefresh, SIGNAL(triggered()), IgSettingsEditorMainWindow, SLOT(refresh()));
        QObject::connect(actionFallback, SIGNAL(triggered(bool)), IgSettingsEditorMainWindow, SLOT(setDockNestingEnabled(bool)));

        QMetaObject::connectSlotsByName(IgSettingsEditorMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *IgSettingsEditorMainWindow)
    {
        IgSettingsEditorMainWindow->setWindowTitle(QApplication::translate("IgSettingsEditorMainWindow", "Application Settings Editor", 0, QApplication::UnicodeUTF8));
        actionClose->setText(QApplication::translate("IgSettingsEditorMainWindow", "Close", 0, QApplication::UnicodeUTF8));
        actionOpen_Settings->setText(QApplication::translate("IgSettingsEditorMainWindow", "Open Application Settings...", 0, QApplication::UnicodeUTF8));
        actionOpen_INI_File->setText(QApplication::translate("IgSettingsEditorMainWindow", "Open INI File...", 0, QApplication::UnicodeUTF8));
        actionOpen_Mac_Property_List->setText(QApplication::translate("IgSettingsEditorMainWindow", "Open Mac Property List...", 0, QApplication::UnicodeUTF8));
        actionOpen_Windows_Registry_Path->setText(QApplication::translate("IgSettingsEditorMainWindow", "Open Windows Registry Path...", 0, QApplication::UnicodeUTF8));
        actionRefresh->setText(QApplication::translate("IgSettingsEditorMainWindow", "Refresh", 0, QApplication::UnicodeUTF8));
        actionAuto_Refresh->setText(QApplication::translate("IgSettingsEditorMainWindow", "Auto-Refresh", 0, QApplication::UnicodeUTF8));
        actionFallback->setText(QApplication::translate("IgSettingsEditorMainWindow", "Fallback", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("IgSettingsEditorMainWindow", "About", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("IgSettingsEditorMainWindow", "File", 0, QApplication::UnicodeUTF8));
        menuOptions->setTitle(QApplication::translate("IgSettingsEditorMainWindow", "Options", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("IgSettingsEditorMainWindow", "Help", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class IgSettingsEditorMainWindow: public Ui_IgSettingsEditorMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IGSETTINGSEDITORMAINWINDOW_H
