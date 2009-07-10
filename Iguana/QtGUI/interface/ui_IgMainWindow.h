/********************************************************************************
** Form generated from reading ui file 'IgMainWindowMDI.1.0.ui'
**
** Created: Wed Jul 8 18:01:36 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef IGMAINWINDOWMDI_H
#define IGMAINWINDOWMDI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QTableView>
#include <QtGui/QToolBar>
#include <QtGui/QTreeWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_IgMainWindow
{
public:
    QAction *actionQuit;
    QAction *actionFileOpen;
    QAction *actionAbout;
    QAction *actionNext;
    QAction *actionPrevious;
    QAction *actionGo_to;
    QAction *actionSkip;
    QAction *actionAuto;
    QAction *action3D;
    QAction *actionRPhi;
    QAction *actionRZ;
    QAction *actionLego;
    QAction *actionPrint;
    QAction *actionClose;
    QAction *actionSave;
    QAction *actionSave_as;
    QAction *actionSave_Image;
    QAction *actionRewind;
    QAction *actionTwig_Explorer;
    QAction *actionObject_Inspector;
    QAction *actionSaveSettings;
    QAction *actionEditSettings;
    QAction *actionMaximize;
    QAction *actionFull_screen;
    QAction *actionTile;
    QAction *actionCascade;
    QAction *actionGive_Feedback;
    QAction *actionISpy_Help;
    QAction *actionFile_Toolbar;
    QAction *actionEvent_Toolbar;
    QAction *action3D_Window;
    QAction *actionOpenWizard;
    QAction *actionOpenUrl;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QMenu *menuOptions;
    QMenu *menuExpert_Debug_Tools;
    QStatusBar *statusbar;
    QToolBar *toolBarEvent;
    QDockWidget *dockTreeWidget;
    QWidget *dockTreeWidgetContents;
    QGridLayout *gridLayout_3;
    QTreeWidget *treeWidget;
    QDockWidget *dockTableWidget;
    QWidget *dockTableWidgetContents;
    QGridLayout *gridLayout_4;
    QTableView *tableView;

    void setupUi(QMainWindow *IgMainWindow)
    {
        if (IgMainWindow->objectName().isEmpty())
            IgMainWindow->setObjectName(QString::fromUtf8("IgMainWindow"));
        IgMainWindow->resize(1050, 855);
        QIcon icon;
        icon.addPixmap(QPixmap(QString::fromUtf8(":/images/iguana-small.xpm")), QIcon::Normal, QIcon::Off);
        IgMainWindow->setWindowIcon(icon);
        IgMainWindow->setDockNestingEnabled(true);
        IgMainWindow->setDockOptions(QMainWindow::AllowNestedDocks|QMainWindow::AllowTabbedDocks|QMainWindow::AnimatedDocks|QMainWindow::VerticalTabs);
        actionQuit = new QAction(IgMainWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        actionFileOpen = new QAction(IgMainWindow);
        actionFileOpen->setObjectName(QString::fromUtf8("actionFileOpen"));
        QIcon icon1;
        icon1.addPixmap(QPixmap(QString::fromUtf8(":/images/fileopen.png")), QIcon::Normal, QIcon::Off);
        actionFileOpen->setIcon(icon1);
        actionFileOpen->setIconVisibleInMenu(false);
        actionAbout = new QAction(IgMainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionAbout->setEnabled(true);
        actionNext = new QAction(IgMainWindow);
        actionNext->setObjectName(QString::fromUtf8("actionNext"));
        QIcon icon2;
        icon2.addPixmap(QPixmap(QString::fromUtf8(":/images/next.png")), QIcon::Normal, QIcon::Off);
        actionNext->setIcon(icon2);
        actionPrevious = new QAction(IgMainWindow);
        actionPrevious->setObjectName(QString::fromUtf8("actionPrevious"));
        actionPrevious->setEnabled(false);
        QIcon icon3;
        icon3.addPixmap(QPixmap(QString::fromUtf8(":/images/previous.png")), QIcon::Normal, QIcon::Off);
        actionPrevious->setIcon(icon3);
        actionGo_to = new QAction(IgMainWindow);
        actionGo_to->setObjectName(QString::fromUtf8("actionGo_to"));
        actionGo_to->setEnabled(false);
        actionSkip = new QAction(IgMainWindow);
        actionSkip->setObjectName(QString::fromUtf8("actionSkip"));
        actionSkip->setEnabled(false);
        actionAuto = new QAction(IgMainWindow);
        actionAuto->setObjectName(QString::fromUtf8("actionAuto"));
        actionAuto->setCheckable(true);
        QIcon icon4;
        icon4.addPixmap(QPixmap(QString::fromUtf8(":/images/play.png")), QIcon::Normal, QIcon::Off);
        icon4.addPixmap(QPixmap(QString::fromUtf8(":/images/pause.png")), QIcon::Normal, QIcon::On);
        actionAuto->setIcon(icon4);
        action3D = new QAction(IgMainWindow);
        action3D->setObjectName(QString::fromUtf8("action3D"));
        actionRPhi = new QAction(IgMainWindow);
        actionRPhi->setObjectName(QString::fromUtf8("actionRPhi"));
        actionRZ = new QAction(IgMainWindow);
        actionRZ->setObjectName(QString::fromUtf8("actionRZ"));
        actionLego = new QAction(IgMainWindow);
        actionLego->setObjectName(QString::fromUtf8("actionLego"));
        actionPrint = new QAction(IgMainWindow);
        actionPrint->setObjectName(QString::fromUtf8("actionPrint"));
        actionPrint->setEnabled(false);
        QIcon icon5;
        icon5.addPixmap(QPixmap(QString::fromUtf8(":/images/print.png")), QIcon::Normal, QIcon::Off);
        actionPrint->setIcon(icon5);
        actionClose = new QAction(IgMainWindow);
        actionClose->setObjectName(QString::fromUtf8("actionClose"));
        actionClose->setEnabled(false);
        actionSave = new QAction(IgMainWindow);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionSave->setEnabled(false);
        QIcon icon6;
        icon6.addPixmap(QPixmap(QString::fromUtf8(":/images/filesave.png")), QIcon::Normal, QIcon::Off);
        actionSave->setIcon(icon6);
        actionSave_as = new QAction(IgMainWindow);
        actionSave_as->setObjectName(QString::fromUtf8("actionSave_as"));
        actionSave_as->setEnabled(false);
        actionSave_Image = new QAction(IgMainWindow);
        actionSave_Image->setObjectName(QString::fromUtf8("actionSave_Image"));
        actionSave_Image->setEnabled(false);
        actionRewind = new QAction(IgMainWindow);
        actionRewind->setObjectName(QString::fromUtf8("actionRewind"));
        actionRewind->setEnabled(false);
        actionTwig_Explorer = new QAction(IgMainWindow);
        actionTwig_Explorer->setObjectName(QString::fromUtf8("actionTwig_Explorer"));
        actionTwig_Explorer->setCheckable(true);
        actionTwig_Explorer->setChecked(true);
        actionObject_Inspector = new QAction(IgMainWindow);
        actionObject_Inspector->setObjectName(QString::fromUtf8("actionObject_Inspector"));
        actionObject_Inspector->setCheckable(true);
        actionObject_Inspector->setChecked(true);
        actionSaveSettings = new QAction(IgMainWindow);
        actionSaveSettings->setObjectName(QString::fromUtf8("actionSaveSettings"));
        actionSaveSettings->setCheckable(true);
        actionEditSettings = new QAction(IgMainWindow);
        actionEditSettings->setObjectName(QString::fromUtf8("actionEditSettings"));
        actionMaximize = new QAction(IgMainWindow);
        actionMaximize->setObjectName(QString::fromUtf8("actionMaximize"));
        actionFull_screen = new QAction(IgMainWindow);
        actionFull_screen->setObjectName(QString::fromUtf8("actionFull_screen"));
        actionTile = new QAction(IgMainWindow);
        actionTile->setObjectName(QString::fromUtf8("actionTile"));
        actionCascade = new QAction(IgMainWindow);
        actionCascade->setObjectName(QString::fromUtf8("actionCascade"));
        actionGive_Feedback = new QAction(IgMainWindow);
        actionGive_Feedback->setObjectName(QString::fromUtf8("actionGive_Feedback"));
        actionISpy_Help = new QAction(IgMainWindow);
        actionISpy_Help->setObjectName(QString::fromUtf8("actionISpy_Help"));
        actionFile_Toolbar = new QAction(IgMainWindow);
        actionFile_Toolbar->setObjectName(QString::fromUtf8("actionFile_Toolbar"));
        actionFile_Toolbar->setCheckable(true);
        actionEvent_Toolbar = new QAction(IgMainWindow);
        actionEvent_Toolbar->setObjectName(QString::fromUtf8("actionEvent_Toolbar"));
        actionEvent_Toolbar->setCheckable(true);
        actionEvent_Toolbar->setChecked(true);
        action3D_Window = new QAction(IgMainWindow);
        action3D_Window->setObjectName(QString::fromUtf8("action3D_Window"));
        actionOpenWizard = new QAction(IgMainWindow);
        actionOpenWizard->setObjectName(QString::fromUtf8("actionOpenWizard"));
        actionOpenUrl = new QAction(IgMainWindow);
        actionOpenUrl->setObjectName(QString::fromUtf8("actionOpenUrl"));
        centralwidget = new QWidget(IgMainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        IgMainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(IgMainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1050, 22));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuFile->setTearOffEnabled(true);
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        menuHelp->setTearOffEnabled(true);
        menuOptions = new QMenu(menubar);
        menuOptions->setObjectName(QString::fromUtf8("menuOptions"));
        menuOptions->setTearOffEnabled(true);
        menuOptions->setSeparatorsCollapsible(true);
        menuExpert_Debug_Tools = new QMenu(menuOptions);
        menuExpert_Debug_Tools->setObjectName(QString::fromUtf8("menuExpert_Debug_Tools"));
        IgMainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(IgMainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        IgMainWindow->setStatusBar(statusbar);
        toolBarEvent = new QToolBar(IgMainWindow);
        toolBarEvent->setObjectName(QString::fromUtf8("toolBarEvent"));
        IgMainWindow->addToolBar(Qt::TopToolBarArea, toolBarEvent);
        dockTreeWidget = new QDockWidget(IgMainWindow);
        dockTreeWidget->setObjectName(QString::fromUtf8("dockTreeWidget"));
        dockTreeWidget->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        dockTreeWidgetContents = new QWidget();
        dockTreeWidgetContents->setObjectName(QString::fromUtf8("dockTreeWidgetContents"));
        gridLayout_3 = new QGridLayout(dockTreeWidgetContents);
        gridLayout_3->setMargin(0);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        treeWidget = new QTreeWidget(dockTreeWidgetContents);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(2, QString::fromUtf8("3"));
        __qtreewidgetitem->setText(1, QString::fromUtf8("2"));
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        treeWidget->setStyleSheet(QString::fromUtf8("QTreeView {\n"
"background-color: #cfdde6;\n"
"alternate-background-color: #cfdde6;\n"
"}\n"
"\n"
" QTreeView {\n"
"     show-decoration-selected: 1;\n"
" }\n"
"\n"
" QTreeView::item {\n"
" }\n"
"\n"
" QTreeView::item:selected {\n"
"     border-top: 1px solid #567dbc;\n"
"     border-bottom: 1px solid #567dbc;\n"
"	 border-left: 0px;\n"
"	 border-right: 0px;\n"
" }\n"
"\n"
" QTreeView::item:selected:active{\n"
"     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #6ea1f1, stop: 1 #567dbc);\n"
" }\n"
"\n"
" QTreeView::item:selected:!active {\n"
"     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #6b9be8, stop: 1 #577fbf);\n"
" }\n"
""));
        treeWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        treeWidget->setAutoScroll(false);
        treeWidget->setDragEnabled(false);
        treeWidget->setAlternatingRowColors(false);
        treeWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        treeWidget->setIndentation(10);
        treeWidget->setColumnCount(3);
        treeWidget->header()->setVisible(false);
        treeWidget->header()->setCascadingSectionResizes(false);
        treeWidget->header()->setDefaultSectionSize(22);
        treeWidget->header()->setStretchLastSection(false);

        gridLayout_3->addWidget(treeWidget, 0, 0, 1, 1);

        dockTreeWidget->setWidget(dockTreeWidgetContents);
        IgMainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockTreeWidget);
        dockTableWidget = new QDockWidget(IgMainWindow);
        dockTableWidget->setObjectName(QString::fromUtf8("dockTableWidget"));
        dockTableWidget->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        dockTableWidgetContents = new QWidget();
        dockTableWidgetContents->setObjectName(QString::fromUtf8("dockTableWidgetContents"));
        gridLayout_4 = new QGridLayout(dockTableWidgetContents);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        tableView = new QTableView(dockTableWidgetContents);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableView->setAlternatingRowColors(true);

        gridLayout_4->addWidget(tableView, 0, 0, 1, 1);

        dockTableWidget->setWidget(dockTableWidgetContents);
        IgMainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(8), dockTableWidget);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuOptions->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionFileOpen);
        menuFile->addAction(actionOpenUrl);
        menuFile->addAction(actionOpenWizard);
        menuFile->addSeparator();
        menuFile->addAction(actionQuit);
        menuHelp->addAction(actionAbout);
        menuOptions->addAction(menuExpert_Debug_Tools->menuAction());
        menuExpert_Debug_Tools->addAction(actionEditSettings);
        menuExpert_Debug_Tools->addSeparator();
        menuExpert_Debug_Tools->addAction(actionSaveSettings);
        toolBarEvent->addAction(actionAuto);
        toolBarEvent->addAction(actionPrevious);
        toolBarEvent->addAction(actionNext);

        retranslateUi(IgMainWindow);
        QObject::connect(actionSaveSettings, SIGNAL(triggered(bool)), IgMainWindow, SLOT(writeSettings(bool)));
        QObject::connect(actionEditSettings, SIGNAL(triggered()), IgMainWindow, SLOT(showSettingsEditor()));
        QObject::connect(actionObject_Inspector, SIGNAL(triggered(bool)), actionObject_Inspector, SLOT(setChecked(bool)));
        QObject::connect(actionTwig_Explorer, SIGNAL(triggered(bool)), actionTwig_Explorer, SLOT(setChecked(bool)));
        QObject::connect(actionTwig_Explorer, SIGNAL(triggered(bool)), dockTreeWidget, SLOT(setVisible(bool)));
        QObject::connect(actionObject_Inspector, SIGNAL(triggered(bool)), dockTableWidget, SLOT(setVisible(bool)));
        QObject::connect(dockTableWidget, SIGNAL(visibilityChanged(bool)), actionObject_Inspector, SLOT(setChecked(bool)));
        QObject::connect(dockTreeWidget, SIGNAL(visibilityChanged(bool)), actionTwig_Explorer, SLOT(setChecked(bool)));
        QObject::connect(actionEvent_Toolbar, SIGNAL(toggled(bool)), toolBarEvent, SLOT(setVisible(bool)));

        QMetaObject::connectSlotsByName(IgMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *IgMainWindow)
    {
        IgMainWindow->setWindowTitle(QApplication::translate("IgMainWindow", "IGUANA iSpy", 0, QApplication::UnicodeUTF8));
        actionQuit->setText(QApplication::translate("IgMainWindow", "&Quit", 0, QApplication::UnicodeUTF8));
        actionQuit->setShortcut(QApplication::translate("IgMainWindow", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        actionFileOpen->setText(QApplication::translate("IgMainWindow", "Open...", 0, QApplication::UnicodeUTF8));
        actionFileOpen->setShortcut(QApplication::translate("IgMainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("IgMainWindow", "About iSpy", 0, QApplication::UnicodeUTF8));
        actionNext->setText(QApplication::translate("IgMainWindow", "&Next", 0, QApplication::UnicodeUTF8));
        actionNext->setShortcut(QApplication::translate("IgMainWindow", "Ctrl+N", "Show next event in the ig file", QApplication::UnicodeUTF8));
        actionPrevious->setText(QApplication::translate("IgMainWindow", "&Previous", 0, QApplication::UnicodeUTF8));
        actionPrevious->setShortcut(QApplication::translate("IgMainWindow", "Ctrl+P", 0, QApplication::UnicodeUTF8));
        actionGo_to->setText(QApplication::translate("IgMainWindow", "&Go to...", 0, QApplication::UnicodeUTF8));
        actionSkip->setText(QApplication::translate("IgMainWindow", "Skip...", 0, QApplication::UnicodeUTF8));
        actionAuto->setText(QApplication::translate("IgMainWindow", "Play", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionAuto->setToolTip(QApplication::translate("IgMainWindow", "Play Events", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        action3D->setText(QApplication::translate("IgMainWindow", "3D", 0, QApplication::UnicodeUTF8));
        actionRPhi->setText(QApplication::translate("IgMainWindow", "RPhi", 0, QApplication::UnicodeUTF8));
        actionRZ->setText(QApplication::translate("IgMainWindow", "RZ", 0, QApplication::UnicodeUTF8));
        actionLego->setText(QApplication::translate("IgMainWindow", "Lego", 0, QApplication::UnicodeUTF8));
        actionPrint->setText(QApplication::translate("IgMainWindow", "Print...", 0, QApplication::UnicodeUTF8));
        actionClose->setText(QApplication::translate("IgMainWindow", "Close", 0, QApplication::UnicodeUTF8));
        actionClose->setShortcut(QApplication::translate("IgMainWindow", "Ctrl+W", 0, QApplication::UnicodeUTF8));
        actionSave->setText(QApplication::translate("IgMainWindow", "&Save", 0, QApplication::UnicodeUTF8));
        actionSave->setShortcut(QApplication::translate("IgMainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        actionSave_as->setText(QApplication::translate("IgMainWindow", "Save as...", 0, QApplication::UnicodeUTF8));
        actionSave_Image->setText(QApplication::translate("IgMainWindow", "Save Image...", 0, QApplication::UnicodeUTF8));
        actionRewind->setText(QApplication::translate("IgMainWindow", "Rewind", 0, QApplication::UnicodeUTF8));
        actionTwig_Explorer->setText(QApplication::translate("IgMainWindow", "Tree View", 0, QApplication::UnicodeUTF8));
        actionObject_Inspector->setText(QApplication::translate("IgMainWindow", "Table View", 0, QApplication::UnicodeUTF8));
        actionSaveSettings->setText(QApplication::translate("IgMainWindow", "Save Settings", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        actionSaveSettings->setWhatsThis(QApplication::translate("IgMainWindow", "Save application settings on exit", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        actionEditSettings->setText(QApplication::translate("IgMainWindow", "Show Settings...", 0, QApplication::UnicodeUTF8));
        actionMaximize->setText(QApplication::translate("IgMainWindow", "Maximize", 0, QApplication::UnicodeUTF8));
        actionFull_screen->setText(QApplication::translate("IgMainWindow", "Full screen", 0, QApplication::UnicodeUTF8));
        actionTile->setText(QApplication::translate("IgMainWindow", "Tile", 0, QApplication::UnicodeUTF8));
        actionCascade->setText(QApplication::translate("IgMainWindow", "Cascade", 0, QApplication::UnicodeUTF8));
        actionGive_Feedback->setText(QApplication::translate("IgMainWindow", "Give Feedback...", 0, QApplication::UnicodeUTF8));
        actionISpy_Help->setText(QApplication::translate("IgMainWindow", "iSpy Help", 0, QApplication::UnicodeUTF8));
        actionFile_Toolbar->setText(QApplication::translate("IgMainWindow", "File Toolbar", 0, QApplication::UnicodeUTF8));
        actionEvent_Toolbar->setText(QApplication::translate("IgMainWindow", "Event Toolbar", 0, QApplication::UnicodeUTF8));
        action3D_Window->setText(QApplication::translate("IgMainWindow", "3D Window", 0, QApplication::UnicodeUTF8));
        actionOpenWizard->setText(QApplication::translate("IgMainWindow", "Show Wizard...", 0, QApplication::UnicodeUTF8));
        actionOpenUrl->setText(QApplication::translate("IgMainWindow", "Open Url...", 0, QApplication::UnicodeUTF8));
        actionOpenUrl->setShortcut(QApplication::translate("IgMainWindow", "Ctrl+Alt+O", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("IgMainWindow", "&File", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("IgMainWindow", "&Help", 0, QApplication::UnicodeUTF8));
        menuOptions->setTitle(QApplication::translate("IgMainWindow", "&Tools", 0, QApplication::UnicodeUTF8));
        menuExpert_Debug_Tools->setTitle(QApplication::translate("IgMainWindow", "Expert Debug Tools...", 0, QApplication::UnicodeUTF8));
        toolBarEvent->setWindowTitle(QApplication::translate("IgMainWindow", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class IgMainWindow: public Ui_IgMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // IGMAINWINDOWMDI_H
