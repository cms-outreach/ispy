//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtBrowser/interface/IgQtCallbacks.h"
#include <qwidget.h>
#include <qtabwidget.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>


void *createQtConnection (QObject *source,
			 const char *signal,
			 QObject *dest,
			 const char *slot)
{
    IgQtConnection *connection = new IgQtConnection;
    connection->m_source = source;
    connection->m_dest = dest;
    connection->m_slot = slot;
    connection->m_signal = signal;    
    return static_cast <void *> (connection);    
}


void qtConnectionCallback (void *data, bool focus)
{
    IgQtConnection *connection = static_cast<IgQtConnection *> (data);
    QObject *source = connection->m_source;
    QObject *dest = connection->m_dest;
    const char *slot = connection->m_slot.c_str ();
    const char *signal = connection->m_signal.c_str ();
    
    if (focus)
    {
	QObject::connect (source, signal, dest, slot);	
    }    
    else
    {
	QObject::disconnect (source, signal, dest, slot);	
    }
}

void *createQtTabConnection (QTabWidget *tab, QWidget *panel)
{
    IgQtTabConnection *connection = new IgQtTabConnection;
    connection->m_tab = tab;
    connection->m_panel = panel;
    return (void *) connection;    
}

void qtTabCallback (void *data, bool focus)
{
    IgQtTabConnection *connection = static_cast<IgQtTabConnection *> (data);
    QTabWidget *tab = connection->m_tab;
    QWidget *panel = connection->m_panel;
    
    tab->setTabEnabled (panel, focus);    
}

void qtShowHideCallback (void *data, bool focus)
{
    QWidget *widget = static_cast<QWidget *> (data);
    widget->setEnabled (focus);    
}
