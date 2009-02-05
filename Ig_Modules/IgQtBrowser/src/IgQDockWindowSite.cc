//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtBrowser/interface/IgQDockWindowSite.h"
#include <qdockwindow.h>
#include <qlayout.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQDockWindowSite::IgQDockWindowSite (IgState */*state*/, IgSite *parent)
    : m_widget (new QDockWindow (QDockWindow::InDock, 
				 hostFrom (parent)))
{
    m_widget->setResizeEnabled (true);    
    m_widget->hide ();
    
    IgQtSite::host (parent, m_widget);
}

IgQDockWindowSite::IgQDockWindowSite (IgSite *parent, QDockWindow *widget)
    : m_widget (widget),
      m_layout (new QGridLayout (m_widget))
{
    IgQtSite::host (parent, m_widget);
}

const char *
IgQDockWindowSite::catalogLabel (void)
{ return "QDockWindow"; }

QWidget *
IgQDockWindowSite::self (void)
{
    
    return m_widget;
}

QWidget *
IgQDockWindowSite::host (void)
{
    return m_widget;
}

void
IgQDockWindowSite::host (QWidget *widget)
{
    m_widget->setWidget (widget);
    m_widget->show ();
    widget->show ();    
}
