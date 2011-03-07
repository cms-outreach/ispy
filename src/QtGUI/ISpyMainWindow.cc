//<<<<<< INCLUDES                                                       >>>>>>

#include "QtGUI/ISpyMainWindow.h"
#include "QtGUI/IgLocationDialog.h"
#include "QtGUI/IgSettingsEditor.h"
#include "QtGUI/ISpySplashScreen.h"
#include <QtGui>
#include <math.h>
#include <iostream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

ISpyMainWindow::ISpyMainWindow(QWidget *parent)
  : QMainWindow(parent),
    m_settingsEditor(new IgSettingsEditor(parent))
{
  setupUi(this);
  setupActions();
}

void
ISpyMainWindow::setupActions(void)
{
  QSettings *settings = new QSettings(QCoreApplication::organizationDomain(),
                                      QCoreApplication::applicationName());
  settings->clear();
  settings->sync();
  m_settingsEditor->setSettingsObject(settings);

  QObject::connect(actionNext, SIGNAL(triggered()), this, SIGNAL(nextEvent()));
  QObject::connect(actionPrevious, SIGNAL(triggered()), this, SIGNAL(previousEvent()));
  QObject::connect(actionPrint, SIGNAL(triggered()), this, SIGNAL(print()));
  QObject::connect(actionSave, SIGNAL(triggered()), this, SIGNAL(save()));
  QObject::connect(actionMaximize, SIGNAL(triggered()), this, SLOT(maximize()));
  QObject::connect(actionFull_screen, SIGNAL(triggered()), this, SLOT(fullScreen()));
  QObject::connect(actionAbout, SIGNAL(triggered()), this, SIGNAL(showAbout()));
  actionFull_screen->setEnabled(false);
  restoreSettings();
}

ISpyMainWindow::~ISpyMainWindow()
{}

void
ISpyMainWindow::restoreSettings(void)
{
  QSettings settings;
  if (settings.contains("mainwindow/treeview/shown"))
    dockTreeWidget->setShown(settings.value("mainwindow/treeview/shown").value<bool>());
  if (settings.contains("mainwindow/treeview/floating"))
    dockTreeWidget->setFloating(settings.value("mainwindow/treeview/floating").value<bool>());
  if (settings.contains("mainwindow/tableview/shown"))
    dockTableWidget->setShown(settings.value("mainwindow/tableview/shown").value<bool>());
  if (settings.contains("mainwindow/tableview/floating"))
    dockTableWidget->setFloating(settings.value("mainwindow/tableview/floating").value<bool>());
  if (settings.contains("mainwindow/configuration/save"))
    actionSaveSettings->setChecked(settings.value("mainwindow/configuration/save").value<bool>());
}

void
ISpyMainWindow::saveSettings(void)
{
  QSettings settings;
  qDebug() << "ISpyMainWindow::saveSettings...\n";
  bool saveFlag = actionSaveSettings->isChecked();
  settings.setValue("mainwindow/configuration/save", saveFlag);
  if (saveFlag)
  {
    bool treeFlag = actionTwig_Explorer->isChecked();
    bool tableFlag = actionObject_Inspector->isChecked();

    settings.setValue("mainwindow/treeview/shown", treeFlag);
    settings.setValue("mainwindow/treeview/floating", dockTreeWidget->isFloating());
    settings.setValue("mainwindow/tableview/shown", tableFlag);
    settings.setValue("mainwindow/tableview/floating", dockTableWidget->isFloating());

    //  settings.setValue("mainwindow/toolbars/file/shown", actionFile_Toolbar->isChecked());
    //  settings.setValue("mainwindow/toolbars/event/shown", actionFile_Event->isChecked());
  }
  else
  {
    const QString visSettings("igtwigs/visibility/");
    settings.remove(visSettings);
  }
}

QWidget *
ISpyMainWindow::workspace(void)
{
  return this->centralwidget;
}

void
ISpyMainWindow::writeSettings(bool /*value*/)
{
  qDebug() << "ISpyMainWindow::writeSettings...\n";
  saveSettings();
}

void
ISpyMainWindow::showSettingsEditor(void)
{
  if (! m_settingsEditor->isVisible())
    m_settingsEditor->show();
  m_settingsEditor->activateWindow();
  m_settingsEditor->raise();
}

void
ISpyMainWindow::maximize(void)
{
  if (this->isMaximized())
  {
    actionMaximize->setText(QApplication::translate("IgMainWindow", "Maximize", 0, QApplication::UnicodeUTF8));
    actionFull_screen->setEnabled(true);
    actionFull_screen->setEnabled(false);
    this->showNormal();
  }
  else
  {
    actionMaximize->setText(QApplication::translate("IgMainWindow", "Show &Normal", 0, QApplication::UnicodeUTF8));
    actionFull_screen->setEnabled(false);
    this->showMaximized();
  }
}

void
ISpyMainWindow::fullScreen(void)
{
  if (isFullScreen())
  {
    actionMaximize->setEnabled(true);
    menubar->show();
    //  toolBarFile->show();
    toolBarEvent->show();
    dockTableWidgetContents->show();
    dockTreeWidgetContents->show();
    showNormal();
  }
  else
  {
    menubar->hide();
    //  toolBarFile->hide();
    toolBarEvent->hide();
    dockTableWidgetContents->hide();
    dockTreeWidgetContents->hide();
    actionMaximize->setEnabled(false);
    showMaximized();
  }
}

void
ISpyMainWindow::setSettingsObject(QSettings *settings)
{
  m_settingsEditor->setSettingsObject(settings);
}
