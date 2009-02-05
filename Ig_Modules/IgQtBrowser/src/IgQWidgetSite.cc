//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtBrowser/interface/IgQWidgetSite.h"
#include <qwidget.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQWidgetSite::IgQWidgetSite (IgState *, IgSite *parent)
    : m_widget (new QWidget (hostFrom (parent)))
{
    IgQtSite::host (parent, m_widget);
}

const char *
IgQWidgetSite::catalogLabel (void)
{ return "QWidget"; }

QWidget *
IgQWidgetSite::self (void)
{
    return m_widget;
}

QWidget *
IgQWidgetSite::host (void)
{
    return m_widget;
}

void
IgQWidgetSite::host (QWidget *)
{}

