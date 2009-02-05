/********************************************************************************
** Form generated from reading ui file 'IgMainWindow.ui'
**
** Created: Tue Jul 29 16:24:04 2008
**      by: Qt User Interface Compiler version 4.4.0
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
#include <QtGui/QGraphicsView>
#include <QtGui/QGridLayout>
#include <QtGui/QMainWindow>
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
    QAction *actionAbout_Iggi;
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
    QAction *actionFileNew;
    QAction *actionToolbars;
    QAction *actionRewind;
    QAction *actionTwig_Explorer;
    QAction *actionObject_Inspector;
    QAction *actionRPhi_Grid;
    QAction *actionRZ_Grid;
    QAction *actionZoom_In;
    QAction *actionZoom_Out;
    QAction *actionShowControlCenter;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QSplitter *splitter_2;
    QDockWidget *dockTreeWidget;
    QWidget *dockTreeWidgetContents;
    QGridLayout *gridLayout_3;
    QTreeView *treeView;
    QSplitter *splitter;
    QGraphicsView *graphicsView;
    QDockWidget *dockTableWidget;
    QWidget *dockTableWidgetContents;
    QGridLayout *gridLayout_2;
    QTableView *tableView;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuRecentOpenedFiles;
    QMenu *menuHelp;
    QMenu *menuEvent;
    QMenu *menuView;
    QMenu *menuAdd;
    QMenu *menuTools;
    QMenu *menu_Display;
    QStatusBar *statusbar;
    QToolBar *toolBarFile;
    QToolBar *toolBarEvent;
    QToolBar *toolBar;

    void setupUi(QMainWindow *IgMainWindow)
    {
    if (IgMainWindow->objectName().isEmpty())
        IgMainWindow->setObjectName(QString::fromUtf8("IgMainWindow"));
    IgMainWindow->resize(1025, 775);
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
    actionAbout_Iggi = new QAction(IgMainWindow);
    actionAbout_Iggi->setObjectName(QString::fromUtf8("actionAbout_Iggi"));
    actionAbout_Iggi->setEnabled(true);
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
    actionSkip = new QAction(IgMainWindow);
    actionSkip->setObjectName(QString::fromUtf8("actionSkip"));
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
    QIcon icon5;
    icon5.addPixmap(QPixmap(QString::fromUtf8(":/images/print.png")), QIcon::Normal, QIcon::Off);
    actionPrint->setIcon(icon5);
    actionClose = new QAction(IgMainWindow);
    actionClose->setObjectName(QString::fromUtf8("actionClose"));
    actionClearMenu = new QAction(IgMainWindow);
    actionClearMenu->setObjectName(QString::fromUtf8("actionClearMenu"));
    actionSave = new QAction(IgMainWindow);
    actionSave->setObjectName(QString::fromUtf8("actionSave"));
    QIcon icon6;
    icon6.addPixmap(QPixmap(QString::fromUtf8(":/images/filesave.png")), QIcon::Normal, QIcon::Off);
    actionSave->setIcon(icon6);
    actionSave_as = new QAction(IgMainWindow);
    actionSave_as->setObjectName(QString::fromUtf8("actionSave_as"));
    actionSave_Image = new QAction(IgMainWindow);
    actionSave_Image->setObjectName(QString::fromUtf8("actionSave_Image"));
    actionFileNew = new QAction(IgMainWindow);
    actionFileNew->setObjectName(QString::fromUtf8("actionFileNew"));
    QIcon icon7;
    icon7.addPixmap(QPixmap(QString::fromUtf8(":/images/filenew.png")), QIcon::Normal, QIcon::Off);
    actionFileNew->setIcon(icon7);
    actionToolbars = new QAction(IgMainWindow);
    actionToolbars->setObjectName(QString::fromUtf8("actionToolbars"));
    actionRewind = new QAction(IgMainWindow);
    actionRewind->setObjectName(QString::fromUtf8("actionRewind"));
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
    QIcon icon8;
    icon8.addPixmap(QPixmap(QString::fromUtf8(":/images/crosshair.png")), QIcon::Normal, QIcon::Off);
    icon8.addPixmap(QPixmap(QString::fromUtf8("crosshair.png")), QIcon::Normal, QIcon::On);
    actionRPhi_Grid->setIcon(icon8);
    actionRZ_Grid = new QAction(IgMainWindow);
    actionRZ_Grid->setObjectName(QString::fromUtf8("actionRZ_Grid"));
    actionRZ_Grid->setCheckable(true);
    QIcon icon9;
    icon9.addPixmap(QPixmap(QString::fromUtf8(":/images/grid.png")), QIcon::Normal, QIcon::Off);
    icon9.addPixmap(QPixmap(QString::fromUtf8("grid.png")), QIcon::Normal, QIcon::On);
    actionRZ_Grid->setIcon(icon9);
    actionZoom_In = new QAction(IgMainWindow);
    actionZoom_In->setObjectName(QString::fromUtf8("actionZoom_In"));
    QIcon icon10;
    icon10.addPixmap(QPixmap(QString::fromUtf8(":/images/zoom_in.png")), QIcon::Normal, QIcon::Off);
    actionZoom_In->setIcon(icon10);
    actionZoom_Out = new QAction(IgMainWindow);
    actionZoom_Out->setObjectName(QString::fromUtf8("actionZoom_Out"));
    QIcon icon11;
    icon11.addPixmap(QPixmap(QString::fromUtf8(":/images/zoom_out.png")), QIcon::Normal, QIcon::Off);
    actionZoom_Out->setIcon(icon11);
    actionShowControlCenter = new QAction(IgMainWindow);
    actionShowControlCenter->setObjectName(QString::fromUtf8("actionShowControlCenter"));
    actionShowControlCenter->setCheckable(true);
    QIcon icon12;
    icon12.addPixmap(QPixmap(QString::fromUtf8(":/images/controlcenter.png")), QIcon::Normal, QIcon::Off);
    icon12.addPixmap(QPixmap(QString::fromUtf8("controlcenter.png")), QIcon::Normal, QIcon::On);
    actionShowControlCenter->setIcon(icon12);
    centralwidget = new QWidget(IgMainWindow);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    centralwidget->setGeometry(QRect(0, 64, 1025, 693));
    gridLayout = new QGridLayout(centralwidget);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
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
    dockTreeWidget->setMinimumSize(QSize(60, 100));
    dockTreeWidget->setFloating(false);
    dockTreeWidget->setFeatures(QDockWidget::AllDockWidgetFeatures);
    dockTreeWidgetContents = new QWidget();
    dockTreeWidgetContents->setObjectName(QString::fromUtf8("dockTreeWidgetContents"));
    dockTreeWidgetContents->setGeometry(QRect(0, 20, 517, 655));
    gridLayout_3 = new QGridLayout(dockTreeWidgetContents);
    gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
    treeView = new QTreeView(dockTreeWidgetContents);
    treeView->setObjectName(QString::fromUtf8("treeView"));
    treeView->setAlternatingRowColors(true);
    treeView->setHeaderHidden(false);

    gridLayout_3->addWidget(treeView, 0, 0, 1, 1);

    dockTreeWidget->setWidget(dockTreeWidgetContents);
    splitter_2->addWidget(dockTreeWidget);
    splitter = new QSplitter(splitter_2);
    splitter->setObjectName(QString::fromUtf8("splitter"));
    splitter->setOrientation(Qt::Vertical);
    graphicsView = new QGraphicsView(splitter);
    graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
    splitter->addWidget(graphicsView);
    dockTableWidget = new QDockWidget(splitter);
    dockTableWidget->setObjectName(QString::fromUtf8("dockTableWidget"));
    dockTableWidget->setFloating(false);
    dockTableWidgetContents = new QWidget();
    dockTableWidgetContents->setObjectName(QString::fromUtf8("dockTableWidgetContents"));
    dockTableWidgetContents->setGeometry(QRect(0, 20, 484, 253));
    gridLayout_2 = new QGridLayout(dockTableWidgetContents);
    gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
    tableView = new QTableView(dockTableWidgetContents);
    tableView->setObjectName(QString::fromUtf8("tableView"));
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->setAlternatingRowColors(true);

    gridLayout_2->addWidget(tableView, 0, 0, 1, 1);

    dockTableWidget->setWidget(dockTableWidgetContents);
    splitter->addWidget(dockTableWidget);
    splitter_2->addWidget(splitter);

    gridLayout->addWidget(splitter_2, 0, 0, 1, 1);

    IgMainWindow->setCentralWidget(centralwidget);
    menubar = new QMenuBar(IgMainWindow);
    menubar->setObjectName(QString::fromUtf8("menubar"));
    menubar->setGeometry(QRect(0, 0, 1025, 25));
    menuFile = new QMenu(menubar);
    menuFile->setObjectName(QString::fromUtf8("menuFile"));
    menuFile->setTearOffEnabled(true);
    menuRecentOpenedFiles = new QMenu(menuFile);
    menuRecentOpenedFiles->setObjectName(QString::fromUtf8("menuRecentOpenedFiles"));
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
    menuTools = new QMenu(menubar);
    menuTools->setObjectName(QString::fromUtf8("menuTools"));
    menuTools->setTearOffEnabled(true);
    menu_Display = new QMenu(menubar);
    menu_Display->setObjectName(QString::fromUtf8("menu_Display"));
    IgMainWindow->setMenuBar(menubar);
    statusbar = new QStatusBar(IgMainWindow);
    statusbar->setObjectName(QString::fromUtf8("statusbar"));
    statusbar->setGeometry(QRect(0, 757, 1025, 18));
    IgMainWindow->setStatusBar(statusbar);
    toolBarFile = new QToolBar(IgMainWindow);
    toolBarFile->setObjectName(QString::fromUtf8("toolBarFile"));
    toolBarFile->setGeometry(QRect(0, 25, 155, 39));
    IgMainWindow->addToolBar(Qt::TopToolBarArea, toolBarFile);
    toolBarEvent = new QToolBar(IgMainWindow);
    toolBarEvent->setObjectName(QString::fromUtf8("toolBarEvent"));
    toolBarEvent->setGeometry(QRect(155, 25, 190, 39));
    IgMainWindow->addToolBar(Qt::TopToolBarArea, toolBarEvent);
    toolBar = new QToolBar(IgMainWindow);
    toolBar->setObjectName(QString::fromUtf8("toolBar"));
    toolBar->setGeometry(QRect(345, 25, 680, 39));
    IgMainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

    menubar->addAction(menuFile->menuAction());
    menubar->addAction(menuView->menuAction());
    menubar->addAction(menuEvent->menuAction());
    menubar->addAction(menuTools->menuAction());
    menubar->addAction(menu_Display->menuAction());
    menubar->addAction(menuHelp->menuAction());
    menuFile->addAction(actionFileNew);
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
    menuHelp->addAction(actionAbout_Iggi);
    menuEvent->addAction(actionNext);
    menuEvent->addAction(actionPrevious);
    menuEvent->addAction(actionGo_to);
    menuEvent->addAction(actionSkip);
    menuEvent->addAction(actionRewind);
    menuEvent->addSeparator();
    menuEvent->addAction(actionAuto);
    menuView->addAction(menuAdd->menuAction());
    menuView->addSeparator();
    menuView->addAction(actionZoom_In);
    menuView->addAction(actionZoom_Out);
    menuAdd->addAction(action3D);
    menuAdd->addAction(actionRPhi);
    menuAdd->addAction(actionRZ);
    menuAdd->addAction(actionLego);
    menuTools->addAction(actionObject_Inspector);
    menuTools->addAction(actionTwig_Explorer);
    menuTools->addSeparator();
    menuTools->addAction(actionToolbars);
    menu_Display->addAction(actionShowControlCenter);
    menu_Display->addSeparator();
    menu_Display->addAction(actionRPhi_Grid);
    menu_Display->addAction(actionRZ_Grid);
    toolBarFile->addAction(actionFileNew);
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
    QObject::connect(actionQuit, SIGNAL(triggered()), IgMainWindow, SLOT(close()));
    QObject::connect(actionObject_Inspector, SIGNAL(toggled(bool)), dockTableWidget, SLOT(setVisible(bool)));
    QObject::connect(actionTwig_Explorer, SIGNAL(toggled(bool)), dockTreeWidget, SLOT(setVisible(bool)));
    QObject::connect(actionRPhi_Grid, SIGNAL(triggered(bool)), IgMainWindow, SLOT(showRPhiGrid(bool)));
    QObject::connect(actionRZ_Grid, SIGNAL(triggered(bool)), IgMainWindow, SLOT(showRZGrid(bool)));
    QObject::connect(actionZoom_In, SIGNAL(triggered()), IgMainWindow, SLOT(zoomIn()));
    QObject::connect(actionZoom_Out, SIGNAL(triggered()), IgMainWindow, SLOT(zoomOut()));
    QObject::connect(actionShowControlCenter, SIGNAL(triggered(bool)), IgMainWindow, SLOT(showControlCenter(bool)));

    QMetaObject::connectSlotsByName(IgMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *IgMainWindow)
    {
    IgMainWindow->setWindowTitle(QApplication::translate("IgMainWindow", "IGUANA Iggi", 0, QApplication::UnicodeUTF8));
    actionQuit->setText(QApplication::translate("IgMainWindow", "&Quit", 0, QApplication::UnicodeUTF8));
    actionQuit->setShortcut(QApplication::translate("IgMainWindow", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
    actionFileOpen->setText(QApplication::translate("IgMainWindow", "Open...", 0, QApplication::UnicodeUTF8));
    actionAbout_Iggi->setText(QApplication::translate("IgMainWindow", "About Iggi", 0, QApplication::UnicodeUTF8));
    actionNext->setText(QApplication::translate("IgMainWindow", "&Next", 0, QApplication::UnicodeUTF8));
    actionNext->setShortcut(QApplication::translate("IgMainWindow", "Ctrl+N", "Show next event in the ig file", QApplication::UnicodeUTF8));
    actionPrevious->setText(QApplication::translate("IgMainWindow", "&Previous", 0, QApplication::UnicodeUTF8));
    actionPrevious->setShortcut(QApplication::translate("IgMainWindow", "Ctrl+P", 0, QApplication::UnicodeUTF8));
    actionGo_to->setText(QApplication::translate("IgMainWindow", "&Go to...", 0, QApplication::UnicodeUTF8));
    actionSkip->setText(QApplication::translate("IgMainWindow", "Skip...", 0, QApplication::UnicodeUTF8));
    actionAuto->setText(QApplication::translate("IgMainWindow", "Auto", 0, QApplication::UnicodeUTF8));
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
    actionFileNew->setText(QApplication::translate("IgMainWindow", "New...", 0, QApplication::UnicodeUTF8));
    actionToolbars->setText(QApplication::translate("IgMainWindow", "Toolbars", 0, QApplication::UnicodeUTF8));
    actionRewind->setText(QApplication::translate("IgMainWindow", "Rewind", 0, QApplication::UnicodeUTF8));
    actionTwig_Explorer->setText(QApplication::translate("IgMainWindow", "Twig Explorer", 0, QApplication::UnicodeUTF8));
    actionObject_Inspector->setText(QApplication::translate("IgMainWindow", "Object Inspector", 0, QApplication::UnicodeUTF8));
    actionRPhi_Grid->setText(QApplication::translate("IgMainWindow", "RPhi Grid", 0, QApplication::UnicodeUTF8));
    actionRZ_Grid->setText(QApplication::translate("IgMainWindow", "RZ Grid", 0, QApplication::UnicodeUTF8));
    actionZoom_In->setText(QApplication::translate("IgMainWindow", "Zoom In", 0, QApplication::UnicodeUTF8));
    actionZoom_Out->setText(QApplication::translate("IgMainWindow", "Zoom Out", 0, QApplication::UnicodeUTF8));
    actionShowControlCenter->setText(QApplication::translate("IgMainWindow", " Control Center", 0, QApplication::UnicodeUTF8));
    dockTreeWidget->setWindowTitle(QApplication::translate("IgMainWindow", "Tree Explorer", 0, QApplication::UnicodeUTF8));
    dockTableWidget->setWindowTitle(QApplication::translate("IgMainWindow", "Object Inspector", 0, QApplication::UnicodeUTF8));
    menuFile->setTitle(QApplication::translate("IgMainWindow", "&File", 0, QApplication::UnicodeUTF8));
    menuRecentOpenedFiles->setTitle(QApplication::translate("IgMainWindow", "Recent", 0, QApplication::UnicodeUTF8));
    menuHelp->setTitle(QApplication::translate("IgMainWindow", "&Help", 0, QApplication::UnicodeUTF8));
    menuEvent->setTitle(QApplication::translate("IgMainWindow", "&Event", 0, QApplication::UnicodeUTF8));
    menuView->setTitle(QApplication::translate("IgMainWindow", "&View", 0, QApplication::UnicodeUTF8));
    menuAdd->setTitle(QApplication::translate("IgMainWindow", "Add...", 0, QApplication::UnicodeUTF8));
    menuTools->setTitle(QApplication::translate("IgMainWindow", "&Tools", 0, QApplication::UnicodeUTF8));
    menu_Display->setTitle(QApplication::translate("IgMainWindow", "&Display", 0, QApplication::UnicodeUTF8));
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
