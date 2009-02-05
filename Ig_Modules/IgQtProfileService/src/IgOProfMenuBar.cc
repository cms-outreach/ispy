//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtProfileService/src/IgOProfMenuBar.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfMainListView.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfConfiguration.h"
#include <qmessagebox.h>
#include <qmenubar.h>
#include <iostream>
#include <qapplication.h>

using namespace std;

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void IgOProfMenuBar::toggleBool (int n, bool *option)
{
    *option = ! (*option);
    m_configurationMenu->setItemChecked (n, *option);
    m_listView->update ();
}

void IgOProfMenuBar::slotConfigurationMenu (int item)
{
    switch (item)
    {
    case CONF_SHOW_ZEROS:
	toggleBool (CONF_SHOW_ZEROS, &m_config->m_showZeros);
	break;
    case CONF_SHOW_LIBRARIES:
	toggleBool (CONF_SHOW_LIBRARIES, &m_config->m_showLibraries);
	break;
    case CONF_SHOW_PERCENT:
	toggleBool (CONF_SHOW_PERCENT, &m_config->m_showPercent);
	break;
    case CONF_SEPARATE_SAMPLES:
	QMessageBox::warning (m_listView, "Oprofile", "This change requires oprofiled server to be restarted.", "Ok");
	toggleBool (CONF_SEPARATE_SAMPLES, &m_config->m_separateSamples);
	break;
    case CONF_OFFSET_ON_START:
	toggleBool (CONF_OFFSET_ON_START, &m_config->m_offsetOnStart);
	break;
    case CONF_SORT_ON_TOTAL:
	toggleBool (CONF_SORT_ON_TOTAL, &m_config->m_sortOnTotal);
	break;
    }
}

void IgOProfMenuBar::slotCounterMenu (int counter)
{
    (void) counter;
    cerr << counter << endl;
    m_listView->setCounter (counter);
}

void IgOProfMenuBar::slotAboutMenu (int counter)
{
    (void) counter;
    QMessageBox::about (m_listView, "Oprofile GUI for IGUANA", "A nice GUI for oprofile (http://oprofile.sf.net).\n");
}

void IgOProfMenuBar::slotSessionMenu (int n)
{
    switch (n)
    {
    case SESSION_SAVE:
	m_listView->saveSession ();
	break;
    case SESSION_LOAD:
	m_listView->loadSession ();
	break;
    case SESSION_HIDE:
	m_menuBar->parentWidget (true)->hide ();	
    }
}

IgOProfMenuBar::IgOProfMenuBar (QMenuBar *menuBar, IgOProfMainListView *listView, IgOProfConfiguration *config, bool inDialog)
    :m_menuBar (menuBar),
     m_sessionMenu (new QPopupMenu (m_menuBar)),
     m_counterMenu (new QPopupMenu (m_menuBar)),
     m_configurationMenu (new QPopupMenu  (m_menuBar)),
     m_aboutMenu (new QPopupMenu (m_menuBar)),
     m_listView (listView),
     m_config (config)
{
    m_menuBar->insertItem ("&Session", m_sessionMenu);
    m_sessionMenu->insertItem ("&Load previous session", SESSION_LOAD);
    m_sessionMenu->insertItem ("&Save current session", SESSION_SAVE);
    if (inDialog == true)
    {
	m_sessionMenu->insertItem ("&Hide GUI", SESSION_HIDE);
    }
            
    m_menuBar->insertItem ("&Counter", m_counterMenu);
    for (IgOProfConfiguration::IgOProfCounterListIterator i = m_config->m_counterList.begin () ;
	 i != m_config->m_counterList.end () ; i++)
    {
	IgOProfCounter &c = **i;	
	if (c.m_enabled)
	{
	    m_counterMenu->insertItem (c.m_name.c_str (), c.m_counter);
	}
    }

    m_configurationMenu->setCheckable (true);
    m_menuBar->insertItem ("C&onfiguration", m_configurationMenu);

    QPopupMenu &cm = *m_configurationMenu;
    cm.insertItem ("Show &Zeros", CONF_SHOW_ZEROS);
    cm.insertItem ("Show &Libraries", CONF_SHOW_LIBRARIES);
    cm.insertItem ("Toggle Number of &Hits/Percent", CONF_SHOW_PERCENT);
    cm.insertItem ("&Separate Samples", CONF_SEPARATE_SAMPLES);
    cm.insertItem ("&Offset on start", CONF_OFFSET_ON_START);
    cm.insertItem ("Sort on &total", CONF_SORT_ON_TOTAL);
    
    cm.setItemChecked (CONF_SHOW_ZEROS, m_config->m_showZeros);
    cm.setItemChecked (CONF_SHOW_LIBRARIES, m_config->m_showLibraries);
    cm.setItemChecked (CONF_SHOW_PERCENT, m_config->m_showPercent);
    cm.setItemChecked (CONF_SEPARATE_SAMPLES, m_config->m_separateSamples);
    cm.setItemChecked (CONF_OFFSET_ON_START, m_config->m_offsetOnStart);
    cm.setItemChecked (CONF_SORT_ON_TOTAL, m_config->m_sortOnTotal);
    cm.setItemEnabled (CONF_OFFSET_ON_START, false);
    cm.setItemEnabled (CONF_SORT_ON_TOTAL, false);
    
    m_menuBar->insertItem ("&About", m_aboutMenu);
    m_aboutMenu->insertItem ("&About", 0);

    connect (m_configurationMenu, SIGNAL (activated (int)), this, SLOT (slotConfigurationMenu (int)));
    connect (m_counterMenu, SIGNAL (activated (int)), this, SLOT (slotCounterMenu (int)));
    connect (m_aboutMenu, SIGNAL (activated (int)), this, SLOT (slotAboutMenu (int)));
    connect (m_sessionMenu, SIGNAL (activated (int)), this, SLOT (slotSessionMenu (int)));
    
    m_menuBar->show ();
}
