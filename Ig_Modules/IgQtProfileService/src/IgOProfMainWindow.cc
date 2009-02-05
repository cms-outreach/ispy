//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtProfileService/src/IgOProfMainWindow.h"
#include <qapplication.h>
#include <qmainwindow.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qmenubar.h>
#include <qtoolbar.h>
#include <qmessagebox.h>
#include <qstatusbar.h>
#include <qfiledialog.h>
#include <qinputdialog.h>
#include <qfile.h>
#include <qdatastream.h>
#include <qhbox.h>
#include <qlineedit.h>
#include <qlabel.h>
#include "Ig_Modules/IgQtProfileService/src/IgOProfMainListView.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfMenuBar.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfConfiguration.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfProcDevice.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgState.h"
#include <qregexp.h>
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


IgOProfMainWindow::IgOProfMainWindow (QMenuBar *parentMenuBar, QWidget *parent,QStatusBar *statusbar, bool inDialog)
    :m_config ( IgOProfConfigurationSingleton::instance () ),
     m_listView (0),
     m_parentMenuBar (parentMenuBar),
     m_menuBar (0),
     m_updateButton (0),
     m_layout (0),
     m_parent (parent),
     m_buttons (0),
     m_filterBox (0),
     m_filterLineEdit (0),
     m_filterLabel (0),
     m_statusBar (statusbar),
     m_inDialog (inDialog),
     m_readyToRun (false)
{
}

void 
IgOProfMainWindow::createGUI (bool disabled)
{
    /*This block of asserts makes sure we don't create the GUI twice.
     */
    ASSERT (m_layout == 0);
    ASSERT (m_layout == 0);
    ASSERT (m_listView == 0);
    ASSERT (m_menuBar == 0);
    ASSERT (m_buttons == 0);
    ASSERT (m_updateButton == 0);
    ASSERT (m_filterBox == 0);
    ASSERT (m_filterLabel == 0);
    ASSERT (m_filterLineEdit == 0);
    
    m_layout = new QVBoxLayout (m_parent);
    m_listView = new IgOProfMainListView (m_parent);
    m_menuBar = new IgOProfMenuBar (m_parentMenuBar,m_listView,m_config,m_inDialog);
    m_buttons = new QHBox (m_parent);
    m_updateButton = new QPushButton ("Update", m_buttons);
    m_filterBox = new QHBox (m_buttons);
    m_filterLabel = new QLabel ("Filter program names",m_filterBox);    
    m_filterLineEdit = new QLineEdit (m_filterBox);
    
    connect (m_updateButton,SIGNAL (clicked ()), m_listView, SLOT (update ()));
    connect (m_filterLineEdit,SIGNAL (returnPressed()), this, SLOT (setFilterAndUpdate ()));

    m_buttons->setMargin (5);
    m_buttons->setSpacing (25);
    m_filterBox->setMargin (5);
    m_filterBox->setSpacing (25);

    m_layout->addWidget (m_listView);
    m_layout->addWidget (m_buttons);

    m_statusBar->show ();
    m_listView->show ();
    m_updateButton->show ();
    m_buttons->show ();
    m_filterBox->show ();
    m_filterLabel->show ();
    m_filterLineEdit->show ();

    setGUIStatus (disabled == false);
    
    m_readyToRun = true;    
}


void 
IgOProfMainWindow::setGUIStatus (bool enabled)
{
    m_updateButton->setEnabled (enabled);	
}

bool 
IgOProfMainWindow::init (void)
{
    int choice = 0;
    IgOProfConfiguration &cfg = *m_config;
    
    //If no profiler is present, ask for remote profiling or quit.
    //If profiler is found, ask for local or remote profiling.
    if (cfg.m_noProfilerFound == true)
    {
	cfg.m_remoteProfiling = (QMessageBox::warning 
				(m_parent, 
				 "Profiler not found", 
				 "oprofile does not seem to be installed on your local box.\n For more informations on how to install it, please have a look at http://oprofile.sf.net .\n Would you like to profile a remote box instead?",
				 "Yes",
				 "No") == 0);	
	if (cfg.m_remoteProfiling == false)
	    return false;	
    }    
    else
    {	
	cfg.m_remoteProfiling = QMessageBox::warning (m_parent, 
						      "Select target",
						      "Would you like to profile a local or remote box?",
						      "Local",
						      "Remote");
    }

    if (cfg.m_remoteProfiling == false)
    {
	/*Checks for Vmlinux*/
	while (QFile::exists (cfg.m_vmLinuxPath.c_str ()) == false)
	{
	    choice = QMessageBox::warning (m_parent,
					   "Oprofile",
					   "Warning: cannot load your kernel image, please, choose one! This is usually found in /boot and it is called vmlinux-KernelVersion",
					   "Ok!",
					   "Abort");
	    if (choice == 1)
	    {
		return false;
	    }
	
	    cfg.m_vmLinuxPath = (const char *) QFileDialog::getOpenFileName ("/boot","All files (*)",m_parent,"Choose kernel image file to use.");
	}
    
	if (cfg.proc ().isRunning ())
	{
	    choice = QMessageBox::warning (m_parent, 
					   "Oprofile", 
					   "Oprofile is already running. What would you like to do?",
					   "Restart and use my configuration",
					   "Use current counters configuration",
					   "Abort");
	    switch (choice)
	    {
	    default:
	    case 0:
		cfg.use ();
		break;
	    case 1:
		cfg.readCountersFromProc ();
		break;
	    case 2:
		return false;	    
		break;
	    }	
	}
	else if (cfg.proc ().canStart () == false)
	{
	    choice = QMessageBox::warning (m_parent, 
					   "Oprofile",
					   "oprofiled deamon is not running. Please ask some authorized user to run it otherwise profiling results won't change.",
					   "Continue anyway",
					   "Abort");
	    switch (choice)
	    {
	    case 0:
		cfg.readCountersFromProc ();
		break;		
	    case 1:
		return false;
		break;
	    }
	
	}else
	{
	    cfg.use ();
	}    
    }
    else
    {
	bool ok;	
	QString text = QInputDialog::getText ("Remote profiler", 
					      "Please, enter remote host", 
					      QLineEdit::Normal, 
					      QString (cfg.m_serverAddress), 
					      &ok, 
					      m_parent );	
	cfg.m_serverAddress = text.ascii ();	
	cfg.m_serverPort = QInputDialog::getInteger ("Remote profiler", 
						     "Please, enter remote port", 
						     cfg.m_serverPort, 
						     0, 
						     65535,
						     1,
						     &ok, 
						     m_parent );
    }
    
    return true;    
}

QWidget &
IgOProfMainWindow::mainWindow (void)
{
    return *m_parent;
}

void 
IgOProfMainWindow::askToSave (void)
{
    int choice;
    choice = QMessageBox::warning  (m_parent, 
				    "OProfile", 
				    "You are about to quit oprofile, would you like to save current config?",
				    "Yes",
				    "No");
    if (choice == 0)
	m_config->write ();

    if (m_config->proc ().canStop ())
    {
	choice = QMessageBox::warning (m_parent, 
				      "OProfile", 
				      "You are about to quit, would you like to stop oprofiler?\nWARNING:this could influence also other people profiling.", 
				      "Yes", 
				      "No");
	if (choice == 0) 
	    m_config->proc ().stop ();
    }
}

void 
IgOProfMainWindow::setFilterAndUpdate (void)
{    
    IgOProfConfiguration &cfg = *m_config;
    
    cfg.m_processMask = (const char *) m_filterLineEdit->text ();
    if (cfg.m_processMask == "" ) cfg.m_maskProcess = false;
    else cfg.m_maskProcess = true;
    
    m_listView->update ();
}
