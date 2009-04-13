//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Iggi/interface/IggiMainWindow.h"
#include "Iguana/Iggi/interface/IgLocationDialog.h"
#include "Iguana/Iggi/interface/IgSettingsTree.h"
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

IggiMainWindow::IggiMainWindow (QWidget *parent)
    : QMainWindow (parent),
      m_RPhiGrid        (0),
      m_RPhiGridControl (0),
      m_RZGrid          (0),
      m_RZGridControl   (0),
      m_scene           (0),
      m_focus           (0),
      m_controlCenter   (new IggiControlCenter),
      m_xScale		(2.), 
      m_yScale		(2.),
      locationDialog_ 	(0),
      settingsTree_	(new IgSettingsTree)
{
    setupUi (this);
    // 	QRectF geo = graphicsView->geometry();
    // 	m_scene = new IggiScene (geo.x (), geo.y (), geo.width (), geo.height ());
    // TODO dynamically adapt size? how do we fit the scene to the view dynamically?
//     m_scene = new IggiScene (-200, -200, 400, 400); 
//     graphicsView->setScene (m_scene);
//     graphicsView->show();
	
    // used in order to uncheck the controlcenter button if it is closed by pressing "x"
    connect( m_controlCenter, SIGNAL( isShown(bool) ), actionShowControlCenter, SLOT( setChecked(bool) ) );

#ifndef Q_WS_MAC
    actionOpen_Mac_Property_List->setEnabled(false);
#endif
#ifndef Q_WS_WIN
    actionOpen_Windows_Registry_Path->setEnabled(false);
#endif
}

IggiMainWindow::~IggiMainWindow ()
{
    delete m_RPhiGrid;
    delete m_RPhiGridControl;
    delete m_RZGrid;
    delete m_RZGridControl;
    delete m_scene;
    delete m_controlCenter;
}

void
IggiMainWindow::setZoomFocus (IggiItem* item)
{
    // this method remembers the last item that has been shown/active,
    // i.e. used for zooming
    m_focus = item;
}

void
IggiMainWindow::zoomIn ()
{
    // using the fact that zoomIn is a virtual function of IggiItem
    if (m_focus != 0)
	m_focus->zoomIn ();

//     graphicsView->scale (m_xScale, m_yScale);
}

void
IggiMainWindow::zoomOut ()
{
    if (m_focus != 0)
	m_focus->zoomOut ();

//     graphicsView->scale (1. / m_xScale, 1. / m_yScale);
}

void 
IggiMainWindow::showControlCenter (bool show)
{
    if (show)
    {
	m_controlCenter->show ();
    }
    else
    {
	m_controlCenter->hide ();
    }
}

void 
IggiMainWindow::showRPhiGrid (bool show)
{
    // show or hide the grid
    if (show)
    {
	// instantiate the grid if it doesn't exist yet, add it to the scene
	// and register it in the control center
	if (m_RPhiGrid == 0)
	{
	    m_RPhiGrid = new IgPolarCoordSystem;
	    m_scene->addItem (m_RPhiGrid);
	    m_controlCenter->setControlledItem (0, m_RPhiGrid);
	}
	m_RPhiGrid->show ();
	// the grid was just actived, so it's in focus
	setZoomFocus (m_RPhiGrid);
    }
    else
    {
	m_RPhiGrid->hide ();
    }
}

void 
IggiMainWindow::showRZGrid (bool show)
{
    if (show)
    {
	if (m_RZGrid == 0)
	{
	    m_RZGrid = new IgRZCoordSystem;
	    m_scene->addItem (m_RZGrid);
	    m_controlCenter->setControlledItem (1, m_RZGrid);
	}
	m_RZGrid->show ();
		
	setZoomFocus (m_RZGrid);
    }
    else
    {
	m_RZGrid->hide ();
    }
}

void
IggiMainWindow::writeSettings (bool value)
{}

void 
IggiMainWindow::showSettingsEditor (void)
{
    if (! settingsTree_->isVisible ())
	settingsTree_->show ();
    settingsTree_->activateWindow ();
    settingsTree_->raise ();
}

void 
IggiMainWindow::about (void)
{
    QMessageBox::about(this, tr("About iSpy"),
		       tr("<b>IGUANA iSpy</b> is an interactive graphics program "
			  "that can be downloaded from the Web and used to display physics "
			  "events without the need of any CMS software."));
}

void 
IggiMainWindow::aboutQt (void)
{
    qApp->aboutQt ();
}

void 
IggiMainWindow::openSettings (void)
{
    if (!locationDialog_)
	locationDialog_ = new IgLocationDialog (this);

    if (locationDialog_->exec ()) 
    {
	QSettings *settings = new QSettings(locationDialog_->format (),
					    locationDialog_->scope (),
					    locationDialog_->organization (),
					    locationDialog_->application ());
	setSettingsObject (settings);
	actionFallbacks->setEnabled (true);
	showSettingsEditor ();	
    }
}

void 
IggiMainWindow::openIniFile (void)
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open INI File"),
						    "", tr("INI Files (*.ini *.conf)"));
    if (! fileName.isEmpty ()) 
    {
	QSettings *settings = new QSettings (fileName, QSettings::IniFormat);
	setSettingsObject (settings);
	actionFallbacks->setEnabled(false);
    }
}

void 
IggiMainWindow::openPropertyList (void)
{
    QString fileName = QFileDialog::getOpenFileName(this,
						    tr("Open Property List"),
						    "", tr("Property List Files (*.plist)"));
    if (! fileName.isEmpty ()) 
    {
	QSettings *settings = new QSettings (fileName, QSettings::NativeFormat);
	setSettingsObject (settings);
	actionFallbacks->setEnabled (false);
    }
}

void 
IggiMainWindow::openRegistryPath (void)
{
    QString path = QInputDialog::getText(this, tr("Open Registry Path"),
					 tr("Enter the path in the Windows registry:"),
					 QLineEdit::Normal, "HKEY_CURRENT_USER\\");
    if (! path.isEmpty ()) 
    {
	QSettings *settings = new QSettings (path, QSettings::NativeFormat);
	setSettingsObject (settings);
	actionFallbacks->setEnabled (false);
    }
}

void 
IggiMainWindow::refreshSettingsEditor (void)
{
    settingsTree_->refresh ();
}

void 
IggiMainWindow::setAutoRefresh (bool value)
{
    settingsTree_->setAutoRefresh (value);
}

void 
IggiMainWindow::setFallbacksEnable (bool value)
{
    settingsTree_->setFallbacksEnabled (value);
}

void 
IggiMainWindow::setSettingsObject (QSettings *settings)
{
    settings->setFallbacksEnabled (actionFallbacks->isChecked ());
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

    settingsTree_->setWindowTitle(tr("%1 - %2").arg(niceName).arg(tr("Settings Editor")));
}
