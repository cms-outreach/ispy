/********************************************************************************
** Form generated from reading ui file 'IgMainWindow-mdi-noQGraphicsView.ui'
**
** Created: Mon Apr 20 14:48:06 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_IGMAINWINDOW_H
#define UI_IGMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMdiArea>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QTableView>
#include <QtGui/QToolBar>
#include <QtGui/QTreeView>
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
    QAction *actionClearMenu;
    QAction *actionSave;
    QAction *actionSave_as;
    QAction *actionSave_Image;
    QAction *actionConnect;
    QAction *actionToolbars;
    QAction *actionRewind;
    QAction *actionTwig_Explorer;
    QAction *actionObject_Inspector;
    QAction *actionRPhi_Grid;
    QAction *actionRZ_Grid;
    QAction *actionZoom_In;
    QAction *actionZoom_Out;
    QAction *actionShowControlCenter;
    QAction *actionSaveSettings;
    QAction *actionEditSettings;
    QAction *actionOpen_Application_Settings;
    QAction *actionOpen_INI_File;
    QAction *actionOpen_Mac_Property_List;
    QAction *actionOpen_Windows_Registry_Path;
    QAction *actionRefresh;
    QAction *actionAuto_Refresh;
    QAction *actionFallbacks;
    QAction *actionAbout_Qt;
    QAction *actionMaximize;
    QAction *actionFull_screen;
    QAction *actionTile;
    QAction *actionCascade;
    QWidget *centralwidget;
    QGridLayout *gridLayout_3;
    QSplitter *splitter_2;
    QDockWidget *dockTreeWidget;
    QWidget *dockTreeWidgetContents;
    QGridLayout *gridLayout_2;
    QTreeView *treeView;
    QSplitter *splitter;
    QMdiArea *mdiArea;
    QDockWidget *dockTableWidget;
    QWidget *dockTableWidgetContents;
    QGridLayout *gridLayout;
    QTableView *tableView;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuRecentOpenedFiles;
    QMenu *menuHelp;
    QMenu *menuEvent;
    QMenu *menuView;
    QMenu *menuAdd;
    QMenu *menuOptions;
    QMenu *menuWindow;
    QStatusBar *statusbar;
    QToolBar *toolBarFile;
    QToolBar *toolBarEvent;
    QToolBar *toolBar;

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
        actionClearMenu = new QAction(IgMainWindow);
        actionClearMenu->setObjectName(QString::fromUtf8("actionClearMenu"));
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
        actionConnect = new QAction(IgMainWindow);
        actionConnect->setObjectName(QString::fromUtf8("actionConnect"));
        actionConnect->setCheckable(true);
        actionConnect->setEnabled(false);
        QIcon icon7;
        icon7.addPixmap(QPixmap(QString::fromUtf8(":/images/network4.png")), QIcon::Normal, QIcon::Off);
        icon7.addPixmap(QPixmap(QString::fromUtf8(":/images/network-red.png")), QIcon::Active, QIcon::Off);
        icon7.addPixmap(QPixmap(QString::fromUtf8(":/images/network-green.png")), QIcon::Active, QIcon::On);
        icon7.addPixmap(QPixmap(QString::fromUtf8(":/images/network-red.png")), QIcon::Selected, QIcon::Off);
        icon7.addPixmap(QPixmap(QString::fromUtf8(":/images/network-green.png")), QIcon::Selected, QIcon::On);
        actionConnect->setIcon(icon7);
        actionToolbars = new QAction(IgMainWindow);
        actionToolbars->setObjectName(QString::fromUtf8("actionToolbars"));
        actionToolbars->setEnabled(false);
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
        actionRPhi_Grid = new QAction(IgMainWindow);
        actionRPhi_Grid->setObjectName(QString::fromUtf8("actionRPhi_Grid"));
        actionRPhi_Grid->setCheckable(true);
        actionRPhi_Grid->setEnabled(false);
        QIcon icon8;
        icon8.addPixmap(QPixmap(QString::fromUtf8(":/images/crosshair.png")), QIcon::Normal, QIcon::Off);
        icon8.addPixmap(QPixmap(QString::fromUtf8(":/images/crosshair.png")), QIcon::Normal, QIcon::On);
        actionRPhi_Grid->setIcon(icon8);
        actionRZ_Grid = new QAction(IgMainWindow);
        actionRZ_Grid->setObjectName(QString::fromUtf8("actionRZ_Grid"));
        actionRZ_Grid->setCheckable(true);
        actionRZ_Grid->setEnabled(false);
        QIcon icon9;
        icon9.addPixmap(QPixmap(QString::fromUtf8(":/images/grid.png")), QIcon::Normal, QIcon::Off);
        icon9.addPixmap(QPixmap(QString::fromUtf8(":/images/grid.png")), QIcon::Normal, QIcon::On);
        actionRZ_Grid->setIcon(icon9);
        actionZoom_In = new QAction(IgMainWindow);
        actionZoom_In->setObjectName(QString::fromUtf8("actionZoom_In"));
        actionZoom_In->setEnabled(false);
        QIcon icon10;
        icon10.addPixmap(QPixmap(QString::fromUtf8(":/images/zoom_in.png")), QIcon::Normal, QIcon::Off);
        actionZoom_In->setIcon(icon10);
        actionZoom_Out = new QAction(IgMainWindow);
        actionZoom_Out->setObjectName(QString::fromUtf8("actionZoom_Out"));
        actionZoom_Out->setEnabled(false);
        QIcon icon11;
        icon11.addPixmap(QPixmap(QString::fromUtf8(":/images/zoom_out.png")), QIcon::Normal, QIcon::Off);
        actionZoom_Out->setIcon(icon11);
        actionShowControlCenter = new QAction(IgMainWindow);
        actionShowControlCenter->setObjectName(QString::fromUtf8("actionShowControlCenter"));
        actionShowControlCenter->setCheckable(true);
        actionShowControlCenter->setEnabled(false);
        QIcon icon12;
        icon12.addPixmap(QPixmap(QString::fromUtf8(":/images/controlcenter.png")), QIcon::Normal, QIcon::Off);
        icon12.addPixmap(QPixmap(QString::fromUtf8(":/images/controlcenter.png")), QIcon::Normal, QIcon::On);
        actionShowControlCenter->setIcon(icon12);
        actionSaveSettings = new QAction(IgMainWindow);
        actionSaveSettings->setObjectName(QString::fromUtf8("actionSaveSettings"));
        actionSaveSettings->setCheckable(true);
        actionEditSettings = new QAction(IgMainWindow);
        actionEditSettings->setObjectName(QString::fromUtf8("actionEditSettings"));
        actionOpen_Application_Settings = new QAction(IgMainWindow);
        actionOpen_Application_Settings->setObjectName(QString::fromUtf8("actionOpen_Application_Settings"));
        actionOpen_INI_File = new QAction(IgMainWindow);
        actionOpen_INI_File->setObjectName(QString::fromUtf8("actionOpen_INI_File"));
        actionOpen_Mac_Property_List = new QAction(IgMainWindow);
        actionOpen_Mac_Property_List->setObjectName(QString::fromUtf8("actionOpen_Mac_Property_List"));
        actionOpen_Windows_Registry_Path = new QAction(IgMainWindow);
        actionOpen_Windows_Registry_Path->setObjectName(QString::fromUtf8("actionOpen_Windows_Registry_Path"));
        actionRefresh = new QAction(IgMainWindow);
        actionRefresh->setObjectName(QString::fromUtf8("actionRefresh"));
        actionRefresh->setCheckable(false);
        actionRefresh->setChecked(false);
        actionAuto_Refresh = new QAction(IgMainWindow);
        actionAuto_Refresh->setObjectName(QString::fromUtf8("actionAuto_Refresh"));
        actionAuto_Refresh->setCheckable(true);
        actionAuto_Refresh->setChecked(true);
        actionFallbacks = new QAction(IgMainWindow);
        actionFallbacks->setObjectName(QString::fromUtf8("actionFallbacks"));
        actionFallbacks->setCheckable(true);
        actionFallbacks->setChecked(true);
        actionAbout_Qt = new QAction(IgMainWindow);
        actionAbout_Qt->setObjectName(QString::fromUtf8("actionAbout_Qt"));
        actionMaximize = new QAction(IgMainWindow);
        actionMaximize->setObjectName(QString::fromUtf8("actionMaximize"));
        actionFull_screen = new QAction(IgMainWindow);
        actionFull_screen->setObjectName(QString::fromUtf8("actionFull_screen"));
        actionTile = new QAction(IgMainWindow);
        actionTile->setObjectName(QString::fromUtf8("actionTile"));
        actionCascade = new QAction(IgMainWindow);
        actionCascade->setObjectName(QString::fromUtf8("actionCascade"));
        centralwidget = new QWidget(IgMainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout_3 = new QGridLayout(centralwidget);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        splitter_2 = new QSplitter(centralwidget);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        splitter_2->setOrientation(Qt::Horizontal);
        dockTreeWidget = new QDockWidget(splitter_2);
        dockTreeWidget->setObjectName(QString::fromUtf8("dockTreeWidget"));
        dockTreeWidget->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(dockTreeWidget->sizePolicy().hasHeightForWidth());
        dockTreeWidget->setSizePolicy(sizePolicy);
        dockTreeWidget->setMinimumSize(QSize(112, 132));
        dockTreeWidget->setFloating(false);
        dockTreeWidget->setFeatures(QDockWidget::AllDockWidgetFeatures);
        dockTreeWidgetContents = new QWidget();
        dockTreeWidgetContents->setObjectName(QString::fromUtf8("dockTreeWidgetContents"));
        gridLayout_2 = new QGridLayout(dockTreeWidgetContents);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        treeView = new QTreeView(dockTreeWidgetContents);
        treeView->setObjectName(QString::fromUtf8("treeView"));
        treeView->setAlternatingRowColors(true);
        treeView->setHeaderHidden(false);

        gridLayout_2->addWidget(treeView, 0, 0, 1, 1);

        dockTreeWidget->setWidget(dockTreeWidgetContents);
        splitter_2->addWidget(dockTreeWidget);
        splitter = new QSplitter(splitter_2);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Vertical);
        mdiArea = new QMdiArea(splitter);
        mdiArea->setObjectName(QString::fromUtf8("mdiArea"));
        mdiArea->setViewMode(QMdiArea::TabbedView);
        splitter->addWidget(mdiArea);
        dockTableWidget = new QDockWidget(splitter);
        dockTableWidget->setObjectName(QString::fromUtf8("dockTableWidget"));
        dockTableWidget->setFloating(false);
        dockTableWidgetContents = new QWidget();
        dockTableWidgetContents->setObjectName(QString::fromUtf8("dockTableWidgetContents"));
        gridLayout = new QGridLayout(dockTableWidgetContents);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tableView = new QTableView(dockTableWidgetContents);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableView->setAlternatingRowColors(true);

        gridLayout->addWidget(tableView, 0, 0, 1, 1);

        dockTableWidget->setWidget(dockTableWidgetContents);
        splitter->addWidget(dockTableWidget);
        splitter_2->addWidget(splitter);

        gridLayout_3->addWidget(splitter_2, 0, 0, 1, 1);

        IgMainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(IgMainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1050, 22));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuFile->setTearOffEnabled(true);
        menuRecentOpenedFiles = new QMenu(menuFile);
        menuRecentOpenedFiles->setObjectName(QString::fromUtf8("menuRecentOpenedFiles"));
        menuRecentOpenedFiles->setEnabled(false);
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        menuHelp->setTearOffEnabled(true);
        menuEvent = new QMenu(menubar);
        menuEvent->setObjectName(QString::fromUtf8("menuEvent"));
        menuEvent->setTearOffEnabled(true);
        menuView = new QMenu(menubar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        menuView->setTearOffEnabled(true);
        menuAdd = new QMenu(menuView);
        menuAdd->setObjectName(QString::fromUtf8("menuAdd"));
        menuAdd->setEnabled(false);
        menuOptions = new QMenu(menubar);
        menuOptions->setObjectName(QString::fromUtf8("menuOptions"));
        menuOptions->setTearOffEnabled(true);
        menuOptions->setSeparatorsCollapsible(true);
        menuWindow = new QMenu(menubar);
        menuWindow->setObjectName(QString::fromUtf8("menuWindow"));
        menuWindow->setTearOffEnabled(true);
        IgMainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(IgMainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        IgMainWindow->setStatusBar(statusbar);
        toolBarFile = new QToolBar(IgMainWindow);
        toolBarFile->setObjectName(QString::fromUtf8("toolBarFile"));
        IgMainWindow->addToolBar(Qt::TopToolBarArea, toolBarFile);
        toolBarEvent = new QToolBar(IgMainWindow);
        toolBarEvent->setObjectName(QString::fromUtf8("toolBarEvent"));
        IgMainWindow->addToolBar(Qt::TopToolBarArea, toolBarEvent);
        toolBar = new QToolBar(IgMainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        IgMainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuView->menuAction());
        menubar->addAction(menuEvent->menuAction());
        menubar->addAction(menuOptions->menuAction());
        menubar->addAction(menuWindow->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionConnect);
        menuFile->addAction(actionFileOpen);
        menuFile->addAction(menuRecentOpenedFiles->menuAction());
        menuFile->addSeparator();
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSave_as);
        menuFile->addSeparator();
        menuFile->addAction(actionPrint);
        menuFile->addAction(actionSave_Image);
        menuFile->addSeparator();
        menuFile->addAction(actionClose);
        menuFile->addSeparator();
        menuFile->addAction(actionQuit);
        menuRecentOpenedFiles->addSeparator();
        menuRecentOpenedFiles->addAction(actionClearMenu);
        menuHelp->addAction(actionAbout);
        menuHelp->addAction(actionAbout_Qt);
        menuEvent->addAction(actionNext);
        menuEvent->addAction(actionPrevious);
        menuEvent->addAction(actionGo_to);
        menuEvent->addAction(actionSkip);
        menuEvent->addAction(actionRewind);
        menuEvent->addSeparator();
        menuEvent->addAction(actionAuto);
        menuView->addAction(menuAdd->menuAction());
        menuView->addSeparator();
        menuView->addAction(actionObject_Inspector);
        menuView->addAction(actionTwig_Explorer);
        menuView->addSeparator();
        menuView->addAction(actionToolbars);
        menuAdd->addAction(action3D);
        menuAdd->addAction(actionRPhi);
        menuAdd->addAction(actionRZ);
        menuAdd->addAction(actionLego);
        menuOptions->addAction(actionOpen_Application_Settings);
        menuOptions->addAction(actionOpen_INI_File);
        menuOptions->addAction(actionOpen_Mac_Property_List);
        menuOptions->addAction(actionOpen_Windows_Registry_Path);
        menuOptions->addSeparator();
        menuOptions->addAction(actionEditSettings);
        menuOptions->addSeparator();
        menuOptions->addAction(actionRefresh);
        menuOptions->addSeparator();
        menuOptions->addAction(actionAuto_Refresh);
        menuOptions->addAction(actionFallbacks);
        menuOptions->addSeparator();
        menuOptions->addAction(actionSaveSettings);
        menuWindow->addAction(actionMaximize);
        menuWindow->addAction(actionFull_screen);
        menuWindow->addSeparator();
        menuWindow->addAction(actionZoom_In);
        menuWindow->addAction(actionZoom_Out);
        menuWindow->addSeparator();
        menuWindow->addAction(actionTile);
        menuWindow->addAction(actionCascade);
        toolBarFile->addAction(actionConnect);
        toolBarFile->addAction(actionFileOpen);
        toolBarFile->addAction(actionSave);
        toolBarFile->addAction(actionPrint);
        toolBarEvent->addAction(actionAuto);
        toolBarEvent->addAction(actionPrevious);
        toolBarEvent->addAction(actionNext);
        toolBarEvent->addAction(actionZoom_In);
        toolBarEvent->addAction(actionZoom_Out);
        toolBar->addAction(actionShowControlCenter);
        toolBar->addAction(actionRPhi_Grid);
        toolBar->addAction(actionRZ_Grid);

        retranslateUi(IgMainWindow);
        QObject::connect(actionObject_Inspector, SIGNAL(triggered(bool)), dockTableWidget, SLOT(setShown(bool)));
        QObject::connect(actionTwig_Explorer, SIGNAL(triggered(bool)), dockTreeWidget, SLOT(setShown(bool)));
        QObject::connect(actionRPhi_Grid, SIGNAL(triggered(bool)), IgMainWindow, SLOT(showRPhiGrid(bool)));
        QObject::connect(actionRZ_Grid, SIGNAL(triggered(bool)), IgMainWindow, SLOT(showRZGrid(bool)));
        QObject::connect(actionZoom_In, SIGNAL(triggered()), IgMainWindow, SLOT(zoomIn()));
        QObject::connect(actionZoom_Out, SIGNAL(triggered()), IgMainWindow, SLOT(zoomOut()));
        QObject::connect(actionShowControlCenter, SIGNAL(triggered(bool)), IgMainWindow, SLOT(showControlCenter(bool)));
        QObject::connect(actionSaveSettings, SIGNAL(triggered(bool)), IgMainWindow, SLOT(writeSettings(bool)));
        QObject::connect(actionEditSettings, SIGNAL(triggered()), IgMainWindow, SLOT(showSettingsEditor()));
        QObject::connect(actionAbout, SIGNAL(triggered()), IgMainWindow, SLOT(about()));
        QObject::connect(actionAbout_Qt, SIGNAL(triggered()), IgMainWindow, SLOT(aboutQt()));
        QObject::connect(actionOpen_Application_Settings, SIGNAL(triggered()), IgMainWindow, SLOT(openSettings()));
        QObject::connect(actionOpen_INI_File, SIGNAL(triggered()), IgMainWindow, SLOT(openIniFile()));
        QObject::connect(actionOpen_Mac_Property_List, SIGNAL(triggered()), IgMainWindow, SLOT(openPropertyList()));
        QObject::connect(actionOpen_Windows_Registry_Path, SIGNAL(triggered()), IgMainWindow, SLOT(openRegistryPath()));
        QObject::connect(actionRefresh, SIGNAL(triggered()), IgMainWindow, SLOT(refreshSettingsEditor()));
        QObject::connect(actionAuto_Refresh, SIGNAL(triggered(bool)), IgMainWindow, SLOT(setAutoRefresh(bool)));
        QObject::connect(actionFallbacks, SIGNAL(triggered(bool)), IgMainWindow, SLOT(setFallbacksEnable(bool)));
        QObject::connect(dockTableWidget, SIGNAL(visibilityChanged(bool)), actionObject_Inspector, SLOT(setChecked(bool)));
        QObject::connect(dockTreeWidget, SIGNAL(visibilityChanged(bool)), actionTwig_Explorer, SLOT(setChecked(bool)));
        QObject::connect(actionObject_Inspector, SIGNAL(triggered(bool)), actionObject_Inspector, SLOT(setChecked(bool)));
        QObject::connect(actionTwig_Explorer, SIGNAL(triggered(bool)), actionTwig_Explorer, SLOT(setChecked(bool)));
        QObject::connect(actionMaximize, SIGNAL(triggered()), IgMainWindow, SLOT(showMaximized()));
        QObject::connect(actionFull_screen, SIGNAL(triggered()), IgMainWindow, SLOT(showFullScreen()));
        QObject::connect(actionTile, SIGNAL(triggered()), mdiArea, SLOT(tileSubWindows()));
        QObject::connect(actionCascade, SIGNAL(triggered()), mdiArea, SLOT(cascadeSubWindows()));

        QMetaObject::connectSlotsByName(IgMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *IgMainWindow)
    {
        IgMainWindow->setWindowTitle(QApplication::translate("IgMainWindow", "IGUANA iSpy", 0, QApplication::UnicodeUTF8));
        actionQuit->setText(QApplication::translate("IgMainWindow", "&Quit", 0, QApplication::UnicodeUTF8));
        actionQuit->setShortcut(QApplication::translate("IgMainWindow", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        actionFileOpen->setText(QApplication::translate("IgMainWindow", "Open...", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("IgMainWindow", "About", 0, QApplication::UnicodeUTF8));
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
        actionClearMenu->setText(QApplication::translate("IgMainWindow", "Clear Menu", 0, QApplication::UnicodeUTF8));
        actionSave->setText(QApplication::translate("IgMainWindow", "&Save", 0, QApplication::UnicodeUTF8));
        actionSave->setShortcut(QApplication::translate("IgMainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        actionSave_as->setText(QApplication::translate("IgMainWindow", "Save as...", 0, QApplication::UnicodeUTF8));
        actionSave_Image->setText(QApplication::translate("IgMainWindow", "Save Image...", 0, QApplication::UnicodeUTF8));
        actionConnect->setText(QApplication::translate("IgMainWindow", "Connect", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionConnect->setToolTip(QApplication::translate("IgMainWindow", "Connect Online to Iguana Producer", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionToolbars->setText(QApplication::translate("IgMainWindow", "Toolbars", 0, QApplication::UnicodeUTF8));
        actionRewind->setText(QApplication::translate("IgMainWindow", "Rewind", 0, QApplication::UnicodeUTF8));
        actionTwig_Explorer->setText(QApplication::translate("IgMainWindow", "Tree Explorer", 0, QApplication::UnicodeUTF8));
        actionObject_Inspector->setText(QApplication::translate("IgMainWindow", "Object Inspector", 0, QApplication::UnicodeUTF8));
        actionRPhi_Grid->setText(QApplication::translate("IgMainWindow", "RPhi Grid", 0, QApplication::UnicodeUTF8));
        actionRZ_Grid->setText(QApplication::translate("IgMainWindow", "RZ Grid", 0, QApplication::UnicodeUTF8));
        actionZoom_In->setText(QApplication::translate("IgMainWindow", "Zoom In", 0, QApplication::UnicodeUTF8));
        actionZoom_In->setShortcut(QApplication::translate("IgMainWindow", "Ctrl++", 0, QApplication::UnicodeUTF8));
        actionZoom_Out->setText(QApplication::translate("IgMainWindow", "Zoom Out", 0, QApplication::UnicodeUTF8));
        actionZoom_Out->setShortcut(QApplication::translate("IgMainWindow", "Ctrl+-", 0, QApplication::UnicodeUTF8));
        actionShowControlCenter->setText(QApplication::translate("IgMainWindow", " Control Center", 0, QApplication::UnicodeUTF8));
        actionSaveSettings->setText(QApplication::translate("IgMainWindow", "Save Settings", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        actionSaveSettings->setWhatsThis(QApplication::translate("IgMainWindow", "Save application settings on exit", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        actionEditSettings->setText(QApplication::translate("IgMainWindow", "Edit Settings...", 0, QApplication::UnicodeUTF8));
        actionOpen_Application_Settings->setText(QApplication::translate("IgMainWindow", "Open Application Settings...", 0, QApplication::UnicodeUTF8));
        actionOpen_INI_File->setText(QApplication::translate("IgMainWindow", "Open INI File...", 0, QApplication::UnicodeUTF8));
        actionOpen_Mac_Property_List->setText(QApplication::translate("IgMainWindow", "Open Mac Property List...", 0, QApplication::UnicodeUTF8));
        actionOpen_Windows_Registry_Path->setText(QApplication::translate("IgMainWindow", "Open Windows Registry Path...", 0, QApplication::UnicodeUTF8));
        actionRefresh->setText(QApplication::translate("IgMainWindow", "Refresh", 0, QApplication::UnicodeUTF8));
        actionAuto_Refresh->setText(QApplication::translate("IgMainWindow", "Auto-Refresh", 0, QApplication::UnicodeUTF8));
        actionFallbacks->setText(QApplication::translate("IgMainWindow", "Fallbacks", 0, QApplication::UnicodeUTF8));
        actionAbout_Qt->setText(QApplication::translate("IgMainWindow", "About Qt", 0, QApplication::UnicodeUTF8));
        actionMaximize->setText(QApplication::translate("IgMainWindow", "Maximize", 0, QApplication::UnicodeUTF8));
        actionFull_screen->setText(QApplication::translate("IgMainWindow", "Full screen", 0, QApplication::UnicodeUTF8));
        actionTile->setText(QApplication::translate("IgMainWindow", "Tile", 0, QApplication::UnicodeUTF8));
        actionCascade->setText(QApplication::translate("IgMainWindow", "Cascade", 0, QApplication::UnicodeUTF8));
        dockTreeWidget->setWindowTitle(QApplication::translate("IgMainWindow", "Tree Explorer", 0, QApplication::UnicodeUTF8));
        dockTableWidget->setWindowTitle(QApplication::translate("IgMainWindow", "Object Inspector", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("IgMainWindow", "&File", 0, QApplication::UnicodeUTF8));
        menuRecentOpenedFiles->setTitle(QApplication::translate("IgMainWindow", "Recent", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("IgMainWindow", "&Help", 0, QApplication::UnicodeUTF8));
        menuEvent->setTitle(QApplication::translate("IgMainWindow", "&Event", 0, QApplication::UnicodeUTF8));
        menuView->setTitle(QApplication::translate("IgMainWindow", "&View", 0, QApplication::UnicodeUTF8));
        menuAdd->setTitle(QApplication::translate("IgMainWindow", "Add...", 0, QApplication::UnicodeUTF8));
        menuOptions->setTitle(QApplication::translate("IgMainWindow", "&Options", 0, QApplication::UnicodeUTF8));
        menuWindow->setTitle(QApplication::translate("IgMainWindow", "&Window", 0, QApplication::UnicodeUTF8));
        toolBarFile->setWindowTitle(QApplication::translate("IgMainWindow", "toolBar", 0, QApplication::UnicodeUTF8));
        toolBarEvent->setWindowTitle(QApplication::translate("IgMainWindow", "toolBar", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("IgMainWindow", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class IgMainWindow: public Ui_IgMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IGMAINWINDOW_H
