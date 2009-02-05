//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtBrowser/interface/IgQSplitterSite.h"
#include <qsplitter.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQSplitterSite::IgQSplitterSite (IgState *, IgSite *parent)
    : m_widget (new QSplitter (hostFrom (parent)))
{
    IgQtSite::host (parent, m_widget);
}

IgQSplitterSite::IgQSplitterSite (IgSite *parent, QSplitter *widget)
    : m_widget (widget)
{
    IgQtSite::host (parent, m_widget);
}

const char *
IgQSplitterSite::catalogLabel (void)
{ return "QSplitter"; }

QWidget *
IgQSplitterSite::self (void)
{
    return m_widget;
}

QWidget *
IgQSplitterSite::host (void)
{
    return m_widget;
}

void
IgQSplitterSite::host (QWidget *)
{}
