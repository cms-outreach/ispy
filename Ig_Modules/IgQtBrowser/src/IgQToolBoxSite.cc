//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtBrowser/interface/IgQToolBoxSite.h"
#include <qtoolbox.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQToolBoxSite::IgQToolBoxSite (IgState *, IgQtSite *parent)
    : m_widget (new QToolBox (hostFrom (parent))),
      m_currentName ("")
{    
    IgQtSite::host (parent, m_widget);
}

const char *
IgQToolBoxSite::catalogLabel (void)
{ 
    return "QToolBox"; 
}

QWidget *
IgQToolBoxSite::self (void)
{
    return m_widget;
}

QWidget *
IgQToolBoxSite::host (void)
{
    return m_widget;
}

void
IgQToolBoxSite::host (QWidget *child)
{    
    m_widget->addItem (child, m_currentName);    
}

IgQtSite *
IgQToolBoxSite::hostWithName (const std::string &name)
{
    m_currentName = name;    
    return this;    
}
