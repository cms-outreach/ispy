//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtBrowser/interface/IgQDockAreaSite.h"
#include <qdockarea.h>
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

Qt::Orientation
decideOrientation (QWidget *parent)
{
    QSplitter *splitter = dynamic_cast<QSplitter *> (parent);
    if (splitter)
    {
	if (splitter->orientation () == Qt::Vertical)
	    return Qt::Horizontal;
	else 
	    return Qt::Vertical;	
    }
    return Qt::Vertical;    
}

IgQDockAreaSite::IgQDockAreaSite (IgState */*state*/, IgSite *parent)
    : m_widget (new QDockArea (decideOrientation (hostFrom (parent)), 
			       QDockArea::Normal, 
			       hostFrom (parent)))
{
    IgQtSite::host (parent, m_widget);
}

IgQDockAreaSite::IgQDockAreaSite (IgSite *parent, QDockArea *widget)
    : m_widget (widget)
{
    IgQtSite::host (parent, m_widget);
}

const char *
IgQDockAreaSite::catalogLabel (void)
{ return "QDockArea"; }

QWidget *
IgQDockAreaSite::self (void)
{
    return m_widget;
}

QWidget *
IgQDockAreaSite::host (void)
{
    return m_widget;
}

void
IgQDockAreaSite::host (QWidget *)
{}
