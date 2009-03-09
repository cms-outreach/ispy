//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Iggi/interface/IgSettingsEditor.h"
#include "Iguana/Iggi/interface/IgLocationDialog.h"
#include "Iguana/Iggi/interface/IgSettingsTree.h"
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

IgSettingsEditor::IgSettingsEditor (QWidget *parent)
    : QMainWindow (parent),
      settingsTree_ (new IgSettingsTree)
{
    setCentralWidget (settingsTree_);

    locationDialog_ = 0;

    setupUi (this);

    QObject::connect(actionOpen_Settings, SIGNAL(triggered()), this, SLOT(openSettings()));
    QObject::connect(actionOpen_INI_File, SIGNAL(triggered()), this, SLOT(openIniFile()));
    QObject::connect(actionOpen_Mac_Property_List, SIGNAL(triggered()), this, SLOT(openPropertyList()));
    QObject::connect(actionOpen_Windows_Registry_Path, SIGNAL(triggered()), this, SLOT(openRegistryPath()));
    QObject::connect(actionClose, SIGNAL(triggered()), this, SLOT(hide()));
    QObject::connect(actionAbout, SIGNAL(triggered()), this, SLOT(about()));

    QObject::connect(actionRefresh, SIGNAL(triggered()), settingsTree_, SLOT(refresh()));
    QObject::connect(actionAuto_Refresh, SIGNAL(triggered(bool)), settingsTree_, SLOT(setAutoRefresh(bool)));
    QObject::connect(actionAuto_Refresh, SIGNAL(triggered(bool)), actionRefresh, SLOT(setDisabled(bool)));
    QObject::connect(actionFallback, SIGNAL(triggered(bool)), settingsTree_, SLOT(setFallbacksEnabled(bool)));

#ifndef Q_WS_MAC
    actionOpen_Mac_Property_List->setEnabled(false);
#endif
#ifndef Q_WS_WIN
    actionOpen_Windows_Registry_Path->setEnabled(false);
#endif
}

void 
IgSettingsEditor::openSettings (void)
{
    if (!locationDialog_)
	locationDialog_ = new IgLocationDialog(this);

    if (locationDialog_->exec ()) 
    {
	QSettings *settings = new QSettings(locationDialog_->format (),
					    locationDialog_->scope (),
					    locationDialog_->organization (),
					    locationDialog_->application ());
	setSettingsObject (settings);
	actionFallback->setEnabled (true);
    }
}

void 
IgSettingsEditor::openIniFile (void)
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open INI File"),
						    "", tr("INI Files (*.ini *.conf)"));
    if (! fileName.isEmpty ()) 
    {
	QSettings *settings = new QSettings (fileName, QSettings::IniFormat);
	setSettingsObject (settings);
	actionFallback->setEnabled(false);
    }
}

void 
IgSettingsEditor::openPropertyList (void)
{
    QString fileName = QFileDialog::getOpenFileName(this,
						    tr("Open Property List"),
						    "", tr("Property List Files (*.plist)"));
    if (! fileName.isEmpty ()) 
    {
	QSettings *settings = new QSettings (fileName, QSettings::NativeFormat);
	setSettingsObject (settings);
	actionFallback->setEnabled (false);
    }
}

void 
IgSettingsEditor::openRegistryPath (void)
{
    QString path = QInputDialog::getText(this, tr("Open Registry Path"),
					 tr("Enter the path in the Windows registry:"),
					 QLineEdit::Normal, "HKEY_CURRENT_USER\\");
    if (! path.isEmpty ()) 
    {
	QSettings *settings = new QSettings (path, QSettings::NativeFormat);
	setSettingsObject (settings);
	actionFallback->setEnabled (false);
    }
}

void 
IgSettingsEditor::about (void)
{
    QMessageBox::about(this, tr("About Settings Editor"),
		       tr("This <b>Settings Editor</b> example shows how to access "
			  "application settings using Qt."));
}

void 
IgSettingsEditor::setSettingsObject (QSettings *settings)
{
    settings->setFallbacksEnabled (actionFallback->isChecked ());
    settingsTree_->setSettingsObject (settings);

    actionRefresh->setEnabled (true);
    actionAuto_Refresh->setEnabled (true);

    QString niceName = settings->fileName ();
    niceName.replace("\\", "/");
    int pos = niceName.lastIndexOf("/");
    if (pos != -1)
	niceName.remove(0, pos + 1);

    if (!settings->isWritable())
	niceName = tr("%1 (read only)").arg(niceName);

    setWindowTitle(tr("%1 - %2").arg(niceName).arg(tr("Settings Editor")));
}
