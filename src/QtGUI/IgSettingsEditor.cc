//<<<<<< INCLUDES                                                       >>>>>>

#include "QtGUI/IgSettingsEditor.h"
#include "QtGUI/IgLocationDialog.h"
#include "QtGUI/IgSettingsTree.h"
#include <QtGui>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgSettingsEditor::IgSettingsEditor(QWidget *parent)
  : QMainWindow(parent),
    m_settingsTree(new IgSettingsTree)
{
  m_locationDialog = 0;

  setupUi(this);
  this->scrollArea->setWidget(m_settingsTree);
  setupActions();
}

void
IgSettingsEditor::setupActions(void)
{
#ifndef Q_WS_MAC
  actionOpen_Mac_Property_List->setEnabled(false);
#endif
#ifndef Q_WS_WIN
  actionOpen_Windows_Registry_Path->setEnabled(false);
#endif
  actionFallback->setEnabled(true);
  setFallbacksEnabled(actionFallback->isChecked());
}

void
IgSettingsEditor::openSettings(void)
{
  if (!m_locationDialog)
    m_locationDialog = new IgLocationDialog(this);

  if (m_locationDialog->exec())
  {
    QSettings *settings = new QSettings(m_locationDialog->format(),
                                        m_locationDialog->scope(),
                                        m_locationDialog->organization(),
                                        m_locationDialog->application());
    setSettingsObject(settings);
    actionFallback->setEnabled(true);
    // showSettingsEditor();
  }
}

void
IgSettingsEditor::openIniFile(void)
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open INI File"),
                                                  "", tr("INI Files(*.ini *.conf)"));
  if (! fileName.isEmpty())
  {
    QSettings *settings = new QSettings(fileName, QSettings::IniFormat);
    setSettingsObject(settings);
    actionFallback->setEnabled(false);
  }
}

void
IgSettingsEditor::openPropertyList(void)
{
  QString fileName = QFileDialog::getOpenFileName(this,
                                                  tr("Open Property List"),
                                                  "", tr("Property List Files(*.plist)"));
  if (! fileName.isEmpty())
  {
    QSettings *settings = new QSettings(fileName, QSettings::NativeFormat);
    setSettingsObject(settings);
    actionFallback->setEnabled(false);
  }
}

void
IgSettingsEditor::openRegistryPath(void)
{
  QString path = QInputDialog::getText(this, tr("Open Registry Path"),
                                       tr("Enter the path in the Windows registry:"),
                                       QLineEdit::Normal, "HKEY_CURRENT_USER\\");
  if (! path.isEmpty())
  {
    QSettings *settings = new QSettings(path, QSettings::NativeFormat);
    setSettingsObject(settings);
    actionFallback->setEnabled(false);
  }
}

void
IgSettingsEditor::refresh(void)
{
  m_settingsTree->refresh();
}

void
IgSettingsEditor::setAutoRefresh(bool value)
{
  m_settingsTree->setAutoRefresh(value);
}

void
IgSettingsEditor::setFallbacksEnabled(bool value)
{
  m_settingsTree->setFallbacksEnabled(value);
}

void
IgSettingsEditor::setSettingsObject(QSettings *settings)
{
  m_settingsTree->setSettingsObject(settings);

  actionRefresh->setEnabled(true);
  actionAuto_Refresh->setEnabled(true);

  QString niceName = settings->fileName();
  niceName.replace("\\", "/");
  int pos = niceName.lastIndexOf("/");
  if (pos != -1)
    niceName.remove(0, pos + 1);

  if (!settings->isWritable())
    niceName = tr("%1(read only)").arg(niceName);

  setWindowTitle(tr("%1 - %2").arg(niceName).arg(tr("Settings Editor")));
}

void
IgSettingsEditor::about(void)
{
  qDebug() << "IgSettingsEditor::about not implemented yet.";
}
