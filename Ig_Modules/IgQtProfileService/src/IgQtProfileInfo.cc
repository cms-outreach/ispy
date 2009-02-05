//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtProfileService/interface/IgQtProfileInfo.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfMainWindow.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfConfiguration.h"
#include <qlayout.h>
#include <qmenubar.h>
#include <qstatusbar.h>

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

IgQtProfileInfo::IgQtProfileInfo (IgState *state)
    :QDialog (),
     m_state (state),
     m_layout (new QVBoxLayout (this)),
     m_menuBar (new QMenuBar (this)),
     m_mainWidget (new QWidget (this)),
     m_statusBar (new QStatusBar (this)),
     m_profiler_widget (new IgOProfMainWindow (m_menuBar, 
					       m_mainWidget, 
					       m_statusBar, 
					       true))
{
    m_layout->addWidget (m_menuBar);
    m_layout->addWidget (m_mainWidget);
    m_layout->addWidget (m_statusBar);
    
    show ();
}

void 
IgQtProfileInfo::show (void)
{
    if (m_profiler_widget->readyToRun () == true)
    {
	m_menuBar->show ();
	m_mainWidget->show ();
	m_statusBar->show ();
	QDialog::show ();
    }else
    {
	m_profiler_widget->createGUI (!m_profiler_widget->init ());
	show ();	    
    }    
}

void 
IgQtProfileInfo::update (const char *filter)
{
    (void) filter;
}
