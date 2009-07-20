/********************************************************************************
** Form generated from reading ui file 'IgMainWindowMDI.1.0.ui'
**
** Created: Sun Jul 19 17:20:49 2009
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
#include <QtGui/QComboBox>
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
    QAction *actionAbout;
    QAction *actionNext;
    QAction *actionPrevious;
    QAction *actionAuto;
    QAction *actionPrint;
    QAction *actionSave;
    QAction *actionTwig_Explorer;
    QAction *actionObject_Inspector;
    QAction *actionSaveSettings;
    QAction *actionEditSettings;
    QAction *actionMaximize;
    QAction *actionFull_screen;
    QAction *actionOpenWizard;
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
    QComboBox *viewSelector;
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
        actionAbout = new QAction(IgMainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionAbout->setEnabled(true);
        actionNext = new QAction(IgMainWindow);
        actionNext->setObjectName(QString::fromUtf8("actionNext"));
        QIcon icon1;
        icon1.addPixmap(QPixmap(QString::fromUtf8(":/images/next.png")), QIcon::Normal, QIcon::Off);
        actionNext->setIcon(icon1);
        actionPrevious = new QAction(IgMainWindow);
        actionPrevious->setObjectName(QString::fromUtf8("actionPrevious"));
        actionPrevious->setEnabled(false);
        QIcon icon2;
        icon2.addPixmap(QPixmap(QString::fromUtf8(":/images/previous.png")), QIcon::Normal, QIcon::Off);
        actionPrevious->setIcon(icon2);
        actionAuto = new QAction(IgMainWindow);
        actionAuto->setObjectName(QString::fromUtf8("actionAuto"));
        actionAuto->setCheckable(true);
        QIcon icon3;
        icon3.addPixmap(QPixmap(QString::fromUtf8(":/images/Play1Normal.png")), QIcon::Normal, QIcon::Off);
        icon3.addPixmap(QPixmap(QString::fromUtf8(":/images/PausePressed.png")), QIcon::Normal, QIcon::On);
        actionAuto->setIcon(icon3);
        actionPrint = new QAction(IgMainWindow);
        actionPrint->setObjectName(QString::fromUtf8("actionPrint"));
        actionPrint->setEnabled(false);
        QIcon icon4;
        icon4.addPixmap(QPixmap(QString::fromUtf8(":/images/print.png")), QIcon::Normal, QIcon::Off);
        actionPrint->setIcon(icon4);
        actionSave = new QAction(IgMainWindow);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionSave->setEnabled(false);
        QIcon icon5;
        icon5.addPixmap(QPixmap(QString::fromUtf8(":/images/filesave.png")), QIcon::Normal, QIcon::Off);
        actionSave->setIcon(icon5);
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
        actionOpenWizard = new QAction(IgMainWindow);
        actionOpenWizard->setObjectName(QString::fromUtf8("actionOpenWizard"));
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
        dockTreeWidget->setFeatures(QDockWidget::DockWidgetMovable);
        dockTreeWidgetContents = new QWidget();
        dockTreeWidgetContents->setObjectName(QString::fromUtf8("dockTreeWidgetContents"));
        dockTreeWidgetContents->setStyleSheet(QString::fromUtf8("#dockTreeWidgetContents {\n"
"background-color: #cfdde6;\n"
"margin: 0px;\n"
"padding: 0px;\n"
"spacing: 0px;\n"
"}"));
        gridLayout_3 = new QGridLayout(dockTreeWidgetContents);
        gridLayout_3->setSpacing(-1);
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
"	margin: 0px;\n"
"    border: 0px;\n"
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
        treeWidget->setFrameShape(QFrame::NoFrame);
        treeWidget->setFrameShadow(QFrame::Plain);
        treeWidget->setLineWidth(0);
        treeWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        treeWidget->setAutoScroll(false);
        treeWidget->setProperty("showDropIndicator", QVariant(false));
        treeWidget->setDragEnabled(false);
        treeWidget->setAlternatingRowColors(false);
        treeWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        treeWidget->setIndentation(10);
        treeWidget->setColumnCount(3);
        treeWidget->header()->setVisible(false);
        treeWidget->header()->setCascadingSectionResizes(false);
        treeWidget->header()->setDefaultSectionSize(22);
        treeWidget->header()->setStretchLastSection(false);

        gridLayout_3->addWidget(treeWidget, 1, 0, 1, 1);

        viewSelector = new QComboBox(dockTreeWidgetContents);
        viewSelector->setObjectName(QString::fromUtf8("viewSelector"));
        viewSelector->setEnabled(true);
        viewSelector->setFocusPolicy(Qt::ClickFocus);
        viewSelector->setStyleSheet(QString::fromUtf8("QComboBox {\n"
"     border-bottom: 1px solid gray;\n"
"     border-radius: 0px;\n"
"     padding: 1px 18px 1px 4px;\n"
" }\n"
"\n"
"QComboBox::drop-down {\n"
"  subcontrol-origin: padding;\n"
"  subcontrol-position: top right;\n"
"  width: 15px;\n"
"  border-width: 0px;\n"
"}\n"
"\n"
"QComboBox::down-arrow {\n"
"  image: url(:/images/stylesheet-branch-open.png);\n"
"  width: 8px;\n"
"}\n"
"/*\n"
" QComboBox:!editable, QComboBox::drop-down:editable {\n"
"      background-color : qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                  stop: 0 #FDFDFD, stop: 0.5 #FFFFFF,\n"
"                                  stop: 0.5 #ECECEC, stop: 1.0 #F7F7F7);\n"
" }\n"
"\n"
" QComboBox:!editable {\n"
"     background-color: black;\n"
" }\n"
"\n"
" QComboBox:!editable, QComboBox::drop-down:editable {\n"
"      background-color : qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                  stop: 0 #FDFDFD, stop: 0.5 #FFFFFF,\n"
"                                  stop: 0.5 #ECECEC,"
                        " stop: 1.0 #F7F7F7);\n"
" }\n"
"\n"
" QComboBox:!editable:on {\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #D3D3D3, stop: 0.4 #D8D8D8,\n"
"                                 stop: 0.5 #DDDDDD, stop: 1.0 #E1E1E1);\n"
"}\n"
"\n"
"QComboBox:!editable:on QAbstractItemView:hover {\n"
"    background-color: #FDFDFD;\n"
"	color: #000000;\n"
"selection-background-color: #000000;\n"
"}\n"
"\n"
"QComboBox::drop-down:editable:on {\n"
"  color: black;\n"
"  background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #D3D3D3, stop: 0.4 #D8D8D8,\n"
"                                 stop: 0.5 #DDDDDD, stop: 1.0 #E1E1E1);\n"
"\n"
"}\n"
"\n"
"QComboBox:on { \n"
"  padding-top: 3px;\n"
"  padding-left: 4px;\n"
"}\n"
"\n"
"QComboBox::drop-down {\n"
"  subcontrol-origin: padding;\n"
"  subcontrol-position: top right;\n"
"  width: 15px;\n"
"  border-width: 0px;\n"
"}\n"
"\n"
"QComboBox::down-arrow {\n"
"  image: url("
                        ":/images/stylesheet-branch-open.png);\n"
"  width: 8px;\n"
"}\n"
"\n"
"QComboBox::down-arrow:on { \n"
"  top: 1px;\n"
"  left: 1px;\n"
"}\n"
"\n"
"QComboBox QListView { \n"
"     color: blue;\n"
"     background-color: white;\n"
"     selection-color: gray;\n"
"     selection-background-color: cyan;\n"
"}\n"
"\n"
"QComboBox QAbstractItemView \n"
"{\n"
"  selection-background-color: #00ff00;\n"
"  selection-color: #000000;\n"
"  background-color:  #FDFDFD;\n"
"}\n"
"\n"
" QComboBox QAbstractItemView:hover {\n"
"    selection-background-color: #00ff00;\n"
"    background-color:  #FD0000;\n"
" }*/"));

        gridLayout_3->addWidget(viewSelector, 0, 0, 1, 1);

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

        QMetaObject::connectSlotsByName(IgMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *IgMainWindow)
    {
        IgMainWindow->setWindowTitle(QApplication::translate("IgMainWindow", "IGUANA iSpy", 0, QApplication::UnicodeUTF8));
        actionQuit->setText(QApplication::translate("IgMainWindow", "&Quit", 0, QApplication::UnicodeUTF8));
        actionQuit->setShortcut(QApplication::translate("IgMainWindow", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("IgMainWindow", "About iSpy", 0, QApplication::UnicodeUTF8));
        actionNext->setText(QApplication::translate("IgMainWindow", "&Next", 0, QApplication::UnicodeUTF8));
        actionNext->setShortcut(QApplication::translate("IgMainWindow", "Ctrl+N", "Show next event in the ig file", QApplication::UnicodeUTF8));
        actionPrevious->setText(QApplication::translate("IgMainWindow", "&Previous", 0, QApplication::UnicodeUTF8));
        actionPrevious->setShortcut(QApplication::translate("IgMainWindow", "Ctrl+P", 0, QApplication::UnicodeUTF8));
        actionAuto->setText(QApplication::translate("IgMainWindow", "Play", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionAuto->setToolTip(QApplication::translate("IgMainWindow", "Play Events", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionPrint->setText(QApplication::translate("IgMainWindow", "Print...", 0, QApplication::UnicodeUTF8));
        actionSave->setText(QApplication::translate("IgMainWindow", "&Save", 0, QApplication::UnicodeUTF8));
        actionSave->setShortcut(QApplication::translate("IgMainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        actionTwig_Explorer->setText(QApplication::translate("IgMainWindow", "Tree View", 0, QApplication::UnicodeUTF8));
        actionObject_Inspector->setText(QApplication::translate("IgMainWindow", "Table View", 0, QApplication::UnicodeUTF8));
        actionSaveSettings->setText(QApplication::translate("IgMainWindow", "Save Settings", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        actionSaveSettings->setWhatsThis(QApplication::translate("IgMainWindow", "Save application settings on exit", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        actionEditSettings->setText(QApplication::translate("IgMainWindow", "Show Settings...", 0, QApplication::UnicodeUTF8));
        actionMaximize->setText(QApplication::translate("IgMainWindow", "Maximize", 0, QApplication::UnicodeUTF8));
        actionFull_screen->setText(QApplication::translate("IgMainWindow", "Full screen", 0, QApplication::UnicodeUTF8));
        actionOpenWizard->setText(QApplication::translate("IgMainWindow", "Open...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionOpenWizard->setToolTip(QApplication::translate("IgMainWindow", "Get data to display", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        menuFile->setTitle(QApplication::translate("IgMainWindow", "&File", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("IgMainWindow", "&Help", 0, QApplication::UnicodeUTF8));
        menuOptions->setTitle(QApplication::translate("IgMainWindow", "&Tools", 0, QApplication::UnicodeUTF8));
        menuExpert_Debug_Tools->setTitle(QApplication::translate("IgMainWindow", "Expert Debug Tools...", 0, QApplication::UnicodeUTF8));
        toolBarEvent->setWindowTitle(QApplication::translate("IgMainWindow", "toolBar", 0, QApplication::UnicodeUTF8));
        viewSelector->clear();
        viewSelector->insertItems(0, QStringList()
         << QApplication::translate("IgMainWindow", "New Item", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("IgMainWindow", "New Item", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("IgMainWindow", "New Item", 0, QApplication::UnicodeUTF8)
        );
    } // retranslateUi

};

namespace Ui {
    class IgMainWindow: public Ui_IgMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // IGMAINWINDOWMDI_H
