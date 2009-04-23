//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/ISpyMainWindow.h"
#include "Iguana/QtGUI/interface/IgLocationDialog.h"
#include "Iguana/QtGUI/interface/IgSettingsEditor.h"
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

ISpyMainWindow::ISpyMainWindow (QWidget *parent)
    : QMainWindow (parent),
      m_RPhiGrid        (0),
      m_RPhiGridControl (0),
      m_RZGrid          (0),
      m_RZGridControl   (0),
      m_scene           (0),
      m_focus           (0),
      m_controlCenter   (new ISpyControlCenter),
      m_xScale		(2.), 
      m_yScale		(2.),
      m_locationDialog	(0),
      m_settingsEditor	(new IgSettingsEditor (this))
{
    setupUi (this);
    // 	QRectF geo = graphicsView->geometry();
    // 	m_scene = new ISpyScene (geo.x (), geo.y (), geo.width (), geo.height ());
    // TODO dynamically adapt size? how do we fit the scene to the view dynamically?
    //     m_scene = new ISpyScene (-200, -200, 400, 400); 
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

    QSettings defaultSettings;
    
    QSettings *settings = new QSettings(defaultSettings.format (),
					defaultSettings.scope (),
					"Iguana",
					"iSpy");

    m_settingsEditor->setSettingsObject (settings);
    actionFallbacks->setEnabled (true);
}

ISpyMainWindow::~ISpyMainWindow ()
{
    delete m_RPhiGrid;
    delete m_RPhiGridControl;
    delete m_RZGrid;
    delete m_RZGridControl;
    delete m_scene;
    delete m_controlCenter;
}

void
ISpyMainWindow::setZoomFocus (ISpyItem* item)
{
    // this method remembers the last item that has been shown/active,
    // i.e. used for zooming
    m_focus = item;
}

void
ISpyMainWindow::zoomIn ()
{
    // using the fact that zoomIn is a virtual function of ISpyItem
    if (m_focus != 0)
	m_focus->zoomIn ();

    //     graphicsView->scale (m_xScale, m_yScale);
}

void
ISpyMainWindow::zoomOut ()
{
    if (m_focus != 0)
	m_focus->zoomOut ();

    //     graphicsView->scale (1. / m_xScale, 1. / m_yScale);
}

void 
ISpyMainWindow::showControlCenter (bool show)
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
ISpyMainWindow::showRPhiGrid (bool show)
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
ISpyMainWindow::showRZGrid (bool show)
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
ISpyMainWindow::writeSettings (bool value)
{}

void 
ISpyMainWindow::showSettingsEditor (void)
{
    if (! m_settingsEditor->isVisible ())
	m_settingsEditor->show ();
    m_settingsEditor->activateWindow ();
    m_settingsEditor->raise ();
}

void 
ISpyMainWindow::about (void)
{
    QMessageBox::about(this, tr("About iSpy"),
		       tr("<b>IGUANA iSpy 1.0a</b> is an interactive graphics program "
			  "that can be downloaded from the Web and used to display physics "
			  "events without the need of any CMS software.\nCopyright (C) 2009 CMS\n"));
}

void 
ISpyMainWindow::aboutQt (void)
{
    qApp->aboutQt ();
}

void 
ISpyMainWindow::openSettings (void)
{
    if (!m_locationDialog)
	m_locationDialog = new IgLocationDialog (this);

    if (m_locationDialog->exec ()) 
    {
	QSettings *settings = new QSettings(m_locationDialog->format (),
					    m_locationDialog->scope (),
					    m_locationDialog->organization (),
					    m_locationDialog->application ());
	setSettingsObject (settings);
	actionFallbacks->setEnabled (true);
	showSettingsEditor ();	
    }
}

void 
ISpyMainWindow::openIniFile (void)
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
ISpyMainWindow::openPropertyList (void)
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
ISpyMainWindow::openRegistryPath (void)
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
ISpyMainWindow::refreshSettingsEditor (void)
{
    m_settingsEditor->refresh ();
}

void 
ISpyMainWindow::setAutoRefresh (bool value)
{
    m_settingsEditor->setAutoRefresh (value);
}

void 
ISpyMainWindow::setFallbacksEnable (bool value)
{
    m_settingsEditor->setFallbacksEnabled (value);
}

void 
ISpyMainWindow::setSettingsObject (QSettings *settings)
{
    settings->setFallbacksEnabled (actionFallbacks->isChecked ());
    m_settingsEditor->setSettingsObject (settings);

    actionRefresh->setEnabled (true);
    actionAuto_Refresh->setEnabled (true);
}
